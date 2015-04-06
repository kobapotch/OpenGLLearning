
#ifndef _KOBA_BMPLOADER_INCLUDED
#define _KOBA_BMPLOADER_INCLUDED

#include <vector>

class BMPLoader{

    int ID;
    int unit;
    static const int unitMacro[];

    unsigned int dataPos;
    unsigned int width,height;
    unsigned int imageSize;
    std::vector<unsigned char> data;

    GLuint textureID;

public:
    void loadBMP(const char* filename);
    GLuint makeTexture(int unit);
    GLuint getTextureID();
    void setTexture();

};

#endif
