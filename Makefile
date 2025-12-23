
default: formatter/formatter.wasm
.PHONY: clean clang/llvm-build/lib/libclangFormat.a

clean:
	formatter/formatter.wasm

clang/llvm-build/lib/libclangFormat.a:
	cd clang/ && \
		./build.sh

formatter/formatter.wasm: formatter/formatter.cpp formatter/clang/llvm-build/lib/libclangFormat.a
	formatter/clang/wasi-sdk-27.0-x86_64-linux/bin/clang++ -o formatter/formatter.wasm formatter/formatter.cpp \
		-fvisibility=hidden \
		--sysroot=formatter/clang/wasi-sdk-27.0-x86_64-linux/share/wasi-sysroot/ \
		-Iformatter/clang/llvm-src/clang/include/ -Iformatter/clang/llvm-src/llvm/include/ -Iformatter/clang/llvm-build/include \
		$(shell ls formatter/clang/llvm-build/lib/lib*.a | sed 's|.*/lib\(.*\)\.a|-l\1|') \
		-Lformatter/clang/llvm-build/lib/ \
		-Wl,--no-entry
