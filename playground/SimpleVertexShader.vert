// OpenGL3を使うよ
#version 330 core
// layout(location = 0) はバッファを表す
// 0はglVertexAttribPointerの第一引数と同じ値。同じ値で12を超えなければ何でもいいらしい。
// このケースだとvertexPosition_modelspaceは、頂点座標を持つ。
// 各頂点ごとに処理が走り、そのたびにvertexPosition_modelspaceに頂点座標が入る？
// in は入力を表す
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

// fragment shaderへの出力
out vec2 UV;
out vec3 Position_worldspace; // ワールド空間での頂点の位置
out vec3 Normal_cameraspace; // カメラ空間での頂点の法線
out vec3 EyeDirection_cameraspace; // カメラ空間で、頂点からカメラに向かうベクトル
out vec3 LightDirection_cameraspace; // カメラ空間で、頂点から光に向かうベクトル

// uniformはすべてのメッシュで一定の値
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

void main() {
    // クリップ空間（？）での頂点の出力位置
    vec4 v = vec4(vertexPosition_modelspace, 1); // wを足す。位置ベクトルなので1
    gl_Position = MVP * v;
    
    // ワールド空間での頂点の位置、M*位置
    Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

    // カメラ空間で、頂点からカメラへ向かうベクトル
    // カメラ空間ではカメラは原点(0,0,0)にあります。
    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    // カメラ空間で、頂点から光へ向かうベクトル。Mは単位行列なので除外します。
    vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    // カメラ空間での、頂点の法線
    Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    
    // そのままフラグメントシェーダにわたすだけ
    UV = vertexUV;
}
