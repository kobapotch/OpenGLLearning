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
                    angle,aspect,front,back);
            return projection;

        }

        float rotAngle=0;
        glm::mat4 getViewMatrix(){

            rotAngle+=0.0005f;
            transform.position = glm::vec3(50*cos(rotAngle),transform.position.y,50*sin(rotAngle));
            // transform.position = glm::vec3(100 + 5*cos(rotAngle),30,10 + 5*sin(rotAngle));
            
            glm::mat4 view = glm::lookAt(
                    transform.position,
                    glm::vec3(0,0,0),
                    glm::vec3(0,1,0)
                    );

            return view;
        }


};

#endif
