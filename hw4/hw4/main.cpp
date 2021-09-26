#define GLM_ENABLE_EXPERIMENTAL

#include "Object.h"
#include "FreeImage.h"
#include "glew.h"
#include "freeglut.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <stb_image.h>

using namespace std;

void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);
void DrawUmbreon(GLuint);
void LoadTexture(unsigned int&, const char*);

bool stop = false;
bool back = false;
GLfloat type = 0.0f;
GLfloat gloss = 0.0f;
GLfloat exp_gloss = 0.0f;
GLfloat b_exp_gloss = 0.0f;
GLfloat disappear_gloss = 0.0f;
GLuint Normalprogram, Umbreonprogram, Explodeprogram,shadertype, Normal_expprogram, Normal_exp_backprogram,disappear;
GLuint VAO, VBO[3];
GLfloat speed = 0.25f;
GLfloat sec = 0.0f;

unsigned int modeltexture;
float windowSize[2] = { 600, 600 };
float angle = 0.0f;
glm::vec3 WorldLightPos = glm::vec3(2, 5, 5);
glm::vec3 WorldCamPos = glm::vec3(7.5, 5.0, 7.5);
glm::mat4 storeM(1.0f);
glm::mat4 M(1.0f);

Object* model = new Object("Umbreon.obj");


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glEnable(GL_BLEND);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw4");

	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void shaderInit() {
	GLuint vert = createShader("Shaders/normal.vert", "vertex");
	GLuint goem = createShader("Shaders/normal.geom", "geometry");
	GLuint frag = createShader("Shaders/normal.frag", "fragment");
	Normalprogram = createProgram(vert, goem,frag);

	vert = createShader("Shaders/Umbreon.vert", "vertex");
	frag = createShader("Shaders/Umbreon.frag", "fragment");
	Umbreonprogram = createProgram(vert, 0, frag);

	vert = createShader("Shaders/normal_exp.vert", "vertex");
	goem = createShader("Shaders/normal_exp.geom", "geometry");
	frag = createShader("Shaders/normal_exp.frag", "fragment");
	Normal_expprogram = createProgram(vert, goem, frag);

	vert = createShader("Shaders/normal_exp_back.vert", "vertex");
	goem = createShader("Shaders/normal_exp_back.geom", "geometry");
	frag = createShader("Shaders/normal_exp_back.frag", "fragment");
	Normal_exp_backprogram = createProgram(vert, goem, frag);

	vert = createShader("Shaders/explode.vert", "vertex");
	goem = createShader("Shaders/explode.geom", "geometry");
	frag = createShader("Shaders/explode.frag", "fragment");
	Explodeprogram = createProgram(vert, goem, frag);

	vert = createShader("Shaders/disappear.vert", "vertex");
	goem = createShader("Shaders/disappear.geom", "geometry");
	frag = createShader("Shaders/disappear.frag", "fragment");
	disappear = createProgram(vert, goem, frag);

	shadertype = Normal_expprogram;
}

void bindbufferInit() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->positions[0]) * model->positions.size(), &model->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->normals[0]) * model->normals.size(), &model->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->texcoords[0]) * model->texcoords.size(), &model->texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void textureInit() {
	LoadTexture(modeltexture, "Umbreon.jpg");
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 getP()
{
	// set perspective view
	float fov = 45.0f;
	float aspect = windowSize[0] / windowSize[1];
	float nearDistance = 1.0f;
	float farDistance = 1000.0f;
	return glm::perspective(glm::radians(fov), aspect, nearDistance, farDistance);
}

void display() {
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawUmbreon(shadertype);
	//DrawUmbreon(Umbreonprogram);

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void idle() {
	glutPostRedisplay();
}

void DrawUmbreon(GLuint program)
{
	glUseProgram(program);


	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	/*if (sec > 50 && sec < 150) {
		shadertype = Normalprogram;
		if (!stop) {
			M = glm::translate(M, glm::vec3(-0.0025 * speed, 0, 0.0025 * speed));
		}
	}
	else if(sec > 150 && sec < 300){
		if (!stop) {
			M = glm::translate(M, glm::vec3(0.0025 * speed, 0, -0.0025 * speed));
		}
	}
	else if (sec > 300 && sec < 350) {
		if (!stop) {
			M = glm::translate(M, glm::vec3(-0.0025 * speed, 0, 0.0025 * speed));
		}
	}
	else if (sec == 350){
		M = storeM;
	}
	else if(sec>350){
		M = glm::translate(M, glm::vec3(0.00075 * speed, 0.0005 * speed, 0.00075 * speed));
		shadertype = Explodeprogram;
	}*/
	//

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	glm::mat4 V = getV();
	ModelMatrixID = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &V[0][0]);

	glm::mat4 P = getP();
	ModelMatrixID = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &P[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);


	if (stop == false)
		sec += 0.01;

	if (stop == false && type == 1){
		if (!back) {
			gloss += 0.001;
		}
		else {
			if (gloss > 0 && back) {
				gloss -= 0.001;
			}
		}
	}
	if (type == 2 && sec < 100 && !stop) {
		//M = glm::translate(M, glm::vec3(0.0015 * speed, 0.00 * speed, 0.0015 * speed));
		shadertype = Umbreonprogram;
		exp_gloss = 0;
	}

	if (stop == false && type == 2) {
		if (angle < 360) {
			angle += 0.0001 * pow(2, angle);
		}
	}

	if (stop == false && type == 2 && sec>100 && !stop) {
		shadertype = Explodeprogram;
		exp_gloss += 0.001;
	}

	if (!stop && type == 3) {
		disappear_gloss += 0.0001;
	}

	GLuint coefficientID;

	coefficientID = glGetUniformLocation(program, "gloss");
	glUniform1fv(coefficientID, 1, &gloss);

	coefficientID = glGetUniformLocation(program, "exp_gloss");
	glUniform1fv(coefficientID, 1, &exp_gloss);

	coefficientID = glGetUniformLocation(program, "disappear_gloss");
	glUniform1fv(coefficientID, 1, &disappear_gloss);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0,  3*model->fNum);
	glBindVertexArray(0);
	glActiveTexture(0);
	glUseProgram(0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1':	//case 1 三角形沿法向量爆炸
	{
		if (type != 1) {
			M = storeM;
		}
		angle = 0.1f;
		shadertype = Normal_expprogram;
		type = 1;
		back = false;
		break;
	}
	case '2':	//case 2 三角形沿法向量吸回來
	{
		if (type != 1) {
			M = storeM;
		}
		shadertype = Normal_exp_backprogram;
		type = 1;
		back = true;
		break;
	}
	case '3':	//case 3 高速旋轉後粒子爆炸
	{
		angle = 0.001f;
		M = storeM;
		back = false;
		type = 2;
		sec = 0.0f;
		shadertype = Explodeprogram;
		break;
	}
	case '4':	//case 4 隨時間點漸漸消失
	{
		M = storeM;
		angle = 0.1f;
		back = false;
		type = 3;
		sec = 0.0f;
		shadertype = disappear;
		break;
	}
	case '5':	//case 4 恢復初始化
	{
		sec = 0.0f;
		angle = 0.0f;
		M = storeM;
		gloss = 0.0f;
		type = 0;
		exp_gloss = 0.0f;
		disappear_gloss = 0.0f;
		shadertype = Umbreonprogram;
		break;
	}
	case 32:
		stop = not stop;
	default:
		break;
	}
}


