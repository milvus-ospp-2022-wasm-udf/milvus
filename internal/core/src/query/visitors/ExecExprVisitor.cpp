// Copyright (C) 2019-2020 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License

#include <deque>
#include <optional>
#include <unordered_set>
#include <utility>
#include <boost/variant.hpp>

#include "query/ExprImpl.h"
#include "query/generated/ExecExprVisitor.h"
#include "segcore/SegmentGrowingImpl.h"
#include "query/Utils.h"
#include "query/Relational.h"
#include "wasm/WasmFunctionManager.h"

namespace milvus::query {
// THIS CONTAINS EXTRA BODY FOR VISITOR
// WILL BE USED BY GENERATOR
namespace impl {
class ExecExprVisitor : ExprVisitor {
 public:
    ExecExprVisitor(const segcore::SegmentInternalInterface& segment, int64_t row_count, Timestamp timestamp)
        : segment_(segment), row_count_(row_count), timestamp_(timestamp) {
    }

    BitsetType
    call_child(Expr& expr) {
        AssertInfo(!bitset_opt_.has_value(), "[ExecExprVisitor]Bitset already has value before accept");
        expr.accept(*this);
        AssertInfo(bitset_opt_.has_value(), "[ExecExprVisitor]Bitset doesn't have value after accept");
        auto res = std::move(bitset_opt_);
        bitset_opt_ = std::nullopt;
        return std::move(res.value());
    }

 public:
    template <typename T, typename IndexFunc, typename ElementFunc>
    auto
    ExecRangeVisitorImpl(FieldId field_id, IndexFunc func, ElementFunc element_func) -> BitsetType;

    template <typename T>
    auto
    ExecUnaryRangeVisitorDispatcher(UnaryRangeExpr& expr_raw) -> BitsetType;

    template <typename T>
    auto
    ExecBinaryArithOpEvalRangeVisitorDispatcher(BinaryArithOpEvalRangeExpr& expr_raw) -> BitsetType;

    template <typename T>
    auto
    ExecBinaryRangeVisitorDispatcher(BinaryRangeExpr& expr_raw) -> BitsetType;

    template <typename T>
    auto
    ExecTermVisitorImpl(TermExpr& expr_raw) -> BitsetType;

    template <typename CmpFunc>
    auto
    ExecCompareExprDispatcher(CompareExpr& expr, CmpFunc cmp_func) -> BitsetType;

    template <typename T, typename IndexFunc, typename ElementFunc>
    auto
    ExecUdfVisitorImpl(FieldId field_id, IndexFunc index_func, ElementFunc element_func) -> BitsetType;

    template <typename T>
    auto
    ExecUdfVisitorDispatcher(UdfExpr& expr_raw) -> BitsetType;

