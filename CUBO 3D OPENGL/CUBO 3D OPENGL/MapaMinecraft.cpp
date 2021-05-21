#include <glutil.h>
#include <figures.h>
#include <vaos.h>
#include <camara.h>

#include<cstdio>
#include <random>

//#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Texture.h"
using namespace std;
//*****************************************
constexpr ui32 MAX_X{ 70 };
constexpr ui32 MAX_Z{ 70 };
//constexpr uint MAX_HEIGHT{100};
constexpr ui32 NUMBER_OF_ITERATIONS{8};
#include "terreno.h"
double terrain[MAX_X][MAX_Z];
std::vector<glm::vec4> transition;
//******************************************
const ui32 FSIZE = sizeof(f32);
const ui32 ISIZE = sizeof(i32);
const ui32 SCR_WIDTH = 960;
const ui32 SCR_HEIGHT = 540;
const f32  ASPECT = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

Camara* camara = new Camara();
bool firstMouse = true;
f32 yaw = -90.0f;
f32 pitch = 0.0f;
f32 lastX = SCR_WIDTH / 2.0f;  //Posicionando el mouse su inicio
f32 lastY = SCR_HEIGHT / 2.0f;
f32 fov = 45.0f; //Angulo de inclinacion que tendremos y cambiara nuestra pespectiva
f32 sensitivity = 0.1f;

f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;


