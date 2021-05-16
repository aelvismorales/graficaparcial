#version 330 core
//in vec4 fragColor;
//uniform vec3 objectColor;
//uniform vec3 lightColor;

in vec3 fragColor;
in vec2 texCoord;
uniform sampler2D texture2;
out vec4 color;
void main() 
{
	//color=vec4(lightColor*objectColor,1.0);
	color=texture(texture2,texCoord);
}



