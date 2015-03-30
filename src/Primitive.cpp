
#include "Primitive.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <random>

#include "Logger.h"



glm::mat4 Projection = glm::perspective(
        30.0f,
        1.0f,
        0.1f,
        10.0f
        );
// LookAt関数
// glm::lookAt(
//      viewpoint   : 視点
//      lookpoint   : 視対象
//      up          : 上方をさすベクトル
glm::mat4 View = glm::lookAt(
        glm::vec3(2,2,4),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
        );


Primitive::Primitive(Camera* m_camera){
    camera = m_camera;
}
Primitive::~Primitive(){
}
// Primitive::Primitive(const Primitive& obj){
//  }

void Primitive::Init(GLuint shaderID){
    Log("Primitive Init");
   
    // VAOの作成とバインド
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    cout << "VAO is " << vao << endl;

    glGenBuffers(VERTEX_INFO_NUM,vbo);
    GLuint positionBufferHandle = vbo[0];
    GLuint colorBufferHandle = vbo[1];
    GLuint normalBufferHandle = vbo[2];
    GLuint uvBufferHandle = vbo[3];

   // 頂点データをOpenGLに登録 
    glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,positionData.size() * sizeof(GLfloat),&positionData[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,colorData.size() * sizeof(GLfloat),&colorData[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,normalData.size() * sizeof(GLfloat),&normalData[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,uvBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,uvData.size() * sizeof(GLfloat) , &uvData[0],GL_STATIC_DRAW);

    // 配列属性の有効化とバッファへの対応付け
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,uvBufferHandle);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(void*)0);

    // uniform変数の関連付け
    ubo.push_back(glGetUniformLocation(shaderID,"MVP"));
    ubo.push_back(glGetUniformLocation(shaderID,"MV"));
    ubo.push_back(glGetUniformLocation(shaderID,"M"));
    ubo.push_back(glGetUniformLocation(shaderID,"textureSampler"));

    glBindVertexArray(0);
}

float angle = 0;

void Primitive::Draw(GLuint shaderID){
   
    if(rotate){
        transform.rotation.y += 1.0f;}
    
    glm::mat4 M = transform.getModelMatrix();
    glm::mat4 MV = camera->getViewMatrix() * M;
    glm::mat4 MVP = camera->getProjectionMatrix() * MV;

    glUseProgram(shaderID);
    glBindVertexArray(vao);



    // 変換行列をシェーダーに送り込む
    glUniformMatrix4fv(ubo[0],1,GL_FALSE,&MVP[0][0]);
    glUniformMatrix4fv(ubo[1],1,GL_FALSE,&MV[0][0]);
    glUniformMatrix4fv(ubo[2],1,GL_FALSE,&M[0][0]);
    // マルチテクスチャの番号を入れることに注意
    glUniform1i(ubo[3],0);

    // 頂点0から初めて，3つの頂点を利用して
    for(int i=0;i<6;i++){
        glDrawArrays(GL_TRIANGLE_FAN,i*4,4);
    }
    glBindVertexArray(0);

}
