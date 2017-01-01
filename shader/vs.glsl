#version 410

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float pX;
uniform float pY;

uniform vec3	eyePos;

in vec4 in_Position;

out vec4 pColor;

void main()
{
	vec4 origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 axeX = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 axeY = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	vec4 well = vec4(pX, pY, 0.0f, 1.0);

	vec4 partColor = vec4(1.f, 0.f, 0.f, 1.f);
	vec4 mixColor  = vec4(0.f, 0.f, 1.f, 1.f);
	vec4 gravColor = vec4(1.f, 1.f, 0.6f, 1.f);
	vec4 fade =		 vec4(0.f, 0.f, 0.f, 1.f);

	float dC = distance(well, in_Position);
	float dX = length(cross(vec3(in_Position - origin), vec3(axeX))) / length(axeX);
	float dY = length(cross(vec3(in_Position - origin), vec3(axeY))) / length(axeY);

	dC /= 10000.f;
	dX /= 10000.f;
	dY /= 10000.f;

	dC = 1 - clamp(dC, 0.f, 1.f);
	dX = 1 - clamp(dX, 0.f, 1.f);
	dY = 1 - clamp(dY, 0.f, 1.f);

	float distView = distance(vec4(eyePos, 1.f), M * in_Position);
	distView = clamp(distView / 1000000, 0.1f, 1.f);
	float distView2 = clamp(distView / 1000, 0.f, 1.f);

//	pColor = vec4(1.0, dC, (dY + dX) / 2, 1.0);
	partColor = vec4(1.0, dC, (dY + dX) / 2, 1.0);

	partColor = mix(partColor, mixColor, distView2);
//	partColor = mix(partColor, gravColor, dC);
	pColor = mix(partColor, fade, distView);

//	pColor = vec4(1.0, 0.0, dC, 1.0);


	gl_Position = P * V * M * in_Position;
	gl_PointSize = 1.0;
}
