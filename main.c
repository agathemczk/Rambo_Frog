#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 560
#define SPEED 20
#define GRAVITY 1

int levelSelected = 0;
SDL_Event event;

void drawLetter (SDL_Renderer *renderer, SDL_Texture *texture_letters, SDL_Rect lettersSrcRect, char letter, int x, int y, int scale) {
    lettersSrcRect.x = ((letter - 'A') % 10) * lettersSrcRect.w;
    lettersSrcRect.y = ((letter - 'A') / 10) * lettersSrcRect.h;

    SDL_Rect lettersDestRect;
    lettersDestRect.x = x;
    lettersDestRect.y = y;
    lettersDestRect.w = lettersSrcRect.w * scale;
    lettersDestRect.h = lettersSrcRect.h * scale;

    SDL_RenderCopy(renderer, texture_letters, &lettersSrcRect, &lettersDestRect);
}


void drawString(SDL_Renderer *renderer, SDL_Texture *texture_letters, SDL_Rect lettersSrcRect, const char* str, int x, int y, int scale) {
    for (int i = 0; str[i] != '\0'; i++) {
        drawLetter(renderer, texture_letters, lettersSrcRect, str[i], x + i * lettersSrcRect.w * scale, y, scale);
    }
}

void drawLevel(SDL_Renderer *renderer, SDL_Texture *texture_level, SDL_Rect levelSrcRect, int x, int y, int scale) {
    SDL_Rect levelDesRect;
    levelDesRect.x = x;
    levelDesRect.y = y;
    levelDesRect.w = levelSrcRect.w * scale;
    levelDesRect.h = levelSrcRect.h * scale;
    SDL_RenderCopy(renderer, texture_level, &levelSrcRect, &levelDesRect);
}

