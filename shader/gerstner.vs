#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoords;

uniform float Time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 UV;
out float Height;

// Functions headers
float noise(float i);
vec2 rotate(vec2 v, float a);
vec2 rotateDir(vec2 dir, float seed);

// UI Uniforms
uniform int   WaveCount;
uniform float Amp;
uniform float Frequency;
uniform float Steepness;
uniform float MAX_ANGLE;
uniform vec2  Direction;

void main()
{
	// Setup variables for sum of waves
	vec3 dir = vec3(Direction.x, 0.0, Direction.y);
	dir = normalize(dir);
    vec3 P = position;
	float t = Time;

	P.y = 0.0;
	float Q = 1.0;
    vec3 N = vec3(0.0);
	
	float Qi = Steepness;
	float A = Amp;
	float w = Frequency;
	float TOTAL_AMP = 0.0;

	for(int i=0; i<WaveCount; i++)
	{
		// Wave parameters
		float l = float(i);
		float p = .5*(l+2.0);

		// Increment for output var Height
		TOTAL_AMP += A;

		// Random Direction
		vec2 rd = rotateDir(dir.xz, l);
        vec3 D = vec3(rd.x, 0.0, rd.y);
		D = normalize(D);

		// Setup
        float steepAmp = Qi*A;
		float inner =  w*dot(D.xz, P.xz)+p*t;

        // Geometry
        P.x += Qi*A * D.x * cos(inner);
        P.z += Qi*A * D.z * cos(inner);
        P.y += A * sin(inner);

        // Normal
		float O = w*dot(D,P)+p*t;
        float WA = w*A;
        float CO = cos(O);

        N.x += D.x * WA * CO; 
        N.z += D.z * WA * CO; 
        N.y += Qi * WA * sin(O);

		// FBM :: To Prevent Values from getting too large (a, Qi) and increase frequency to have finer details as wave count increases
		A  *= 0.81;
		w  *= 1.18;
		Qi *= 0.9;
	}

	// Output Normal
    N.x *= -1.0;
    N.z *= -1.0;
    N.y = 1.0 - N.y;
	Normal = normalize(N);

	// Should be range [0,1] for lerping purposes in fragment shader
	Height  = (P.y+TOTAL_AMP) / (TOTAL_AMP*2.0);

	// FragPos in view space to calculate view direction in fragment shader
	FragPos = vec3(model*vec4(P, 1.0));
	gl_Position = projection * view * model * vec4(P, 1.0);
}

float noise(float i)
{
	return fract( sin(i)*43758.5453123 );
}

vec2 rotate(vec2 v, float a)
{
	return vec2( v.x*cos(a) - v.y*sin(a) ,
				 v.x*sin(a) + v.y*cos(a) );
}

vec2 rotateDir(vec2 dir, float seed)
{
	float r = ( noise(seed)*2.0 ) - 1.0;
	float angle = r*MAX_ANGLE;
	return rotate(dir, radians(angle));
}