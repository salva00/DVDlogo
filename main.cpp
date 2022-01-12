#include <iostream>
#include <SDL2/SDL.h>

bool running, fullscreen;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Rect dstrect;
SDL_DisplayMode dm;
SDL_Surface* image;

int r, g , b = 255;

int dirX, dirY, deltaS;

void update(){
    if(fullscreen) SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
    if(!fullscreen) SDL_SetWindowFullscreen(window,0);
}

void poll(){
    SDL_Event e;
    while (SDL_PollEvent(&e)) if(e.type== SDL_QUIT) running=false;
}

void randColor(){
    r = (int)random() % 256;
    g = (int)random() % 256;
    b = (int)random() % 256;
}

void draw(){
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Rect rect;
    rect.x = rect.y=0;
    rect.w = dm.w;
    rect.h = dm.h;
    SDL_RenderFillRect(renderer, &rect);


    if ( !image ) {
        SDL_Log("SDL_LoadBMP : failed");
    }
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, image);
    SDL_SetTextureColorMod( texture1, r, g, b);

    if (dstrect.x + deltaS > dm.w - dstrect.w|| dstrect.x + deltaS < 0){
        dirX= -dirX;
        randColor();
    }
    if (dstrect.y + deltaS > dm.h - dstrect.h|| dstrect.y + deltaS < 0){
        dirY= -dirY;
        randColor();
    }

    if (dirX==1) dstrect.x+=deltaS;
    else if(dirX==-1) dstrect.x-=deltaS;
    if (dirY==1) dstrect.y+=deltaS;
    else if(dirY==-1) dstrect.y-=deltaS;

    SDL_RenderCopy(renderer, texture1, NULL,  &dstrect);
    SDL_RenderPresent(renderer);
}

int main() {

    running = true;
    fullscreen = true;
    dstrect = { 5, 5, 240, 240 };
    deltaS = 2;
    dirX = dirY = 1;
    int display_count = 0;
    image = SDL_LoadBMP( "dvd_video.bmp" );
    srand(time(nullptr));

    if(SDL_Init(SDL_INIT_EVERYTHING)<0) std::cout << "Failed at SDL_Init!" << std::endl;

    if ((display_count = SDL_GetNumVideoDisplays()) < 1) {
        SDL_Log("SDL_GetNumVideoDisplays returned: %i", display_count);
        return 1;
    }
    dm  = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
    if(SDL_GetDisplayMode(0,0,&dm)){
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        return 1;
    }

    if(SDL_CreateWindowAndRenderer(dm.w,dm.h,0,&window,&renderer) <0){
        std::cout << "Failed at SDL_CreateWindowAndRenderer" << std::endl;
    }
    SDL_SetWindowTitle(window,"DVDlogo");
    SDL_ShowCursor(1);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
    while(running){
        poll();
        update();
        draw();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}