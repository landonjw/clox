#include <stdio.h>
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    writeConstant(&chunk, 1, 1);
    writeConstant(&chunk, 4, 1);
    writeChunk(&chunk, OP_ADD, 1);
    writeConstant(&chunk, 2, 1);
    writeChunk(&chunk, OP_DIVIDE, 1);
    writeChunk(&chunk, OP_NEGATE, 1);
    writeConstant(&chunk, 10, 1);
    writeChunk(&chunk, OP_MULTIPLY, 1);
    writeConstant(&chunk, -5, 1);
    writeChunk(&chunk, OP_SUBTRACT, 1);
    writeChunk(&chunk, OP_RETURN, 1);

    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);
    return 0;
}