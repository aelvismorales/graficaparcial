#version 330 core

uniform vec3 mycolor;
out vec4 color;

void main() 
{
	color=vec4(1.0)*vec4(mycolor,1.0);
}



