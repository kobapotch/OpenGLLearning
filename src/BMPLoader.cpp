
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "BMPLoader.h"

using namespace std;

void BMPLoader::loadBMP(const char* filename){
    char header[54];

    ifstream ifs(filename,ios::binary);
    if(!ifs) {
        cout << "can't open file : " << filename << endl;
        return;
    }

    // ヘッダの読み込み
    ifs.read(header,54);
    if(ifs.bad() || header[0] != 'B' || header[1] != 'M'){
        cout << filename << " is not BMP" << endl;
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
        data.push_back(buf);
    }
    cout << "datasize" << data.size() << " in " << width * height* 3 <<  endl;

}

GLuint BMPLoader::makeTexture(){

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1,&textureID);

    glBindTexture(GL_TEXTURE_2D,textureID);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_BGR,GL_UNSIGNED_BYTE,&data[0]);
   /*
    float buf[] = {
        1,0,0, 0,1,0, 0,0,1,
        0,0,0, 1,1,1, 0,0,0,
        0,0,1, 0,1,0, 0,0,1
    };
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,3,3,0,GL_RGB,GL_FLOAT,buf);
*/
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    return textureID;
}

void BMPLoader::setTexture(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureID);
    // glTexSubImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width,height,0, GL_BGR,GL_UNSIGNED_BYTE, &data[0]);

}



