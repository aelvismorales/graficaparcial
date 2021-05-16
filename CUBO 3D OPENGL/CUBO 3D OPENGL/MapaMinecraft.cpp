#include <glutil.h>
#include <figures.h>
#include <vaos.h>
#include <camara.h>

#include "Figure.h"

#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

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

f32* makeVertices(ui32 xblocks, ui32 yblocks, ui32 comps,
	f32 from, f32 to) {	f32* vertices = new f32[xblocks * yblocks * comps];
	f32 incx = (to - from) / (xblocks - 1);
	f32 incy = (to - from) / (yblocks - 1);
	f32* c = new f32[comps];
	for (ui32 i = 0; i < comps; ++i) {
		c[i] = 0.0f;
	}
	c[2] = to;                             // z component
	for (ui32 x = 0; x < xblocks; ++x) {
		c[0] = from;                         // x component
		ui32 offx = x * yblocks * comps;
		for (ui32 y = 0; y < yblocks; ++y) {
			c[1] = sin(c[0] * c[2] * 3.0f);  // y component
			c[3] = 1.0f - c[1] / 2.0f + 0.5f;//colores
			ui32 offy = offx + y * comps;
			for (ui32 k = 0; k < comps; ++k) {
				vertices[offy + k] = c[k];
			}
			c[0] += incy;
		}
		c[2] -= incx;
	}
	return vertices;
}

ui32* makeIdxs(ui32 xblocks, ui32 yblocks) {
	ui32* idxs = new ui32[(xblocks - 1) * (yblocks - 1) * 6];

	for (ui32 x = 1; x < xblocks; ++x) {
		ui32 offx = (x - 1) * (yblocks - 1) * 6;
		for (ui32 y = 1; y < yblocks; ++y) {
			ui32 offy = offx + (y - 1) * 6;
			idxs[offy + 0] = (x - 1) * yblocks + y - 1;
			idxs[offy + 1] = (x - 1) * yblocks + y;
			idxs[offy + 2] = x * yblocks + y - 1;
			idxs[offy + 3] = (x - 1) * yblocks + y;
			idxs[offy + 4] = x * yblocks + y - 1;
			idxs[offy + 5] = x * yblocks + y;
		}
	}
	return idxs;
}

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

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		//camara->inputprocces(GODOWN, deltaTime);
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

void scroll_Callback(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
	camara->scroll_mouse(xoffset, yoffset);
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Terrain?", mouse_Callback, scroll_Callback);

	Shader* shader = new Shader();

	Shader* shader_tierra = new Shader("bin", "resources/textures", "shader_tierra.vert", "shader_tierra.frag");


	Cube* cubex = new Cube(shader);
	Cube* cubito = new Cube(shader_tierra);
	


	ui32 o = 10;

	std::vector<glm::vec3> positions(o*o*o);
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
		
	}
	
	/*ui32 n = 3;
	std::vector<glm::vec3> positions(n *n*n);//PINTA
	
	for (ui32 i = 0; i < n; ++i)
	{
		for (ui32 j = 0; j < n; ++j)
		{
			for (ui32 h = 0; h < n; h++)
			{
				f32 x = i - n / 2.0f;
				f32 z = i - n / 2.0f; //j - n / 2.0f
				f32 y = i - n / 2.0f;/*x*x / 10 - z*z / 10;*/
				//i - n/2.0f,pow((i-2/2.0f),2)/10- pow((j - 2 / 2.0f), 2) / 10, j - n/2.0f
		/*		positions[(i * n * n) + (j * n) + h] = glm::vec3(x, y, z);
			}
			
		}

	}*/
	/*
	ui32 g = 3;
	std::vector<glm::vec3> positions_tierra(g * g);//PINTA
	for (ui32 i = 0; i < g; ++i)
	{
		for (ui32 j = 0; j < g; ++j)
		{
			f32 x = (i - g / 2.0f);
			f32 z = 0.0;
			f32 y = 0.0;/*x*x / 10 - z*z / 10;*/
			//i - n/2.0f,pow((i-2/2.0f),2)/10- pow((j - 2 / 2.0f), 2) / 10, j - n/2.0f
		/*	positions_tierra[i * g + j] = glm::vec3(x, y, z);

		}

	}*/


	cubex->inicializar_vaos(true, true);
	cubito->inicializar_vaos(true, true);
	ui32 texture1 = cubex->loadTexture("tierra_ver.jpg", texture1);
	ui32 texture2 = cubito->loadTexture("rubik.jpg", texture2);

	
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

	
		glBindTexture(GL_TEXTURE_2D, texture1);
		

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Solo mostrara como lineas 
		shader->useProgram();

		glm::mat4 projection = glm::perspective(glm::radians(camara->getFov()), ASPECT, 0.1f, 100.0f);

		shader->setMat4("proj", projection);

		shader->setMat4("view", camara->getViewM4());

		glBindVertexArray(cubex->get_VAO());
		
	

		for (ui32 i = 0; i < positions.size(); ++i) {  // Importantisimo para la generacion de los terrenos
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			shader->setMat4("model", model);

			glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
			
			}		
		
		//glBindTexture(GL_TEXTURE_2D, texture2);
/*
		shader_tierra->useProgram();
		shader_tierra->setMat4("proj", projection);
		shader_tierra->setMat4("view", camara->getViewM4());

		glBindVertexArray(cubito->get_VAO());

		for (ui32 i = 0; i < positions_tierra.size(); ++i) {  // Importantisimo para la generacion de los terrenos
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions_tierra[i]);

			/*f32 theta = (f32)glfwGetTime();
			model = glm::rotate(model, theta, glm::vec3(0.3f, 0.6f, 0.9f));*/
			/*if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			}
			shader_tierra->setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
		}*/
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	delete shader_tierra;
	delete shader;
	//delete figure;
	cubex->~Cube();
	cubito->~Cube();

	//delete[] vertices;
	//delete[] idxs;

	return 0;
}