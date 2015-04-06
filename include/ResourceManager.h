#ifndef _KOBA_RESOURCE_MANAGER_INCLUDED
#define _KOBA_RESOURCE_MANAGER_INCLUDED

#include <vector>
#include <memory>

#include "Material.h"
#include "BMPLoader.h"
#include "Logger.h"


class ResourceManager{
    
    public:

        std::vector<std::shared_ptr<Material> > materials;
        std::vector<std::shared_ptr<BMPLoader> > textures;

        void loadTexture(const char* filename){
            std::shared_ptr<BMPLoader> texture(new BMPLoader);
            texture->loadBMP(filename);
            texture->makeTexture(0);            
            textures.push_back(texture);
        }

        void makeResource(){
            
            Logger::Log("Make Resources");

            loadTexture("hima.bmp");
            loadTexture("garasubo.bmp");

            std::shared_ptr<Material> mat(new Material);
            materials.push_back(mat);

        }

        void initResources(GLuint shaderID){
            for(auto mat : materials){
                mat->Init(shaderID);
            }
        }

};

#endif
