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
#include "paddle.h"
#include "renderer.h"
#include "types.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Bricks"

#define MOV_AMOUNT 5

#define BRICK_WIDTH 40
#define BRICK_HEIGHT 10

int life_count = 10;

const color_t COLOR_BRICK = { .r = 255, .g = 0, .b = 0, .a = 0 };
const color_t COLOR_BRICK_WEAK = { .r = 155, .g = 0, .b = 0, .a = 0 };

void (*paddle_mov[2])(paddle_t*, int) = { paddle_move_left, paddle_move_right };
brick_t* bricks[20];

void collide_with_bricks(ball_t* ball);
void collide_with_brick(ball_t* ball, brick_t* brick);
void create_bricks();
void draw_bricks(renderer_t* ren);
void destroy_bricks();
void collide_with_paddle(paddle_t* paddle, ball_t* ball);
void check_loose_life(ball_t* ball);
int has_lost();

int main()
{
    renderer_t* ren = renderer_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    paddle_t* paddle = paddle_create(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 30, 100, 20, WINDOW_WIDTH, COLOR_WHITE);
    ball_t* ball = ball_create(395, 295, 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_WHITE);

    create_bricks();

    short quit = FALSE;
    while (!quit) {
        event_t* event = event_poll();
        if (event->kind == QUIT) {
            quit = TRUE;
        }
        if (event->kind == KEY) {
            (*paddle_mov[event->key])(paddle, MOV_AMOUNT);
        }
        event_destroy(event);

        collide_with_bricks(ball);
        ball_move(ball, MOV_AMOUNT);
        collide_with_paddle(paddle, ball);
        check_loose_life(ball);

        if (has_lost()) {
            quit = TRUE;
        }

        renderer_clear(ren, COLOR_BLACK);
        renderer_draw_rect(ren, paddle->x, paddle->y, paddle->width, paddle->height, paddle->color);
        renderer_draw_rect(ren, ball->x, ball->y, ball->width, ball->height, ball->color);
        draw_bricks(ren);
        renderer_present(ren);
    }
    destroy_bricks();
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
        ball->x = 395;
        ball->y = 295;
        ball->y_direction = -1;
    }
}

int has_lost()
{
    return life_count == 0;
}

void create_bricks()
{
    int current_x = 10, current_y = 10;
    for (size_t i = 0; i < 20; i++) {
        brick_t* b = brick_create(current_x, current_y, BRICK_WIDTH, BRICK_HEIGHT, 2, COLOR_BRICK);
        bricks[i] = b;
        current_x += BRICK_WIDTH + 10;
        if (current_x + BRICK_WIDTH > WINDOW_WIDTH) {
            current_x = 10;
            current_y += BRICK_HEIGHT + 10;
        }
    }
}

void draw_bricks(renderer_t* ren)
{
    for (size_t i = 0; i < 20; i++) {
        if (bricks[i] != NULL) {
            brick_t* b = bricks[i];
            renderer_draw_rect(ren, b->x, b->y, b->width, b->height, b->color);
        }
    }
}

void destroy_bricks()
{
    for (size_t i = 0; i < 20; i++) {
        if (bricks[i] != NULL) {
            brick_t* b = bricks[i];
            brick_destroy(b);
            bricks[i] = NULL;
        }
    }
}

void collide_with_bricks(ball_t* ball)
{
    for (size_t i = 0; i < 20; i++) {
        if (bricks[i] != NULL) {
            brick_t* b = bricks[i];
            collide_with_brick(ball, b);
            if (b->life_count == 0) {
                brick_destroy(b);
                bricks[i] = NULL;
            }
        }
    }
}

void collide_with_brick(ball_t* ball, brick_t* brick)
{
    if (ball->x >= brick->x && ball->x < brick->x + brick->width) {
        if (ball->y < brick->y + brick->height) {
            ball->y_direction = 1;
            brick->color = COLOR_BRICK_WEAK;
            brick->life_count--;
        }
    }
}
