#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexPosition_wspace;
in vec3 vertexNormal_cspace;
in vec3 lightDirection_cspace;

// Output data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform vec3 lightPosition_wspace;

void main(){
	// Light emission properties
	// You'll probably want to put them as uniforms
	vec3 lightColor = vec3(1,1,1);
	float lightPower = 50.f;
	
	// Material properties
	vec3 materialDiffuseColor = vec3(0,1,1);
	vec3 materialAmbientColor = vec3(0.1,0.1,0.1);

	// Distance to the light
	float distance = length( lightPosition_wspace - vertexPosition_wspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( vertexNormal_cspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( lightDirection_cspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Final colour
	color = 
		materialAmbientColor +
		materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance);
}
