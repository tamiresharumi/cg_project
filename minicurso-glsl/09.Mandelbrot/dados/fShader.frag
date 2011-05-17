varying vec2 texCoord;

uniform int maxIter;

void main()
{

	vec2 c = texCoord;
	vec2 z = vec2(0.0);

	int iter;

	for (iter=0 ; iter<maxIter && z.x*z.x+z.y*z.y < 4.0 ; ++iter)
	{
		z = vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y) + c;
	}

	vec4 corFinal = vec4(1.0 - float(iter)/float(maxIter));

	gl_FragColor = corFinal;
}
