varying vec2 texCoord;
varying vec3 normal;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	texCoord = gl_MultiTexCoord0.st;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
