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


struct Transform {
    float x, y, z;

};




//Abstraccion del mesh
struct Mesh3D {

    //Vertex array object VAO
    GLuint mVertexArrayObject = 0;

    //Vertex array object VBO
    GLuint mVertexBufferObject = 0;

    // Index Buffer Object IBO
    GLuint mIndexBufferObject = 0;
    //graphic pipeline usado con el mesh
    GLuint mPipeline = 0;

    Transform mTransform;

    float m_uOffset = -2.0f;
    float m_uRotate = 0.0f;
    float m_uScale = 0.5f;

};

/**
* Pone la geometria durante la vertexspecification por cada mesh
* es un constucctor
*/
void MeshCreate(Mesh3D* mesh) {
   

    //Lives on the CPU, the reiangle
    const std::vector<GLfloat> vertexData
    { //     COORDINATES     /        COLORS      /   TexCoord  //
   -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
   -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
    0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
    0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
    };

    //Settings things on the GPU
    glGenVertexArrays(1, &mesh->mVertexArrayObject);
    glBindVertexArray(mesh->mVertexArrayObject);

    //Genera el VBO
    glGenBuffers(1, &mesh->mVertexBufferObject);
    //Selecciona el objeto del buffer con el que trabajaremos
    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBufferObject);
    //Ponemos los datos en el array (traslada de la CPU al la GPU)
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);


    const std::vector<GLuint> indexBufferData
    {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    //Crear el Index Buffer Object (IBO i.e. EBO)
    glGenBuffers(1, &mesh->mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexBufferObject);
    // Poblar el Index Bufer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);


    //Dice a openGL como se usa la informacion
    glEnableVertexAttribArray(0);
    //Por cada atributo especifica como se mueve por los datos
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)0);

    //Linkearlos al VAO
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));



    //Linkearlos al VAO
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 6));

    glBindVertexArray(0);
    //Descativar atributos
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}
void MeshDelete(Mesh3D* mesh) {
    glDeleteBuffers(1, &mesh->mVertexArrayObject);
    glDeleteVertexArrays(1, &mesh->mVertexArrayObject);
}
void MeshUpdate(Mesh3D* mesh) {
    

    glUseProgram(mesh->mPipeline);

    mesh->m_uRotate += 0.5f;
    //std::cout << "g_uRotate: " << g_uRotate << std::endl;

    //Model Transformation 
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(mesh->mTransform.x, mesh->mTransform.y, mesh->mTransform.z));
    model = glm::rotate(model, glm::radians(mesh->m_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(mesh->m_uScale, mesh->m_uScale, mesh->m_uScale));

    //Devuelve la localizacion de la matriz
    GLint u_ModelMatrixLocation = glGetUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, false, &model[0][0]);
    }
    else {
        std::cout << "Could not find u_ModelMatrix " << std::endl;
        exit(EXIT_FAILURE);
    }

    // Camara
    glm::mat4 view = gApp.mCamera.GetViewMatrix();
    GLint u_ViewLocation = glGetUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_ViewMatrix");
    if (u_ViewLocation >= 0) {
        glUniformMatrix4fv(u_ViewLocation, 1, false, &view[0][0]);
    }
    else {
        std::cout << "Could not find u_ViewMatrix " << std::endl;
        exit(EXIT_FAILURE);
    }

    //Projection Transformation 
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
        (float)gApp.mScreenWidth / (float)gApp.mScreenHeight,
        0.1f,
        10.0f);

    //Devuelve la localizacion de la perspectiva
    GLint u_ProjectionLocation = glGetUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, false, &perspective[0][0]);
    }
    else {
        std::cout << "Could not find u_Projection " << std::endl;
        exit(EXIT_FAILURE);
    }

    /*
    //Textura
    GLint u_TextureLocation0 = glGetUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_Tex0");
    if (u_TextureLocation0 >= 0) {
        glUniform1i(u_TextureLocation0, 0);
    }
    else {
        std::cout << "Could not find u_Tex0 " << std::endl;
        exit(EXIT_FAILURE);
    }
    */

}
void MeshSetPipeline(Mesh3D* mesh, GLuint pipeline) {
    //Pipeline setup
    mesh->mPipeline = pipeline;
}

