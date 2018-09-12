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

	glfwWindowHint(GLFW_SAMPLES, 4);	//線のアンチエリアシングの美しさ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//メジャー
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//マイナー
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL1.1以下は使用しない
	
	// ウィンドウを開き、OpenGLコンテキストを作る
	window = glfwCreateWindow(1024, 768, "Tutorial 01", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		return ;
	}
	// 指定したウィンドウのOpenGlまた
	glfwMakeContextCurrent(window);
	
	// GLEWを初期化
	if (glewInit() != GLEW_OK) 
	{
		return ;
	}
	
	// エスケープキーの押下の確認を保証
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	

	do 
	{
		// ダークブルーの背景
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// バッファを交換する
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // ESCキーが押されたか、ウィンドウが閉じられたかを確認する
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// OpenGLウィンドウを閉じ、GLFWを終了する
	glfwTerminate();

	return;
	
}