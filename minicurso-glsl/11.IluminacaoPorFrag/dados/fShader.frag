varying vec2 texCoord;
varying vec3 normal;

uniform sampler2D textura;
uniform vec3 dirLuz;

void main()
{
	vec3 N = normalize(normal);
	vec3 L = normalize(dirLuz);

	vec3 corDif = vec3(1.0) * max(0.0, dot(N, L));

	gl_FragColor = vec4(corDif, 0.0);
}
