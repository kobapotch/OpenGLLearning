#ifndef _KOBA_MATERIAL_INCLUDED
#define _KOBA_MATERIAL_INCLUDED

#include <glm/glm.hpp>
#include <iostream>

#include "Logger.h"

using namespace std;

class Material{
    private:
        GLuint blockIndex;
        GLuint uboHandle;
    
    public:

    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float shininess;

    Material(){
        Ka = glm::vec3(1,1,1);
        Kd = glm::vec3(1,1,1);
        Ks = glm::vec3(1,1,1);
        shininess = 20;
    }

    // シェーダー内のuniform blockにデータをバインド 
    void Init(GLuint shaderID){

        Logger::Log("Material Init");        

        blockIndex = glGetUniformBlockIndex(shaderID,"MaterialInfo");
        
        GLint blockSize;
        glGetActiveUniformBlockiv(shaderID,blockIndex,GL_UNIFORM_BLOCK_DATA_SIZE,&blockSize);
        GLubyte* blockBuffer = new GLubyte[blockSize];

        // 各ブロック内の変数オフセットを求める
        const GLchar* names[] = {"Ka","Kd","Ks","shininess" };
        GLuint indices[4];
        glGetUniformIndices(shaderID,4,names,indices);

        GLint offset[4];
        glGetActiveUniformsiv(shaderID,4,indices,GL_UNIFORM_OFFSET,offset);

        cout << "offset is :";
        for(int i=0;i<4;i++){
            cout << " " << offset[i];
        }cout << endl;

        // バッファにデータを配置
        memcpy(blockBuffer + offset[0],&Ka[0],3*sizeof(GLfloat));
        memcpy(blockBuffer + offset[1],&Kd[0],3*sizeof(GLfloat));
        memcpy(blockBuffer + offset[2],&Ks[0],3*sizeof(GLfloat));
        memcpy(blockBuffer + offset[3],&shininess,sizeof(GLfloat));

        glGenBuffers(1,&uboHandle);
        glBindBuffer(GL_UNIFORM_BUFFER,uboHandle);
        glBufferData(GL_UNIFORM_BUFFER,blockSize,blockBuffer,GL_DYNAMIC_DRAW);

        Logger::Log("UBO is" + std::to_string(uboHandle));


        glBindBufferBase(GL_UNIFORM_BUFFER,blockIndex,uboHandle);

        delete[] blockBuffer;
    }

    void Set(){
        glBindBufferBase(GL_UNIFORM_BUFFER,blockIndex,uboHandle);
    }
};
    

#endif
