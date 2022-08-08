set(WASMTIME_LOCAL_PATH "${CMAKE_SOURCE_DIR}/thirdparty/wasmtime-c-api")
set(WASMTIME_INCLUDE_DIR ${WASMTIME_LOCAL_PATH}/include)
set(WASMTIME_LIBRARY ${WASMTIME_LOCAL_PATH}/lib)
set(WASMTIME_LIBRARYS libwasmtime.a libwasmtime.so stdc++ -lpthread -ldl -lm)

message(${WASMTIME_LOCAL_PATH})
message(${WASMTIME_INCLUDE_DIR})
message(${WASMTIME_LIBRARY})
message(${WASMTIME_LIBRARYS})

include_directories(${WASMTIME_INCLUDE_DIR})
link_directories(${WASMTIME_LIBRARY})