enable_testing()

add_executable(test
        init_gtest.cpp
        test_wasm_function_manager.cpp
        )

target_link_libraries( test
        $<TARGET_OBJECTS:function_manager_obj>
        gtest
        wasmtime
        )