#include "include\GL\glew.h"
#include "include\GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp" 

#pragma comment(lib, "OpenGL32.lib")
#pragma comment (lib,"lib/Release/Win32/glew32.lib")
#pragma comment (lib,"lib/Release/Win32/glew32s.lib")
#pragma comment (lib,"lib-vc2015/glfw3.lib")
#pragma comment (lib,"lib-vc2015/glfw3dll.lib")

GLFWwindow* window;
using namespace glm;


//頂点バッファ（位置）
static const GLfloat g_vertex_pos_buffer_data[8][3] =
{
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f,-1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f },
	{ 1.0f,-1.0f,-1.0f },
	{ -1.0f, 1.0f,-1.0f },
	{ -1.0f,-1.0f,-1.0f },
};

//頂点バッファ(UV）
static const GLfloat g_vertex_uv_buffer_data[8][2] =
{
	{ 1.00000f, 1.00000f },
	{ 1.00000f, 0.00000f },
	{ 0.00000f, 1.00000f },
	{ 0.00000f, 0.00000f },
	{ 1.00000f, 1.00000f },
	{ 1.00000f, 0.00000f },
	{ 0.00000f, 1.00000f },
	{ 0.00000f, 0.00000f },
};

//頂点バッファ（カラー）
static const GLfloat g_vertex_collar_buffer_data[8][4] =
{
	{ 0.0f, 0.0f, 1.0f ,1.0f },
	{ 0.0f, 1.0f, 0.0f ,1.0f },
	{ 1.0f, 0.0f, 0.0f ,1.0f },
	{ 0.0f, 1.0f, 1.0f ,1.0f },
	{ 1.0f, 0.0f, 1.0f ,1.0f },
	{ 0.0f, 0.0f, 0.5f ,1.0f },
	{ 0.5f, 0.0f, 0.0f ,1.0f },
	{ 0.0f, 0.5f, 0.0f ,1.0f },
};

//インデックスバッファ
static const GLint g_index_buffer_data[12][3] =
{
	{ 2,3,1 },
	{ 2,1,0 },
	{ 4,5,3 },
	{ 4,3,2 },
	{ 6,7,5 },
	{ 6,5,4 },
	{ 0,1,7 },
	{ 0,7,6 },
	{ 4,2,0 },
	{ 4,0,6 },
	{ 3,5,7 },
	{ 3,7,1 },
};

//テクスチャデータ(テスト用)
static const GLbyte g_texture_data[4*4*3]=
{
	  0x00,0x00,0x00 , 0xFF,0xFF,0xFF , 0x00,0x00,0x00 , 0xFF,0xFF,0xFF ,
	  0xFF,0xFF,0xFF , 0x00,0x00,0x00 , 0xFF,0xFF,0xFF , 0x00,0x00,0x00 ,
	  0x00,0x00,0x00 , 0xFF,0xFF,0xFF , 0x00,0x00,0x00 , 0xFF,0xFF,0xFF ,
	  0xFF,0xFF,0xFF , 0x00,0x00,0x00 , 0xFF,0xFF,0xFF , 0x00,0x00,0x00 ,
};

//行列
glm::mat4 mat_world;		//ワールド変換行列
glm::mat4 mat_view;			//ビュー変換行列
glm::mat4 mat_projection;	//射影変換行列
glm::mat4 mat_wvp;			//ワールド・ビュー・射影合成行列

							//シェーダプログラムオブジェクト
GLuint g_ProgramID;

//バッファオブジェクト
GLuint g_vertexbuffer;		//頂点バッファオブジェクト
GLuint g_colorbuffer;		//カラーバッファオブジェクト
GLuint g_uvbuffer;			//UVバッファオブジェクト
GLuint g_indexbuffer;		//インデックスバッファオブジェクト




//頂点シェーダ
const char g_vertex_shader[] =
{
	//シェーダのバージョン
	"#version 330 core								\n"
	//CPUから輸送される頂点情報群
	"layout(location = 0) in vec3 pos;				\n"
	"layout(location = 1) in vec4 col;				\n"
	"layout(location = 2) in vec2 uv;				\n"

	//フラグメントシェーダに送る情報群
	"out vec4 fragmentColor;						\n"
	"out vec2 fragmentUv;							\n"

	//コンスタントグローバル情報
	"uniform mat4 MVP;								\n"

	"void main()									\n"
	"{												\n"
	"	gl_Position =MVP * vec4(pos,1);				\n"//ラスタライズ用頂点位置
	"	fragmentColor=col;							\n"
	"	fragmentUv   =uv;							\n"
	"}												\n"
};

