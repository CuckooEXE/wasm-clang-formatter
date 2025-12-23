# clang-format-web

WASM-compiled `clang-format` to try out various `.clang-format` configurations. This project completely relies on [YoWASP/clang](https://github.com/YoWASP/clang/), and I highly recommend you check out the [discussion](https://discourse.llvm.org/t/rfc-building-llvm-for-webassembly/79073) on the LLVM forum about this.

## Building

To build this, just run `git submodule update --init --recursive` to get YoWASP's repository (that does all the hard work!), and run `make`. This will build the WebAssembly-built LLVM and then the WASM `formatter` module.

> [!NOTE]  
> :robot: ChatGPT was used in the creation of this project. I am not familiar with WASM and web-technologies, so while I did the head-banging trying to get the WASM to build, ChatGPT did a good amount of lifting for writing the Javascript. :robot: