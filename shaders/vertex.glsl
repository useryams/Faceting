#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPos_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 pos_worldspace;
out vec3 normal_cameraspace;
out vec3 eyeDirection_cameraspace;
out vec3 lightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;
uniform vec3 lightPos_worldspace;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  mvp * vec4(vertexPos_modelspace,1);
	
	// Position of the vertex, in worldspace : M * position
	pos_worldspace = (m * vec4(vertexPos_modelspace,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPos_cameraspace = ( v * m * vec4(vertexPos_modelspace,1)).xyz;
	eyeDirection_cameraspace = vec3(0,0,0) - vertexPos_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPos_cameraspace = ( v * vec4(lightPos_worldspace,1)).xyz;
	lightDirection_cameraspace = lightPos_cameraspace + eyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	normal_cameraspace = ( v * m * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}
