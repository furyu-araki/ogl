// 標準ヘッダをインクルード
#include <stdio.h>
#include <stdlib.h>

// GLEWをインクルード。gl.hとglfw.hより先にインクルードする魔法の言葉らしい。
// ググったらOpenGL Extension Wrangler Libraryとあったので、とりあえずライブラリなんだろう。
#include <GL/glew.h>

// ウィンドウとキーボードを使うためにGLFWをインクルード
#include <GLFW/glfw3.h>
GLFWwindow* window;

// GLM（3D数学のライブラリ）をインクルード。めっちゃ便利
#include <glm/glm.hpp>
// 変形系の行列計算ができる
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
// namespaceを書いておくと "glm::vec3" と書く代わりに "vec3" と書ける
using namespace glm;

// シェーダを読み込むメソッドをインクルード
#include <common/shader.hpp>

// テクスチャを読み込むメソッドをインクルード
#include <common/texture.hpp>

int main( void )
{
	// GLFWを初期化する
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x アンチエイリアス
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE); // OpenGL3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac用の設定
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 古いOpenGLは使わないと書いてあったが、よくわからん

	// ウィンドウを開いて、OpenGLコンテキストを作る。（コンテキストってなんだろう？）
    // サイズとタイトル
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
    // GLEWを初期化と書いてあったけど、GLFWじゃないのか？
	glfwMakeContextCurrent(window);

    // コアプロファイルで必要となりますと書いてあった。コアプロファイルとは？
    glewExperimental=true;
	// GLEWの初期化
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// キーボードタップをハンドリングするために必要っぽい
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// クリア（画面を全部ある色で塗る操作）するときの色は青
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // デプステストを有効にする。デプステストに不合格なものは映らない
    glEnable(GL_DEPTH_TEST);
    // 前のものよりもカメラに近ければ合格とする
    glDepthFunc(GL_LESS);
    
    // VAO(Vertex Array Object)を作って、現在のものとしてセット
    // Vertexは日本語だと頂点という意味
    // OpenGLコンテキストが作られた後、そして描画前にする
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID); // 作成？
    glBindVertexArray(VertexArrayID); // セット？
    
    // GLSLのプログラムを作成してコンパイルする
    GLuint programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");
    
    // Matrixをシェーダに渡すための準備
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    // 射影行列。45度の画角、アスペクト比4:3、表示範囲0.1~100
    mat4 Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // カメラ行列
    mat4 View = lookAt(
                       vec3(4, 3, 3), // カメラの場所
                       vec3(0, 0, 0), // 原点を見る
                       vec3(0, 1, 0)  // 頭が上方向
                       );
    // モデル行列
    mat4 Model = mat4(1.0f);
    mat4 MVP = Projection * View * Model;
    
    // テクスチャを読み込む
    GLuint Texture = loadDDS("uvtemplate.DDS");
    
    // テクスチャをシェーダにわたす準備
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
    
    
    // 三角形を12枚描いている。3頂点が12個
    // Xは右方向が正、Yは上方向が正、Zは手前方向（自分の後ろの方向）が正
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // 三角形1:開始
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // 三角形1:終了
         1.0f, 1.0f,-1.0f, // 三角形2:開始
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // 三角形2:終了
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f
    };
    
    // UV座標のデータ。ブレンダーで作ったやつ
    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };
    
    // バッファを表す。
    // バッファは、データを一時的に保持する場所という認識でOKかな？
    GLuint vertexbuffer;
    // バッファを作る。
    // vertexbufferのアドレスを引数として入れている。
    // これによってvertexbufferが生成されたバッファを表す？vertexbufferにバッファのIDが入る
    glGenBuffers(1, &vertexbuffer);
    // これ以降のコマンドはvertexbufferに関するものだよという宣言
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // 頂点のデータをOpenGLに渡す
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    // UV座標用のバッファ
    GLuint uvbuffer;
    // バッファを作る
    glGenBuffers(1, &uvbuffer);
    // これ以降のコマンドはuvbufferに関するもの
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    // UV座標用バッファのデータをOpenGLに渡す
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	do{
        // クリア（画面を全部ある色で塗る操作、デプスもクリアする）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // コンパイルしたシェーダプログラムを利用する
        glUseProgram(programID);
        
        // Matrixをシェーダに送る
        // 2次元の行列だから、[0][0]で行列の最初の要素のアドレスを渡している？
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// index 0（1つ目）の頂点属性配列（vertex attribute array）を有効化する
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0, // シェーダーのlayoutと同じ値にする
                              3, // 1頂点のサイズ。頂点がvec3で表されるので3。
                              GL_FLOAT,  // タイプ。頂点のデータ型。
                              GL_FALSE, // [-1, 1]への正規化をするか
                              0, // 属性の各セットがどれくらい長さなのかをOpenGLに伝える。0の場合は最小（3 * sizeof(GL_FLOAT)かな？）。後ろに頂点座標以外のデータが来る場合に使う？
                              (void*) 0 // オフセットは0なので無し。普通に最初からデータを読む
                              );
        
        // index 1（2つ目）の属性バッファはUV座標のやつ
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,                                // シェーダー側のlayoutとと合わせる。UV用は1とした。
            2,                                // size。UVはUとVのvec2なので2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        
        // 三角形を書く！
        // 頂点0から頂点36(12*3)まで描く
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        // 頂点属性配列（vertex attribute array）を無効化する。おまじないにしか見えない。
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

		// バッファをスワップする。バッファとは？スワップは入れ替えるってことだよな。
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // ESCキーが押されたらループを抜ける
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    // 後処理。VertexBuffer、VertexArray、プログラム、テクスチャを削除
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);

	// OpenGLのウィンドウを閉じ、GLFWも終了する
	glfwTerminate();

	return 0;
}

