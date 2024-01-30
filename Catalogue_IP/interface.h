
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

void creation_rectangle(SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rectangle = {x, y, 300, 100};

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
