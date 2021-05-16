#pragma once

#include <glutil.h>

class Cube
{
	f32 width;
	f32 height;
	f32 depth;
	f32* vertices;
	ui32* indices;
	ui32 vao;
	ui32 vbo;
	ui32 ebo;
	bool rgb;
	bool texture;
	Shader* shader;
public:
	Cube(Shader* shader,f32 width = 1.0f, f32 height = 1.0f, f32 depth = 1.0f)
		: width(width), height(height), depth(depth),
		vertices(new f32[16 * 8]), indices(new ui32[6 * 6]) {
		this->shader = shader;
		f32 wm = width / 2.0f;
		f32 hm = height / 2.0f;
		f32 dm = depth / 2.0f;
		f32 temp[] = {
			// posiciones        colores          texturas
		   -wm,  hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 0
			wm,  hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 1
		   -wm, -hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 2
			wm, -hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 3
		   -wm,  hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 4
			wm,  hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 5
		   -wm, -hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 6
			wm, -hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 7

		   -wm,  hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 8
			wm,  hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 9
		   -wm, -hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 10
			wm, -hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 11
		   -wm,  hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 12
			wm,  hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 13
		   -wm, -hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 14
			wm, -hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f }; // 15
		for (ui32 i = 0; i < 16 * 8; ++i) {
			vertices[i] = temp[i];
		}
		ui32 temp2[] = {
			 0,  1,  2,   1,  2,  3,
			 8,  9, 12,   9, 12, 13,
			 1,  5,  3,   3,  5,  7,
			11, 14, 15,  10, 11, 14,
			 0,  4,  6,   0,  2,  6,
			 4,  5,  6,   5,  6,  7 };
		for (ui32 i = 0; i < 6 * 6; ++i) {
			indices[i] = temp2[i];
		}
	}
	~Cube() {
		delete[] vertices;
		delete[] indices;
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
	f32* getVertices() {
		return vertices;
	}
	ui32* getIndices() {
		return indices;
	}
	ui32 getVSize() {
		return 16 * 8;
	}
	ui32 getISize() {
		return 6 * 6;
	}
	void inicializar_vaos(bool rgb, bool texture)
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);


		if (this->getISize() > 0) {
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i32) * this->getISize(), this->getIndices(), GL_STATIC_DRAW);
		}
		if (this->rgb == false && this->texture == false) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * this->getVSize(), this->getVertices(), GL_STATIC_DRAW);
			// posiciones
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
		}
		if (this->rgb == true)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * this->getVSize(), this->getVertices(), GL_STATIC_DRAW);
			// posiciones
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// colores
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));
			glEnableVertexAttribArray(1);
		}
		if (this->texture == true)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * this->getVSize(), this->getVertices(), GL_STATIC_DRAW);
			// posiciones
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
			glEnableVertexAttribArray(0);
			// textures
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
			glEnableVertexAttribArray(2);
		}
		if (!this->rgb == true && !this->texture == true)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * this->getVSize(), this->getVertices(), GL_DYNAMIC_DRAW);
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
	
	ui32 loadTexture(std::string path, unsigned int& texture)
	{
		return(shader->loadTexture(path, texture));
	}
	void setIntero(const i8* name, const int& i)
	{
		shader->setI32(name,i);
	}
	ui32 get_VAO() { return vao; }
	ui32 get_VBO() { return vbo; }
	ui32 get_EBO() { return ebo; }

};