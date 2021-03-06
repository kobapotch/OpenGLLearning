
#include <random>
#include <iostream>

#include "ResourceManager.h"
#include "SceneMaker.h"
#include "Primitive.h"
#include "BMPLoader.h"
#include "Animation.h"

using namespace std;

class maniSpace{
    friend ostream& operator <<(ostream& ros,maniSpace space){
        return space(ros);
    }
    private:
    int num;
    public:
    maniSpace(int value){
        num = value;
    }
    private:
    ostream& operator()(ostream& ros){
        string buf;
        for(int i=0;i<num;i++){
            buf += "    ";
        }
        return ros << buf;
    }
};

string GetAttributeTypeName(FbxNodeAttribute::EType type) { 
    switch(type) { 
        case FbxNodeAttribute::eUnknown: return "unidentified"; 
        case FbxNodeAttribute::eNull: return "null"; 
        case FbxNodeAttribute::eMarker: return "marker"; 
        case FbxNodeAttribute::eSkeleton: return "skeleton"; 
        case FbxNodeAttribute::eMesh: return "mesh"; 
        case FbxNodeAttribute::eNurbs: return "nurbs"; 
        case FbxNodeAttribute::ePatch: return "patch"; 
        case FbxNodeAttribute::eCamera: return "camera"; 
        case FbxNodeAttribute::eCameraStereo: return "stereo"; 
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
        case FbxNodeAttribute::eLight: return "light"; 
        case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
        case FbxNodeAttribute::eOpticalMarker: return "marker"; 
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
        case FbxNodeAttribute::eBoundary: return "boundary"; 
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
        case FbxNodeAttribute::eShape: return "shape"; 
        case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
        case FbxNodeAttribute::eSubDiv: return "subdiv"; 
        default: return "unknown"; 
    } 
}

void PrintAttributeData(FbxNode* node, int depth = 0){

    cout << maniSpace(depth) <<  "Nodename : " << node->GetName() << endl;

    for(int j=0;j<node->GetNodeAttributeCount();j++){
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(j);
        string typeName = GetAttributeTypeName(attribute->GetAttributeType());
        string attrName = attribute->GetName();
        int childCount = node->GetChildCount();

        cout << maniSpace(depth) << "Attribute type : "
            <<  typeName << "," << attrName << endl;
    }

    if(node->GetChildCount() <= 0) return;

    int childCount = node->GetChildCount();
    cout << maniSpace(depth)<< "Show Child nums : " << childCount << endl;

    for(int i=0;i<node->GetChildCount();i++){
        PrintAttributeData(node->GetChild(i),depth + 1);
    }

}



SceneMaker::SceneMaker(ResourceManager& rManager){
    resourceManager = &rManager;

}

