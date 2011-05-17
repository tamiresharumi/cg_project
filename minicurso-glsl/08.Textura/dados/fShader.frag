varying vec2 texCoord;

uniform sampler2D textura1;
uniform sampler2D textura2;

uniform float larguraTela;

void main()
{
	float fator = gl_FragCoord.x/larguraTela;
	vec4 cor1 = texture2D(textura1, texCoord);
	vec4 cor2 = texture2D(textura2, texCoord);

	gl_FragColor = mix(cor1, cor2, fator);
}
