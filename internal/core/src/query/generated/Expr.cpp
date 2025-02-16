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

// Generated File
// DO NOT EDIT
#include "query/Expr.h"
#include "ExprVisitor.h"

namespace milvus::query {
void
LogicalUnaryExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

void
LogicalBinaryExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

void
TermExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

void
UnaryRangeExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

void
BinaryArithOpEvalRangeExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}
void
BinaryRangeExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

void
CompareExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

void
UdfExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

}  // namespace milvus::query
