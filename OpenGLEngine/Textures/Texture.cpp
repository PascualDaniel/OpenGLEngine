#include "Texture.hpp"


void Texture::CreateTexture(GLuint& texture, const char* filePath) {
    // Importar la imagen
    int widthImg, heightImg, numColCh;
    unsigned char* bytes = stbi_load(filePath, &widthImg, &heightImg, &numColCh, 0);
    if (!bytes) {
        std::cerr << "Error: No se pudo cargar la imagen en " << filePath << std::endl;
        return;
    }

    // Crear la textura
    glGenTextures(1, &texture);
    // Enlazar la textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Tipo de interpolación 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Tipo de repetición
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Puede dar error GL_RGB jpegs / GL_RGBA pngs / rgb jpg
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    // Añadir mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::DeleteTexture(GLuint& texture) {
    glDeleteTextures(1, &texture);
}

