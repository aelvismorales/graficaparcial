#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertColor;
layout (location = 2) in vec2 myTexCoord;

out vec3 fragColor;
out vec2 texCoord;
out vec3 frag_Pos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() 
{
	frag_Pos=vec3(model * vec4(vertPos, 1.0));
	gl_Position = proj * view * model * vec4(vertPos, 1.0);
	fragColor = vertColor;
	texCoord = vec2(myTexCoord.x, myTexCoord.y);
}



