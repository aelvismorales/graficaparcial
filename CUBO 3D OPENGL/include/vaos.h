#pragma once
#include <glutil.h>
#include <figures.h>

class VAO
{
	ui32 vao;
	ui32 vbo;
	ui32 ebo;
	Cube* cubito;
	//Shader* shader;
	bool rgb;
	bool texture;
public:
	VAO(Cube* cubito):cubito(cubito){}

	void inicializar_vaos(bool rgb,bool texture) 
	{
		glGenVertexArrays(1,&vao);
		glGenBuffers(1, &vbo);
		
		glBindVertexArray(vao);

		
		if (cubito->getISize() > 0) {
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i32) * cubito->getISize(), cubito->getIndices(), GL_STATIC_DRAW);
		}
		if (this->rgb == false && this->texture == false) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * cubito->getVSize(), cubito->getVertices(), GL_STATIC_DRAW);
			// posiciones
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
		}
		if (this->rgb==true)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * cubito->getVSize(), cubito->getVertices(), GL_STATIC_DRAW);
			// posiciones
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// colores
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));
			glEnableVertexAttribArray(1);
		}
		if (this->texture==true)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * cubito->getVSize(), cubito->getVertices(), GL_STATIC_DRAW);
			// posiciones
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// textures
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
			glEnableVertexAttribArray(2);
		}
		if(!this->rgb == true && !this->texture == true)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * cubito->getVSize(), cubito->getVertices(), GL_STATIC_DRAW);
			// posiciones
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// colores
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
			glEnableVertexAttribArray(1);
			// textures
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
			glEnableVertexAttribArray(2);
		}
	
		glEnable(GL_DEPTH_TEST);
		
	}
	ui32 get_VAO() { return vao; }
	ui32 get_VBO() { return vbo; }
	ui32 get_EBO() { return ebo; }
	~VAO()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
};