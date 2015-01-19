#version 330 core

in vec2 uv_pos;
in vec3 PSNormal;
in vec4 ViewPosition;

in vec4 ViewLightPosition;
in vec4 ViewLightDirection;

out vec3 color;

uniform sampler2D tex_sample0;
// Light
// uniform vec4 light_position;
// uniform vec4 light_direction;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform float light_cutoff;
uniform float light_exponent;
//uniform vec3 light_Intensity;

// Material
uniform vec4 material_diffuse;
uniform vec4 material_ambient;
uniform vec4 material_specular;
uniform float material_shininess;

void main()
{
	vec3 uvcolor = texture(tex_sample0, uv_pos).rgb;
	
    vec3 LightVector = normalize(ViewLightPosition.xyz - ViewPosition.xyz);
    vec3 LightDirection = normalize(ViewLightDirection.xyz);

    vec3 ambient = (light_ambient * material_ambient).rgb;

    float LightAngle = acos(dot(-LightVector, LightDirection));
    float cutoff = radians(clamp(light_cutoff, 0.0, 90.0));

    if(LightAngle < cutoff)
    {
        float diffuseFactor = max(dot(LightVector, PSNormal), 0.0);
        vec3 diffuse = (light_diffuse * material_diffuse).rgb * diffuseFactor;

        float LightFactor = pow(dot(-LightVector, LightDirection), light_exponent);
        
        vec3 ViewDirection = normalize(vec3(-ViewPosition));
        vec3 h = normalize(ViewDirection + LightVector);
        float spotFactor = pow(max(dot(h, PSNormal), 0.0), material_shininess);
        vec3 specular = (light_specular * material_specular).rgb * spotFactor;

        color = ambient + LightFactor * (diffuse + specular);
    }
    else
    {
        color = ambient;
    }
}