SceneManager SceneMaker::makeScene(){

    Texture* hima = resourceManager->textures[0].get();
    Texture* garasubo = resourceManager->textures[1].get();

    SceneManager scene(resourceManager);

    scene.camera.transform.position = glm::vec3(3,8,40);
    scene.cubeMap = resourceManager->cubeMap;

    std::random_device rnd;
    std::mt19937 mt( rnd() );
    std::uniform_int_distribution<> rand100(-10,10);

    for(int i=0;i<0;i++){

        std::shared_ptr<Primitive> cube(new Cube(&scene.camera) );
        cube->texture = garasubo;

        cube->transform.position = glm::vec3(rand100(mt),rand100(mt),rand100(mt));
        cube->transform.rotation = glm::vec3(rand100(mt)*100,rand100(mt)*100,rand100(mt)*100);
        cube->transform.scale = glm::vec3(2,2,2);

        cube->material = resourceManager->materials[0];

        scene.primitives.push_back(cube);
    }
    for(int i=0;i<5;i++){
        std::shared_ptr<Primitive> torus(new Torus(&scene.camera) );
        torus->texture = garasubo;

        torus->transform.position = glm::vec3(rand100(mt),rand100(mt),rand100(mt));
        torus->transform.rotation = glm::vec3(rand100(mt)*100,rand100(mt)*100,rand100(mt)*100);
        torus->transform.scale = glm::vec3(3);

        torus->material = resourceManager->materials[0];

        torus->animation = new RotateAnimation(&torus->transform);

        scene.primitives.push_back(torus);
    }
    for(int i=0;i<2;i++){
        std::shared_ptr<Primitive> sphere(new Sphere(&scene.camera) );
        sphere->texture = garasubo;

        sphere->transform.position = glm::vec3(rand100(mt),rand100(mt),rand100(mt));

        sphere->transform.scale = glm::vec3(5);

        sphere->material = resourceManager->materials[0];

        scene.primitives.push_back(sphere);
    }

    std::shared_ptr<Primitive> cube(new Cube(&scene.camera));
    cube->transform.scale  = glm::vec3(2,2,2);
    cube->transform.position = glm::vec3(0,0,0);
    cube->transform.rotation = glm::vec3(0,0,0);
    cube->texture = garasubo;
    // scene.primitives.push_back(cube);


/*------------------- FBXファイルの読み込み----------------------*/

    FbxManager* fbxManager = FbxManager::Create();

    FbxIOSettings* fbxIOs = FbxIOSettings::Create(fbxManager,IOSROOT);
    fbxManager->SetIOSettings(fbxIOs);

    // importerはファイルを読み込んでシーンに渡すだけ
    FbxImporter* fbxImporter = FbxImporter::Create(fbxManager,"Importer");
    const char* filename = "voronoi2.fbx";
    if(!fbxImporter->Initialize(filename,-1,fbxManager->GetIOSettings())){
        cout << "fbx import failure" << endl;
        cout << "Error of " << fbxImporter->GetStatus().GetErrorString() << endl;
    }

    FbxScene* fbxScene = FbxScene::Create(fbxManager,"Scene");
    fbxImporter->Import(fbxScene);
    fbxImporter->Destroy();

    // 三角形メッシュのみにしておく
    FbxGeometryConverter geoConverter(fbxManager);
    geoConverter.Triangulate(fbxScene,true);

    // Fbxのシーンを，自分のシーンにPrimitiveとして読み込む
    FbxToMyScene(fbxScene,scene);

    fbxScene->Destroy();
    fbxManager->Destroy();

/*-------------------------ライトの設定-----------------------------*/

    Light light;
    light.position = glm::vec4(-15,-5,15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.5,0.2,0.2);
    light.Ls = glm::vec3(1.0,0.2,0.2);
    scene.addLight(light);

    light.position = glm::vec4(-15,-5,-15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.2,0.5,0.2);
    light.Ls = glm::vec3(0.2,1.0,0.2);
    scene.addLight(light);

    light.position = glm::vec4(15,-5,-15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.2,0.2,0.5);
    light.Ls = glm::vec3(0.2,0.2,1.0);
    scene.addLight(light);

    light.position = glm::vec4(15,15,15,1);
    light.La = glm::vec3(0.02,0.02,0.02);
    light.Ld = glm::vec3(0.5,0.5,0.5);
    light.Ls = glm::vec3(1.0,1.0,1.0);
    scene.addLight(light);

    return scene;
}


void SceneMaker::FbxToMyScene(FbxScene* fbxScene,SceneManager& scene){



    FbxNode* rootNode = fbxScene->GetRootNode();

    cout << rootNode->GetName() << endl;

    cout << "GetMaterial" << endl;
    int materialCount = fbxScene->GetMaterialCount();
    for(int i=0;i<materialCount;i++){
        FbxSurfaceMaterial* material = fbxScene->GetMaterial(i);
        if(material->GetClassId().Is(FbxSurfacePhong::ClassId)){

            cout << "Phong Material" << endl;
            FbxSurfacePhong* phongMaterial = (FbxSurfacePhong*)material;

            shared_ptr<Material> mat(new Material);
            for(int m = 0;m<3;m++){

                mat->Ka[m] = phongMaterial->Ambient.Get()[m];
                mat->Kd[m] = phongMaterial->Diffuse.Get()[m];
                mat->Ks[m] = phongMaterial->Specular.Get()[m];
            }
            mat->shininess = phongMaterial->Shininess;

            cout << "Material Info :" << endl;
            cout << "ambient (" << mat->Ka[0] << "," << mat->Ka[1] << "," << mat->Ka[2] << endl;
            cout << "diffuse (" << mat->Kd[0] << "," << mat->Kd[1] << "," << mat->Kd[2] << endl;
            cout << "specular (" << mat->Ks[0] << "," << mat->Ks[1] << "," << mat->Ks[2] << endl;


            resourceManager->materials.push_back(mat);

        }else if(material->GetClassId().Is(FbxSurfaceLambert::ClassId)){
            cout << "Lambert Material" << endl;

            FbxSurfaceLambert* lambertMaterial = (FbxSurfaceLambert*)material;
            shared_ptr<Material> mat(new Material);
            for(int m=0;m<3;m++){
                mat->Ka[m] = lambertMaterial->Ambient.Get()[m];
                mat->Kd[m] = lambertMaterial->Diffuse.Get()[m];
                mat->Ks[m] = 0;
            }
            mat->shininess = 0;

            cout << "Material Info :" << endl;
            cout << "ambient (" << mat->Ka[0] << "," << mat->Ka[1] << "," << mat->Ka[2] << endl;
            cout << "diffuse (" << mat->Kd[0] << "," << mat->Kd[1] << "," << mat->Kd[2] << endl;
            cout << "specular (" << mat->Ks[0] << "," << mat->Ks[1] << "," << mat->Ks[2] << endl;


            resourceManager->materials.push_back(mat);
        }

    }




    cout << "Show Attibute Data" << endl;
    PrintAttributeData(rootNode); 

    cout << "MakePrimitives" << endl;
    RecursiveMakeMesh(rootNode,scene);

}

