//=================================================Estandar=================================================
//=================================================Estandar=================================================
#include <iostream>
#include <vector>
#include <string>
#include <fstream>


//=================================================Externas=================================================
//=================================================Externas=================================================
#include <SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//=================================================Propias=================================================
//=================================================Propias=================================================
#include "Camera.hpp"

#include "Shader.hpp"


#include "Mesh.hpp"




//=================================================Errores=================================================

static void GLClearAllErrors() {
    while (glGetError() != GL_NO_ERROR) {

    }
}

static bool GLCheckErrorStatus(const char* function, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error
            << "\tLine: " << line
            << "\tFunction: " << function
            << std::endl;
        return true;
    }
    return false;
}
#define GLCheck(x) GLClearAllErrors(); x;  GLCheckErrorStatus(#x,__LINE__);


// Dimensiones de la pantalla
struct App{
    int mScreenHeight = 480;
    int mScreenWidth = 640;
    SDL_Window* mGraphicsApplicationWindow = nullptr;
    SDL_GLContext mOpenGLContext = nullptr;

    //MainLoop Flag
    bool mQuit = false; //Si true, cierra la app

    // Program Object for our shaders (Graphics pipeline)
    GLuint mGraphicsPipelineShaderProgram = 0;

    //Camara global Unica
    Camera mCamera;
};

App gApp;


//=================================================Globals=================================================
//=================================================Globals=================================================


// Define los vértices y los índices para las mallas
std::vector<GLfloat> vertices1 = { //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
    0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
    0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

std::vector<GLuint> indices1 = {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
};

std::vector<GLfloat> vertices2 = { //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
    0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
    0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};;

std::vector<GLuint> indices2 = {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
};


//=================================================Funcionalidades=================================================


void GetOpenGLVersionInfo(){
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Lenguage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}



void InitialiceProgram(App* app) {
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
    app->mGraphicsApplicationWindow =  SDL_CreateWindow("OpenGl Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        app->mScreenWidth, app->mScreenHeight,
        SDL_WINDOW_OPENGL);
    if (app->mGraphicsApplicationWindow == nullptr) {
        std::cout << "Error: SDL_Window was not able to create" << std::endl;
        exit(1);
    }
    //Crear contexto
    app->mOpenGLContext = SDL_GL_CreateContext(app->mGraphicsApplicationWindow);
    if (app->mOpenGLContext == nullptr) {
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




void CreateGraphicsPipeline() {
    Shader gShader = Shader("path/to/vertexShader.glsl", "path/to/fragmentShader.glsl");

    gApp.mGraphicsPipelineShaderProgram = gShader.getGraphicsPipeline();
}

void Input() {
    static int mouseX = gApp.mScreenWidth  / 2;
    static int mouseY = gApp.mScreenHeight / 2;

    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "Bye :3" << std::endl;
            gApp.mQuit = true;
        }
        else if (e.type == SDL_MOUSEMOTION) {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gApp.mCamera.MouseLook(mouseX, mouseY);
        }
    }   
    const Uint8* state = SDL_GetKeyboardState(NULL);
    float speed = 0.1f;
    if (state[SDL_SCANCODE_W]) {
        gApp.mCamera.MoveForward(speed);
    }
    if (state[SDL_SCANCODE_S]) {
        gApp.mCamera.MoveBackward(speed);
    }
    if (state[SDL_SCANCODE_A]) {
        gApp.mCamera.MoveLeft(speed);
    }
    if (state[SDL_SCANCODE_D]) {
        gApp.mCamera.MoveRight(speed);
    }
    if (state[SDL_SCANCODE_SPACE]) {
        gApp.mCamera.MoveUp(speed);
    }
    if (state[SDL_SCANCODE_LCTRL]) {
        gApp.mCamera.MoveDown(speed);
    }
    if (state[SDL_SCANCODE_ESCAPE]) {
        std::cout << "Bye :3" << std::endl;
        gApp.mQuit = true;
    }


}


void MainLoop() {
    //Encerrrar al raton dentro de la pantalla
    SDL_WarpMouseInWindow(gApp.mGraphicsApplicationWindow, gApp.mScreenWidth / 2, gApp.mScreenHeight / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);


    //2. Inicializar la jometria
    //gMesh1.Create();
    Mesh gMesh1(vertices1, indices1);
    gMesh1.Translate(0.0f, 0.0f, -2.0f);
    gMesh1.Scale(1.0f, 1.0f, 1.0f);
    gMesh1.SetPipeline(gApp.mGraphicsPipelineShaderProgram);
    //gMesh2.Create();
    Mesh gMesh2(vertices2, indices2);
    gMesh2.Translate(0.0f, 0.0f, -4.0f);
    gMesh2.Scale(1.0f, 2.0f, 1.0f);
    gMesh2.SetPipeline(gApp.mGraphicsPipelineShaderProgram);
    while (!gApp.mQuit) {
        Input();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glViewport(0, 0, gApp.mScreenWidth, gApp.mScreenHeight);
        glClearColor(1.f, 1.f, 0.1f, 1.f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        static float rotate = 0.5f;       
        gMesh1.Rotate( rotate, glm::vec3(0.0f, 1.0f, 0.0f));

    
        // El dibujo
        gMesh1.Draw(gApp.mCamera);
        gMesh2.Draw(gApp.mCamera);
        //Actualiza la pantalla
        SDL_GL_SwapWindow(gApp.mGraphicsApplicationWindow);
    }
    gMesh1.Delete();
    gMesh2.Delete();
}

void CleanUp() {
    SDL_DestroyWindow(gApp.mGraphicsApplicationWindow);
    gApp.mGraphicsApplicationWindow = nullptr;

    //Delete graphisc pipeline
    glDeleteProgram(gApp.mGraphicsPipelineShaderProgram);


    SDL_Quit();
}




int main(int argc, char* args[])


{
    //1. Inicializar el programa de graficos
    InitialiceProgram(&gApp);

    //Setup Camera
    gApp.mCamera.SetProjectionMatrix(glm::radians(45.0f),(float)gApp.mScreenWidth / (float)gApp.mScreenHeight,0.1f,10.0f);

    //3. Crear la graphics pipeline
    // Vertex y fragment shader como minimo
    CreateGraphicsPipeline();
    //4. La funcionalidad de la aplicacion (Dibujo)
    MainLoop();
    //5. Limpieza de funciones
    CleanUp();

    std::cout << "Fin!\n";
    return 0;

}
