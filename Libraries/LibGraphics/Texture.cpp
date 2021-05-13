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
#include "Texture.h"
#include <iostream>

Graphics::Texture::Texture(const std::string& filename, int x, int y, SDL_Renderer* renderer)
    : m_texture(load_texture(filename, renderer))
    , m_renderer(renderer)
    , m_position(Graphics::Types::Point(x, y))
{
}

SDL_Texture* Graphics::Texture::load_texture(const std::string& filename, SDL_Renderer* renderer)
{
    SDL_Texture* texture;
    SDL_Surface* loaded_image = SDL_LoadBMP(filename.c_str());
    if (loaded_image != nullptr) {
        texture = SDL_CreateTextureFromSurface(renderer, loaded_image);
        SDL_FreeSurface(loaded_image);

        if (texture == nullptr) {
            std::cerr << "Failed to load texture: " << filename << " with error: " << SDL_GetError() << '\n'
                      << std::flush;
            return nullptr;
        }
    } else {
        std::cerr << "Failed to load texture: " << filename << " with error: " << SDL_GetError() << '\n'
                  << std::flush;
        return nullptr;
    }
    return texture;
}

void Graphics::Texture::render()
{
    SDL_Rect destination_rect;
    destination_rect.x = m_position.get_first();
    destination_rect.y = m_position.get_second();

    SDL_QueryTexture(m_texture, nullptr, nullptr, &destination_rect.w, &destination_rect.h);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &destination_rect);
}

void Graphics::Texture::set_x(int x)
{
    m_position.set_first(x);
}

void Graphics::Texture::set_y(int y)
{
    m_position.set_second(y);
}
