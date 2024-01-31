#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>
#include <SDL_ttf.h>

SDL_Window *creation_de_fenetre();
void creation_rectangle(SDL_Renderer *renderer, int x, int y, int h, int l);
void creation_texte(SDL_Renderer *renderer, TTF_Font *snowman, const char *texte, int x, int y);
void menu(SDL_Window *window, SDL_Renderer *renderer);  // Ajout des types pour window et renderer


extern TTF_Font *snowman;
extern TTF_Font *snowman_titre;
extern SDL_Window *window;
extern SDL_Renderer *renderer;

#endif


//compilation : gcc interface.c -o interface.o -I/usr/include/SDL2 -I./include -lSDL2 -lSDL2_ttf

