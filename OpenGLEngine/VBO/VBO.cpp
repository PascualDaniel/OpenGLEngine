#include "VBO.hpp"

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
    //Genera el VBO
    glGenBuffers(1, &ID);
    //Selecciona el objeto del buffer con el que trabajaremos
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    //Ponemos los datos en el array (traslada de la CPU al la GPU)
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}


