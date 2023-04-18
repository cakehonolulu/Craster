#include <SDL2/SDL.h>

#include <stdio.h>

int main(char argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("Craster",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window,
    -1, SDL_RENDERER_PRESENTVSYNC);

    while (1)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) exit(0);
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderDrawLine(renderer, 0, 0, 640, 480);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
