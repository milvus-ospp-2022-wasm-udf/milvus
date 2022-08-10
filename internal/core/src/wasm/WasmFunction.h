//
// Created by wzy on 22-8-6.
//

#ifndef MILVUS_WASMFUNCTION_H
#define MILVUS_WASMFUNCTION_H

#include "wasmtime.hh"

class WasmFunction {
private:
    wasmtime::Engine *engine;
    wasmtime::Store *store;
public:
    WasmFunction();
    ~WasmFunction();
    void runWatFile(const std::string &filename) const;
    void runWat(const std::string &filename) const;
};


#endif //MILVUS_WASMFUNCTION_H