 private:
    const segcore::SegmentInternalInterface& segment_;
    int64_t row_count_;
    Timestamp timestamp_;
    BitsetTypeOpt bitset_opt_;
};
}  // namespace impl

void
ExecExprVisitor::visit(LogicalUnaryExpr& expr) {
    using OpType = LogicalUnaryExpr::OpType;
    auto child_res = call_child(*expr.child_);
    BitsetType res = std::move(child_res);
    switch (expr.op_type_) {
        case OpType::LogicalNot: {
            res.flip();
            break;
        }
        default: {
            PanicInfo("Invalid Unary Op");
        }
    }
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}

void
ExecExprVisitor::visit(LogicalBinaryExpr& expr) {
    using OpType = LogicalBinaryExpr::OpType;
    auto left = call_child(*expr.left_);
    auto right = call_child(*expr.right_);
    AssertInfo(left.size() == right.size(), "[ExecExprVisitor]Left size not equal to right size");
    auto res = std::move(left);
    switch (expr.op_type_) {
        case OpType::LogicalAnd: {
            res &= right;
            break;
        }
        case OpType::LogicalOr: {
            res |= right;
            break;
        }
        case OpType::LogicalXor: {
            res ^= right;
            break;
        }
        case OpType::LogicalMinus: {
            res -= right;
            break;
        }
        default: {
            PanicInfo("Invalid Binary Op");
        }
    }
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}

static auto
Assemble(const std::deque<BitsetType>& srcs) -> BitsetType {
    BitsetType res;

    int64_t total_size = 0;
    for (auto& chunk : srcs) {
        total_size += chunk.size();
    }
    res.resize(total_size);

    int64_t counter = 0;
    for (auto& chunk : srcs) {
        for (int64_t i = 0; i < chunk.size(); ++i) {
            res[counter + i] = chunk[i];
        }
        counter += chunk.size();
    }
    return res;
}

template <typename T, typename IndexFunc, typename ElementFunc>
auto
ExecExprVisitor::ExecRangeVisitorImpl(FieldId field_id, IndexFunc index_func, ElementFunc element_func) -> BitsetType {
    auto& schema = segment_.get_schema();
    auto& field_meta = schema[field_id];
    auto indexing_barrier = segment_.num_chunk_index(field_id);
    auto size_per_chunk = segment_.size_per_chunk();
    auto num_chunk = upper_div(row_count_, size_per_chunk);
    std::deque<BitsetType> results;

    using Index = scalar::ScalarIndex<T>;
    for (auto chunk_id = 0; chunk_id < indexing_barrier; ++chunk_id) {
        const Index& indexing = segment_.chunk_scalar_index<T>(field_id, chunk_id);
        // NOTE: knowhere is not const-ready
        // This is a dirty workaround
        auto data = index_func(const_cast<Index*>(&indexing));
        AssertInfo(data->size() == size_per_chunk, "[ExecExprVisitor]Data size not equal to size_per_chunk");
        results.emplace_back(std::move(*data));
    }
    for (auto chunk_id = indexing_barrier; chunk_id < num_chunk; ++chunk_id) {
        auto this_size = chunk_id == num_chunk - 1 ? row_count_ - chunk_id * size_per_chunk : size_per_chunk;
        BitsetType result(this_size);
        auto chunk = segment_.chunk_data<T>(field_id, chunk_id);
        const T* data = chunk.data();
        for (int index = 0; index < this_size; ++index) {
            result[index] = element_func(data[index]);
        }
        AssertInfo(result.size() == this_size, "");
        results.emplace_back(std::move(result));
    }
    auto final_result = Assemble(results);
    AssertInfo(final_result.size() == row_count_, "[ExecExprVisitor]Final result size not equal to row count");
    return final_result;
}

template <typename T, typename IndexFunc, typename ElementFunc>
auto
ExecExprVisitor::ExecDataRangeVisitorImpl(FieldId field_id, IndexFunc index_func, ElementFunc element_func)
    -> BitsetType {
    auto& schema = segment_.get_schema();
    auto& field_meta = schema[field_id];
    auto size_per_chunk = segment_.size_per_chunk();
    auto num_chunk = upper_div(row_count_, size_per_chunk);
    auto indexing_barrier = segment_.num_chunk_index(field_id);
    auto data_barrier = segment_.num_chunk_data(field_id);
    AssertInfo(std::max(data_barrier, indexing_barrier) == num_chunk,
               "max(data_barrier, index_barrier) not equal to num_chunk");
    std::deque<BitsetType> results;

    // for growing segment, indexing_barrier will always less than data_barrier
    // so growing segment will always execute expr plan using raw data
    // if sealed segment has loaded raw data on this field, then index_barrier = 0 and data_barrier = 1
    // in this case, sealed segment execute expr plan using raw data
    for (auto chunk_id = 0; chunk_id < data_barrier; ++chunk_id) {
        auto this_size = chunk_id == num_chunk - 1 ? row_count_ - chunk_id * size_per_chunk : size_per_chunk;
        BitsetType result(this_size);
        auto chunk = segment_.chunk_data<T>(field_id, chunk_id);
        const T* data = chunk.data();
        for (int index = 0; index < this_size; ++index) {
            result[index] = element_func(data[index]);
        }
        AssertInfo(result.size() == this_size, "[ExecExprVisitor]Chunk result size not equal to expected size");
        results.emplace_back(std::move(result));
    }

    // if sealed segment has loaded scalar index for this field, then index_barrier = 1 and data_barrier = 0
    // in this case, sealed segment execute expr plan using scalar index
    using Index = scalar::ScalarIndex<T>;
    for (auto chunk_id = data_barrier; chunk_id < indexing_barrier; ++chunk_id) {
        auto& indexing = segment_.chunk_scalar_index<T>(field_id, chunk_id);
        auto this_size = const_cast<Index*>(&indexing)->Count();
        BitsetType result(this_size);
        for (int offset = 0; offset < this_size; ++offset) {
            result[offset] = index_func(const_cast<Index*>(&indexing), offset);
        }
        results.emplace_back(std::move(result));
    }

    auto final_result = Assemble(results);
    AssertInfo(final_result.size() == row_count_, "[ExecExprVisitor]Final result size not equal to row count");
    return final_result;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
template <typename T>
auto
ExecExprVisitor::ExecUnaryRangeVisitorDispatcher(UnaryRangeExpr& expr_raw) -> BitsetType {
    auto& expr = static_cast<UnaryRangeExprImpl<T>&>(expr_raw);
    using Index = scalar::ScalarIndex<T>;
    auto op = expr.op_type_;
    auto val = expr.value_;
    switch (op) {
        case OpType::Equal: {
            auto index_func = [val](Index* index) { return index->In(1, &val); };
            auto elem_func = [val](T x) { return (x == val); };
            return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
        }
        case OpType::NotEqual: {
            auto index_func = [val](Index* index) { return index->NotIn(1, &val); };
            auto elem_func = [val](T x) { return (x != val); };
            return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
        }
        case OpType::GreaterEqual: {
            auto index_func = [val](Index* index) { return index->Range(val, OpType::GreaterEqual); };
            auto elem_func = [val](T x) { return (x >= val); };
            return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
        }
        case OpType::GreaterThan: {
            auto index_func = [val](Index* index) { return index->Range(val, OpType::GreaterThan); };
            auto elem_func = [val](T x) { return (x > val); };
            return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
        }
        case OpType::LessEqual: {
            auto index_func = [val](Index* index) { return index->Range(val, OpType::LessEqual); };
            auto elem_func = [val](T x) { return (x <= val); };
            return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
        }
        case OpType::LessThan: {
            auto index_func = [val](Index* index) { return index->Range(val, OpType::LessThan); };
            auto elem_func = [val](T x) { return (x < val); };
            return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
        }
        case OpType::PrefixMatch: {
            auto index_func = [val](Index* index) {
                auto dataset = std::make_unique<knowhere::Dataset>();
                dataset->Set(scalar::OPERATOR_TYPE, OpType::PrefixMatch);
                dataset->Set(scalar::PREFIX_VALUE, val);
                return index->Query(std::move(dataset));
            };
            auto elem_func = [val, op](T x) { return Match(x, val, op); };
            return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
        }
        // TODO: PostfixMatch
        default: {
            PanicInfo("unsupported range node");
        }
    }
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
template <typename T>
auto
ExecExprVisitor::ExecBinaryArithOpEvalRangeVisitorDispatcher(BinaryArithOpEvalRangeExpr& expr_raw) -> BitsetType {
    auto& expr = static_cast<BinaryArithOpEvalRangeExprImpl<T>&>(expr_raw);
    using Index = scalar::ScalarIndex<T>;
    auto arith_op = expr.arith_op_;
    auto right_operand = expr.right_operand_;
    auto op = expr.op_type_;
    auto val = expr.value_;

    switch (op) {
        case OpType::Equal: {
            switch (arith_op) {
                case ArithOpType::Add: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x + right_operand) == val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x + right_operand) == val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Sub: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x - right_operand) == val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x - right_operand) == val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Mul: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x * right_operand) == val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x * right_operand) == val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Div: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x / right_operand) == val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x / right_operand) == val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Mod: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return static_cast<T>(fmod(x, right_operand)) == val;
                    };
                    auto elem_func = [val, right_operand](T x) {
                        return (static_cast<T>(fmod(x, right_operand)) == val);
                    };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                default: {
                    PanicInfo("unsupported arithmetic operation");
                }
            }
        }
        case OpType::NotEqual: {
            switch (arith_op) {
                case ArithOpType::Add: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x + right_operand) != val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x + right_operand) != val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Sub: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x - right_operand) != val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x - right_operand) != val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Mul: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x * right_operand) != val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x * right_operand) != val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Div: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return (x / right_operand) != val;
                    };
                    auto elem_func = [val, right_operand](T x) { return ((x / right_operand) != val); };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                case ArithOpType::Mod: {
                    auto index_func = [val, right_operand](Index* index, size_t offset) {
                        auto x = index->Reverse_Lookup(offset);
                        return static_cast<T>(fmod(x, right_operand)) != val;
                    };
                    auto elem_func = [val, right_operand](T x) {
                        return (static_cast<T>(fmod(x, right_operand)) != val);
                    };
                    return ExecDataRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
                }
                default: {
                    PanicInfo("unsupported arithmetic operation");
                }
            }
        }
        default: {
            PanicInfo("unsupported range node with arithmetic operation");
        }
    }
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
template <typename T>
auto
ExecExprVisitor::ExecBinaryRangeVisitorDispatcher(BinaryRangeExpr& expr_raw) -> BitsetType {
    auto& expr = static_cast<BinaryRangeExprImpl<T>&>(expr_raw);
    using Index = scalar::ScalarIndex<T>;
    bool lower_inclusive = expr.lower_inclusive_;
    bool upper_inclusive = expr.upper_inclusive_;
    T val1 = expr.lower_value_;
    T val2 = expr.upper_value_;

    auto index_func = [=](Index* index) { return index->Range(val1, lower_inclusive, val2, upper_inclusive); };
    if (lower_inclusive && upper_inclusive) {
        auto elem_func = [val1, val2](T x) { return (val1 <= x && x <= val2); };
        return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
    } else if (lower_inclusive && !upper_inclusive) {
        auto elem_func = [val1, val2](T x) { return (val1 <= x && x < val2); };
        return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
    } else if (!lower_inclusive && upper_inclusive) {
        auto elem_func = [val1, val2](T x) { return (val1 < x && x <= val2); };
        return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
    } else {
        auto elem_func = [val1, val2](T x) { return (val1 < x && x < val2); };
        return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
    }
}
#pragma clang diagnostic pop

