#ifndef _KOBA_RESOURCE_MANAGER_INCLUDED
#define _KOBA_RESOURCE_MANAGER_INCLUDED

#include <vector>
#include <memory>

#include "Material.h"
#include "BMPLoader.h"
#include "Logger.h"
#include "CubeMap.h"

class ResourceManager{
    int shaderID;
    
    public:

        std::vector<std::shared_ptr<Material> > materials;
        std::vector<std::shared_ptr<BMPLoader> > textures;
        std::shared_ptr<CubeMap> cubeMap;

        ResourceManager(int shaderID){
            this->shaderID = shaderID;
        }

        void loadTexture(const char* filename,int unit){
            std::shared_ptr<BMPLoader> texture(new BMPLoader);
            texture->loadBMP(filename);
            texture->makeTexture(shaderID,unit);            
            textures.push_back(texture);
        }

        void makeResource(){
            
            Logger::Log("Make Resources");

            loadTexture("hima.bmp",0);
            loadTexture("garasubo.bmp",0);

            std::shared_ptr<CubeMap> cMap(new CubeMap);
            cMap->loadCubeTex("right.bmp","left.bmp",
                    "up.bmp","down.bmp",
                    "front.bmp","back.bmp"
                    );
            cubeMap = cMap;

            std::shared_ptr<Material> mat(new Material);
            materials.push_back(mat);

        }

        void initResources(GLuint shaderID){
            for(auto mat : materials){
                mat->Init(shaderID);
            }
            cubeMap->makeCubeMap(shaderID,2);

        }

};

#endif
