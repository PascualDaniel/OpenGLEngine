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

#include "Texture.hpp"

#include "VAO.hpp"
#include "VBO.hpp"

//=================================================Globals=================================================
//=================================================Globals=================================================

// Dimensiones de la pantalla
int gScreenHeight = 480;
int gScreenWidth = 640;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

//MainLoop Flag
bool gQuit = false; //Si true, cierra la app

// Program Object for our shaders (Graphics pipeline)
GLuint gGraphicsPipelineShaderProgram = 0;

//Vertex array object VAO
GLuint gVertexArrayObject = 0;

//Vertex array object VBO
GLuint gVertexBufferObject = 0;

// Index Buffer Object IBO
GLuint gIndexBufferObject = 0;


float g_uOffset = -2.0f;
float g_uRotate = 0.0f;
float g_uScale  = 0.5f;

//Camara global Unica
Camera gCamera;



//=================================================Errores=================================================

static void GLClearAllErrors() {
    while (glGetError() != GL_NO_ERROR) {

    }
}

static bool GLCheckErrorStatus(const char* function, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error
            <<"\tLine: " << line
            <<"\tFunction: " << function
            << std::endl;
        return true;
    }
    return false;
}
#define GLCheck(x) GLClearAllErrors(); x;  GLCheckErrorStatus(#x,__LINE__);
//=================================================Funcionalidades=================================================


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
        gScreenWidth, gScreenHeight,
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

void VertexSpecification() {
    //Lives on the CPU, the reiangle
    const std::vector<GLfloat> vertexData
     { //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

    //Settings things on the GPU
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    //Genera el VBO
    glGenBuffers(1, &gVertexBufferObject);
    //Selecciona el objeto del buffer con el que trabajaremos
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    //Ponemos los datos en el array (traslada de la CPU al la GPU)
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);


    const std::vector<GLuint> indexBufferData
    {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    //Crear el Index Buffer Object (IBO i.e. EBO)
    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    // Poblar el Index Bufer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);


    //Dice a openGL como se usa la informacion
    glEnableVertexAttribArray(0);
    //Por cada atributo especifica como se mueve por los datos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

    //Linkearlos al VAO
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)( sizeof(GLfloat)*3));

    

    //Linkearlos al VAO
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 6));

    glBindVertexArray(0);
    //Descativar atributos
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    

}


void CreateGraphicsPipeline() {
    Shader gShader = Shader("path/to/vertexShader.glsl", "path/to/fragmentShader.glsl");

    gGraphicsPipelineShaderProgram = gShader.getGraphicsPipeline();
}

void Input() {
    static int mouseX = gScreenWidth  / 2;
    static int mouseY = gScreenHeight / 2; 

    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "Bye :3" << std::endl;
            gQuit = true;
        }
        else if (e.type == SDL_MOUSEMOTION) {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gCamera.MouseLook(mouseX, mouseY);
        }
    }   
    const Uint8* state = SDL_GetKeyboardState(NULL);
    float speed = 0.1f;
    if (state[SDL_SCANCODE_W]) {
        gCamera.MoveForward(speed);
    }
    if (state[SDL_SCANCODE_S]) {
        gCamera.MoveBackward(speed);
    }
    if (state[SDL_SCANCODE_A]) {
        gCamera.MoveLeft(speed);    
    }
    if (state[SDL_SCANCODE_D]) {
        gCamera.MoveRight(speed);    
    }
    if (state[SDL_SCANCODE_SPACE]) {
        gCamera.MoveUp(speed);
    }
    if (state[SDL_SCANCODE_LCTRL]) {
        gCamera.MoveDown(speed);
    }
    if (state[SDL_SCANCODE_UP]) {
        g_uOffset += 0.01f;
        std::cout << "g_uOffset: " << g_uOffset << std::endl;

    }if (state[SDL_SCANCODE_DOWN]) {
        g_uOffset -= 0.01f;
        std::cout << "g_uOffset: " << g_uOffset << std::endl;
    }
    if (state[SDL_SCANCODE_ESCAPE]) {
        std::cout << "Bye :3" << std::endl;
        gQuit = true;
    }


}
void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.1f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glUseProgram(gGraphicsPipelineShaderProgram);
   
    g_uRotate += 0.5f;
    //std::cout << "g_uRotate: " << g_uRotate << std::endl;

    //Model Transformation 
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g_uOffset));
    model           = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model           = glm::scale(model,  glm::vec3(g_uScale, g_uScale, g_uScale));

    //Devuelve la localizacion de la matriz
    GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {      
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else {
        std::cout << "Could not find u_ModelMatrix "  << std::endl;
        exit(EXIT_FAILURE);
    }

    // Camara
    glm::mat4 view = gCamera.GetViewMatrix();
    GLint u_ViewLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ViewMatrix");
    if (u_ViewLocation >= 0) {
        glUniformMatrix4fv(u_ViewLocation, 1, GL_FALSE, &view[0][0]);
    }
    else {
        std::cout << "Could not find u_ViewMatrix " << std::endl;
        exit(EXIT_FAILURE);
    }

    //Projection Transformation 
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                            (float)gScreenWidth/(float)gScreenHeight,
                                            0.1f,
                                            10.0f);

    //Devuelve la localizacion de la perspectiva
    GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
    }
    else {
        std::cout << "Could not find u_Projection " << std::endl;
        exit(EXIT_FAILURE);
    }


    //Textura
    GLint u_TextureLocation0 = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Tex0");
    if (u_TextureLocation0 >= 0) {
        glUniform1i(u_TextureLocation0, 0);
    }
    else {
        std::cout << "Could not find u_Tex0 " << std::endl;
        exit(EXIT_FAILURE);
    }

}


void Draw() {
    GLuint texture = 0;
    CreateTexture(texture);
    

    //Activa atributos
    glBindVertexArray(gVertexArrayObject);

    //Selecciona el objeto a activar
    //glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBindTexture(GL_TEXTURE_2D, texture);
    //Renderiza lso datos
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    GLCheck(glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0));


    DeleteTexture(texture);
    //Para de usar el pipeline (Necesario si solo hay un pipeline)
    glUseProgram(0);
}

void MainLoop() {
    //Encerrrar al raton dentro de la pantalla
    SDL_WarpMouseInWindow(gGraphicsApplicationWindow, gScreenWidth / 2, gScreenHeight / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!gQuit) {    
        Input();
        //Todo lo que OpenGl necesita antes de dibujar
        PreDraw();
        // El dibujo
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
    //1. Inicializar el programa de graficos
    InitialiceProgram();
    //2. Inicializar la jometria
    VertexSpecification();
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
