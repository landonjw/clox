//
// Created by lando on 2023-03-17.
//
#include <stdlib.h>
#include "chunk.h"
#include "memory.h"
#include "int_conversion.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t data, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk-> code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = data;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

static int addConstantToPool(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

static void writeLongConstant(Chunk* chunk, Value value, int line) {
    writeChunk(chunk, OP_CONSTANT_LONG, line);
    int constantIndex = addConstantToPool(chunk, value);
    uint8_t splitIndex[3] = {};
    splitInt32(constantIndex, splitIndex);
    for (int i = 0; i < 3; i++) {
        writeChunk(chunk, splitIndex[i], line);
    }
}

static void writeShortConstant(Chunk* chunk, Value value, int line) {
    writeChunk(chunk, OP_CONSTANT, line);
    int constantIndex = addConstantToPool(chunk, value);
    writeChunk(chunk, constantIndex, line);
}

void writeConstant(Chunk* chunk, Value value, int line) {
    if (chunk->constants.count >= 256) {
        writeLongConstant(chunk, value, line);
    }
    else {
        writeShortConstant(chunk, value, line);
    }
}