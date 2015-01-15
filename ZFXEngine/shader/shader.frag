#version 330 core

in vec2 uv_pos;
in vec3 normal;

out vec3 postion_worldspace;
out vec3 EyeDirection;
out vec3 lightDirection;

out vec3 color;

uniform sampler2D tex_sample0;

uniform mat4 modelview_matrix;
uniform vec4 light_position;

// uniform vec4 light_ambient;
// uniform vec4 light_diffuse;
// uniform vec4 light_specular;
// uniform vec4 light_direction;
// uniform float light_shininess;
// uniform float light_strength;

void main()
{
	// //vec4 uvcolor = vec4(texture(tex_sample0, uv_pos).rgb, 1);
	
	// float diffuse = max(0.0, dot(normal, light_direction));
	// float specular = max(0.0, dot(normal, light))
	// color = min(uvcolor * light_ambient, vec4(1.0));
	
	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0f;
	
	// Material properties
	vec3 MaterialDiffuseColor = texture2D( tex_sample0, uv_pos ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distance = length( light_position.xyz - postion_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( normal );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( lightDirection );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}