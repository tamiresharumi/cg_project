varying vec2 texCoord;

void main()
{
	texCoord = gl_MultiTexCoord0.st;

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
