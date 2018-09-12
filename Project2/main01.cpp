#include "include\GL\glew.h"
#include "include\GLFW\glfw3.h"
#include "glm\glm.hpp"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment (lib,"lib/Release/Win32/glew32.lib")
#pragma comment (lib,"lib/Release/Win32/glew32s.lib")
#pragma comment (lib,"lib-vc2015/glfw3.lib")
#pragma comment (lib,"lib-vc2015/glfw3dll.lib")

GLFWwindow* window;
using namespace glm;

void main()
{
	// GLFWを初期化
	if (!glfwInit())
	{
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ウィンドウを開き、OpenGLコンテキストを作る
	window = glfwCreateWindow(1024, 768, "Tutorial 01", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// コアプロファイルで必要
	glewExperimental = true; 

	// GLEWを初期化
	if (glewInit() != GLEW_OK)
	{
		return;
	}

	// エスケープキーの押下の確認を保証
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//頂点レイアウト作成
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);//第一引数分の要素分の頂点レイアウト配列を作成
	glBindVertexArray(VertexArrayID);	 //VertexArrayIDのレイアウトメンバを作成


	//glBindVertexArray(0);				//VertexArrayIDレイアウトメンバ作成完了
	

	//頂点情報を頂点バッファをする
	static const GLfloat g_vertex_buffer_data[] = 
	{
	    1.0f,1.0f, 0.0f,
		0.0f,-1.0f, 0.0f,
		-1.0f,1.0f, 0.0f,
	};

	GLuint vertexbuffer;	//頂点バッファオブジェクト
	glGenBuffers(1, &vertexbuffer);//第一引数の要素分の配列を作成
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),g_vertex_buffer_data, GL_STATIC_DRAW);


	do
	{
		//背景色の設定
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//頂点レイアウトのメンバの有効設定
		glEnableVertexAttribArray(0);	//0：　頂点

		//転送する頂点バッファ指定
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//転送するバッファのメンバの数と型を設定し0番として送る
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

		// 三角形を描く
		glDrawArrays(GL_TRIANGLES, 0, 3); // 0から始まる3個の頂点　→　1個の三角形


		//頂点レイアウトのメンバの無効設定
		glDisableVertexAttribArray(0);



		// バッファを交換する
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // ESCキーが押されたか、ウィンドウが閉じられたかを確認する
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// VBOを片付ける
	glDeleteBuffers(1, &vertexbuffer);		//バッファ破棄
	glDeleteVertexArrays(1, &VertexArrayID);//レイアウト情報破棄
	

	// OpenGLウィンドウを閉じ、GLFWを終了する
	glfwTerminate();

	return;

}