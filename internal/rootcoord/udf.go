package rootcoord

type WasmFunction struct {
	functionMap map[string][]byte
}

func (t *WasmFunction) CreateFunction(functionName string, wasmBinaryFile []byte) {
	t.functionMap[functionName] = wasmBinaryFile
}

func (t *WasmFunction) DropFunction(functionName string) {
	delete(t.functionMap, functionName)
}

func (t *WasmFunction) RunWasmFunction(functionName string) error {
	//TODO(Ziyu Wang) multiple definition of `HUF_compress2'... in wasmtime-go and datadog/zstd, fix it later.
	//wasm := t.functionMap[functionName]
	//
	//module, err := wasmtime.NewModule(t.store.Engine, wasm)
	//if err != nil {
	//	panic(err)
	//}
	//
	//instance, err := wasmtime.NewInstance(t.store, module, []wasmtime.AsExtern{})
	//if err != nil {
	//	panic(err)
	//}
	//
	//function := instance.GetFunc(t.store, functionName)
	//result, err := function.Call(t.store, 5, 37)
	//
	//return result, err
	return nil
}