//フラグメントシェーダ（ピクセルシェーダ）
const char g_fragment_shader[] =
{
	"#version 330 core										\n"
	"in  vec4 fragmentColor;								\n"
	"in  vec2 fragmentUv;									\n"
	"out vec4 color;										\n"
	"														\n"
	"uniform sampler2D Tex01;								\n"
	"														\n"
	"void main()											\n"
	"{														\n"
	"	color =fragmentColor;								\n"
	"	color*=vec4(texture2D(Tex01, fragmentUv ).rgb,1);	\n"
	"}														\n"
};

void main()
{
	// GLFWを初期化
	if (!glfwInit())
		return;

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
		return;

	// エスケープキーの押下の確認を保証
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//頂点レイアウトオブジェクト作成
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);//頂点レイアウトオブジェクトを作成
	glBindVertexArray(VertexArrayID);	 //設定する



	//モデルバッファオブジェクト作成---------
	//頂点バッファオブジェクト作成
	glGenBuffers(1, &g_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_pos_buffer_data), g_vertex_pos_buffer_data, GL_STATIC_DRAW);

	//カラーバッファオブジェクト作成
	glGenBuffers(1, &g_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_collar_buffer_data), g_vertex_collar_buffer_data, GL_STATIC_DRAW);

	//UVバッファオブジェクト作成
	glGenBuffers(1, &g_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_uv_buffer_data), g_vertex_uv_buffer_data, GL_STATIC_DRAW);

	//インデックスオブジェクト作成
	glGenBuffers(1, &g_indexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_indexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_STATIC_DRAW);
	//----------------------------------------

	//GLSLプログラムオブジェクト作成----------
	// GLSLプログラムをシェーダから作成、コンパイル
	g_ProgramID = glCreateProgram();//シェーダプログラム
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//頂点シェーダコンパイル
	char const* v = g_vertex_shader;
	glShaderSource(VertexShaderID, 1, &v, NULL);
	glCompileShader(VertexShaderID);

	//フラグメントシェーダコンパイル
	char const* f = g_fragment_shader;
	glShaderSource(FragmentShaderID, 1, &f, NULL);
	glCompileShader(FragmentShaderID);

	//頂点とフラグメントシェーダをプログラムにリンクする
	glAttachShader(g_ProgramID, VertexShaderID);
	glAttachShader(g_ProgramID, FragmentShaderID);
	glLinkProgram(g_ProgramID);

	//プログラムにしたのでシェーダは破棄
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// "MVP" uniformのハンドルを取得
	GLuint MatrixID = glGetUniformLocation(g_ProgramID, "MVP");
	
	// "Tex01"uniformハンドルを取得
	GLuint TextureID = glGetUniformLocation(g_ProgramID, "Tex01");
	glUniform1i(TextureID, 0);	//TextureIDをGL_TEXTURE0にセット
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, g_texture_data);

	//　サンプラセット
	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//-----------------------------------------

	//奥行きテストの有効化
	glEnable(GL_DEPTH_TEST);
	
	do
	{
		//各行列の計算

		//単位行列化
		mat_world = glm::mat4(1.0f);
		mat_view = glm::mat4(1.0f);
		mat_projection = glm::mat4(1.0f);
		mat_wvp = glm::mat4(1.0f);

		//カメラ行列作成
		static float f = 0.0f;
		f += 0.001f;
		glm::vec3 c_pos(cos(f)*3.0f, 2.0f, sin(f)*3.0f);
		glm::vec3 c_tar(0.0f, 0.0f, 0.0f);
		glm::vec3 c_vec(0.0f, 1.0f, 0.0f);
		mat_view = glm::lookAt(c_pos, c_tar, c_vec);

		//パーステクティブ行列作成
		mat_projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.01f, 100.0f);

		//ＷＶＰ行列作成
		mat_wvp = mat_projection *mat_view * mat_world;

		
		// シェーダを使う
		glUseProgram(g_ProgramID);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mat_wvp[0][0]);

		//テクスチャを使う
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		

		glBindSampler(0, sampler);

		//背景色の設定
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//layout(location = 0)に頂点を送る
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, g_vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//layout(location = 1)にカラーを送る
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, g_colorbuffer);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//layout(location = 2)にUVを送る
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, g_uvbuffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// インデックスバッファを元に三角形を描画
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexbuffer);
		glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);


		glDisableVertexAttribArray(0);
		// バッファを交換する
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // ESCキーが押されたか、ウィンドウが閉じられたかを確認する
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	//破棄
	glDeleteBuffers(1, &g_vertexbuffer);	//頂点バッファオブジェクト破棄
	glDeleteBuffers(1, &g_colorbuffer);		//カラーバッファオブジェクト破棄
	glDeleteBuffers(1, &g_indexbuffer);		//インデックスバッファオブジェクト破棄
	glDeleteVertexArrays(1, &VertexArrayID);//レイアウトオブジェクト破棄
	glDeleteProgram(g_ProgramID);			//シェーダプログラムオブジェクト破棄

	glfwTerminate();// OpenGLウィンドウを閉じ、GLFWを終了する

	return;

}