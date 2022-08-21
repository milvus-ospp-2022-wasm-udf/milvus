cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/wasmtime/crates/c-api
#        ${CMAKE_CURRENT_BINARY_DIR}/wasmtime)

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