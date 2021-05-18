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
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "ball.h"
#include "brick.h"
#include "event.h"
#include "level.h"
#include "paddle.h"
#include "renderer.h"
#include "types.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Bricks"

const int PADDLE_MOV_AMOUNT = 10;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int PADDLE_START_X = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2;
const int PADDLE_START_Y = WINDOW_HEIGHT - 30;

const int BALL_MOV_AMOUNT = 5;
const int BALL_WIDTH = 10;
const int BALL_START_X = WINDOW_WIDTH / 2 - BALL_WIDTH / 2;
const int BALL_START_Y = WINDOW_HEIGHT / 2 - BALL_WIDTH / 2;

int life_count = 10;

const color_t COLOR_BRICK = { .r = 255, .g = 0, .b = 0, .a = 0 };
const color_t COLOR_BRICK_WEAK = { .r = 155, .g = 0, .b = 0, .a = 0 };

void (*paddle_mov[2])(paddle_t*, int) = { paddle_move_left, paddle_move_right };

void collide_with_bricks(ball_t* ball);
void collide_with_brick(ball_t* ball, brick_t* brick);
void draw_bricks(renderer_t* ren);

void collide_with_paddle(paddle_t* paddle, ball_t* ball);
void check_loose_life(ball_t* ball);
int has_lost(int);
void render_life_count(renderer_t* ren);

level_t* level;

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: ./bricks <LEVEL_FILE>\n");
        return -1;
    }
    const char* filename = argv[1];
    level = level_create(filename);
    if (level == NULL) {
        printf("what?!\n");
        return -1;
    }
    renderer_t* ren = renderer_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    paddle_t* paddle = paddle_create(PADDLE_START_X, PADDLE_START_Y, PADDLE_WIDTH, PADDLE_HEIGHT, WINDOW_WIDTH, COLOR_WHITE);
    ball_t* ball = ball_create(BALL_START_X, BALL_START_Y, BALL_WIDTH, BALL_WIDTH, WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_WHITE);
    printf("%d\n", level->brick_count);

    short quit = FALSE;
    while (!quit) {
        event_t* event = event_poll();
        if (event->kind == QUIT) {
            quit = TRUE;
        }
        if (event->kind == KEY) {
            (*paddle_mov[event->key])(paddle, PADDLE_MOV_AMOUNT);
        }
        event_destroy(event);

        collide_with_bricks(ball);
        ball_move(ball, BALL_MOV_AMOUNT);
        collide_with_paddle(paddle, ball);
        check_loose_life(ball);

        if (has_lost(life_count)) {
            quit = TRUE;
        }

        renderer_clear(ren, COLOR_BLACK);

        render_life_count(ren);
        renderer_draw_rect(ren, paddle->x, paddle->y, paddle->width, paddle->height, paddle->color);
        renderer_draw_rect(ren, ball->x, ball->y, ball->width, ball->height, ball->color);
        draw_bricks(ren);

        renderer_present(ren);
    }
    level_destroy(level);
    paddle_destroy(paddle);
    ball_destroy(ball);
    renderer_destroy(ren);
    return 0;
}

void collide_with_paddle(paddle_t* paddle, ball_t* ball)
{
    if (ball->x >= paddle->x && ball->x <= paddle->x + paddle->width) {
        if (ball->y + ball->height >= paddle->y) {
            ball->y_direction = -1;
        }
    }
}

void check_loose_life(ball_t* ball)
{
    if (ball->y + ball->height > WINDOW_HEIGHT) {
        life_count--;
        ball->x = BALL_START_X;
        ball->y = BALL_START_Y;
        ball->y_direction = -1;
    }
}

int has_lost(int lc)
{
    return lc == 0;
}

void draw_bricks(renderer_t* ren)
{
    for (size_t i = 0; i < level->brick_count; i++) {
        if (level->bricks[i] != NULL) {
            brick_t* b = level->bricks[i];
            renderer_draw_rect(ren, b->x, b->y, b->width, b->height, b->color);
        }
    }
}

void collide_with_bricks(ball_t* ball)
{
    for (size_t i = 0; i < level->brick_count; i++) {
        if (level->bricks[i] != NULL) {
            brick_t* b = level->bricks[i];
            collide_with_brick(ball, b);
            if (b->life_count == 0) {
                brick_destroy(b);
                level->bricks[i] = NULL;
            }
        }
    }
}

void collide_with_brick(ball_t* ball, brick_t* brick)
{
    if (ball->x >= brick->x && ball->x < brick->x + brick->width) {
        if (ball->y < brick->y + brick->height && ball->y > brick->y) {
            ball->y_direction = 1;
            brick->color = COLOR_BRICK_WEAK;
            brick->life_count--;
        } else if (ball->y < brick->y + brick->height && ball->y >= brick->y) {
            ball->y_direction = -1;
            brick->color = COLOR_BRICK_WEAK;
            brick->life_count--;
        }
    }
}

void render_life_count(renderer_t* ren)
{
    char str[10];
    sprintf(str, "Lives: %d", life_count);
    renderer_draw_text(ren, str, 10, 10, COLOR_WHITE);
}
