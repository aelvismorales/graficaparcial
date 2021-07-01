#include <glutil.h>
#include <figures.h>
#include <vaos.h>
#include <camara.h>

#include<cstdio>
#include <random>
#include "Figure.h"
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
//#include "Texture.h"
using namespace std;
//*****************************************

constexpr ui32 MAX_X{15};
constexpr ui32 MAX_Z{15};
//constexpr uint MAX_HEIGHT{100};
constexpr ui32 NUMBER_OF_ITERATIONS{10};
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

void key_callback(GLFWwindow* window, int key, int code, int action, int mods) {
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		//generate_transitions<MAX_X, MAX_Z>(terrain, transition, camara->getPos());
		//cout << "GG" << endl;
	}

}
void mouse_Callback(GLFWwindow* window, f64 xpos, f64 ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			return;
		}
		camara->mouse_proccess((f32)(xpos - lastX), (f32)(lastY - ypos));
		lastX = xpos;
		lastY = ypos;
	}
	else 
	{
		firstMouse = true;
	}
}


void scroll_Callback(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
	camara->scroll_mouse(xoffset, yoffset);
}

i32 main() {
	ui32 set_pos_z=0;
	ui32 set_pos_x=0;
	generate_terrain<MAX_X, MAX_Z>(terrain, NUMBER_OF_ITERATIONS);
	generate_transitions<MAX_X, MAX_Z>(terrain, transition, camara->getPos(), set_pos_x, set_pos_z);

	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Terrain?", mouse_Callback, scroll_Callback);

	Shader* shader = new Shader();

	Shader* shader_tierra = new Shader("shader.vert", "shader_tierra.frag");

	Shader* shader_material = new Shader();
	
	Cube* cubex = new Cube();
	


	VAO* vao_cubo = new VAO(cubex);
	VAO* vao_cubo_light = new VAO(cubex);
	//VAO* vao_cubos_material = new VAO(cubex_material);

	vao_cubo->inicializar_vaos(true, true,false);
	vao_cubo_light->inicializar_vaos(true, true, false);
	//vao_cubos_material->inicializar_vaos(true, false, true);

	ui32 texture1 = shader->loadTexture("arena.jpg", texture1);
	ui32 texture2 = shader->loadTexture("tierra.jpg", texture2);
	ui32 texture3 = shader->loadTexture("roca.jpg", texture3);
	ui32 texture4 = shader->loadTexture("cesped.jpg", texture4);


	ui32 limite = 0.5;
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		set_pos_z = camara->getPos().z/16;
		set_pos_x = camara->getPos().x / 16;
		/*if (set_pos_z == 16) 
		{
			generate_transitions<MAX_X, MAX_Z>(terrain, transition, camara->getPos(), set_pos_x, set_pos_z);
			set_pos_z = 0;
		}*/
		/*if (set_pos_z >limite )
		{
			camara->set_speed(0);
			cout << camara->getPos().z << endl;
			generate_transitions<MAX_X, MAX_Z>(terrain, transition, camara->getPos(), set_pos_x, set_pos_z);

			limite += limite;
			
		}*/

		
		shader_tierra->useProgram();
		//Ligth Properties
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::mat4 projection = glm::perspective(glm::radians(camara->getFov()), ASPECT, 0.1f, 100.0f);
		f32 theta = (f32)glfwGetTime();
		shader_tierra->setMat4("proj", projection);
		shader_tierra->setVec3("mycolor", lightColor.x, lightColor.y, lightColor.z);
		shader_tierra->setMat4("view", camara->getViewM4());

		glBindVertexArray(vao_cubo_light->get_VAO());
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, theta, glm::vec3(0.0f, 0.3f, 0.0f));
		model = glm::translate(model, glm::vec3(5.0f, 13.0f, 4.0f));
		shader_tierra->setMat4("model", model);

		glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
	

		
		shader->useProgram();
		shader->setVec3("objectColor", 1.0f, 1.0f, 1.0f);
		//Material properties
		shader->setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
		shader->setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		shader->setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		shader->setF32("material.shininess", 32.0f);

		//light properties 
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		shader->setVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
		shader->setVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
		shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		shader->setVec3("viewPos", camara->getPos().x, camara->getPos().y, camara->getPos().z);
		shader->setVec3("lightPos", 5.0f, 13.0f, 4.0f);
		shader->setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
		shader->setMat4("modelo", glm::mat4(1.0f));
		// agregarle el rotate al modelo rotate(glm::mat4(1.0),theta, glm::vec3(0.0f, 0.3f, 0.0f))

		shader->setMat4("proj", projection);
		shader->setMat4("view", camara->getViewM4());

		glBindVertexArray(vao_cubo->get_VAO());

		for (auto& vec : transition)
		{
			//cout << camara->getPos().x << endl;
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
			//model = glm::rotate(model, theta, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::translate(model, glm::vec3(vec.x, vec.y, vec.z) * glm::vec3(2.0f, 2.0f, 2.0f));
			shader->setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);

		}

	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	delete shader_tierra;
	delete shader;
	delete shader_material;

	cubex->~Cube();

	return 0;
}