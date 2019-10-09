#version 330 core

// vertex shaderからの入力
in vec2 UV;

// out は出力を表す
out vec3 color;

// テクスチャ
// uniformはすべてのメッシュで一定の値
uniform sampler2D myTextureSampler;

void main() {
    // 指定したUV座標のテクスチャの色
    vec2 flipped = vec2(UV.x, 1.0 - UV.y);
    color = texture(myTextureSampler, flipped).rgb;
}
