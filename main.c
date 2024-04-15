#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 560

int main(int argc, char* argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface_frog, *surface_ground, *surface_platform, *surface_background;
    SDL_Texture *texture_frog, *texture_ground, *texture_platform, *texture_background;
    SDL_Rect srcRect, destRect, groundSrcRect, platformSrcRect, backgroundSrcRect;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
            "Running for cherries",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    // For the background
    surface_background = IMG_Load("game_images/Background/Brown.png");
    texture_background = SDL_CreateTextureFromSurface(renderer, surface_background);

    backgroundSrcRect.x = 0;
    backgroundSrcRect.y = 0;
    backgroundSrcRect.w = 64;
    backgroundSrcRect.h = 64;

    for (int i = 0; i < WINDOW_WIDTH; i += backgroundSrcRect.w) {
        for (int j = 0; j < WINDOW_HEIGHT; j += backgroundSrcRect.h) {
            destRect.x = i;
            destRect.y = j;
            destRect.w = backgroundSrcRect.w ;
            destRect.h = backgroundSrcRect.h ;
            SDL_RenderCopy(renderer, texture_background, &backgroundSrcRect, &destRect);
        }
    }


    // For Ninja Frog
    surface_frog = IMG_Load("game_images/Main_Characters/Ninja_Frog/Idle (32x32).png");
    texture_frog = SDL_CreateTextureFromSurface(renderer, surface_frog);

    srcRect.x = 0;     // X position of the sprite on the sprite sheet
    srcRect.y = 0;     // Y position of the sprite on the sprite sheet
    srcRect.w = 32;    // Width of the sprite
    srcRect.h = 32;    // Height of the sprite

    destRect.w = 32 * 3;   // Width of the sprite on the screen
    destRect.h = 32 * 3;   // Height of the sprite on the screen
    destRect.x = WINDOW_WIDTH/2 - destRect.w/2;  // X position on the screen
    destRect.y = WINDOW_HEIGHT - (48 * 2) - destRect.h;  // Y position on the screen

    // Load the ground and the platform
    surface_ground = IMG_Load("game_images/Terrain/Terrain (16x16).png");
    texture_ground = SDL_CreateTextureFromSurface(renderer, surface_ground);

    surface_platform = IMG_Load("game_images/Terrain/Terrain (16x16).png");
    texture_platform = SDL_CreateTextureFromSurface(renderer, surface_platform);

    // Define the source rectangles for the ground and platform sprites
    groundSrcRect.x = 96;
    groundSrcRect.y = 0;
    groundSrcRect.w = 48;
    groundSrcRect.h = 48;

    platformSrcRect.x = 272;
    platformSrcRect.y = 16;
    platformSrcRect.w = 48;
    platformSrcRect.h = 5;

    //Draw the ninja
    SDL_RenderCopy(renderer, texture_frog, &srcRect, &destRect);

    // Draw the ground
    for (int i = 0; i <= WINDOW_WIDTH; i += 48) {
        destRect.x = i * 2;
        destRect.y = WINDOW_HEIGHT - (48 * 2);
        destRect.w = 48 * 2;
        destRect.h = 48 * 2;
        SDL_RenderCopy(renderer, texture_ground, &groundSrcRect, &destRect);
    }

    // Draw the platform
    for (int i = 450; i < 620; i += 48) {
        destRect.x = i;
        destRect.y = WINDOW_HEIGHT - 200;
        destRect.w = 48;
        destRect.h = 5 * 3;
        SDL_RenderCopy(renderer, texture_platform, &platformSrcRect, &destRect);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    SDL_DestroyTexture(texture_background);
    SDL_DestroyTexture(texture_frog);
    SDL_DestroyTexture(texture_ground);
    SDL_DestroyTexture(texture_platform);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