void
ExecExprVisitor::visit(UnaryRangeExpr& expr) {
    auto& field_meta = segment_.get_schema()[expr.field_id_];
    AssertInfo(expr.data_type_ == field_meta.get_data_type(),
               "[ExecExprVisitor]DataType of expr isn't field_meta data type");
    BitsetType res;
    switch (expr.data_type_) {
        case DataType::BOOL: {
            res = ExecUnaryRangeVisitorDispatcher<bool>(expr);
            break;
        }
        case DataType::INT8: {
            res = ExecUnaryRangeVisitorDispatcher<int8_t>(expr);
            break;
        }
        case DataType::INT16: {
            res = ExecUnaryRangeVisitorDispatcher<int16_t>(expr);
            break;
        }
        case DataType::INT32: {
            res = ExecUnaryRangeVisitorDispatcher<int32_t>(expr);
            break;
        }
        case DataType::INT64: {
            res = ExecUnaryRangeVisitorDispatcher<int64_t>(expr);
            break;
        }
        case DataType::FLOAT: {
            res = ExecUnaryRangeVisitorDispatcher<float>(expr);
            break;
        }
        case DataType::DOUBLE: {
            res = ExecUnaryRangeVisitorDispatcher<double>(expr);
            break;
        }
        case DataType::VARCHAR: {
            res = ExecUnaryRangeVisitorDispatcher<std::string>(expr);
            break;
        }
        default:
            PanicInfo("unsupported");
    }
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}

