#version 410

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float pX;
uniform float pY;
uniform float pZ;

uniform vec3	eyePos;

in vec4 in_Position;

out vec4 pColor;

void main()
{
	vec4 origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 axeX = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 axeY = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	vec4 well = vec4(pX, pY, pZ, 1.f);
	vec4 fade = vec4(0.f, 0.f, 0.f, 1.f);

	float dC = distance(well, in_Position);
	float dX = length(cross(vec3(in_Position - origin), vec3(axeX))) / length(axeX);
	float dY = length(cross(vec3(in_Position - origin), vec3(axeY))) / length(axeY);

	dC /= 10000.f;
	dX /= 10000.f;
	dY /= 10000.f;

	dC = 1.f - clamp(dC, 0.f, 1.f);
	dX = 1.f - clamp(dX, 0.f, 1.f);
	dY = 1.f - clamp(dY, 0.f, 1.f);

	float distView = distance(vec4(eyePos, 1.f), in_Position);
	distView = clamp(distView / 1000000.0f, 0.1f, 1.f);

	vec4 partColor = vec4(1.f, dC, (dY + dX) / 2.f, 1.f);
	pColor = mix(partColor, fade, distView);


	gl_Position = P * V * M * in_Position;
	gl_PointSize = 1.f;
}
