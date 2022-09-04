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
#include <wasmtime/wasmtime.hh>

using namespace wasmtime;

std::string readFile(const char* name) {
    std::ifstream watFile;
    watFile.open(name);
    std::stringstream strStream;
    strStream << watFile.rdbuf();
    return strStream.str();
}

int main() {
    // Load our WebAssembly (parsed WAT in our case), and then load it into a
    // `Module` which is attached to a `Store`. After we've got that we
    // can instantiate it.
    Engine engine;
    Store store(engine);
    auto WatStr = "(module\n"
                  "  (func $gcd (param i32 i32) (result i32)\n"
                  "    (local i32)\n"
                  "    block  ;; label = @1\n"
                  "      block  ;; label = @2\n"
                  "        local.get 0\n"
                  "        br_if 0 (;@2;)\n"
                  "        local.get 1\n"
                  "        local.set 2\n"
                  "        br 1 (;@1;)\n"
                  "      end\n"
                  "      loop  ;; label = @2\n"
                  "        local.get 1\n"
                  "        local.get 0\n"
                  "        local.tee 2\n"
                  "        i32.rem_u\n"
                  "        local.set 0\n"
                  "        local.get 2\n"
                  "        local.set 1\n"
                  "        local.get 0\n"
                  "        br_if 0 (;@2;)\n"
                  "      end\n"
                  "    end\n"
                  "    local.get 2\n"
                  "  )\n"
                  "  (export \"gcd\" (func $gcd))\n"
                  ")";
    auto module = Module::compile(engine, WatStr).unwrap();
    auto instance = Instance::create(store, module, {}).unwrap();

    // Invoke `gcd` export
    auto gcd = std::get<Func>(*instance.get(store, "gcd"));
    auto results = gcd.call(store, {6, 27}).unwrap();

    std::cout << "gcd(6, 27) = " << results[0].i32() << "\n";
}