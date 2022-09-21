#pragma once
#ifndef EKG_CORE_H
#define EKG_CORE_H

#include <SDL2/SDL.h>

struct ekg_core {
protected:
    SDL_Window* root {};
public:
    void set_root(SDL_Window* sdl_win_root);
    SDL_Window* get_root();

    void init();
    void quit();

    void process_event(SDL_Event &sdl_event);
    void process_update();
    void process_render();
};

#endif