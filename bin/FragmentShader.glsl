#version 410 core

struct LightInfo{
    vec4 position;
    vec3 La;    // アンビエント強度RGB
    vec3 Ld;
    vec3 Ls;
};
uniform LightInfo Light;

uniform MaterialInfo{
    vec3 Ka;    // アンビエント反射率
    vec3 Kd;    // ディフューズ反射率
    vec3 Ks;    // スペキュラ反射率
    float shininess;    // スペキュラ輝き係数
};

// ビュー座標に統一
in vec4 fragmentPosition;
in vec4 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;

uniform sampler2D textureSampler;

out vec4 color;


// フォーンモデルに基づくADS計算
vec3 phongADS(vec4 pos, vec3 norm){

    norm = normalize(norm);
    // s:positionから見た光の方向　r:光の反射方向　v:視点方向
    vec3 s = normalize(Light.position.xyz - pos.xyz);
    vec3 r = reflect(-s,norm);
    vec3 v = normalize(-pos.xyz);

    vec3 ambient = Light.La * Ka;

    float SdotN = max( dot(s,norm) , 0 );
    vec3 diffuse = Light.Ld * Kd * SdotN;

    vec3 spec = vec3(0.0);
    if(SdotN > 0.0)
        spec = Light.Ls * Ks * pow( max(dot(r,v),0.0), shininess );

    return ambient + diffuse + spec;

}

void main(){
    vec3 lightIntensity = phongADS(fragmentPosition,fragmentNormal);
    color.rgb = (lightIntensity + vec3(0.2)) * texture(textureSampler,fragmentUV).xyz;
}
