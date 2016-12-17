#version 410

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float pX;
uniform float pY;

in vec4 in_Position;

out vec4 pColor;

void main()
{
	vec4 origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 axeX = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 axeY = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	vec4 well = vec4(pX, pY, 0.0f, 1.0);

	float dC = distance(well, in_Position);
	float dX = length(cross(vec3(in_Position - origin), vec3(axeX))) / length(axeX);
	float dY = length(cross(vec3(in_Position - origin), vec3(axeY))) / length(axeY);

	dC /= 10000.f;
	dX /= 10000.f;
	dY /= 10000.f;

	dC = 1 - clamp(dC, 0.f, 1.f);
	dX = 1 - clamp(dX, 0.f, 1.f);
	dY = 1 - clamp(dY, 0.f, 1.f);

	pColor = vec4(1.0, dC, (dY + dX) / 2, 1.0);
//	pColor = vec4(1.0, 0.0, dC, 1.0);
	gl_Position = P * V * M * in_Position;
	gl_PointSize = 1.0;
}
