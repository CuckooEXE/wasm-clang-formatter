
default: formatter/formatter.wasm
.PHONY: clean clang/llvm-build/lib/libclangFormat.a

clean:
	formatter/formatter.wasm

clang/llvm-build/lib/libclangFormat.a:
	cd clang/ && \
		./build.sh

formatter/formatter.wasm: formatter/formatter.cpp clang/llvm-build/lib/libclangFormat.a
	clang/wasi-sdk-27.0-x86_64-linux/bin/clang -c formatter/utils.c -o formatter/utils.o
	clang/wasi-sdk-27.0-x86_64-linux/bin/clang++ -o formatter/formatter.wasm formatter/formatter.cpp \
		-fvisibility=hidden \
		--sysroot=clang/wasi-sdk-27.0-x86_64-linux/share/wasi-sysroot/ \
		-Iclang/llvm-src/clang/include/ -Iclang/llvm-src/llvm/include/ -Iclang/llvm-build/include \
		formatter/utils.o \
		$(shell ls clang/llvm-build/lib/lib*.a | sed 's|.*/lib\(.*\)\.a|-l\1|') \
		-Lclang/llvm-build/lib/ \
		-Wl,--no-entry