void SceneMaker::RecursiveMakeMesh(FbxNode* node,SceneManager& scene){
    cout << "Nodename : " << node->GetName() << endl;

    for(int j=0;j<node->GetNodeAttributeCount();j++){
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(j);
        if(attribute->GetAttributeType() == FbxNodeAttribute::eMesh){
            shared_ptr<Primitive> prim = meshToPrimitive( (FbxMesh*)attribute,scene.camera);
            scene.primitives.push_back(prim);
        }
    }

    if(node->GetChildCount() <= 0){
        cout << "Recurse end" << endl;
        return;
    }

    int childCount = node->GetChildCount();
    cout << "Show Child nums : " << childCount << endl;

    for(int i=0;i<node->GetChildCount();i++){
        RecursiveMakeMesh(node->GetChild(i),scene);

    }
}


shared_ptr<Primitive> SceneMaker::meshToPrimitive(FbxMesh* mesh,Camera& camera){
    shared_ptr<Primitive> prim(new Primitive(&camera));
    prim->material = resourceManager->materials[resourceManager->materials.size()-1];
    prim->texture = resourceManager->textures[1].get();

    prim->transform.scale = glm::vec3(0.3,0.3,0.3);
    prim->transform.position = glm::vec3(0,0,-10);

    // 頂点を取得
    int vertexNum = mesh->GetControlPointsCount();
    FbxDouble4* vertexs = mesh->GetControlPoints();

    // 法線を見るためにレイヤーをチェック
    int layerNum = mesh->GetLayerCount();
    Logger::Log("layerNum : " + std::to_string(layerNum));
    FbxGeometryElementNormal* normalElem = mesh->GetElementNormal();

    if(normalElem != 0){

        FbxGeometryElement::EMappingMode mappingMode = normalElem->GetMappingMode();
        FbxGeometryElement::EReferenceMode refMode = normalElem->GetReferenceMode();

        // マッピングモードとリファレンスモードそれぞれで場合分けしてメッシュを取得する
        // ポリゴンとは別のコントロールポイントに格納されている場合
        if( mappingMode == FbxGeometryElement::eByControlPoint){
            Logger::Log("mappingMode : eByControlPoint");

            for(int vertexIndex=0;vertexIndex<mesh->GetControlPointsCount();vertexIndex++){

                int normalIndex = 0;
                if(refMode == FbxGeometryElement::eDirect){
                    normalIndex = vertexIndex;
                }
                if(refMode == FbxGeometryElement::eIndexToDirect){
                    normalIndex = normalElem->GetIndexArray().GetAt(vertexIndex);
                }
                FbxVector4 normal = normalElem->GetDirectArray().GetAt(normalIndex);
                for(int k = 0;k<3;k++) prim->normalData.push_back(normal[k]);
            }

            // ポリゴンごとに法線が格納されている
        }else if(mappingMode == FbxGeometryElement::eByPolygonVertex){
            Logger::Log("mappingMode : eByPolygonVertex");

            int indexByPolygonVertex = 0;

            for(int polygonIndex=0;polygonIndex < mesh->GetPolygonCount();polygonIndex++){

                // ポリゴン頂点のサイズ
                int polygonSize = mesh->GetPolygonSize(polygonIndex);

                for(int i=0;i<polygonSize;i++){
                    int normalIndex = 0;

                    if(refMode == FbxGeometryElement::eDirect){
                        normalIndex = indexByPolygonVertex;
                    }
                    if(refMode == FbxGeometryElement::eIndexToDirect){
                        normalIndex = normalElem->GetIndexArray().GetAt(indexByPolygonVertex);
                    }
                    FbxVector4 normal = normalElem->GetDirectArray().GetAt(normalIndex);
                    for(int k=0;k<3;k++) prim->normalData.push_back(normal[k]);
                }
            }

        }

    }

    for(int i=0;i<vertexNum;i++){
        for(int j=0;j<3;j++){
            prim->positionData.push_back(vertexs[i][j]);
            prim->colorData.push_back(0.5);
            // prim->normalData.push_back(vertexs[i][j]);
        } 

    }cout << endl;
    for(int i=0;i<vertexNum;i++){
        prim->uvData.push_back(0);
        prim->uvData.push_back(0);
    }


    int polygonNum = mesh->GetPolygonCount();
    int polygonVertexNum = mesh->GetPolygonVertexCount();
    int* indexarray = mesh->GetPolygonVertices();
    for(int p=0;p<polygonNum;p++){
        int indexNumInPolygon = mesh->GetPolygonSize(p);
        for(int n=0;n<indexNumInPolygon;n++){
            prim->indexData.push_back(mesh->GetPolygonVertex(p,n));
        }
    }


    cout << "meshToPrimitive" << endl;

    return prim;

}
