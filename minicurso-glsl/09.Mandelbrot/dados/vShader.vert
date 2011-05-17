varying vec2 texCoord;

void main()
{
	texCoord = gl_MultiTexCoord0.st;
	texCoord = texCoord * 3.0 - vec2(2.0);
	texCoord += vec2(-0.2,0.5);

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
