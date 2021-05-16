#version 330 core
//in vec4 fragColor;
//uniform vec3 objectColor;
//uniform vec3 lightColor;

in vec3 fragColor; // Dato que recibe de colores del vertex shader
in vec2 texCoord;

uniform sampler2D texture1; //sampler son especificos para los valores de textura
uniform sampler2D texture2;

out vec4 color; //color que finalmente se mostrara en pantalla
uniform vec3 ourcolor;

void main() 
{
	//color=vec4(lightColor*objectColor,1.0);
	//color=mix(texture(texture1,texCoord),texture(texture2,texCoord),0.9);
	color=texture(texture1,texCoord);
	
}



