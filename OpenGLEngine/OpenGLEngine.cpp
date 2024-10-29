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



// Vértices de una pirámide
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};


//Light source cube
// Vértices de un cubo
Vertex lightVertices[] =
{ //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};






const char* texturePath = "C:/Users/Daniel/Desktop/VFX/GraphicsEngine/OpenGLEngine/textures/container.jpg";
const char* texturePlankPath = "C:/Users/Daniel/Desktop/VFX/GraphicsEngine/OpenGLEngine/textures/planks.png";
const char* texturePlankSpecPath = "C:/Users/Daniel/Desktop/VFX/GraphicsEngine/OpenGLEngine/textures/planksSpec.png";
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

//prueba 3
void MainLoop() {
    //Encerrrar al raton dentro de la pantalla
    SDL_WarpMouseInWindow(gApp.mGraphicsApplicationWindow, gApp.mScreenWidth / 2, gApp.mScreenHeight / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);


	//Texture const (const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);
    Texture gTexture1[]
    {
        Texture(texturePath, "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE)
	};
    Texture gTexture2[]
    {
        Texture(texturePath, "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE)
    };
    Texture textures[]
    {
        Texture(texturePlankPath, "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
      
        Texture(texturePlankSpecPath, "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    std::vector <Texture> tex2(gTexture1, gTexture1 + sizeof(gTexture1) / sizeof(Texture));
    std::vector <Texture> tex1(gTexture2, gTexture2 + sizeof(gTexture2) / sizeof(Texture));

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
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));

    Mesh gMesh2(verts, ind, tex);
    gMesh2.Translate(0.0f, 0.0f, 0.0f);
    gMesh2.Scale(1.0f, 1.0f, 1.0f);
    gMesh2.SetPipeline(gApp.mGraphicsPipelineShaders[0].getGraphicsPipeline());

    std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

    Mesh lightCube(lightVerts, lightInd, tex2);
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