void matrix() {
    // 平行移動
    mat4 myMatrix = translate(mat4(), vec3(10.0f, 0.0f, 0.0f));
    vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
    vec4 transformedVector = myMatrix * myVector;
    
    // 単位行列。ベクトルに掛けても何も起きない。数字の1的な存在
    mat4 myIdentityMatrix = mat4(1.0f);
    
    // 拡大縮小行列
    mat4 myScalingMatrix = scale(vec3(2.0f, 2.0f, 2.0f));
    
    // 回転行列
    mat4 myRotatingMatrix = rotate(90.f, vec3(1.0 , 0.0, 0.0));
    
    // 拡大縮小して、回転して、平行移動する
    mat4 allTransformingMatrix = myMatrix * myRotatingMatrix * myScalingMatrix;
    vec4 allTransformedVector = allTransformingMatrix * myVector;
}

void ModelViewProjection() {
    // 最初はModel Coodinates（モデル空間）
    // モデルを移動する
    mat4 ModelMatrix = translate(mat4(), vec3(10.0f, 0.0f, 0.0f));
    // World Coodinates（ワールド空間）になる
    
    // 世界を移動する
    mat4 ViewMatrix = translate(mat4(), vec3(-3.0f, 0.0f, 0.0f));
    // Camera Coodinates（カメラ空間）になる
    
    // カメラを移動する≒世界を移動するのと実質同じこと
    mat4 CameraMatrix = lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    // まだCamera Coodinates（カメラ空間）
    
    // 射影行列。z方向を考慮して描ける。x, yが同じ場合、カメラ空間のzが大きいほうが近いので大きく見える。
    // 要は、PCのディスプレイにどう映すかを表している？
    // 四角錐台を立方体にするイメージ。（本当はアスペクト比3:4なので直方体）
    mat4 ProjectionMatrix = perspective(
                                        radians(90.0f), // 画角？30~90くらいが普通。90だと超ワイド、30だと超ズーム
                                        4.0f / 3.0f, // アスペクト比
                                        0.1f, // 近くのクリッピング平面
                                        100.0f // 遠くのクリッピング平面
                                        );
    // Homogeneous Coodinates（同次空間）になる
    // 同次座標というのはn次元ベクトルをn+1次元ベクトルで表す的な意味があるっぽい。そうすると便利らしい。
    
    // すべて合わせる
    mat4 MVPmatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
    vec4 myVertex(10.0f, 10.0f, 10.0f, 1.0f);
    vec4 transformedVertex = MVPmatrix * myVertex;
}
