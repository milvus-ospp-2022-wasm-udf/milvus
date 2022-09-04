# Copyright (C) 2019-2020 Zilliz. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed under the License
# is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing permissions and limitations under the License

cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/wasmtime/crates/c-api
#        ${CMAKE_CURRENT_BINARY_DIR}/wasmtime)
option( EMBEDDED_MILVUS "Enable embedded Milvus" OFF )

if ( EMBEDDED_MILVUS )
    add_compile_definitions( EMBEDDED_MILVUS )
endif()

add_executable(TestWasm TestWasm.cpp)

target_include_directories(TestWasm PUBLIC wasmtime)
target_link_libraries(TestWasm PUBLIC wasmtime)

#WasmFunction
add_library(
        wasm_function_obj OBJECT
        WasmFunction.cpp
)

add_executable(TestWasmFunction
        TestWasmFunction.cpp
        $<TARGET_OBJECTS:wasm_function_obj>
)

target_link_libraries(wasm_function_obj PUBLIC wasmtime)

target_include_directories(TestWasmFunction PUBLIC wasmtime)
target_link_libraries(TestWasmFunction PUBLIC wasmtime)