void
ExecExprVisitor::visit(BinaryArithOpEvalRangeExpr& expr) {
    auto& field_meta = segment_.get_schema()[expr.field_id_];
    AssertInfo(expr.data_type_ == field_meta.get_data_type(),
               "[ExecExprVisitor]DataType of expr isn't field_meta data type");
    BitsetType res;
    switch (expr.data_type_) {
        case DataType::INT8: {
            res = ExecBinaryArithOpEvalRangeVisitorDispatcher<int8_t>(expr);
            break;
        }
        case DataType::INT16: {
            res = ExecBinaryArithOpEvalRangeVisitorDispatcher<int16_t>(expr);
            break;
        }
        case DataType::INT32: {
            res = ExecBinaryArithOpEvalRangeVisitorDispatcher<int32_t>(expr);
            break;
        }
        case DataType::INT64: {
            res = ExecBinaryArithOpEvalRangeVisitorDispatcher<int64_t>(expr);
            break;
        }
        case DataType::FLOAT: {
            res = ExecBinaryArithOpEvalRangeVisitorDispatcher<float>(expr);
            break;
        }
        case DataType::DOUBLE: {
            res = ExecBinaryArithOpEvalRangeVisitorDispatcher<double>(expr);
            break;
        }
        default:
            PanicInfo("unsupported");
    }
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}

