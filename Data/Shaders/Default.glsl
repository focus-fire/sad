#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec4 v_Color;
out vec2 v_TexCoord;

uniform vec4 u_Color;
uniform mat4 u_MvpMatrix;

void main()
{
    v_TexCoord = texCoord;

    v_Color = u_Color;
    
    gl_Position = u_MvpMatrix * position;
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);

    color = u_Color * texColor;
}