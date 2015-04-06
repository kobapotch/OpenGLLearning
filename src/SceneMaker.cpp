
#include <random>

#include "ResourceManager.h"
#include "SceneMaker.h"
#include "Primitive.h"
#include "BMPLoader.h"

SceneMaker::SceneMaker(ResourceManager& rManager){
    resourceManager = &rManager;

}

SceneManager SceneMaker::makeScene(){

    BMPLoader* hima = resourceManager->textures[0].get();
   BMPLoader* garasubo = resourceManager->textures[1].get();
    
    SceneManager scene(hima);

    scene.camera.transform.position = glm::vec3(3,8,40);

    std::random_device rnd;
    std::mt19937 mt( rnd() );
    std::uniform_int_distribution<> rand100(-10,10);

    for(int i=0;i<0;i++){

        std::shared_ptr<Primitive> cube(new Cube(&scene.camera) );
        cube->texture = garasubo;

        cube->transform.position = glm::vec3(rand100(mt),rand100(mt),rand100(mt));
        cube->transform.rotation = glm::vec3(rand100(mt)*100,rand100(mt)*100,rand100(mt)*100);
        cube->transform.scale = glm::vec3(2,2,2);
        cube->rotate = true;

        cube->material = resourceManager->materials[0];

        scene.primitives.push_back(cube);
    }
    for(int i=0;i<10;i++){
        std::shared_ptr<Primitive> torus(new Torus(&scene.camera) );
        torus->texture = garasubo;

        torus->transform.position = glm::vec3(rand100(mt),rand100(mt),rand100(mt));
        torus->transform.rotation = glm::vec3(rand100(mt)*100,rand100(mt)*100,rand100(mt)*100);
        torus->transform.scale = glm::vec3(3);
        torus->rotate = true;

        torus->material = resourceManager->materials[0];

        scene.primitives.push_back(torus);
    }

    std::shared_ptr<Primitive> cube(new Cube(&scene.camera));
    cube->transform.scale  = glm::vec3(2,2,2);
    cube->transform.position = glm::vec3(0,0,0);
    cube->transform.rotation = glm::vec3(0,0,0);
    cube->rotate = false;
    cube->texture = garasubo;
    scene.primitives.push_back(cube);

    Light light;
    light.position = glm::vec4(-15,-5,15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(1.0,0.2,0.2);
    scene.addLight(light);

    light.position = glm::vec4(-15,-5,-15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(0.2,1.0,0.2);
    scene.addLight(light);
 
    light.position = glm::vec4(15,-5,-15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(0.2,0.2,1.0);
     scene.addLight(light);

    light.position = glm::vec4(100,50,-30,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(0.2,0.2,1.0);
    scene.addLight(light);

   return scene;
}

