// Copyright (c) 2021, Patrick Wilmes <patrick.wilmes@bit-lake.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#include "level.h"
#include "brick.h"
#include "types.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BRICK_WIDTH = 40;
const int BRICK_HEIGHT = 10;

level_t* level_create(const char* level_filename)
{
    level_t* level = calloc(sizeof(level_t), 20);
    level->bricks = calloc(sizeof(brick_t), 500);
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    file = fopen(level_filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", level_filename);
        return NULL;
    }
    int brick_count = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        int x, y, life_count;
        int separator_count = 0;
        int start = 0;
        for (size_t i = 0; i < len; i++) {
            if (line[i] == ';') {
                char temp[i - start + 1];
                strncpy(temp, line + start, i - start);
                temp[i - start] = '\0';
                switch (separator_count) {
                case 0:
                    x = atoi(temp);
                    break;
                case 1:
                    y = atoi(temp);
                    break;
                case 2:
                    life_count = atoi(temp);
                    break;
                }
                start = i + 1;
                separator_count++;
            }
        }
        level->bricks[brick_count] = brick_create(x, y, BRICK_WIDTH, BRICK_HEIGHT, life_count, COLOR_WHITE);
        brick_count++;
        level->brick_count++;
    }
    fclose(file);
    return level;
}

level_t* level_create_random_level(int window_width, int window_height)
{
    const int DEFAULT_X_DISTANCE = 10;
    const int DEFAULT_Y_DISTANCE = 10;
    level_t* level = calloc(sizeof(level_t), 20);
    level->bricks = calloc(sizeof(brick_t), 500);
    // start with an offset to not have bricks directly at the window border
    int x = 10;
    int y = 50;
    for (size_t i = 0; i < 500; ++i) { // 500 bricks
        level->bricks[i] = brick_create(x, y, BRICK_WIDTH, BRICK_HEIGHT, 2, COLOR_WHITE);
        level->brick_count++;
        x += BRICK_WIDTH + DEFAULT_X_DISTANCE;
        if (x + BRICK_WIDTH > window_width) { // start a new line of bricks
            x = 10;
            y += BRICK_HEIGHT + DEFAULT_Y_DISTANCE;
        }
        if (y + BRICK_HEIGHT > window_height / 2) {
            break;
        }
    }
    return  level;
}

void level_destroy(level_t* level)
{
    if (level == NULL) {
        return;
    }
    if (level->bricks != NULL) {
        free(level->bricks);
    }
    free(level);
}
