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
// namespaceを書いておくと "glm::vec3" と書く代わりに "vec3" と書ける
using namespace glm;

// シェーダを読み込むメソッドをインクルード
#include <common/shader.hpp>

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
    
    // VAO(Vertex Array Object)を作って、現在のものとしてセット
    // Vertexは日本語だと頂点という意味
    // OpenGLコンテキストが作られた後、そして描画前にする
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID); // 作成？
    glBindVertexArray(VertexArrayID); // セット？
    
    // GLSLのプログラムを作成してコンパイルする
    GLuint programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");
    
    // 3つの頂点を表す3つのベクトル。つないだら三角形になる。
    // Xは右方向が正、Yは上方向が正、Zは手前方向（自分の後ろの方向）が正
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, // 1つ目
         1.0f, -1.0f, 0.0f, // 2つ目
         0.0f,  1.0f, 0.0f  // 3つ目
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

	do{
        // クリア（画面を全部ある色で塗る操作）
		glClear(GL_COLOR_BUFFER_BIT);
        
        // コンパイルしたシェーダプログラムを利用する
        glUseProgram(programID);

		// index 0 の頂点属性配列（vertex attribute array）を有効化する？意味分かんない。
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0, // シェーダーのlayoutと同じ値にする
                              3, // サイズ。頂点の数？
                              GL_FLOAT,  // タイプ。頂点のデータ型？
                              GL_FALSE, // [-1, 1]への正規化をするか
                              0, // 属性の各セットがどれくらい長さなのかをOpenGLに伝える。0の場合は最小（3 * sizeof(GL_FLOAT)かな？）
                              (void*) 0 // オフセットは0なので無し。普通に最初からデータを読む
                              );
        // 三角形を書く！
        // 頂点0から3つ描く
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // 頂点属性配列（vertex attribute array）を無効化する。おまじないにしか見えない。
        glDisableVertexAttribArray(0);

		// バッファをスワップする。バッファとは？スワップは入れ替えるってことだよな。
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // ESCキーが押されたらループを抜ける
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    // 後処理。VertexBuffer、VertexArray、プログラムを削除
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

	// OpenGLのウィンドウを閉じ、GLFWも終了する
	glfwTerminate();

	return 0;
}

