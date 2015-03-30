
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <random>

#include "ShaderManager.h"
#include "SceneManager.h"
#include "BMPLoader.h"

using namespace std;

GLfloat g_vertex_buffer_data[] ={
    // 前面
    -0.5f,0.5f,0.5f,
    -0.5f,-0.5f,0.5f,
    0.5f,-0.5f,0.5f,
    0.5f,0.5f,0.5f,
    // 上面
    -0.5f,0.5f,-0.5f,
    -0.5f,0.5f,0.5f,
    0.5f,0.5f,0.5f,
    0.5f,0.5f,-0.5f,
    // 左面
    -0.5f,0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,0.5f,
    -0.5f,0.5f,0.5f,
    // 下面
    -0.5f,-0.5f,0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,0.5f,
    // 右面
    0.5f,0.5f,0.5f,
    0.5f,-0.5f,0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,0.5f,-0.5f,
    // 裏面
    0.5f,0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,0.5f,-0.5f
};



GLfloat g_color_data[] = {
    // 前面
    0.8f,  0.5f,  0.2f,
    0.8f,  0.5f,  0.2f,
    0.8f,  0.5f,  0.2f,
    0.8f,  0.5f,  0.2f,
    // 上面
    0.5f,0.2f,0.8f,
    0.5f,0.2f,0.8f,
    0.5f,0.2f,0.8f,
    0.5f,0.2f,0.8f,
    // 左面
    0.2f,0.8f,0.5f,
    0.2f,0.8f,0.5f,
    0.2f,0.8f,0.5f,
    0.2f,0.8f,0.5f,
    // 下面
    0.8f,0.2f,0.5f,
    0.8f,0.2f,0.5f,
    0.8f,0.2f,0.5f,
    0.8f,0.2f,0.5f,
    // 右面
    0.2f,0.5f,0.8f,
    0.2f,0.5f,0.8f,
    0.2f,0.5f,0.8f,
    0.2f,0.5f,0.8f,
    // 下面
    0.5f,0.8f,0.2f,
    0.5f,0.8f,0.2f,
    0.5f,0.8f,0.2f,
    0.5f,0.8f,0.2f
};

GLfloat g_normal_data[] = {
    // 前面
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    // 上面
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    // 左面
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    // 下面
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    // 右面
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    // 奥面
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f
};

GLfloat g_uv_data[] ={
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f
};


float tripos[] = {
    0.0f,1.0f,0.0f,
    -1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f
};
float tricolor[] = {
    1.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,0.0f,1.0f
};

Material myMaterial;

SceneManager makeScene(){
    SceneManager myScene;
    myScene.camera.transform.position = glm::vec3(3,8,20);

    Primitive cube(&myScene.camera);
    for(int i=0;i<36*3;i++){
        cube.positionData.push_back(g_vertex_buffer_data[i]);
        cube.colorData.push_back( g_color_data[i]);
        cube.normalData.push_back( g_normal_data[i]);
    }
    for(int i=0;i<36*2;i++){
        cube.uvData.push_back(g_uv_data[i]);
    }

    std::random_device rnd;
    std::mt19937 mt( rnd() );
    std::uniform_int_distribution<> rand100(-6,6);

    for(int i=0;i<10;i++){
        cube.transform.position = glm::vec3(rand100(mt),rand100(mt)+3,rand100(mt));
        cube.transform.rotation = glm::vec3(rand100(mt)*100,rand100(mt)*100,rand100(mt)*100);
        cube.transform.scale = glm::vec3(2,2,2);
        cube.rotate = true;

        cube.material = &myMaterial;

        myScene.primitives.push_back(cube);
    }

    cube.transform.position = glm::vec3(0,0,0);
    cube.transform.rotation = glm::vec3(0,0,0);
    cube.rotate = false;
    myScene.primitives.push_back(cube);

    Light light;
    light.position = glm::vec4(-8,15,8,1);
    light.La = glm::vec3(0.05,0.05,0.05);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(1.0,0.2,0.2);
    myScene.addLight(light);

    light.position = glm::vec4(-8,15,-8,1);
    light.La = glm::vec3(0.05,0.05,0.05);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(0.2,1.0,0.2);
    myScene.addLight(light);
 
    light.position = glm::vec4(8,15,-8,1);
    light.La = glm::vec3(0.1,0.1,0.1);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(0.2,0.2,1.0);
    myScene.addLight(light);

   return myScene;
}



int main(int argc,char* argv[]){

    GLFWwindow* window;

    // GLFW初期化
    if(!glfwInit()) {
        cout << "glfwInit failure" << endl;
        return -1;
    }

    // OpenGLのバージョン設定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // glfwCreateWindow(width,height,title,monitor,share)
    //      monitor : 使用するモニタ　フルスクリーンでない場合にはNULLを指定
    //      share : 他のウィンドウとのリソースの共有に使う　windowのハンドルを指定
    window = glfwCreateWindow(640,640, "Sample",NULL,NULL);
    if(!window){
        cout << "window open error" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // GLEWの初期化
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        cout << "Glew Init Failulre" << endl;
        return 0;
    }

    // この時点でのOpenGLバージョン確認
    cout << "GPU:" << glGetString(GL_VERSION) << endl
        << "OpenGL version : " << glGetString( GL_VERSION) << endl
        << "GLSL version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // テクスチャの読み込み 
    BMPLoader loader;
    loader.loadBMP(argc >=2 ? argv[1] :"hima.bmp");
    cout << loader.makeTexture() << endl;

    ShaderManager shaderManager;
    shaderManager.compileVertexShader("VertexShader.glsl");
    shaderManager.compileFragmentShader("FragmentShader.glsl");
    GLuint programID = shaderManager.linkShader();

    SceneManager myScene = makeScene();

    myScene.initScene(programID);
    myMaterial.Init(programID);

    cout << "Draw Start" << endl;

    while(!glfwWindowShouldClose(window)){

        myScene.drawScene(programID);

        glfwSwapBuffers(window);
        glfwPollEvents();
        // glfwWaitEvents();
    }

    glfwTerminate();
}



