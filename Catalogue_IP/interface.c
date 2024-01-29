#include <stdio.h>
#include <SDL.h>

SDL_Window* creation_de_fenetre() {
    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur : Initialisation SDL > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("fenetre de test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    
    if (window == NULL) {
        SDL_Log("Erreur : Création de la fenêtre > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return window;
}

void creation_rectangle(SDL_Renderer* renderer, int x, int y) {

    SDL_Rect rectangle = {x, y, 300, 100};
    
     // Remplit le rectangle avec la couleur noire
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);

    // Dessine le contour du rectangle en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rectangle);

    // Mettre à jour le rendu
    SDL_RenderPresent(renderer);
}

int main(int argc, char const *argv[]) {

    
    SDL_Window* window = creation_de_fenetre();
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
 

    if(renderer == NULL) {

        SDL_Log("Erreur : Création du renderer > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    int ev = 20;
    int eh = 175;

    for(int compteur = 1; compteur <= 2; compteur ++){
        int py = compteur * (150 + ev);

        for(int c = 1; c <= 2; c++){
            int px = c * (50 + eh);
            if( c ==1 ){
                creation_rectangle(renderer,50, py);
            }
            else{
                creation_rectangle(renderer,px, py);
            }
            
        }

    
    }
    

    int boutton = 1;

    while (boutton) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                boutton = 0;
            }
        }
    }
    SDL_Delay(100);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
