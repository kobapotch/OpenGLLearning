#ifndef _KOBA_CAMERA_INCLUDED
#define _KOBA_CAMERA_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

#include "Transform.h"

class Camera{
    private:
        float aspect;
        float front;
        float back;

    public:
        Transform transform;

        float angle;

        Camera(float angle){
            this->angle = angle;
            aspect = 1.0f;
            front = 0.1f;
            back = 100.0f;

        }

        glm::mat4 getProjectionMatrix(){
            glm::mat4 projection = glm::perspective(
                    45.0f,aspect,front,back);
            return projection;

        }

        float rotAngle=0;
        glm::mat4 getViewMatrix(){

            rotAngle+=0.0005f;
            transform.position = glm::vec3(20*cos(rotAngle),transform.position.y,20*sin(rotAngle)+3);
            
            glm::mat4 view = glm::lookAt(
                    transform.position,
                    glm::vec3(0,0,0),
                    glm::vec3(0,1,0)
                    );

            return view;
        }


};

#endif
