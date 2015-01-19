#version 330 core

in vec2 uv_pos;
in vec3 PSNormal;
in vec4 ViewPosition;
in vec4 LightDirection;

out vec3 color;

uniform sampler2D tex_sample0;
// Light
uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
//uniform vec3 light_Intensity;

// Material
uniform vec4 material_diffuse;
uniform vec4 material_ambient;
uniform vec4 material_specular;
uniform float material_shininess;

void main()
{
	vec3 uvcolor = texture(tex_sample0, uv_pos).rgb;
	
    vec3 LightVector = normalize(-LightDirection.xyz);

    vec4 ambient = light_ambient * material_ambient;

    float diffuseFactor = dot(LightVector, PSNormal);
    vec4 diffuse = light_diffuse * material_diffuse * diffuseFactor;

    // in View Space , Eye Position is (0, 0, 0)
    vec3 ViewDirection = normalize(-ViewPosition.xyz);
    vec3 reflection = reflect(-LightVector, PSNormal);
    float specularFactor = pow(max(dot(reflection, ViewDirection), 0.0f), material_shininess);
    vec4 specular = light_specular * material_specular * specularFactor;

    color = (ambient + diffuse).rgb * uvcolor + specular.rgb;
}