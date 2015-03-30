#ifndef _PRIMITIVE_INCLUDED
#define _PRIMITIVE_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Transform.h"
#include "Camera.h"
#include "Material.h"

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

        void Init(GLuint shaderID);
        void SetTexture(GLuint id){
            textureID = id;}
        void Draw(GLuint shaderID);

};

#endif
