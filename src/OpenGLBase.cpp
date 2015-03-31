
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <random>
#include <new>
#include <vector>
#include <memory>

#include "ShaderManager.h"
#include "SceneManager.h"
#include "BMPLoader.h"

using namespace std;

// 素材達　とりあえずここにドカドカ
Material myMaterial;
BMPLoader loader1;
BMPLoader loader2;

SceneManager makeScene(){
    SceneManager myScene(loader1);
    myScene.camera.transform.position = glm::vec3(3,8,30);

    
    std::random_device rnd;
    std::mt19937 mt( rnd() );
    std::uniform_int_distribution<> rand100(-8,8);

    for(int i=0;i<0;i++){

        std::shared_ptr<Primitive> cube(new Cube(&myScene.camera) );
        cube->texture = &loader2;

        cube->transform.position = glm::vec3(rand100(mt),rand100(mt),rand100(mt));
        cube->transform.rotation = glm::vec3(rand100(mt)*100,rand100(mt)*100,rand100(mt)*100);
        cube->transform.scale = glm::vec3(2,2,2);
        cube->rotate = true;

        cube->material = &myMaterial;

        myScene.primitives.push_back(cube);
    }
    for(int i=0;i<10;i++){
        std::shared_ptr<Primitive> torus(new Torus(&myScene.camera) );
        torus->texture = &loader2;

        torus->transform.position = glm::vec3(rand100(mt),rand100(mt),rand100(mt));
        torus->transform.rotation = glm::vec3(rand100(mt)*100,rand100(mt)*100,rand100(mt)*100);
        torus->transform.scale = glm::vec3(3);
        torus->rotate = true;

        torus->material = &myMaterial;

        myScene.primitives.push_back(torus);
    }

    std::shared_ptr<Primitive> cube(new Cube(&myScene.camera));
    cube->transform.scale  = glm::vec3(2,2,2);
    cube->transform.position = glm::vec3(0,0,0);
    cube->transform.rotation = glm::vec3(0,0,0);
    cube->rotate = false;
    cube->texture = &loader2;
    myScene.primitives.push_back(cube);

    Light light;
    light.position = glm::vec4(-15,-5,15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(1.0,0.2,0.2);
    myScene.addLight(light);

    light.position = glm::vec4(-15,-5,-15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.4,0.4,0.4);
    light.Ls = glm::vec3(0.2,1.0,0.2);
    myScene.addLight(light);
 
    light.position = glm::vec4(15,-5,-15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
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
    loader1.loadBMP(argc >=2 ? argv[1] :"hima.bmp");
    loader1.makeTexture();
    loader2.loadBMP(argc >=3 ? argv[2] :"garasubo.bmp");
    loader2.makeTexture();

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



