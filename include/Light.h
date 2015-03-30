#ifndef _KOBA_LIGHT_INCLUDED
#define _KOBA_LIGHT_INCLUDED

#include <glm/glm.hpp>
#include <vector>

#include "Logger.h"

using namespace std;

class Light{
    private:
        std::vector<GLuint> ubo;

    public:

    glm::vec4 position;
    glm::vec3 La;
    glm::vec3 Ld;
    glm::vec3 Ls;

    Light(){
        position = glm::vec4(0,0,0,1);
        La = glm::vec3(0.1,0.1,0.1);
        Ld = glm::vec3(0.6,0.6,0.6);
        Ls = glm::vec3(1,1,1);
    }

    // シェーダー内のuniform blockにデータをバインド 
    void Init(GLuint shaderID){
        ubo.push_back(glGetUniformLocation(shaderID,"Light.position"));
        ubo.push_back(glGetUniformLocation(shaderID,"Light.La"));
        ubo.push_back(glGetUniformLocation(shaderID,"Light.Ld"));
        ubo.push_back(glGetUniformLocation(shaderID,"Light.Ls"));
    }

    void Set(glm::mat4 viewMatrix){
        
        glm::vec4 viewLight = viewMatrix * position;
        glUniform4fv(ubo[0],1,&viewLight[0]);

        glUniform3fv(ubo[1],1,&La[0]);
        glUniform3fv(ubo[2],1,&Ld[0]);
        glUniform3fv(ubo[3],1,&Ls[0]);

    }


};

#endif
