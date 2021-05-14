// Copyright (c) 2021, Patrick Wilmes <patrick.wilmes@bit-lake.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "ball.h"
#include <malloc.h>
#include <stddef.h>

ball_t* ball_create(int x, int y, int width, int height, int window_width, int window_height, color_t color)
{
    ball_t* ball = malloc(sizeof(ball_t));
    ball->x = x;
    ball->y = y;
    ball->width = width;
    ball->height = height;
    ball->window_width = window_width;
    ball->window_height = window_height;
    ball->color = color;
    ball->x_direction = 1;
    ball->y_direction = -1;
    return ball;
}

void ball_destroy(ball_t* ball)
{
    if (ball == NULL)
        return;
    free(ball);
}

void ball_move(ball_t* ball, int amount)
{
    if (ball->x <= 0) {
        ball->x_direction = 1;
    }
    if (ball->x >= ball->window_width) {
        ball->x_direction = -1;
    }
    if (ball->y <= 0) {
        ball->y_direction = 1;
    }
    if (ball->y >= ball->window_height) {
        ball->y_direction = -1;
    }
    ball->x += amount * ball->x_direction;
    ball->y += amount * ball->y_direction;
}
