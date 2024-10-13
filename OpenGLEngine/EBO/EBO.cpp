#include "EBO.hpp"

EBO::EBO(GLuint* indices, GLsizeiptr size) {
    //Genera el EBO
	glGenBuffers(1, &ID);
	//Selecciona el objeto del buffer con el que trabajaremos
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	//Ponemos los datos en el array (traslada de la CPU al la GPU)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}


