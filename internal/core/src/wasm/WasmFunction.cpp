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