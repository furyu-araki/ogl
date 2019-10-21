#version 330 core

// vertex shaderからの入力
in vec2 UV;
in vec3 Position_worldspace; // ワールド空間での頂点の位置
in vec3 Normal_cameraspace; // カメラ空間での頂点の法線
in vec3 EyeDirection_cameraspace; // カメラ空間で、頂点からカメラに向かうベクトル
in vec3 LightDirection_cameraspace; // カメラ空間で、頂点から光に向かうベクトル

// out は出力を表す
out vec3 color;

// テクスチャ
// uniformはすべてのメッシュで一定の値
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

void main() {
    vec3 LightColor = vec3(1, 1, 1); // 光の色は白色
    float LightPower = 50.0f; // 光の強さ
    
    vec3 MaterialDiffuseColor = texture(myTextureSampler, UV).rgb; // 拡散光＝物体の色。テクスチャからもらう
    
    // 光源と物体の距離
    float distance = length( LightPosition_worldspace - Position_worldspace );
    
    // 頂点法線（カメラ空間）を正規化
    vec3 n = normalize( Normal_cameraspace );
    // 頂点から光へ向かうベクトル（カメラ空間）を正規化
    vec3 l = normalize( LightDirection_cameraspace );
    // 法線と光の方向の確度のコサイン
    // 0以上になるようにクランプする
    //  - 光が三角形に水平の場合 -> 1
    //  - 光が三角形に垂直の場合 -> 0
    //  - 光が三角形の裏側の場合 -> 負の値 -> 0以上にクランプされる -> 0
    float cosTheta = clamp( dot( n,l ), 0,1 );
    
    // Eye vector。カメラの方に向かう
    vec3 E = normalize(EyeDirection_cameraspace);
    // Reflect vector。三角形が光を反射する方向
    vec3 R = reflect(-l,n);
    // Eye vector と Reflect vectorがなす角度のコサイン
    // 0以上にクランプする
    //  - 反射する方向を見ている -> 1
    //  - それ以外 -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );
    
    color =
        // 拡散光＝物体の色
        MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance)
    ;
}
