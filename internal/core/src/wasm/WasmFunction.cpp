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
#include "WasmFunction.h"

WasmFunction::WasmFunction() {
    engine = new wasmtime::Engine;
    store = new wasmtime::Store(*engine);
}


WasmFunction::~WasmFunction() {
    delete (store);
    delete (engine);
}

std::string readFile(const std::string filename) {
    std::ifstream watFile;
    watFile.open(filename);
    std::stringstream strStream;
    strStream << watFile.rdbuf();
    return strStream.str();
}

void WasmFunction::runWatFile(const std::string &filename) const {
    auto wat = readFile(filename);
    runWat(wat);
}



void WasmFunction::runWat(const std::string &watString) const {
    auto module = wasmtime::Module::compile(*engine, watString).unwrap();
    auto instance = wasmtime::Instance::create(store, module, {}).unwrap();

    // Invoke `gcd` export
    auto func = std::get<wasmtime::Func>(*instance.get(store, "main"));
    auto results = func.call(store, {6, 27}).unwrap();

    std::cout << "gcd(6, 27) = " << results[0].i32() << "\n";

}