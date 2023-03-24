//
// Created by lando on 2023-03-22.
//

#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"
#include "int_conversion.h"
#include "compiler.h"

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

static Value readShortConstant(uint8_t byte) {
    return vm.chunk->constants.values[byte];
}

static Value readLongConstant(uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    uint8_t bytes[3] = { byte1, byte2, byte3 };
    uint32_t index = buildInt32(bytes);
    return vm.chunk->constants.values[index];
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = readShortConstant(READ_BYTE());
                push(constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = readLongConstant(READ_BYTE(), READ_BYTE(), READ_BYTE());
                push(constant);
                break;
            }
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_NEGATE: push(-pop()); break;
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
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