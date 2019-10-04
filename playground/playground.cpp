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

	// 青く塗る
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
	do{
		glClear(GL_COLOR_BUFFER_BIT);

		// 何も描かない

		// バッファをスワップする。バッファとは？スワップは入れ替えるってことだよな。
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // ESCキーが押されたらループを抜ける
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// OpenGLのウィンドウを閉じ、GLFWも終了する
	glfwTerminate();

	return 0;
}

