
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "ShaderManager.h"
#include "SceneMaker.h"
#include "SceneManager.h"
#include "ResourceManager.h"

using namespace std;
    

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
    window = glfwCreateWindow(800,800, "Sample",NULL,NULL);
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

    // シェーダーの準備
    ShaderManager shaderManager;
    shaderManager.compileVertexShader("VertexShader.glsl");
    shaderManager.compileFragmentShader("FragmentShader.glsl");
    GLuint programID = shaderManager.linkShader();

    ResourceManager resourceManager;
    resourceManager.makeResource();

    SceneMaker sceneMaker(resourceManager);
    SceneManager myScene = sceneMaker.makeScene();

    myScene.initScene(programID);
    resourceManager.initResources(programID);

    cout << "Draw Start" << endl;

    while(!glfwWindowShouldClose(window)){

        myScene.drawScene(programID);

        glfwSwapBuffers(window);
        glfwPollEvents();
        // glfwWaitEvents();
    }

    glfwTerminate();

}



