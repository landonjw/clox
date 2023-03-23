//
// Created by lando on 2023-03-22.
//

#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"
#include "int_conversion.h"

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT_LONG: {
                uint8_t indexBytes[3] = { READ_BYTE(), READ_BYTE(), READ_BYTE() };
                uint32_t index = buildInt32(indexBytes);
                Value constant = vm.chunk->constants.values[index];
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

void freeVM() {

}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}