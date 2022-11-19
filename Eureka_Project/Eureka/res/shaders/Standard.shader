#shader vertex
#version 330 core

layout(location = 0) in vec4 position; // The 0 is the index of the attribute
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 color;
layout(location = 3) in vec3 normal;
// Create layout for block type. It can be an int, right?

out vec2 v_TexCoord; //! v stands for varying
out vec3 v_Color;
out vec3 v_Normal;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

void main()
{
   gl_Position = u_ViewProj * u_Transform * position; // gl_Position is predefined
   v_TexCoord = texCoord;
   v_Color = color;
   v_Normal = normal;
};

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Color;
in vec3 v_Normal;

uniform vec4 u_Color;
uniform float u_Time;
uniform float u_UseTexture;
uniform sampler2D u_Texture;
uniform sampler2D u_DepthTexture;

float near = 0.1f;
float far = 100.0f;

// Credit to david van brink on stack overflow (temporary code)
float linearDepth(float depthSample)
{
    depthSample = 2.0 * depthSample - 1.0;
    float zLinear = 2.0 * near * far / (far + near - depthSample * (far - near));
    return zLinear;
}

void main()
{
	float ambient = 0.20f;

	vec2 coord = vec2(gl_FragCoord.x / 920.0, gl_FragCoord.y / 640.0);
	float depth = linearDepth(texture(u_DepthTexture, coord).r);
	float space = 0.1 / (depth / far);
	int r = 10;
	int sampleCount = (2 * r + 1) * (2 * r + 1);
	float shade = 0.0;
	for (int i = -r; i < r + 1; i++)
	{
		for (int j = -r; j < r; j++)
		{
			float x = i * space;
			float y = j * space;
			vec2 sampleCoord = coord + vec2(x / 920.0, y / 640.0);
			float sampleDepth = linearDepth(texture(u_DepthTexture, sampleCoord).r);
			if (sampleDepth < depth)
			{
				shade += 1.0;
			}
		}
	}
	shade /= sampleCount;
	float lightness = 1.0 - shade;
	lightness = min(lightness * 2.0, 1.0);

	vec4 texColor = texture(u_Texture, v_TexCoord);
	vec4 baseColor = u_UseTexture * texColor + 1.0 - u_UseTexture;
	float value = (sin(u_Time * 5.0) + 1.0) * 0.5;
	float diffuse = max(dot(v_Normal, normalize(vec3(1, 2, 3))), 0.0);
	color = baseColor * u_Color * vec4(v_Color.r, v_Color.g, v_Color.b, 1.0) * (diffuse + ambient * lightness);
	color.a = 1.0;
};