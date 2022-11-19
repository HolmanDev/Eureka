#shader vertex
#version 330 core

layout(location = 0) in vec4 position; // The 0 is the index of the attribute
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 color;
layout(location = 3) in vec3 normal;

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

void main()
{
	color = u_Color;
};