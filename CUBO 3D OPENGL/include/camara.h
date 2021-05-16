#pragma once
#include <glutil.h>

enum MovimientoCamara {
	LEFT, RIGHT, UP, DOWN,GOUP,GODOWN
};

class Camara
{
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	f32 yaw;
	f32 pitch;
	f32 fov;
	f32 speed;
	f32 sensitivity;

public:
	Camara(f32 px=4.0f,f32 py=3.0f,f32 pz=10.0f ,f32 fx=0.0f ,f32 fy=0.0f, f32 fz=-1.0f ,f32 ux=0.0f,f32 uy=1.0f,f32 uz=0.0f,
		f32 yaw = -90.0f, f32 pitch = 0.0f, f32 fov = 45.0f, f32 speed = 8.5, f32 sensitivity = 0.1) :position({ px,py,pz }), front({fx,fy,fz}),
		up({ ux,uy,uz }),yaw(yaw),pitch(pitch),fov(fov),speed(speed),sensitivity(sensitivity)
	{}
	void inputprocces(MovimientoCamara direccion,f32 deltatime)
	{
		f32 velocidad = speed * deltatime;
		if(direccion==UP)
		{
			position += velocidad * front;
		}
		if(direccion==GOUP)
		{
			
			position += velocidad*up;
		
		}
		if(direccion==LEFT)
		{
			position -= glm::normalize(glm::cross(front, up)) * velocidad;
		}
		if(direccion==DOWN)
		{
			position -= velocidad * front;
		}
		if (direccion == GODOWN)
		{
			position -= velocidad * up;
		}
		if(direccion==RIGHT)
		{
			position += glm::normalize(glm::cross(front, up)) * velocidad;
		}
	}
	
	void mouse_proccess(f32 xoffset, f32 yoffset) 
	{
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
			//pitch *= -1;
		}
		else if (pitch < -89.0f)
		{
			pitch = -89.0f;
			//pitch *= -1;
		}
		glm::vec3 f;
		f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		f.y = sin(glm::radians(pitch));
		f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(f);
	}
	void scroll_mouse(f64 xoffset,f64 yoffset)
	{
		fov -= (f32)yoffset; //Zoom
		if (fov < 1.0f) { fov = 1.0f; }
		else if (fov > 45.0f) { fov = 45.0f; }
	}
	glm::mat4 getViewM4()
	{
		return glm::lookAt(position, position + front, up);
	}
	
	f32 getFov()
	{
		return fov;
	}
};