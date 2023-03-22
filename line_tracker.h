//
// Created by lando on 2023-03-21.
//

#ifndef clox_line_tracker_h
#define clox_line_tracker_h

typedef struct {
    int capacity;
    int count;
    int* values;
} LineTracker;

void initLineTracker(LineTracker* tracker);
void freeLineTracker(LineTracker* tracker);
void writeLine(LineTracker* tracker, int value);
int getLine(LineTracker* tracker, int offset);

#endif