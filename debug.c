//
// Created by lando on 2023-03-20.
//
#include <stdio.h>
#include "debug.h"
#include "value.h"
#include "int_conversion.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int shortConstantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int longConstantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t bytes[3] = { chunk->code[offset + 1], chunk->code[offset + 2], chunk->code[offset + 3]};
    uint32_t constant = buildInt32(bytes);
    printf("%-16s %8d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 4;
}

static void printLineNumber(Chunk* chunk, int offset) {
    int lineNumber = getLine(&chunk->lineTracker, offset);
    if (lineNumber == -1) {
        printf("   ? ");
    }
    else if (offset > 0 && lineNumber == getLine(&chunk->lineTracker, offset - 1)) {
        printf("   | ");
    }
    else {
        printf("%4d ", getLine(&chunk->lineTracker, offset));
    }
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    printLineNumber(chunk, offset);
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return shortConstantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}