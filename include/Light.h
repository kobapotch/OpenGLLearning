#ifndef _KOBA_LIGHT_INCLUDED
#define _KOBA_LIGHT_INCLUDED

#include <glm/glm.hpp>
#include <cstdlib>

#include "Logger.h"

class Light{
    private:
        GLuint blockIndex;
        GLuint uboHandle;

    public:

    glm::vec4 position;
    glm::vec3 La;
    glm::vec3 Ld;
    glm::vec3 Ls;

    Light(){
        position = glm::vec4(0.0);
        La = glm::vec3(1,1,1);
        Ld = glm::vec3(1,1,1);
        Ls = glm::vec3(1,1,1);
    }

    // シェーダー内のuniform blockにデータをバインド 
    void Init(GLuint shaderID){

        Logger::Log("Light Init");

        blockIndex = glGetUniformBlockIndex(shaderID,"LightInfo");
        cout << "blockIndex is " << (blockIndex == GL_INVALID_INDEX) << endl;

        GLint blockSize;
        glGetActiveUniformBlockiv(shaderID,blockIndex,GL_UNIFORM_BLOCK_DATA_SIZE,&blockSize);
        GLubyte* blockBuffer = new GLubyte[blockSize];

        // 各ブロック内の変数オフセットを求める
        Logger::Log("offset");
        const GLchar* names[] = {"position","La","Ld","Ls" };
        GLuint indices[4];
        glGetUniformIndices(shaderID,4,names,indices);

        cout << "indices is :";
        for(int i=0;i<4;i++){
            cout << " " << indices[i];
        }cout << endl;

        GLint offset[4];
        glGetActiveUniformsiv(shaderID,4,indices,GL_UNIFORM_OFFSET,offset);

        cout << "offset is :";
        for(int i=0;i<4;i++){
            cout << " " << offset[i];
        }cout << endl;

        // バッファにデータを配置
        Logger::Log("BufferData");
        memcpy(blockBuffer + offset[0],&position[0],4*sizeof(GLfloat));
        memcpy(blockBuffer + offset[1],&La[0],3*sizeof(GLfloat));
        memcpy(blockBuffer + offset[2],&Ld[0],3*sizeof(GLfloat));
        memcpy(blockBuffer + offset[3],&Ls[0],3*sizeof(GLfloat));

        Logger::Log("bind");
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
