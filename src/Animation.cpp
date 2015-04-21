
#include <cmath>

#include "Animation.h"

Animation::Animation(Primitive* primitive){
    prim = primitive;
}

void RotateAnimation::Animate(){
    prim->transform.rotation.y += dAngle;
}

void CircleTurningAnimation::Animate(){
    float x = rad * cos(angle);
    float y = prim->transform.position.y;
    float z = rad * sin(angle);

    prim->transform.position = glm::vec3(x,y,z);

    angle += dAngle;
}
