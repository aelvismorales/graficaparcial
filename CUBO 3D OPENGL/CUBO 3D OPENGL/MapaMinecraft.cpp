#include <glutil.h>
#include <figures.h>
#include <vaos.h>
#include <camara.h>

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


	Cube* cubex = new Cube();
	Cube* cubito = new Cube();
	

	ui32 o = 5;

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
	
	//cubex->inicializar_vaos(true, true);
	//cubito->inicializar_vaos(true, true);
	/*ui32 vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) *cubex->getVSize(), cubex->getVertices(),GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(f32)*cubex->getISize(), cubex->getIndices(), GL_STATIC_DRAW);

	// posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * FSIZE, (void*)0);
	glEnableVertexAttribArray(0);
	// colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * FSIZE, (void*)(3 * FSIZE));
	glEnableVertexAttribArray(1);
	// textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
	glEnableVertexAttribArray(2);
	
	glEnable(GL_DEPTH_TEST);*/
	VAO* vao_cubo = new VAO(cubex);
	vao_cubo->inicializar_vaos(true, true);
	ui32 texture1 = shader->loadTexture("tierra_ver.jpg", texture1);
	//ui32 texture2 = shader->loadTexture("rubik.jpg", texture2);
	
	
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

		glBindVertexArray(vao_cubo->get_VAO());
		
		for (ui32 i = 0; i < positions.size(); ++i) {  // Importantisimo para la generacion de los terrenos
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			shader->setMat4("model", model);

			glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
			
			}		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	delete shader_tierra;
	delete shader;

	cubex->~Cube();
	cubito->~Cube();
	return 0;
}