#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <SDL.h>
#include <glad/glad.h>


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



//=================================================Funcionalidades=================================================

std::string LoadShaderAsString(const std::string& filename) {
    //Resulting shader program as as single string
    std::string result = "";
    std::string line = "";
    std::ifstream myFile(filename.c_str());

    if (myFile.is_open()) {
        while (std::getline(myFile, line)) {
            result += line + '\n';
        }
        myFile.close();
    }

    return result;

}


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

void VertexSpecification() {
    //Lives on the CPU, the reiangle
    const std::vector<GLfloat> vertexData{
        // x    y     z
        -0.8f, -0.8f, 0.0f, //Vertex left
        1.0f, 0.0f, 0.0f,   //Color1
        0.8f, -0.8f, 0.0f,  //Vertex rigth
        0.0f, 1.0f, 0.0f,   //Color2
        0.0f, 0.8f, 0.0f,   //Vertex top
        0.0f, 0.0f, 1.0f    //Color3
    };
   
    //Settings things on the GPU
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray( gVertexArrayObject);

    //Genera el VBO
    glGenBuffers(1, &gVertexBufferObject);
    //Selecciona el objeto del buffer con el que trabajaremos
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    //Ponemos los datos en el array (traslada de la CPU al la GPU)
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
    
    //Dice a openGL como se usa la informacion
    glEnableVertexAttribArray(0);
    //Por cada atributo especifica como se mueve por los datos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    //Linkearlos al VAO
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)( sizeof(GLfloat)*3));

    glBindVertexArray(0);
    //Descativar atributos
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    

}

GLuint CompileShader(GLuint type, const std::string& shadersource) {
    GLuint shaderObject;
    //Crea un shader dependiendo del tipo
    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char* src = shadersource.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    //Devuelve el resultado de la compilación
    int result;
    //Devolver el estatus
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length]; //Se puede usar alloca aqui
        glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

        if (type == GL_VERTEX_SHADER) {
            std::cout << "Error: GL_VERTEX_SHADER compilation failed.\n" << errorMessages << std::endl;
        }
        else if (type == GL_FRAGMENT_SHADER) {
            std::cout << "Error: GL_FRAGMENT_SHADER compilation failed.\n" << errorMessages << std::endl;
        }
        //Reclaim memory
        delete[] errorMessages;

        //Delete broken shaders
        glDeleteShader(shaderObject);

        return 0;
    }

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexshaderssource, const std::string& fragmentshadersource) {
    //Create graphics pipeline
    GLuint programObject = glCreateProgram();

    //Compila los shaders
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshaderssource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    //Validate
    glValidateProgram(programObject);
    //Detach Shaders

    return programObject;

}

void CreateGraphicsPipeline() {
    std::string vertexShaderSource = LoadShaderAsString("../shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("../shaders/frag.glsl");

    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
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
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.1f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw() {
    //Activa atributos
    glBindVertexArray(gVertexArrayObject);
    //Selecciona el objeto a activar
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    //Renderiza lso datos
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //Para de usar el pipeline (Necesario si solo hay un pipeline)
    glUseProgram(0);
}

void MainLoop() {
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



    std::cout << "Sacabo!\n";
    return 0;

}
