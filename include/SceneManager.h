#ifndef _KOBA_SCENEMANAGER_INCLUDED
#define _KOBA_SCENEMANAGER_INCLUDED

#include <vector>
#include <memory>

#include "ResourceManager.h"
#include "Primitive.h"
#include "Camera.h"
#include "Light.h"
#include "Logger.h"



class SceneManager{
    private:
        
       
    public:

        std::vector< std::shared_ptr<Primitive> > primitives;
        std::vector<std::shared_ptr<Plane> > planes;
        std::vector<Light> lights;
        Camera camera;

        shared_ptr<CubeMap> cubeMap;

        SceneManager(ResourceManager* resourceManager) : camera(45.0f){
            // 背景平面の準備
            Plane plane(&camera);

            std::shared_ptr<Material> mat(new Material);
            resourceManager->materials.push_back(mat);
            std::shared_ptr<BMPLoader> texture = resourceManager->textures[0];

            // 奥
            std::shared_ptr<Plane> planeBack(new Plane(&camera));
            planeBack->texture = texture.get();
            planeBack->material = mat;
            planeBack->transform.scale = glm::vec3(40,40,1);
            planeBack->transform.position = glm::vec3(0,10,-20);
            planes.push_back(planeBack);

            // 左
            std::shared_ptr<Plane> planeLeft(new Plane(&camera));
            planeLeft->texture = texture.get();
            planeLeft->material = mat;
            planeLeft->transform.scale = glm::vec3(40,40,1);
            planeLeft->transform.position = glm::vec3(-20,10,0);
            planeLeft->transform.rotation = glm::vec3(0,90,0);
            planes.push_back(planeLeft);
        
            // 下
            std::shared_ptr<Plane> planeButtom(new Plane(&camera));
            planeButtom->texture = texture.get();
            planeButtom->material = mat;
            planeButtom->transform.scale = glm::vec3(100,100,1);
            planeButtom->transform.position = glm::vec3(0,-10,0);
            planeButtom->transform.rotation = glm::vec3(-90,0,0);
            planes.push_back(planeButtom);

        }

        void addLight(Light light){
            light.setID(lights.size());
            lights.push_back(light);
        }

        void initScene(GLuint &shaderID){
            glClearColor(0.1f,0.1f,0.2f,1.0f);
            // glViewPort(1,1,1,1);
           
            Logger::Log("Init Planes");
            for(auto &plane : planes){
                plane->Init(shaderID);
            }

            Logger::Log("Init Primitives");
            for(auto &prim : primitives){
                prim->Init(shaderID);
            }
            
            Logger::Log("Init Lights");
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

            cubeMap->setCubeMap();

            for(auto light : lights){
                light.Set(camera.getViewMatrix());
            }

            for(auto &plane : planes){
                plane->Draw(shaderID);
            }

            for(auto &prim : primitives){
                prim->Draw(shaderID);
            }
            
        }
};

#endif
