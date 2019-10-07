// OpenGL3を使うよ
#version 330 core
// layout(location = 0) はバッファを表す
// 0はglVertexAttribPointerの第一引数と同じ値。同じ値で12を超えなければ何でもいいらしい。
// このケースだとvertexPosition_modelspaceは、頂点座標を持つ。
// 各頂点ごとに処理が走り、そのたびにvertexPosition_modelspaceに頂点座標が入る？
// in は入力を表す
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 MVP;

void main() {
    // gl_Positionに結果が入る。今回はそのまま頂点座標を渡す
    // gl_Position.xyz = vertexPosition_modelspace;
    // gl_Position.w = 1.0;
    
    // 変換かける
    vec4 v = vec4(vertexPosition_modelspace, 1); // wを足す。位置ベクトルなので1
    gl_Position = MVP * v;
}
