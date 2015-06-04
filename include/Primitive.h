#ifndef _PRIMITIVE_INCLUDED
#define _PRIMITIVE_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Texture.h"
#include "Animation.h"

class Animation;
class Camera;

class Primitive{

    private:
        static const int VERTEX_INFO_NUM = 4; 

        GLuint vbo[VERTEX_INFO_NUM];
        GLuint vao;

        // Uniform buffer Object
        std::vector<GLuint> ubo;
        
        // 視点クラス
        Camera* camera;
        // texture参照
        GLuint textureID = 0;

    protected:
        GLuint ibo;
        // 頂点描画部分
        virtual void DrawVertex();

    public:
        Primitive(Camera*);
        ~Primitive();

        // 頂点生データ 
        std::vector<GLfloat> positionData;
        std::vector<GLfloat> colorData;
        std::vector<GLfloat> normalData;
        std::vector<GLfloat> uvData;

        std::vector<GLuint> indexData; 

        Transform transform;
        std::shared_ptr<Material> material = nullptr;
        Texture* texture;
        Animation* animation = NULL;

        virtual void Init(GLuint shaderID);
        void SetTexture(GLuint id){ textureID = id;}
        void Draw(GLuint shaderID);

};

// 平面
class Plane : public Primitive{
    protected:
        void DrawVertex();
    public:
        Plane(Camera*);

};

// 立方体
class Cube : public Primitive{
    protected:
        void DrawVertex();
    public:
        Cube(Camera*);
};

// 球
class Sphere : public Primitive{
    private:
        static constexpr float radius = 1;
        static constexpr int precision = 20;
    protected:
        void DrawVertex();
    public:
        Sphere(Camera*);
        void Init(GLuint shaderID);
};


// トーラス
class Torus : public Primitive{
    private:
        static constexpr float majorRadius = 0.5f;
        static constexpr int majorPrecision = 40;
        static constexpr float minorRadius = 0.25f;
        static constexpr int minorPrecision = 20;

    protected:
        void DrawVertex();

    public:
        Torus(Camera*);
        void Init(GLuint shaderID);

};




#endif
