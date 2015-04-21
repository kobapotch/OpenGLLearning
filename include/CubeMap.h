#ifndef _KOBA_CUBEMAP_INCLUDED
#define _KOBA_CUBEMAP_INCLUDED

#include <vector>

class CubeMap{ 
    private:
        int ubo;
        int unit;

        unsigned int dataPos;
    unsigned int width,height;
    unsigned int imageSize;

        // posx,negx ... negzの順に画像データを格納する
        std::vector<unsigned char> data[6];

        unsigned int textureID;

        static const int unitMacro[];

    public:
        void loadCubeTex(const char* posx,const char* negx, 
                const char* posy,const char* negy,
                const char* posz, const char* negz);
        void makeCubeMap(unsigned int shaderID,int unit);
        void setCubeMap();

};

#endif