void showLevelMenu(SDL_Renderer *renderer, SDL_Texture *texture_level_menu, SDL_Rect levelsMenuSrcRect, SDL_Rect levelSrcRect, SDL_Rect lettersSrcRect, SDL_Texture *texture_letters, SDL_Event event) {
    for (int i = 0; i < WINDOW_WIDTH; i += levelsMenuSrcRect.w) {
        for (int j = 0; j < WINDOW_HEIGHT; j += levelsMenuSrcRect.h) {
            SDL_Rect levelsMenuDesRect;
            levelsMenuDesRect.x = i;
            levelsMenuDesRect.y = j;
            levelsMenuDesRect.w = levelsMenuSrcRect.w;
            levelsMenuDesRect.h = levelsMenuSrcRect.h;
            SDL_RenderCopy(renderer, texture_level_menu, &levelsMenuSrcRect, &levelsMenuDesRect);
        }
    }

    const char* text = "RAMBO FROG";
    int textScale = 4;
    int textWidth = strlen(text) * lettersSrcRect.w * textScale;
    int textHeight = lettersSrcRect.h * textScale;
    int textX = (WINDOW_WIDTH - textWidth) / 2;
    int textY = WINDOW_HEIGHT - 50;
    drawString(renderer, texture_letters, lettersSrcRect, text, textX, textY, textScale);

    int margin = 90;
    int gap = 60;
    int scale = 3;
    int levelWidth = levelSrcRect.w * scale;
    int levelHeight = levelSrcRect.h * scale;
    int levelsPerRow = (WINDOW_WIDTH - 2 * margin + gap) / (levelWidth + gap);
    int levelsPerColumn = (WINDOW_HEIGHT - 2 * margin + gap) / (levelHeight + gap);

    for (int i = 0; i <= 50; i++) {
        int row = i / levelsPerRow;
        int col = i % levelsPerRow;
        int x = margin + col * (levelSrcRect.w * scale + gap);
        int y = margin + row * (levelSrcRect.h * scale + gap);

        char filename[120];
        sprintf(filename, "game_images/Menu/Levels/%02d.png", i + 1);
        SDL_Surface *surface_level = IMG_Load(filename);
        SDL_Texture *texture_level = SDL_CreateTextureFromSurface(renderer, surface_level);

        SDL_Rect levelSrcRect = {0, 0, 19, 17};
        drawLevel(renderer, texture_level, levelSrcRect, x, y, scale);

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_Rect levelDestRect = {x, y, levelSrcRect.w * scale, levelSrcRect.h * scale};
            if (SDL_PointInRect(&(SDL_Point) {mouseX, mouseY}, &levelDestRect)) {
                levelSelected = i + 1;
            }
        }

        SDL_FreeSurface(surface_level);
        SDL_DestroyTexture(texture_level);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Surface *surface_frog_idle, *surface_frog_run, *surface_frog_jump, *surface_ground, *surface_platform, *surface_background, *surface_home, *surface_letters, *surface_letters_black, *surface_level_menu, *surface_level;
    SDL_Texture *texture_frog, *texture_frog_idle, *texture_frog_run, *texture_frog_jump, *texture_ground, *texture_platform, *texture_background, *texture_home, *texture_letters, *texture_letters_black, *texture_level_menu, *texture_level;
    SDL_Rect frogSrcRect, frogDestRect, groundSrcRect, groundDestRect, platformSrcRect, platformDestRect, backgroundSrcRect, backgroundDestRect, homeSrcRect, homeDestRect, lettersSrcRect, quitButtonRect, startButtonRect, levelsMenuSrcRect, levelsMenuDesRect, levelSrcRect, levelDesRect ;
    SDL_Event event;
    int quit = 0;
    int jumping = 0;
    int menuShown = 0;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
            "Rambo's Run",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    // For the homepage
    surface_home = IMG_Load("game_images/Terrain/Terrain (16x16).png");
    texture_home = SDL_CreateTextureFromSurface(renderer, surface_home);

    homeSrcRect.x = 208;
    homeSrcRect.y = 144;
    homeSrcRect.w = 32;
    homeSrcRect.h = 32;

    // For the levels'menu
    surface_level_menu = IMG_Load("game_images/Terrain/Terrain (16x16).png");
    texture_level_menu = SDL_CreateTextureFromSurface(renderer, surface_level_menu);

    levelsMenuSrcRect.x = 56;
    levelsMenuSrcRect.y = 7;
    levelsMenuSrcRect.w = 16;
    levelsMenuSrcRect.h = 18;

    // For the letters
    surface_letters = IMG_Load("game_images/Menu/Text/Text (White) (8x10).png");
    texture_letters = SDL_CreateTextureFromSurface(renderer, surface_letters);

    lettersSrcRect.w = 8 ;
    lettersSrcRect.h = 10 ;

    // For the buttons
    const char* text = "RAMBO FROG";
    int scale = 5;
    int second_scale = 4;
    int gap = 20;
    int text_width = strlen(text) * lettersSrcRect.w * scale;

    int quitButtonWidth = strlen("QUIT") * lettersSrcRect.w * second_scale;
    int startButtonWidth = strlen("START") * lettersSrcRect.w * second_scale;

    startButtonRect.x = WINDOW_WIDTH / 2 - startButtonWidth / 2;
    startButtonRect.y = WINDOW_HEIGHT / 2 + lettersSrcRect.h * scale + gap;
    startButtonRect.w = startButtonWidth;
    startButtonRect.h = lettersSrcRect.h * second_scale;

    quitButtonRect.x = WINDOW_WIDTH / 2 - quitButtonWidth / 2;
    quitButtonRect.y = startButtonRect.y + startButtonRect.h + gap;
    quitButtonRect.w = quitButtonWidth;
    quitButtonRect.h = lettersSrcRect.h * second_scale;

    surface_letters_black = IMG_Load("game_images/Menu/Text/Text (Black) (8x10).png");
    texture_letters_black = SDL_CreateTextureFromSurface(renderer, surface_letters_black);

    // Go to the homepage
    int start = 0;
    while (!start && !quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (SDL_PointInRect(&(SDL_Point) {mouseX, mouseY}, &quitButtonRect)) {
                    quit = 1;
                    SDL_FreeSurface(surface_letters_black);
                    SDL_DestroyTexture(texture_letters_black);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                }
                if (SDL_PointInRect(&(SDL_Point) {mouseX, mouseY}, &startButtonRect)) {
                    SDL_Delay(200);
                    menuShown = 1;
                    start = 1;
                }
            }

            for (int i = 0; i < WINDOW_WIDTH; i += homeSrcRect.w) {
                for (int j = 0; j < WINDOW_HEIGHT; j += homeSrcRect.h) {
                    homeDestRect.x = i;
                    homeDestRect.y = j;
                    homeDestRect.w = homeSrcRect.w;
                    homeDestRect.h = homeSrcRect.h;
                    SDL_RenderCopy(renderer, texture_home, &homeSrcRect, &homeDestRect);
                }
            }

            text = "RAMBO FROG";
            text_width = strlen(text) * lettersSrcRect.w * scale;
            int text_height = lettersSrcRect.h * scale;
            drawString(renderer, texture_letters, lettersSrcRect, text, WINDOW_WIDTH / 2 - text_width / 2,
                       WINDOW_HEIGHT / 2 - text_height / 2, scale);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (SDL_PointInRect(&(SDL_Point) {mouseX, mouseY}, &quitButtonRect)) {
                drawString(renderer, texture_letters_black, lettersSrcRect, "QUIT", quitButtonRect.x, quitButtonRect.y,
                           second_scale);
            } else {
                drawString(renderer, texture_letters, lettersSrcRect, "QUIT", quitButtonRect.x, quitButtonRect.y,
                           second_scale);
            }

            if (SDL_PointInRect(&(SDL_Point) {mouseX, mouseY}, &startButtonRect)) {
                drawString(renderer, texture_letters_black, lettersSrcRect, "START", startButtonRect.x,
                           startButtonRect.y, second_scale);
            } else {
                drawString(renderer, texture_letters, lettersSrcRect, "START", startButtonRect.x, startButtonRect.y,
                           second_scale);
            }

            SDL_RenderPresent(renderer);
        }
    }
    SDL_RenderClear(renderer);

    // For the background
    surface_background = IMG_Load("game_images/Background/Brown.png");
    texture_background = SDL_CreateTextureFromSurface(renderer, surface_background);

    backgroundSrcRect.x = 0;
    backgroundSrcRect.y = 0;
    backgroundSrcRect.w = 64;
    backgroundSrcRect.h = 64;

    // For Rambo Frog
    surface_frog_idle = IMG_Load("game_images/Main_Characters/Ninja_Frog/Idle (32x32).png");
    texture_frog_idle = SDL_CreateTextureFromSurface(renderer, surface_frog_idle);

    surface_frog_run = IMG_Load("game_images/Main_Characters/Ninja_Frog/Run (32x32).png");
    texture_frog_run = SDL_CreateTextureFromSurface(renderer, surface_frog_run);
    int sprite_index_for_run = 0;

    surface_frog_jump = IMG_Load("game_images/Main_Characters/Ninja_Frog/Jump (32x32).png");
    texture_frog_jump = SDL_CreateTextureFromSurface(renderer, surface_frog_jump);

    texture_frog = texture_frog_idle;

    frogSrcRect.x = 0;     // X position of the sprite on the sprite sheet
    frogSrcRect.y = 0;     // Y position of the sprite on the sprite sheet
    frogSrcRect.w = 32;    // Width of the sprite
    frogSrcRect.h = 32;    // Height of the sprite

    frogDestRect.w = 32 * 3;   // Width of the sprite on the screen
    frogDestRect.h = 32 * 3;   // Height of the sprite on the screen
    frogDestRect.x = WINDOW_WIDTH/2 - frogDestRect.w/2;  // X position on the screen
    frogDestRect.y = WINDOW_HEIGHT - (48 * 2) - frogDestRect.h;  // Y position on the screen

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

    while (!quit) {

        //Apply gravity without falling below the ground
        if (jumping) {
            frogDestRect.y += GRAVITY;
            if (frogDestRect.y > WINDOW_HEIGHT - (48 * 2) - frogDestRect.h) {
                frogDestRect.y = WINDOW_HEIGHT - (48 * 2) - frogDestRect.h;
                jumping = 0;
            }
        }

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                quit = 1;
            }

            if (menuShown && !levelSelected) {
                showLevelMenu(renderer, texture_level_menu, levelsMenuSrcRect, levelSrcRect, lettersSrcRect, texture_letters, event);
            }

            else if (levelSelected != 0) {

                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {

                        case SDLK_z:
                            if (!jumping) {
                                frogDestRect.y -= SPEED * 7;
                                jumping = 1;
                                texture_frog = texture_frog_jump;
                                sprite_index_for_run = 0;
                            }
                            break;

                        case SDLK_s:
                            frogDestRect.y += SPEED;
                            texture_frog = texture_frog_idle;
                            sprite_index_for_run = 0;
                            jumping = 0;
                            break;

                        case SDLK_q:
                            frogDestRect.x -= SPEED;
                            if (!jumping) {
                                texture_frog = texture_frog_run;
                                frogSrcRect.x = sprite_index_for_run * 32;
                                sprite_index_for_run = (sprite_index_for_run + 1) % 12; // Increment and loop after 12 sprites
                            }
                            break;

                        case SDLK_d:
                            frogDestRect.x += SPEED;
                            if (!jumping) {
                                texture_frog = texture_frog_run;
                                frogSrcRect.x = sprite_index_for_run * 32;
                                sprite_index_for_run = (sprite_index_for_run + 1) % 12;
                            }
                            break;
                    }

                } else if (event.type == SDL_KEYUP) {
                    if (!jumping) {
                        texture_frog = texture_frog_idle;
                        sprite_index_for_run = 0;
                    }
                }

                SDL_RenderClear(renderer);


                // Draw the background
                for (int i = 0; i < WINDOW_WIDTH; i += backgroundSrcRect.w) {
                    for (int j = 0; j < WINDOW_HEIGHT; j += backgroundSrcRect.h) {
                        backgroundDestRect.x = i;
                        backgroundDestRect.y = j;
                        backgroundDestRect.w = backgroundSrcRect.w;
                        backgroundDestRect.h = backgroundSrcRect.h;
                        SDL_RenderCopy(renderer, texture_background, &backgroundSrcRect, &backgroundDestRect);
                    }
                }

                // Draw the ground
                for (int i = 0; i <= WINDOW_WIDTH; i += 48) {
                    groundDestRect.x = i * 2;
                    groundDestRect.y = WINDOW_HEIGHT - (48 * 2);
                    groundDestRect.w = 48 * 2;
                    groundDestRect.h = 48 * 2;
                    SDL_RenderCopy(renderer, texture_ground, &groundSrcRect, &groundDestRect);
                }

//        // Draw the platform
//        for (int i = 450; i < 620; i += 48) {
//            platformDestRect.x = i;
//            platformDestRect.y = WINDOW_HEIGHT - 200;
//            platformDestRect.w = 48;
//            platformDestRect.h = 5 * 3;
//            SDL_RenderCopy(renderer, texture_platform, &platformSrcRect, &platformDestRect);
//        }

                //Draw the ninja
                SDL_RenderCopy(renderer, texture_frog, &frogSrcRect, &frogDestRect);
            }

            if (quit) {
                levelSelected = 0;
            }

            SDL_RenderPresent(renderer);
        }
    }

    SDL_FreeSurface(surface_home);
    SDL_DestroyTexture(texture_home);
    SDL_FreeSurface(surface_letters_black);
    SDL_DestroyTexture(texture_letters_black);
    SDL_DestroyTexture(texture_background);
    SDL_DestroyTexture(texture_frog);
    SDL_DestroyTexture(texture_ground);
    SDL_DestroyTexture(texture_platform);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    levelSelected = 0;
    SDL_Quit();

    return 0;
}
