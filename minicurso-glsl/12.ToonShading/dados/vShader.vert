varying vec2 texCoord;
varying vec3 normal;
varying vec4 pos;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	texCoord = gl_MultiTexCoord0.st;
	pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = pos;
}
