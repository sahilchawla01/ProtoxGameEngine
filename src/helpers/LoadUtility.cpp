#include <helpers/LoadUtility.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

int LoadUtility::loadTexture(char const* path)
{
    unsigned int textureID;
    //Generates a unique number, that refers to the current texture
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    //Loads the image data 
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        //Bind a named texture to a texture target (GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, textureID);
        //Generate the texture by using the image data, and bind it to the currently bound texture object
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        //Generate mipmaps and add it to the object 
        glGenerateMipmap(GL_TEXTURE_2D);

        //Set wrapping and min-max filters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }

    //Free the data
    stbi_image_free(data);

    return textureID;
}
