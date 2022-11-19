#shader vertex
#version 410 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 in_TexCoord;

out vec4 v_Color; // vector color
out vec2 v_TexCoord; // vector texture coordinates

uniform vec3 u_Color; // light color
uniform mat4 u_MvpMatrix; // model view projection matrix

void main()
{
    v_TexCoord = in_TexCoord;

    v_Color = vec4(u_Color, 1.0);
    
    gl_Position = u_MvpMatrix * in_Position;
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec4 v_Color; // link into shader from previous
in vec2 v_TexCoord;

uniform vec3 u_Color; // light uniform
uniform sampler2D u_Texture; // texture uniform

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_Color;
    vec4 result = vec4(ambient, 1.0) * texColor;
    color = result;
}