void
ExecExprVisitor::visit(BinaryRangeExpr& expr) {
    auto& field_meta = segment_.get_schema()[expr.field_id_];
    AssertInfo(expr.data_type_ == field_meta.get_data_type(),
               "[ExecExprVisitor]DataType of expr isn't field_meta data type");
    BitsetType res;
    switch (expr.data_type_) {
        case DataType::BOOL: {
            res = ExecBinaryRangeVisitorDispatcher<bool>(expr);
            break;
        }
        case DataType::INT8: {
            res = ExecBinaryRangeVisitorDispatcher<int8_t>(expr);
            break;
        }
        case DataType::INT16: {
            res = ExecBinaryRangeVisitorDispatcher<int16_t>(expr);
            break;
        }
        case DataType::INT32: {
            res = ExecBinaryRangeVisitorDispatcher<int32_t>(expr);
            break;
        }
        case DataType::INT64: {
            res = ExecBinaryRangeVisitorDispatcher<int64_t>(expr);
            break;
        }
        case DataType::FLOAT: {
            res = ExecBinaryRangeVisitorDispatcher<float>(expr);
            break;
        }
        case DataType::DOUBLE: {
            res = ExecBinaryRangeVisitorDispatcher<double>(expr);
            break;
        }
        case DataType::VARCHAR: {
            res = ExecBinaryRangeVisitorDispatcher<std::string>(expr);
            break;
        }
        default:
            PanicInfo("unsupported");
    }
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}

template <typename Op>
struct relational {
    template <typename T, typename U>
    bool
    operator()(T const& a, U const& b) const {
        return Op{}(a, b);
    }
    template <typename... T>
    bool
    operator()(T const&...) const {
        PanicInfo("incompatible operands");
    }
};

