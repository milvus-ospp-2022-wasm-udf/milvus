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
// Created by wzy on 22-8-8.
//
#include <gtest/gtest.h>
#include "wasm/WasmFunctionManager.h"
namespace milvus {

TEST(WasmFunctionManagerTest, gcd){
    auto WatBase64Str = "KG1vZHVsZQogIChmdW5jICRnY2QgKHBhcmFtIGkzMiBpMzIpIChyZXN1bHQgaTMyKQogICAgKGxvY2FsIGkzMikKICAgIGJsb2NrICA7OyBsYWJlbCA9IEAxCiAgICAgIGJsb2NrICA7OyBsYWJlbCA9IEAyCiAgICAgICAgbG9jYWwuZ2V0IDAKICAgICAgICBicl9pZiAwICg7QDI7KQogICAgICAgIGxvY2FsLmdldCAxCiAgICAgICAgbG9jYWwuc2V0IDIKICAgICAgICBiciAxICg7QDE7KQogICAgICBlbmQKICAgICAgbG9vcCAgOzsgbGFiZWwgPSBAMgogICAgICAgIGxvY2FsLmdldCAxCiAgICAgICAgbG9jYWwuZ2V0IDAKICAgICAgICBsb2NhbC50ZWUgMgogICAgICAgIGkzMi5yZW1fdQogICAgICAgIGxvY2FsLnNldCAwCiAgICAgICAgbG9jYWwuZ2V0IDIKICAgICAgICBsb2NhbC5zZXQgMQogICAgICAgIGxvY2FsLmdldCAwCiAgICAgICAgYnJfaWYgMCAoO0AyOykKICAgICAgZW5kCiAgICBlbmQKICAgIGxvY2FsLmdldCAyCiAgKQogIChleHBvcnQgIm1haW4iIChmdW5jICRnY2QpKSA7OyBleHBvcnQgd2l0aCBtYWluCikK";
    WasmFunctionManager& wasmFunctionManager = WasmFunctionManager::getInstance();
    wasmFunctionManager.RegisterFunction(WasmFunctionManager::TYPE_WAT_MODULE,"gcd","main",WatBase64Str);
    std::vector<int> args = {6, 27};
    auto result = wasmFunctionManager.run("gcd", args);
    printf("The result of gcd(%d, %d) is %d\n", args[0], args[1], result[0]);
}

TEST(WasmFunctionManagerTest, equal){
    /* `equal` source code
     * #[no_mangle]
     * pub fn equal(a: i32, b: i32) -> bool {
     *      return a == b;
     * }
     */
    auto WatBase64Str = "KG1vZHVsZQogICh0eXBlICg7MDspIChmdW5jIChwYXJhbSBpMzIgaTMyKSAocmVzdWx0IGkzMikpKQogIChmdW5jICRlcXVhbCAodHlwZSAwKSAocGFyYW0gaTMyIGkzMikgKHJlc3VsdCBpMzIpCiAgICAobG9jYWwgaTMyIGkzMiBpMzIgaTMyIGkzMiBpMzIgaTMyIGkzMikKICAgIGdsb2JhbC5nZXQgJF9fc3RhY2tfcG9pbnRlcgogICAgbG9jYWwuc2V0IDIKICAgIGkzMi5jb25zdCAxNgogICAgbG9jYWwuc2V0IDMKICAgIGxvY2FsLmdldCAyCiAgICBsb2NhbC5nZXQgMwogICAgaTMyLnN1YgogICAgbG9jYWwuc2V0IDQKICAgIGxvY2FsLmdldCA0CiAgICBsb2NhbC5nZXQgMAogICAgaTMyLnN0b3JlIG9mZnNldD04CiAgICBsb2NhbC5nZXQgNAogICAgbG9jYWwuZ2V0IDEKICAgIGkzMi5zdG9yZSBvZmZzZXQ9MTIKICAgIGxvY2FsLmdldCAwCiAgICBsb2NhbC5zZXQgNQogICAgbG9jYWwuZ2V0IDEKICAgIGxvY2FsLnNldCA2CiAgICBsb2NhbC5nZXQgNQogICAgbG9jYWwuZ2V0IDYKICAgIGkzMi5lcQogICAgbG9jYWwuc2V0IDcKICAgIGkzMi5jb25zdCAxCiAgICBsb2NhbC5zZXQgOAogICAgbG9jYWwuZ2V0IDcKICAgIGxvY2FsLmdldCA4CiAgICBpMzIuYW5kCiAgICBsb2NhbC5zZXQgOQogICAgbG9jYWwuZ2V0IDkKICAgIHJldHVybikKICAodGFibGUgKDswOykgMSAxIGZ1bmNyZWYpCiAgKG1lbW9yeSAoOzA7KSAxNikKICAoZ2xvYmFsICRfX3N0YWNrX3BvaW50ZXIgKG11dCBpMzIpIChpMzIuY29uc3QgMTA0ODU3NikpCiAgKGdsb2JhbCAoOzE7KSBpMzIgKGkzMi5jb25zdCAxMDQ4NTc2KSkKICAoZ2xvYmFsICg7MjspIGkzMiAoaTMyLmNvbnN0IDEwNDg1NzYpKQogIChleHBvcnQgIm1lbW9yeSIgKG1lbW9yeSAwKSkKICAoZXhwb3J0ICJlcXVhbCIgKGZ1bmMgJGVxdWFsKSkKICAoZXhwb3J0ICJfX2RhdGFfZW5kIiAoZ2xvYmFsIDEpKQogIChleHBvcnQgIl9faGVhcF9iYXNlIiAoZ2xvYmFsIDIpKSkK";
    WasmFunctionManager& wasmFunctionManager = WasmFunctionManager::getInstance();
    wasmFunctionManager.RegisterFunction(WasmFunctionManager::TYPE_WAT_MODULE,"equal","equal",WatBase64Str);

    std::vector<int> args = {6, 27};
    auto result = wasmFunctionManager.run("equal", args);
    printf("The result of equal(%d, %d) is %d\n", args[0], args[1], result[0]);

    args = {27, 27};
    result = wasmFunctionManager.run("equal", args);
    printf("The result of equal(%d, %d) is %d\n", args[0], args[1], result[0]);
}

TEST(WasmFunctionManagerTest, larger_than){

    auto WatBase64Str = "KG1vZHVsZQogICh0eXBlICg7MDspIChmdW5jIChwYXJhbSBmNjQgZjY0KSAocmVzdWx0IGkzMikpKQogIChmdW5jICRsYXJnZXJfdGhhbiAodHlwZSAwKSAocGFyYW0gZjY0IGY2NCkgKHJlc3VsdCBpMzIpCiAgICAobG9jYWwgaTMyIGkzMiBpMzIgaTMyIGkzMiBpMzIpCiAgICBnbG9iYWwuZ2V0ICRfX3N0YWNrX3BvaW50ZXIKICAgIGxvY2FsLnNldCAyCiAgICBpMzIuY29uc3QgMTYKICAgIGxvY2FsLnNldCAzCiAgICBsb2NhbC5nZXQgMgogICAgbG9jYWwuZ2V0IDMKICAgIGkzMi5zdWIKICAgIGxvY2FsLnNldCA0CiAgICBsb2NhbC5nZXQgNAogICAgbG9jYWwuZ2V0IDAKICAgIGY2NC5zdG9yZQogICAgbG9jYWwuZ2V0IDQKICAgIGxvY2FsLmdldCAxCiAgICBmNjQuc3RvcmUgb2Zmc2V0PTgKICAgIGxvY2FsLmdldCAwCiAgICBsb2NhbC5nZXQgMQogICAgZjY0Lmd0CiAgICBsb2NhbC5zZXQgNQogICAgaTMyLmNvbnN0IDEKICAgIGxvY2FsLnNldCA2CiAgICBsb2NhbC5nZXQgNQogICAgbG9jYWwuZ2V0IDYKICAgIGkzMi5hbmQKICAgIGxvY2FsLnNldCA3CiAgICBsb2NhbC5nZXQgNwogICAgcmV0dXJuKQogICh0YWJsZSAoOzA7KSAxIDEgZnVuY3JlZikKICAobWVtb3J5ICg7MDspIDE2KQogIChnbG9iYWwgJF9fc3RhY2tfcG9pbnRlciAobXV0IGkzMikgKGkzMi5jb25zdCAxMDQ4NTc2KSkKICAoZ2xvYmFsICg7MTspIGkzMiAoaTMyLmNvbnN0IDEwNDg1NzYpKQogIChnbG9iYWwgKDsyOykgaTMyIChpMzIuY29uc3QgMTA0ODU3NikpCiAgKGV4cG9ydCAibWVtb3J5IiAobWVtb3J5IDApKQogIChleHBvcnQgImxhcmdlcl90aGFuIiAoZnVuYyAkbGFyZ2VyX3RoYW4pKQogIChleHBvcnQgIl9fZGF0YV9lbmQiIChnbG9iYWwgMSkpCiAgKGV4cG9ydCAiX19oZWFwX2Jhc2UiIChnbG9iYWwgMikpKQo=";
    WasmFunctionManager& wasmFunctionManager = WasmFunctionManager::getInstance();
    wasmFunctionManager.RegisterFunction(WasmFunctionManager::TYPE_WAT_MODULE,"larger_than","larger_than",WatBase64Str);

    std::vector<wasmtime::Val> args = {0.5, 0.6};
    bool result = wasmFunctionManager.runElemFunc("larger_than", args);
    printf("The result of larger_than is %d\n", result);

    WasmFunctionManager::getInstance().RegisterFunction(WasmFunctionManager::TYPE_WAT_MODULE,"larger_than","larger_than",WatBase64Str);

    args = {0.5, 0.4};
    result = wasmFunctionManager.runElemFunc("larger_than", args);
    printf("The result of larger_than is %d\n", result);
}

}  // namespace milvus