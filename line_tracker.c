//
// Created by lando on 2023-03-21.
//
#include <stdio.h>

#include "memory.h"
#include "line_tracker.h"

void initLineTracker(LineTracker* tracker) {
    tracker->values = NULL;
    tracker->capacity = 0;
    tracker->count = 0;
}

void freeLineTracker(LineTracker* tracker) {
    FREE_ARRAY(int, tracker->values, tracker->capacity);
    initLineTracker(tracker);
}

static void writeNewLineFrequency(LineTracker* tracker, int lineNumber) {
    if (tracker->capacity < tracker->count + 2) {
        int oldCapacity = tracker->capacity;
        tracker->capacity = GROW_CAPACITY(oldCapacity);
        tracker->values = GROW_ARRAY(int, tracker->values, oldCapacity, tracker->capacity);
    }
    tracker->values[tracker->count] = lineNumber;
    tracker->values[tracker->count + 1] = 1;
    tracker->count += 2;
}

void writeLine(LineTracker* tracker, int lineNumber) {
    if (tracker->count == 0) {
        writeNewLineFrequency(tracker, lineNumber);
    }
    else {
        int lastLine = tracker->values[tracker->count - 2];
        int frequency = tracker->values[tracker->count - 1];
        if (lineNumber == lastLine) {
            tracker->values[tracker->count - 1] = frequency + 1;
        }
        else {
            writeNewLineFrequency(tracker, lineNumber);
        }
    }
}

int getLine(LineTracker* tracker, int offset) {
    int sum = 0;
    for (int i = 0; i < tracker->count - 1; i += 2) {
        int lineNumber = tracker->values[i];
        int frequency = tracker->values[i + 1];
        if (offset >= sum && offset < sum + frequency) {
            return lineNumber;
        }
        sum += frequency;
    }

    return -1;
}