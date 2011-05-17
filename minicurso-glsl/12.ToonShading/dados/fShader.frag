varying vec2 texCoord;
varying vec3 normal;
varying vec4 pos;

uniform sampler2D textura;
uniform vec3 posLuz;

void main()
{
	vec3 N = normalize(normal);
	vec3 L = normalize(posLuz - pos.xyz/pos.w);

	float fator = max(0.0, dot(N, L));

	if (fator < 0.2)
		fator = 0.0;
	else if (fator < 0.4)
		fator = 0.4;
	else if (fator < 0.7)
		fator = 0.7;
	else
		fator = 1.0;

	vec3 corDif = vec3(1.0) * fator;

	gl_FragColor = vec4(corDif, 0.0);
}
