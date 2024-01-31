#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "interface.h"

TTF_Font *snowman = NULL;
TTF_Font *snowman_titre = NULL;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;


SDL_Window *creation_de_fenetre() {
    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur : Initialisation SDL > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("IP C-atalogue", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    

    if (window == NULL) {
        SDL_Log("Erreur : Création de la fenêtre > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return window;
}

void creation_rectangle(SDL_Renderer *renderer, int x, int y, int h , int l) {
    SDL_Rect rectangle = {x, y, l, h};

    // Remplit le rectangle avec la couleur noire
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);

    // Dessine le contour du rectangle en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rectangle);

}

void creation_texte(SDL_Renderer *renderer, TTF_Font *snowman, const char *texte, int x, int y) {

    SDL_Surface *surface_texte = TTF_RenderText_Blended(snowman, texte, (SDL_Color){255, 255, 255, 255});
    if (surface_texte == NULL) {
        SDL_Log("Erreur : Rendu du texte > %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture_texte = SDL_CreateTextureFromSurface(renderer, surface_texte);
    if (texture_texte == NULL) {
        SDL_Log("Erreur : Création de la texture du texte > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Rect dest_rect = {x, y, surface_texte->w, surface_texte->h};
    SDL_RenderCopy(renderer, texture_texte, NULL, &dest_rect);

    SDL_FreeSurface(surface_texte);
    SDL_DestroyTexture(texture_texte);
}

void menu(SDL_Window *window, SDL_Renderer *renderer){

    int ev = 20;
    int eh = 175;
    

    if (renderer == NULL) {
        SDL_Log("Erreur : Création du renderer > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
         exit(EXIT_FAILURE);
    }

    snowman = TTF_OpenFont("Snowman.ttf", 35);
    snowman_titre = TTF_OpenFont("Snowman.ttf", 100);


    if (snowman == NULL) {
        SDL_Log("Erreur : Chargement de la police > %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    for (int compteur = 1; compteur <= 2; compteur++) {
        int py = compteur * (150 + ev);

        for (int c = 1; c <= 2; c++) {

            int px = c * (50 + eh);
            if (c == 1 && compteur == 1) {
                
                creation_rectangle(renderer, 50, py,100,300);
                creation_texte(renderer, snowman, "Ajouter une adresse IP", 110, py + 25);
                
            }
            else if (c == 1 && compteur == 2) {
                
                creation_rectangle(renderer, 50, py,100,300);
                creation_texte(renderer, snowman, "Utiliser les adresse dans la base", 70, py + 25);
            }
            else if (c == 2 && compteur == 1) {
                creation_rectangle(renderer, px, py,100,300);
                creation_texte(renderer, snowman, "Selectionner une adresse", px +50 , py + 25);
            } 
            else if (c == 2 && compteur == 2) {
                creation_rectangle(renderer, px, py,100,300);
                creation_texte(renderer, snowman, "Recherche par masque", px + 50, py + 25);
            } 
        }
    }

    
    creation_texte(renderer, snowman_titre, "IP C-atalogue", 250, 25);
    SDL_RenderPresent(renderer);
}


int main(int argc, char const *argv[]) {

    int boutton = 1;
    int b_menu = 1;

    snowman = TTF_OpenFont("Snowman.ttf", 35);
    snowman_titre = TTF_OpenFont("Snowman.ttf", 100);
    window = creation_de_fenetre();
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    menu(window, renderer);

    while (boutton) {

        

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            
            if (event.type == SDL_QUIT) {
                boutton = 0;
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN) {

                if (event.button.button == SDL_BUTTON_LEFT) {

                    if(b_menu){

                        //printf("Clic gauche à la position : (%d, %d)\n", event.button.x, event.button.y);
                        if((event.button.x >= 48 && event.button.x <= 346) && (event.button.y >= 168 && event.button.y <= 265)){
                            SDL_RenderClear(renderer);
                            b_menu = 0;
                                                        
                        }
                        else if((event.button.x >= 48 && event.button.x <= 346) && (event.button.y >= 339 && event.button.y <= 436)){
                            SDL_RenderClear(renderer);
                            b_menu = 0;
                            
                        }
                        else if((event.button.x >= 450 && event.button.x <= 747) && (event.button.y >= 168 && event.button.y <= 265)){
                            SDL_RenderClear(renderer);
                            b_menu = 0;
                            
                        }
                        else if((event.button.x >= 450 && event.button.x <= 747) && (event.button.y >= 339 && event.button.y <= 436)){
                            SDL_RenderClear(renderer);
                            b_menu = 0;
                            
                        }
     
                    }

                    if( b_menu == 0){

                        creation_rectangle(renderer, 600, 20,50,150);
                        creation_texte(renderer, snowman, "Retour", 650, 25);

                    }
                    

                    if((event.button.x >= 600 && event.button.x <= 747) && (event.button.y >= 20 && event.button.y <= 66) && b_menu == 0){
                            
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            
                            menu(window, renderer); 
                            b_menu = 1;
                    }
                } 

            }   
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
    }

    
    TTF_CloseFont(snowman);
    TTF_CloseFont(snowman_titre);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