/**
* Metodo que dibuja un objeto
* Falta txturas
*/
void MeshDraw(Mesh3D* mesh ) {
    //GLuint texture = 0;
    //CreateTexture(texture);


    if (mesh == nullptr) {
        return;
    }

    //Setup pipeline que vamos a utilizar
    glUseProgram(mesh->mPipeline);

    //Activa atributos
    glBindVertexArray(mesh->mVertexArrayObject);

    //Selecciona el objeto a activar
        //glBindTexture(GL_TEXTURE_2D, texture);
    //Renderiza los datos
    GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    //DeleteTexture(texture);


    //Para de usar el pipeline (Necesario si solo hay un pipeline)
    glUseProgram(0);
}
//=================================================Globals=================================================
//=================================================Globals=================================================

Mesh3D gMesh1;
Mesh3D gMesh2;


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

void CreateTexture(GLuint& texture) {
    //importar la imagen
    int widthImg, heightImg, numColCh;
    unsigned char* bytes = stbi_load("C:/Users/Daniel/Desktop/VFX/GraphicsEngine/OpenGLEngine/textures/container.jpg", &widthImg, &heightImg, &numColCh, 0);
    //crea la textura
    glGenTextures(1, &texture);
    //la enlaza
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    //tipo de interpolacion 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //tipo de repeticion
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Puede dar error GL_RGB jpegs / GL_RGBA pngs / rgb jpg
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    //añade mipmaps, lo de repetir con distancia mas pequiño
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void DeleteTexture(GLuint& texture) {
    glDeleteTextures(1, &texture);
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
    if (state[SDL_SCANCODE_UP]) {
        gMesh1.m_uOffset += 0.01f;
        std::cout << "g_uOffset: " << gMesh1.m_uOffset << std::endl;

    }if (state[SDL_SCANCODE_DOWN]) {
        gMesh1.m_uOffset -= 0.01f;
        std::cout << "g_uOffset: " << gMesh1.m_uOffset << std::endl;
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

    while (!gApp.mQuit) {
        Input();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glViewport(0, 0, gApp.mScreenWidth, gApp.mScreenHeight);
        glClearColor(1.f, 1.f, 0.1f, 1.f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        //Todo lo que OpenGl necesita antes de dibujar
        MeshUpdate(&gMesh1);      
        // El dibujo
        MeshDraw(&gMesh1);

        MeshUpdate(&gMesh2);
        MeshDraw(&gMesh2);
        //Actualiza la pantalla
        SDL_GL_SwapWindow(gApp.mGraphicsApplicationWindow);
    }
}

void CleanUp() {
    SDL_DestroyWindow(gApp.mGraphicsApplicationWindow);
    gApp.mGraphicsApplicationWindow = nullptr;

    MeshDelete(&gMesh1);
    MeshDelete(&gMesh2);

 

    //Delete graphisc pipeline
    glDeleteProgram(gApp.mGraphicsPipelineShaderProgram);


    SDL_Quit();
}


int main(int argc, char* args[])
{
    //1. Inicializar el programa de graficos
    InitialiceProgram(&gApp);
    //2. Inicializar la jometria
    MeshCreate(&gMesh1);
    gMesh1.mTransform.x = 0.0f;
    gMesh1.mTransform.y = 0.0f;
    gMesh1.mTransform.z = -2.0f;

    MeshCreate(&gMesh2);
    gMesh2.mTransform.x = 2.0f;
    gMesh2.mTransform.y = 0.0f;
    gMesh2.mTransform.z = -2.0f;


    //3. Crear la graphics pipeline
    // Vertex y fragment shader como minimo
    CreateGraphicsPipeline();
    //3.5 Por cada mesh pone la pipeline
    MeshSetPipeline(&gMesh1, gApp.mGraphicsPipelineShaderProgram);

    MeshSetPipeline(&gMesh2, gApp.mGraphicsPipelineShaderProgram);
    //4. La funcionalidad de la aplicacion (Dibujo)
    MainLoop();
    //5. Limpieza de funciones
    CleanUp();

    std::cout << "Fin!\n";
    return 0;

}
