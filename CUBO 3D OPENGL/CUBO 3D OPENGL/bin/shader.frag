#version 330 core


uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 fragColor; // Dato que recibe de colores del vertex shader
in vec2 texCoord;

in vec3 frag_Pos;


uniform sampler2D texture1; //sampler son especificos para los valores de textura
uniform sampler2D texture2;

out vec4 color; //color que finalmente se mostrara en pantalla
   
void main() 
{
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength*lightColor;
  	
    // diffuse 
    vec3 norm = normalize(fragColor); //fragoColor lo  estoy tomando como mi normal dado que no me reconoce la otra variable
    vec3 lightDir = normalize(lightPos - frag_Pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;     
    
	//Specular
	 float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - frag_Pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
	
	color=texture(texture1,texCoord)*vec4(result,1.0);
}



