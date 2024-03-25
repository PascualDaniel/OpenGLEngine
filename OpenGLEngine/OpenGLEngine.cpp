#include <iostream>
#include <SDL.h>
#include <glad/glad.h>
#include <vector>

//Globals
int gScreenHeight = 480;
int gScreenWidth = 640;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

bool gQuit = false; //Si true, cierra la app

//Vertex array object VAO
GLuint gVertexArrayObject = 0;

//Vertex array object VBO
GLuint gVertexBufferObject = 0;





void GetOpenGLVersionInfo(){
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Lenguage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


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

    //Crear Ventana
    gGraphicsApplicationWindow =  SDL_CreateWindow("OpenGl Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        gScreenHeight, gScreenWidth,
        SDL_WINDOW_OPENGL);
    if (gGraphicsApplicationWindow == nullptr) {
        std::cout << "Error: SDL_Window was not able to create" << std::endl;
        exit(1);
    }
    //Crear contexto
    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    if (gOpenGLContext == nullptr) {
        std::cout << "Error: SDL_GLContex was not available" << std::endl;
        exit(1);
    }

    //initialize GLAD
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Error: GLAD was not initialized" << std::endl;
        exit(1);
    }
    GetOpenGLVersionInfo();

}

void VertexSpecification() {
    //Lives on the CPU
    const std::vector<GLfloat> vertexPosition{
        // x    y     z
        -0.8f, -0.8f, 0.0f, //Vertex 1
        0.8f, -0.8f, 0.0f,  //Vertex 2
        0.0f, -0.8f, 0.0f   //Vertex 3
    };
    //Settings things on the GPU
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    //Start generation VBO
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    

}

void CreateGraphicsPipeline() {

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

    VertexSpecification();

    CreateGraphicsPipeline();

    MainLoop();

    CleanUp();



    std::cout << "Hola!\n";
    return 0;

}
