#ifndef _KOBA_SCENEMANAGER_INCLUDED
#define _KOBA_SCENEMANAGER_INCLUDED

#include <vector>

#include "Primitive.h"
#include "Camera.h"

GLfloat planePos[] = {
    -10,0,-10,
    -10,0,10,
    10,0,10,
    10,0,-10,
};
GLfloat planeuv[] = {
    0,1,
    0,0,
    1,0,
    1,1,

};
GLfloat planeNor[] = {
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0
};


class SceneManager{
    private:
        GLuint ubo;
    public:
        SceneManager() : camera(45.0f){
            Primitive plane(&camera);
            for(int i=0;i<12;i++){
                plane.positionData.push_back(planePos[i]);
                plane.colorData.push_back(0);
                plane.normalData.push_back(planeNor[i]);
            }for(int i=0;i<8;i++){
                plane.uvData.push_back(planeuv[i]);
            }
            plane.transform.position = glm::vec3(0,-3,0);
            primitives.push_back(plane);

            plane.transform.position = glm::vec3(0,7,-10);
            plane.transform.rotation = glm::vec3(90,0,0);
            primitives.push_back(plane);
        
             plane.transform.position = glm::vec3(-10,7,0);
            plane.transform.rotation = glm:: vec3(0,0,-90);
            primitives.push_back(plane);

        }

        std::vector<Primitive> primitives;
        Camera camera;

        void initScene(GLuint &shaderID){
            glClearColor(0.1f,0.1f,0.2f,1.0f);
            // glViewPort(1,1,1,1);

            for(auto &prim : primitives){
                prim.Init(shaderID);
            }

            ubo = glGetUniformLocation(shaderID,"lightPos");


            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LESS);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }

        void drawScene(GLuint &shaderID){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shaderID);

            glm::vec4 lightPos = glm::vec4(-3,10,-6,1);
            lightPos = camera.getViewMatrix() * lightPos;
            glUniform4f(ubo,lightPos[0],lightPos[1],lightPos[2],lightPos[3]);
            
            for(auto &prim : primitives){
                prim.Draw(shaderID);
            }
            
        }
};

#endif
