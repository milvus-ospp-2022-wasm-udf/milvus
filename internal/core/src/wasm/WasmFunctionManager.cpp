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


//
// Created by wzy on 22-8-6.
//


#include <fstream>
#include <iostream>
#include <sstream>
#include "WasmFunctionManager.h"

namespace milvus {

const std::string WasmFunctionManager::TYPE_WAT_MODULE = "WAT";    // wat str base64
const std::string WasmFunctionManager::TYPE_WASM_MODULE = "WASM";  // wasm str base64bindata

std::string readFile(const std::string name) {
    std::ifstream watFile;
    watFile.open(name);
    std::stringstream strStream;
    strStream << watFile.rdbuf();
    return strStream.str();
}

WasmtimeRunInstance WasmFunctionManager::createInstanceAndFunction(const std::string &watString, const std::string functionHandler) {
    auto module = wasmtime::Module::compile(*engine, watString).unwrap();
    auto instance = wasmtime::Instance::create(store, module, {}).unwrap();

    auto function_obj = instance.get(store, functionHandler);

    wasmtime::Func *func = std::get_if<wasmtime::Func>(&*function_obj);
    return WasmtimeRunInstance(*func, instance);
}

std::vector<int> WasmFunctionManager::run(std::string functionName, std::vector<int> args) {
    auto gotType = typeMap.find(functionName);

    if (gotType == typeMap.end()) {
        // TODO(TripleZ): cannot find function type in wasm, should throw an error
        return {};
    }

    if (gotType->second == TYPE_WAT_MODULE) {
        auto module = modules.at(functionName);
        return run(module, args);
    }

    auto module = modules.at(functionName);
    return run(module, args);
}

template<typename T>
bool
WasmFunctionManager::runElemFunc(std::string functionName, std::vector<T> args) {
    auto module = modules.at(functionName);
    std::vector<wasmtime::Val> argv;
    for (size_t i = 0; i < args.size(); ++i) {
        argv.emplace_back(static_cast<int32_t>(args[i]));
    }

    // the return
    std::vector<int> result;
    auto results = module.func.call(store, argv).unwrap();

    return results[0].i32();
}

std::vector<int> WasmFunctionManager::run(const WasmtimeRunInstance &wasmTimeRunInstance,
                                          std::vector<int> args) {
    // the args which wasm run

    std::vector<wasmtime::Val> argv;
    for (size_t i = 0; i < args.size(); ++i) {
        argv.emplace_back(static_cast<int32_t>(args[i]));
    }

    // the return
    std::vector<int> result;
    auto results = wasmTimeRunInstance.func.call(store, argv).unwrap();

    for (size_t i = 0; i < results.size(); ++i) {
        result.push_back(static_cast<int>(results[i].i32()));
    }

    return result;
}

// TODO(wzymumon): Add RegisterFunction in rootcoord and core
bool WasmFunctionManager::RegisterFunction(
                                           std::string moduleType,
                                           std::string functionName,
                                           std::string functionHandler,
                                           const std::string &base64OrOtherString) {
    // for WAT
    if (moduleType == TYPE_WAT_MODULE) {
        auto watString = myBase64Decode(base64OrOtherString);
        auto wasmRuntime = createInstanceAndFunction(watString, functionHandler);
        modules.emplace(functionName, wasmRuntime);
        typeMap.emplace(functionName, TYPE_WAT_MODULE);
        return true;
    }

    if (moduleType == TYPE_WASM_MODULE) {
        //TODO(wzymumon): run `.wasm` in wasm runtime
    }

    return false;
}

bool WasmFunctionManager::DeleteFunction(std::string functionName) {
    if (typeMap.find(functionName) == typeMap.end()) {
        return true;
    }

    auto gotType = typeMap.find(functionName);
    if (gotType->second == TYPE_WAT_MODULE) {
        modules.erase(functionName);
    }
    if (gotType->second == TYPE_WASM_MODULE) {
        //TODO(wzymumon): run `.wasm` in wasm runtime
        //wasmModules.erase(functionName);
    }
    return true;
}

}  // namespace milvus