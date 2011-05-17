varying vec2 texCoord;

uniform sampler2D textura;

varying vec4 cor;

void main()
{
	gl_FragColor = cor;
}
