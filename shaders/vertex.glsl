#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_mspace;
layout(location = 1) in vec3 vertexNormal_mspace;

// Output data ; will be interpolated for each fragment.
out vec3 vertexPosition_wspace;
out vec3 vertexNormal_cspace;
out vec3 eyeDirection_cspace;
out vec3 lightDirection_cspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 cameraTransform_wspace;
uniform mat4 modelTransform_wspace;
uniform vec3 lightPosition_wspace;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  mvp * vec4(vertexPosition_mspace,1);
	
	// Vertex position in world space.
	vertexPosition_wspace = (modelTransform_wspace * vec4(vertexPosition_mspace,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cspace = ( cameraTransform_wspace * vec4(vertexPosition_wspace,1)).xyz;
	eyeDirection_cspace = vec3(0,0,0) - vertexPosition_cspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPosition_cspace = ( cameraTransform_wspace * vec4(lightPosition_wspace,1)).xyz;
	lightDirection_cspace = lightPosition_cspace + eyeDirection_cspace;
	
	// Vertex normal in camera space.
	vertexNormal_cspace = ( cameraTransform_wspace * modelTransform_wspace * vec4(vertexNormal_mspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}
