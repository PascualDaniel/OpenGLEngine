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


//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

//=================================================Propias=================================================
//=================================================Propias=================================================
#include "Camera.hpp"

#include "Shader.hpp"


#include "Mesh.hpp"
#include "Texture.hpp"
#include <glm/gtc/type_ptr.hpp>



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


void CheckShaderCompilation(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

void CheckProgramLinking(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}


// Dimensiones de la pantalla
struct App{
    int mScreenHeight = 480;
    int mScreenWidth = 640;
    SDL_Window* mGraphicsApplicationWindow = nullptr;
    SDL_GLContext mOpenGLContext = nullptr;

    //MainLoop Flag
    bool mQuit = false; //Si true, cierra la app

    // Program Object for our shaders (Graphics pipeline)
    std::vector<Shader> mGraphicsPipelineShaders;


    //Camara global Unica
    Camera mCamera;
};

App gApp;


//=================================================Globals=================================================
//=================================================Globals=================================================


// Define los vértices y los índices para las mallas
std::vector<GLfloat> vertices1 = { //     COORDINATES     /        COLORS      /   TexCoord  /       NORMALS      //
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f,    0.0f, 0.0f, 1.0f, // Lower left corner
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f,    0.0f, 0.0f, 1.0f, // Upper left corner
    0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f,    0.0f, 0.0f, 1.0f, // Upper right corner
    0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f,    0.0f, 0.0f, 1.0f  // Lower right corner
};

std::vector<GLuint> indices1 = {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
};


//Vertices 2 piramide


// Vértices de una pirámide
std::vector<GLfloat> vertices2 = { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

std::vector<GLuint> indices2 = {
    0, 1, 2, // Bottom side
    0, 2, 3, // Bottom side
    4, 6, 5, // Left side
    7, 9, 8, // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14 // Facing side
};

//Light source cube
// Vértices de un cubo
std::vector<GLfloat> verticesCube = {
    // Coordenadas        // Colores         // Coordenadas de textura  // Normales
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  -1.0f, -1.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   1.0f, -1.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,   1.0f,  1.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  -1.0f,  1.0f, -1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  -1.0f, -1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   1.0f, -1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,   1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  -1.0f,  1.0f,  1.0f
};

std::vector<GLuint> indicesCube = {
    0, 1, 2, 2, 3, 0, // Cara trasera
    4, 5, 6, 6, 7, 4, // Cara delantera
    0, 1, 5, 5, 4, 0, // Cara inferior
    2, 3, 7, 7, 6, 2, // Cara superior
    0, 3, 7, 7, 4, 0, // Cara izquierda
    1, 2, 6, 6, 5, 1  // Cara derecha
};






const char* texturePath = "C:/Users/Daniel/Desktop/VFX/GraphicsEngine/OpenGLEngine/textures/container.jpg";
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


	//Texture const (const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);
    Texture gTexture1(texturePath, "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture gTexture2(texturePath, "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE);




    //3. Crear la graphics pipeline
    Shader gShader("../shaders/vert.glsl", "../shaders/frag.glsl");
    CheckShaderCompilation(gShader.ID, "VERTEX");
    CheckShaderCompilation(gShader.ID, "FRAGMENT");
    CheckProgramLinking(gShader.ID);
    gApp.mGraphicsPipelineShaders.push_back(gShader);

    Shader lightShader("../shaders/lightvert.glsl", "../shaders/lightfrag.glsl");
    CheckShaderCompilation(lightShader.ID, "VERTEX");
    CheckShaderCompilation(lightShader.ID, "FRAGMENT");
    CheckProgramLinking(lightShader.ID);
    gApp.mGraphicsPipelineShaders.push_back(lightShader);
	

    //2. Inicializar la jometria
   // Mesh gMesh1(vertices1, indices1, gTexture1);
    //gMesh1.Translate(0.0f, 0.0f, 0.0f);
    //gMesh1.Scale(1.0f, 1.0f, 1.0f);
    //gMesh1.SetPipeline(gApp.mGraphicsPipelineShaders[0].getGraphicsPipeline());

    Mesh gMesh2(vertices2, indices2, gTexture2);
    gMesh2.Translate(0.0f, 0.0f, 0.0f);
    gMesh2.Scale(1.0f, 1.0f, 1.0f);
    gMesh2.SetPipeline(gApp.mGraphicsPipelineShaders[0].getGraphicsPipeline());

    Mesh lightCube(verticesCube, indicesCube, gTexture1);
	lightCube.Translate(0.5f, 0.5f, 0.5f);
	lightCube.Scale(0.2f, 0.2f, 0.2f);
	lightCube.SetPipeline(gApp.mGraphicsPipelineShaders[1].getGraphicsPipeline());



    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

	
    


    gApp.mCamera.ProjectionMatrix(gApp.mGraphicsPipelineShaders[0], "u_ViewMatrix");
    gApp.mCamera.ProjectionMatrix(gApp.mGraphicsPipelineShaders[1], "u_ViewMatrix");


    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    while (!gApp.mQuit) {
        Input();

        

        glViewport(0, 0, gApp.mScreenWidth, gApp.mScreenHeight);
        glClearColor(1.f, 1.f, 0.1f, 1.f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        static float rotate = 0.5f;       
      //  gMesh1.Rotate( rotate, glm::vec3(0.0f, 1.0f, 0.0f));

    
        // El dibujo
        //gMesh1.Draw(gApp.mCamera);
        gMesh2.Draw(gApp.mCamera);
		lightCube.Draw(gApp.mCamera);

        gApp.mGraphicsPipelineShaders[1].use();
        glUniformMatrix4fv(glGetUniformLocation(gApp.mGraphicsPipelineShaders[1].ID, "u_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniform4f(glGetUniformLocation(gApp.mGraphicsPipelineShaders[1].ID, "u_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        gApp.mGraphicsPipelineShaders[0].use();
        glUniformMatrix4fv(glGetUniformLocation(gApp.mGraphicsPipelineShaders[0].ID, "u_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
        glUniform4f(glGetUniformLocation(gApp.mGraphicsPipelineShaders[0].ID, "u_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(gApp.mGraphicsPipelineShaders[0].ID, "u_lightPos"), lightPos.x, lightPos.y, lightPos.z);


        //Actualiza la pantalla
        SDL_GL_SwapWindow(gApp.mGraphicsApplicationWindow);
    }
    //gMesh1.Delete();
    gMesh2.Delete();
	lightCube.Delete();
}

void CleanUp() {
    SDL_DestroyWindow(gApp.mGraphicsApplicationWindow);
    gApp.mGraphicsApplicationWindow = nullptr;

    //Delete graphisc pipeline
    for (auto& shader : gApp.mGraphicsPipelineShaders) {
        glDeleteProgram(shader.getGraphicsPipeline());
    }


    SDL_Quit();
}




int main(int argc, char* args[])


{
    //1. Inicializar el programa de graficos
    InitialiceProgram(&gApp);

    //Setup Camera
    gApp.mCamera.SetProjectionMatrix(glm::radians(45.0f),(float)gApp.mScreenWidth / (float)gApp.mScreenHeight,0.1f,10.0f);

    

	//3. Crear el pipeline de graficos
    // Vertex y fragment shader como minimo
    //CreateGraphicsPipeline();
    
    //4. La funcionalidad de la aplicacion (Dibujo)
    MainLoop();

   
    //5. Limpieza de funciones
    CleanUp();

    std::cout << "Fin!\n";
    return 0;

}
