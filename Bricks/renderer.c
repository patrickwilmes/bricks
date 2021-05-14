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
#include "renderer.h"
#include <stdio.h>

#define WINDOW_POS(screen_dimension, window_dimension) (screen_dimension / 2) - (window_dimension / 2)
#define DISPLAY_INDEX 0
#define RENDERER_INDEX -1

renderer_t* renderer_create(const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Failed to initialize SDL!\n");
        return NULL;
    }

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(DISPLAY_INDEX, &dm);

    int x = WINDOW_POS(dm.w, width);
    int y = WINDOW_POS(dm.h, height);

    SDL_Window* window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Failed to create SDL window!\n");
        return NULL;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, RENDERER_INDEX, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        fprintf(stderr, "Failed to create SDL renderer!\n");
        return NULL;
    }

    renderer_t* ren = malloc(sizeof(renderer_t));
    ren->window = window;
    ren->renderer = renderer;
    return ren;
}

void renderer_destroy(renderer_t* renderer)
{
    if (renderer == NULL)
        return;
    SDL_DestroyRenderer(renderer->renderer);
    SDL_DestroyWindow(renderer->window);
    SDL_Quit();
    free(renderer);
}

void renderer_draw_rect(renderer_t *ren, int x, int y, int width, int height, color_t color)
{
    SDL_Rect rect = { .x = x, .y = y, .w = width, .h = height};
    unsigned char old_r, old_g, old_b, old_a;
    SDL_GetRenderDrawColor(ren->renderer, &old_r, &old_g, &old_b, &old_a);
    SDL_SetRenderDrawColor(ren->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(ren->renderer, &rect);
    SDL_SetRenderDrawColor(ren->renderer, old_r, old_g, old_b, old_a);
}

void renderer_clear(renderer_t *ren, color_t clear_color) {
    SDL_SetRenderDrawColor(ren->renderer, clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    SDL_RenderClear(ren->renderer);

}

void renderer_present(renderer_t *ren) {
    SDL_RenderPresent(ren->renderer);
}
