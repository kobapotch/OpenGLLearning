#ifndef _PRIMITIVE_INCLUDED
#define _PRIMITIVE_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "BMPLoader.h"

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

        // 頂点描画部分
        virtual void DrawVertex();

    public:
        Primitive(Camera*);
        ~Primitive();

        bool rotate = false;

        // 頂点生データ 
        std::vector<GLfloat> positionData;
        std::vector<GLfloat> colorData;
        std::vector<GLfloat> normalData;
        std::vector<GLfloat> uvData;

        Transform transform;
        Material* material;
        BMPLoader* texture;

        void Init(GLuint shaderID);
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

// トーラス
class Torus : public Primitive{
    private:
        float majorRadius;
        float minorRadius;

    protected:
        void DrawVertex();

    public:
        Torus(Camera*);

};




#endif
