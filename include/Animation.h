#ifndef _KOBA_ANIMATION_INCLUDED
#define _KOBA_ANIMATION_INCLUDED


#include "Primitive.h"

class Primitive;

class Animation{

    protected:

        Primitive* prim;

    public:
        Animation(Primitive* prim);
        virtual void Animate() = 0;
};

class RotateAnimation : public Animation{
    private:
        static constexpr float dAngle = 1;
    public:
        RotateAnimation(Primitive* prim): Animation(prim){}
        void Animate();
};

class CircleTurningAnimation : public Animation{
    private:
        float angle;
        static constexpr float dAngle = 1;
        static constexpr float rad = 30;
    public:
        CircleTurningAnimation(Primitive* prim) : Animation(prim) { angle = 0; }
        void Animate();
};

#endif
