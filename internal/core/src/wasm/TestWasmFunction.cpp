//
// Created by wzy on 22-8-8.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include "wasm/WasmFunction.h"

int main() {
    WasmFunction wasmFunction;
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
                  "  (export \"main\" (func $gcd))\n"
                  ")";
    wasmFunction.runWat(WatStr);
}