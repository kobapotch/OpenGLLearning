#ifndef _KOBA_SCENEMAKER_INCLUDED
#define _KOBA_SCENEMAKER_INCLUDED

#include "SceneManager.h"
#include "ResourceManager.h"

class SceneMaker{
    private:
        ResourceManager* resourceManager; 

    public:
        SceneMaker(ResourceManager&);
        SceneManager makeScene();    

};

#endif
