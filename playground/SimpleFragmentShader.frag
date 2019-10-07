#version 330 core

// vertex shaderからの入力
in vec3 fragmentColor;

// out は出力を表す
out vec3 color;

void main() {
    // vertex shaderから渡された色をそのままセット
    color = fragmentColor;
}
