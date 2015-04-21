
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "CubeMap.h"
#include "Logger.h"

using namespace std;

const int CubeMap::unitMacro[] = {
    GL_TEXTURE0,GL_TEXTURE1,GL_TEXTURE2,GL_TEXTURE3,GL_TEXTURE4};

void CubeMap::loadCubeTex(const char* posx,const char* negx,
        const char* posy,const char* negy,
        const char* posz, const char* negz){

    const char* filenames[] = { posx,negx,posy,negy,posz,negz};

    for(int i=0;i<6;i++){
        char header[54];

        ifstream ifs(filenames[i],ios::binary);
        if(!ifs) {
            cout << "can't open file : " << filenames[i] << endl;
            return;
        }

        // ヘッダの読み込み
        ifs.read(header,54);
        if(ifs.bad() || header[0] != 'B' || header[1] != 'M'){
            cout << filenames[i] << " is not BMP" << endl;
            return;
        }
        for(int i=0;i<54;i++){
            cout << header[i] ;
        }cout << endl;
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);
        cout << dataPos << " : " << imageSize << " : " << width << " : " << height << endl; 

        char buf;
        while(ifs.get(buf)){
            data[i].push_back(buf);
        }
        cout << "datasize" << data[i].size() << " in " << width * height* 3 <<  endl;

    }

}

void CubeMap::makeCubeMap(unsigned int shaderID,int unit){

    GLuint targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    Logger::Log("Load CubeMap");

    this->unit = unit;

    glActiveTexture(unitMacro[unit]);


    glGenTextures(1,&textureID);

    glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);

    for(int i=0;i<6;i++){

        glTexImage2D(targets[i],0,GL_RGB,width,height,0,GL_BGR,GL_UNSIGNED_BYTE,&data[i][0]);
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    ubo = glGetUniformLocation(shaderID,"cubeMap");

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    Logger::Log("ubo is " + std::to_string(ubo));
}

void CubeMap::setCubeMap(){

    glActiveTexture(unitMacro[unit]);
    glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);
    glUniform1i(ubo,unit);
}