/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camara->inputprocces(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camara->inputprocces(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camara->inputprocces(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camara->inputprocces(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camara->inputprocces(GOUP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camara->inputprocces(GODOWN, deltaTime);
	}
}
void mouse_Callback(GLFWwindow* window, f64 xpos, f64 ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	f32 xoffset = xpos - lastX; // cuanto se ha movido en X 
	f32 yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	camara->mouse_proccess(xoffset, yoffset);
}
/*f32 top[]={
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0, 0.0, 0.0f, 0.0f,  // 8
	0.5f, 0.5f, 0.5f, 1.0f, 0.0, 0.0, 1.0f, 0.0f,  // 9
	-0.5f, -0.5f, 0.5f, 1.0f, 0.0, 0.0, 0.0f, 0.0f,  // 10
	0.5f, -0.5f, 0.5f, 1.0f, 0.0, 0.0, 1.0f, 0.0f,  // 11
	-0.5f, 0.5f, -0.5f, 1.0f, 0.0, 0.0, 0.0f, 1.0f,  // 12
	0.5f, 0.5f, -0.5f, 1.0f, 0.0, 0.0, 1.0f, 1.0f,  // 13
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0, 0.0, 0.0f, 1.0f,  // 14
	0.5f, -0.5f, -0.5f, 1.0f, 0.0, 0.0, 1.0f, 1.0f }; // 15*/

void scroll_Callback(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
	camara->scroll_mouse(xoffset, yoffset);
}

i32 main() {
	generate_terrain<MAX_X, MAX_Z>(terrain, NUMBER_OF_ITERATIONS);
	generate_transitions<MAX_X, MAX_Z>(terrain, transition);

	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Terrain?", mouse_Callback, scroll_Callback);

	Shader* shader = new Shader();

	Shader* shader_tierra = new Shader("bin", "resources/textures", "shader.vert", "shader_tierra.frag");

	Cube* cubex = new Cube();
	//(i - g / 2.0f)Cube* cubito = new Cube();
	
	//ui32 o = 5;

	/*std::vector<glm::vec3> positions(o*o*o);
	for (ui32 i=0; i < o; ++i)
	{
		for( ui32 j=0;j<o;++j)
		{
			for (ui32 h = 0; h < o; ++h)
			{
				f32 x = i - o / 2.0f;
				f32 y = j - o/ 2.0f;
				f32 z = h - o / 2.0f;
				positions[(i * o * o) + (j * o) + h] = glm::vec3(x, y, z);
				std::cout << x << "\t" << y << "\t" << z << "\n";
			}
		}
	}*/

	ui32 g = 1;
	std::vector<glm::vec3> positions_tierra(g * g);//PINTA
	for (ui32 i = 0; i < g; ++i)
	{
		for (ui32 j = 0; j < g; ++j)
		{
			f32 x = (i - g / 2.0f);
			f32 z = (i - j / 2.0f);
			f32 y = 8.0;/*x*x / 10 - z*z / 10;*/
			//i - n/2.0f,pow((i-2/2.0f),2)/10- pow((j - 2 / 2.0f), 2) / 10, j - n/2.0f
			positions_tierra[i * g + j] = glm::vec3(x, y, z);
		}
	}

	VAO* vao_cubo = new VAO(cubex);
	VAO* vao_cubo_light = new VAO(cubex);

	vao_cubo->inicializar_vaos(true, true);
	vao_cubo_light->inicializar_vaos(true, true);

	ui32 texture1 = shader->loadTexture("arena.jpg", texture1);
	ui32 texture2 = shader->loadTexture("tierra.jpg", texture2);
	ui32 texture3 = shader->loadTexture("roca.jpg", texture3);
	ui32 texture4 = shader->loadTexture("cesped.jpg", texture4);

	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//glBindTexture(GL_TEXTURE_2D, texture1);
		
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Solo mostrara como lineas 
		shader_tierra->useProgram();
		

		glm::mat4 projection = glm::perspective(glm::radians(camara->getFov()), ASPECT, 0.1f, 100.0f);

		shader_tierra->setMat4("proj", projection);

		shader_tierra->setMat4("view", camara->getViewM4());

		glBindVertexArray(vao_cubo_light->get_VAO());
		
		for (ui32 i = 0; i < positions_tierra.size(); ++i) {  // Importantisimo para la generacion de los terrenos
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions_tierra[i]);
			shader_tierra->setMat4("model", model);

			glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
			
			}		
		
		shader->useProgram();
		shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader->setVec3("lightColorr", 1.0f, 1.0f, 1.0f);
		//glm::mat4 projection = glm::perspective(glm::radians(camara->getFov()), ASPECT, 0.1f, 100.0f);
		shader->setMat4("proj", projection);
		shader->setMat4("view", camara->getViewM4());

		glBindVertexArray(vao_cubo->get_VAO());
		for (auto& vec : transition)
		{
			if(vec.y<=0.3)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture2);
			}
			else if(vec.y>=0.4 && vec.y<0.8)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture3);
			}
			else if(vec.y>0.8 && vec.z>0.5)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture4);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture3);
			}
			glm::mat4 model = glm::mat4(1.0f);
			//std::cout << vec.x << ' ' << vec.y << " " << vec.z << "\n";
			model = glm::translate(model, glm::vec3(vec.x, vec.y, vec.z) * glm::vec3(2.0f, 2.0f, 2.0f));
			shader->setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);

			/*if(vec.w==1)
			{
				//glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textura_side);
				glBindVertexArray(VAOside);
				glDrawArrays(GL_TRIANGLES, 0, 24);

				//glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, top_Texture);
				glBindVertexArray(VAOtop);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else
			{
				//glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, dirtTexture);
				glBindVertexArray(VAOdirt);
				glDrawArrays(GL_TRIANGLES, 0, 36);

			}*/
			/*if (vec.w == 1) {
				glActiveTexture(GL_TEXTURE0);
				sideTexture.bind(GL_TEXTURE_2D);

				glBindVertexArray(VAOside);
				glDrawArrays(GL_TRIANGLES, 0, 24);

				glActiveTexture(GL_TEXTURE0);
				topTexture.bind(GL_TEXTURE_2D);

				glBindVertexArray(VAOtop);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				dirtTexture.bind(GL_TEXTURE_2D);

				glBindVertexArray(VAOdirt);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}*/
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	delete shader_tierra;
	delete shader;

	cubex->~Cube();
	//cubito->~Cube();
	return 0;
}