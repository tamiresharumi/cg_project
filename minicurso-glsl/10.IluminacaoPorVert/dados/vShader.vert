varying vec2 texCoord;

varying vec4 cor;

uniform vec3 dirLuz;

void main()
{
	vec3 N = gl_NormalMatrix * gl_Normal;
	vec3 L = normalize(dirLuz);

	vec3 corDif = vec3(1.0) * max(0.0, dot(N, L));

	cor = vec4(corDif, 0.0);

	texCoord = gl_MultiTexCoord0.st;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
