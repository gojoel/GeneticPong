

#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_

#include <SDL2/SDL.h>

#include <string>

//returns a random integer between x and y
inline int RandInt(int x, int y) { return rand () % (y - x + 1) + x; }

//returns a random float between zero and 1
inline double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }

//returns a random float in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat();}


void renderTexture(SDL_Texture *tex,
        SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr);

void renderTexture(SDL_Texture *tex,
        SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr);

SDL_Texture* renderText(const std::string &message,
        const std::string &fontFile, SDL_Color color,
        int fontSize, SDL_Renderer *renderer);

#endif  // SRC_UTILITIES_H_