template <typename Op>
auto
ExecExprVisitor::ExecCompareExprDispatcher(CompareExpr& expr, Op op) -> BitsetType {
    using number = boost::variant<bool, int8_t, int16_t, int32_t, int64_t, float, double, std::string>;
    auto size_per_chunk = segment_.size_per_chunk();
    auto num_chunk = upper_div(row_count_, size_per_chunk);
    std::deque<BitsetType> bitsets;

    // check for sealed segment, load either raw field data or index
    auto left_indexing_barrier = segment_.num_chunk_index(expr.left_field_id_);
    auto left_data_barrier = segment_.num_chunk_data(expr.left_field_id_);
    AssertInfo(std::max(left_data_barrier, left_indexing_barrier) == num_chunk,
               "max(left_data_barrier, left_indexing_barrier) not equal to num_chunk");

    auto right_indexing_barrier = segment_.num_chunk_index(expr.right_field_id_);
    auto right_data_barrier = segment_.num_chunk_data(expr.right_field_id_);
    AssertInfo(std::max(right_data_barrier, right_indexing_barrier) == num_chunk,
               "max(right_data_barrier, right_indexing_barrier) not equal to num_chunk");

    for (int64_t chunk_id = 0; chunk_id < num_chunk; ++chunk_id) {
        auto size = chunk_id == num_chunk - 1 ? row_count_ - chunk_id * size_per_chunk : size_per_chunk;
        auto getChunkData = [&, chunk_id](DataType type, FieldId field_id,
                                          int64_t data_barrier) -> std::function<const number(int)> {
            switch (type) {
                case DataType::BOOL: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<bool>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<bool>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                case DataType::INT8: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<int8_t>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<int8_t>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                case DataType::INT16: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<int16_t>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<int16_t>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                case DataType::INT32: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<int32_t>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<int32_t>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                case DataType::INT64: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<int64_t>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<int64_t>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                case DataType::FLOAT: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<float>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<float>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                case DataType::DOUBLE: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<double>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<double>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                case DataType::VARCHAR: {
                    if (chunk_id < data_barrier) {
                        auto chunk_data = segment_.chunk_data<std::string>(field_id, chunk_id).data();
                        return [chunk_data](int i) -> const number { return chunk_data[i]; };
                    } else {
                        // for case, sealed segment has loaded index for scalar field instead of raw data
                        auto& indexing = segment_.chunk_scalar_index<std::string>(field_id, chunk_id);
                        return [&indexing](int i) -> const number { return indexing.Reverse_Lookup(i); };
                    }
                }
                default:
                    PanicInfo("unsupported datatype");
            }
        };
        auto left = getChunkData(expr.left_data_type_, expr.left_field_id_, left_data_barrier);
        auto right = getChunkData(expr.right_data_type_, expr.right_field_id_, right_data_barrier);

        BitsetType bitset(size);
        for (int i = 0; i < size; ++i) {
            bool is_in = boost::apply_visitor(Relational<decltype(op)>{}, left(i), right(i));
            bitset[i] = is_in;
        }
        bitsets.emplace_back(std::move(bitset));
    }
    auto final_result = Assemble(bitsets);
    AssertInfo(final_result.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    return final_result;
}

void
ExecExprVisitor::visit(CompareExpr& expr) {
    auto& schema = segment_.get_schema();
    auto& left_field_meta = schema[expr.left_field_id_];
    auto& right_field_meta = schema[expr.right_field_id_];
    AssertInfo(expr.left_data_type_ == left_field_meta.get_data_type(),
               "[ExecExprVisitor]Left data type not equal to left field mata type");
    AssertInfo(expr.right_data_type_ == right_field_meta.get_data_type(),
               "[ExecExprVisitor]right data type not equal to right field mata type");

    BitsetType res;
    switch (expr.op_type_) {
        case OpType::Equal: {
            res = ExecCompareExprDispatcher(expr, std::equal_to<>{});
            break;
        }
        case OpType::NotEqual: {
            res = ExecCompareExprDispatcher(expr, std::not_equal_to<>{});
            break;
        }
        case OpType::GreaterEqual: {
            res = ExecCompareExprDispatcher(expr, std::greater_equal<>{});
            break;
        }
        case OpType::GreaterThan: {
            res = ExecCompareExprDispatcher(expr, std::greater<>{});
            break;
        }
        case OpType::LessEqual: {
            res = ExecCompareExprDispatcher(expr, std::less_equal<>{});
            break;
        }
        case OpType::LessThan: {
            res = ExecCompareExprDispatcher(expr, std::less<>{});
            break;
        }
        case OpType::PrefixMatch: {
            res = ExecCompareExprDispatcher(expr, MatchOp<OpType::PrefixMatch>{});
            break;
        }
            // case OpType::PostfixMatch: {
            // }
        default: {
            PanicInfo("unsupported optype");
        }
    }
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}

template <typename T>
auto
ExecExprVisitor::ExecTermVisitorImpl(TermExpr& expr_raw) -> BitsetType {
    auto& expr = static_cast<TermExprImpl<T>&>(expr_raw);
    auto& schema = segment_.get_schema();
    auto primary_filed_id = schema.get_primary_field_id();
    auto field_id = expr_raw.field_id_;
    auto& field_meta = schema[field_id];

    bool use_pk_index = false;
    if (primary_filed_id.has_value()) {
        use_pk_index = primary_filed_id.value() == field_id && IsPrimaryKeyDataType(field_meta.get_data_type());
    }

    if (use_pk_index) {
        auto id_array = std::make_unique<IdArray>();
        switch (field_meta.get_data_type()) {
            case DataType::INT64: {
                auto dst_ids = id_array->mutable_int_id();
                for (const auto& id : expr.terms_) {
                    dst_ids->add_data((int64_t&)id);
                }
                break;
            }
            case DataType::VARCHAR: {
                auto dst_ids = id_array->mutable_str_id();
                for (const auto& id : expr.terms_) {
                    dst_ids->add_data((std::string&)id);
                }
                break;
            }
            default: {
                PanicInfo("unsupported type");
            }
        }

        auto [uids, seg_offsets] = segment_.search_ids(*id_array, timestamp_);
        BitsetType bitset(row_count_);
        for (const auto& offset : seg_offsets) {
            auto _offset = (int64_t)offset.get();
            bitset[_offset] = true;
        }
        AssertInfo(bitset.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
        return bitset;
    }

    // not use pk index
    std::deque<BitsetType> bitsets;
    auto size_per_chunk = segment_.size_per_chunk();
    auto num_chunk = upper_div(row_count_, size_per_chunk);
    std::unordered_set<T> term_set(expr.terms_.begin(), expr.terms_.end());
    for (int64_t chunk_id = 0; chunk_id < num_chunk; ++chunk_id) {
        Span<T> chunk = segment_.chunk_data<T>(field_id, chunk_id);
        auto chunk_data = chunk.data();
        auto size = (chunk_id == num_chunk - 1) ? row_count_ - chunk_id * size_per_chunk : size_per_chunk;
        BitsetType bitset(size);
        for (int i = 0; i < size; ++i) {
            bitset[i] = (term_set.find(chunk_data[i]) != term_set.end());
        }
        bitsets.emplace_back(std::move(bitset));
    }
    auto final_result = Assemble(bitsets);
    AssertInfo(final_result.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    return final_result;
}

// TODO: refactor this to use `scalar::ScalarIndex::In`.
//      made a test to compare the performance.
//      vector<bool> don't match the template.
//      boost::container::vector<bool> match.
template <>
auto
ExecExprVisitor::ExecTermVisitorImpl<std::string>(TermExpr& expr_raw) -> BitsetType {
    using T = std::string;
    auto& expr = static_cast<TermExprImpl<T>&>(expr_raw);
    using Index = scalar::ScalarIndex<T>;
    const auto& terms = expr.terms_;
    auto n = terms.size();
    std::unordered_set<T> term_set(expr.terms_.begin(), expr.terms_.end());

    auto index_func = [&terms, n](Index* index) { return index->In(n, terms.data()); };
    auto elem_func = [&terms, &term_set](T x) {
        //// terms has already been sorted.
        // return std::binary_search(terms.begin(), terms.end(), x);
        return term_set.find(x) != term_set.end();
    };

    return ExecRangeVisitorImpl<T>(expr.field_id_, index_func, elem_func);
}

void
ExecExprVisitor::visit(TermExpr& expr) {
    auto& field_meta = segment_.get_schema()[expr.field_id_];
    AssertInfo(expr.data_type_ == field_meta.get_data_type(),
               "[ExecExprVisitor]DataType of expr isn't field_meta data type ");
    BitsetType res;
    switch (expr.data_type_) {
        case DataType::BOOL: {
            res = ExecTermVisitorImpl<bool>(expr);
            break;
        }
        case DataType::INT8: {
            res = ExecTermVisitorImpl<int8_t>(expr);
            break;
        }
        case DataType::INT16: {
            res = ExecTermVisitorImpl<int16_t>(expr);
            break;
        }
        case DataType::INT32: {
            res = ExecTermVisitorImpl<int32_t>(expr);
            break;
        }
        case DataType::INT64: {
            res = ExecTermVisitorImpl<int64_t>(expr);
            break;
        }
        case DataType::FLOAT: {
            res = ExecTermVisitorImpl<float>(expr);
            break;
        }
        case DataType::DOUBLE: {
            res = ExecTermVisitorImpl<double>(expr);
            break;
        }
        case DataType::VARCHAR: {
            res = ExecTermVisitorImpl<std::string>(expr);
            break;
        }
        default:
            PanicInfo("unsupported");
    }
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
auto
ExecExprVisitor::ExecUdfVisitorDispatcher(UdfExpr& expr) -> BitsetType {
    using param = boost::variant<bool, int8_t, int16_t, int32_t, int64_t, float, double, FieldId>;
    using number = boost::variant<bool, int8_t, int16_t, int32_t, int64_t, float, double, std::string>;
    // function name
    const std::string func_name = expr.func_name_;
    // function body
    const std::string wasm_body = expr.wasm_body_;
    WasmFunctionManager& wasmFunctionManager = WasmFunctionManager::getInstance();
    wasmFunctionManager.RegisterFunction(WasmFunctionManager::TYPE_WAT_MODULE,func_name,func_name,wasm_body);
    // function parameter
    const std::vector<param> values = expr.values_;
    const std::vector<DataType> value_types = expr.arg_types_;
    const std::vector<bool> is_field = expr.is_field_;
    auto params_size = values.size();
    std::vector<wasmtime::Val> params;
    params.reserve(params_size);// TODO(wang ziyu): add getChunkData

    std::deque<BitsetType> bitsets;
    auto size_per_chunk = segment_.size_per_chunk();
    auto num_chunk = upper_div(row_count_, size_per_chunk);
    for (int i = 0; i < params_size; ++i) {
        if(is_field[i]){
            auto field_id = boost::get<FieldId>(values[i]);
            auto data_barrier = segment_.num_chunk_data(field_id);
            AssertInfo(data_barrier == num_chunk, "data_barrier not equal to num_chunk");
        }
    }
    for (int64_t chunk_id = 0; chunk_id < num_chunk; ++chunk_id) {
        auto size = chunk_id == num_chunk - 1 ? row_count_ - chunk_id * size_per_chunk : size_per_chunk;
        auto getChunkData = [&, chunk_id](DataType type, FieldId field_id) -> std::function<const number(int)> {
            switch (type) {
                case DataType::BOOL: {
                    auto chunk_data = segment_.chunk_data<bool>(field_id, chunk_id).data();
                    return [chunk_data](int i) -> const number { return chunk_data[i]; };
                }
                case DataType::INT8: {
                    auto chunk_data = segment_.chunk_data<int8_t>(field_id, chunk_id).data();
                    return [chunk_data](int i) -> const number { return chunk_data[i]; };
                }
                case DataType::INT16: {
                    auto chunk_data = segment_.chunk_data<int16_t>(field_id, chunk_id).data();
                    return [chunk_data](int i) -> const number { return chunk_data[i]; };
                }
                case DataType::INT32: {
                    auto chunk_data = segment_.chunk_data<int32_t>(field_id, chunk_id).data();
                    return [chunk_data](int i) -> const number { return chunk_data[i]; };
                }
                case DataType::INT64: {
                    auto chunk_data = segment_.chunk_data<int64_t>(field_id, chunk_id).data();
                    return [chunk_data](int i) -> const number { return chunk_data[i]; };
                }
                case DataType::FLOAT: {
                    auto chunk_data = segment_.chunk_data<float>(field_id, chunk_id).data();
                    return [chunk_data](int i) -> const number { return chunk_data[i]; };
                }
                case DataType::DOUBLE: {
                    auto chunk_data = segment_.chunk_data<double>(field_id, chunk_id).data();
                    return [chunk_data](int i) -> const number { return chunk_data[i]; };
                }
                default:
                    PanicInfo("unsupported datatype");
            }
        };
        BitsetType bitset(size);
        for (int i = 0; i < size; ++i) {
            for (int param_index = 0; param_index < params_size; ++param_index) {
                if(is_field[param_index]){
                    auto field_id = boost::get<FieldId>(values[param_index]);
                    auto data_barrier = segment_.num_chunk_data(field_id);
                    auto data_type = value_types[param_index];
                    auto chunkData = getChunkData(data_type, field_id);
                    switch (data_type) {
                        case DataType::BOOL:
                            params.emplace_back(boost::get<bool>(chunkData(i)));
                            break;
                        case DataType::INT8:
                            params.emplace_back(boost::get<int8_t>(chunkData(i)));
                            break;
                        case DataType::INT16:
                            params.emplace_back(boost::get<int16_t>(chunkData(i)));
                            break;
                        case DataType::INT32:
                            params.emplace_back(boost::get<int32_t>(chunkData(i)));
                            break;
                        case DataType::INT64:
                            params.emplace_back(boost::get<int64_t>(chunkData(i)));
                            break;
                        case DataType::FLOAT:
                            params.emplace_back(boost::get<float>(chunkData(i)));
                            break;
                        case DataType::DOUBLE:
                            params.emplace_back(boost::get<double>(chunkData(i)));
                            break;
                        default: {
                            PanicInfo("unsupported data type");
                        }
                    }
                } else {
                    auto data_type = value_types[param_index];
                    auto value = values[param_index];
                    switch (data_type) {
                        case DataType::BOOL:
                            params.emplace_back(boost::get<bool>(value));
                            break;
                        case DataType::INT8:
                            params.emplace_back(boost::get<int8_t>(value));
                            break;
                        case DataType::INT16:
                            params.emplace_back(boost::get<int16_t>(value));
                            break;
                        case DataType::INT32:
                            params.emplace_back(boost::get<int32_t>(value));
                            break;
                        case DataType::INT64:
                            params.emplace_back(boost::get<int64_t>(value));
                            break;
                        case DataType::FLOAT:
                            params.emplace_back(boost::get<float>(value));
                            break;
                        case DataType::DOUBLE:
                            params.emplace_back(boost::get<double>(value));
                            break;
                        default: {
                            PanicInfo("unsupported data type");
                        }
                    }
                }
            }
            bool is_in = wasmFunctionManager.runElemFunc(func_name, params);
            bitset[i] = is_in;
            params.clear();
        }

        bitsets.emplace_back(std::move(bitset));
    }
    auto final_result = Assemble(bitsets);
    AssertInfo(final_result.size() == row_count_, "[ExecExprVisitor]Final result size not equal to row count");
    return final_result;
}
#pragma clang diagnostic pop

void
ExecExprVisitor::visit(UdfExpr& expr) {
    auto& schema = segment_.get_schema();
    auto values = expr.values_;
    auto is_field = expr.is_field_;
    auto arg_types = expr.arg_types_;
    auto size = values.size();
    for(int i = 0; i < size; ++i){
        if(is_field[i]){
            auto field_id = boost::get<FieldId>(values[i]);
            auto &field_meta = schema[field_id];
            AssertInfo(arg_types[i] == field_meta.get_data_type(),
                       "[ExecExprVisitor]field Data type not equal to field mata type");
        }
    }
    BitsetType res;
    res = ExecUdfVisitorDispatcher(expr);
    AssertInfo(res.size() == row_count_, "[ExecExprVisitor]Size of results not equal row count");
    bitset_opt_ = std::move(res);
}
}  // namespace milvus::query
