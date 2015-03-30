#ifndef _KOBA_SCENEMANAGER_INCLUDED
#define _KOBA_SCENEMANAGER_INCLUDED

#include <vector>

#include "Primitive.h"
#include "Camera.h"
#include "Light.h"

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

       
    public:

        std::vector<Primitive> primitives;
        std::vector<Light> lights;
        Camera camera;


        SceneManager() : camera(45.0f){
            // 背景平面の準備
            Primitive plane(&camera);
            for(int i=0;i<12;i++){
                plane.positionData.push_back(planePos[i]);
                plane.colorData.push_back(0);
                plane.normalData.push_back(planeNor[i]);
            }for(int i=0;i<8;i++){
                plane.uvData.push_back(planeuv[i]);
            }
            plane.transform.position = glm::vec3(0,-6,0);
            primitives.push_back(plane);

            plane.transform.position = glm::vec3(0,4,-10);
            plane.transform.rotation = glm::vec3(90,0,0);
            primitives.push_back(plane);
        
             plane.transform.position = glm::vec3(-10,4,0);
            plane.transform.rotation = glm:: vec3(0,0,-90);
            primitives.push_back(plane);

        }

        void addLight(Light light){
            light.setID(lights.size());
            lights.push_back(light);
        }

        void initScene(GLuint &shaderID){
            glClearColor(0.1f,0.1f,0.2f,1.0f);
            // glViewPort(1,1,1,1);

            for(auto &prim : primitives){
                prim.Init(shaderID);
            }
            
            for(auto &light : lights){
                light.Init(shaderID);
            }

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LESS);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }

        void drawScene(GLuint &shaderID){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shaderID);

            for(auto light : lights){
                light.Set(camera.getViewMatrix());
            }

            for(auto &prim : primitives){
                prim.Draw(shaderID);
            }
            
        }
};

#endif
