
#include <iostream>
#include <SDL.h>

//Globals
int gScreenHeight = 480;
int gScreenWidth = 640;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

bool gQuit = false; //Si true, cierra la app


void InitialiceProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize video subsitem" << std::endl;
        exit(1);
    }
    //Veriones 4.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //Compatibilidad
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //Precision de profundidad y Overlap
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    gGraphicsApplicationWindow =  SDL_CreateWindow("OpenGl Window",
        0, 0,
        gScreenHeight, gScreenWidth,
        SDL_WINDOW_OPENGL);
    if (gGraphicsApplicationWindow == nullptr) {
        std::cout << "Error: SDL_Window was not able to create" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    if (gOpenGLContext == nullptr) {
        std::cout << "Error: SDL_GLContex was not available" << std::endl;
        exit(0);
    }

}

void Input() {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "Bye :3" << std::endl;
            gQuit = true;
        }
    }
}
void PreDraw() {

}

void Draw() {

}

void MainLoop() {
    while (!gQuit) {
        Input();

        PreDraw();
        Draw();

        //Actualiza la pantalla
        SDL_GL_SwapWindow(gGraphicsApplicationWindow);

    }

}

void CleanUp() {
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();

}


int main(int argc, char* args[])
{
    InitialiceProgram();

    MainLoop();

    CleanUp();



    std::cout << "Hola!\n";
    return 0;

}
