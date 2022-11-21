#shader vertex
#version 410 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec4 in_Color;

out vec4 v_Color;

uniform mat4 u_VpMatrix;

void main()
{
    gl_Position = u_VpMatrix * vec4(in_Position, 1.0);

    v_Color = in_Color;
}

#shader fragment
#version 410 core

in vec4 v_Color;

out vec4 out_FragColor;

void main()
{
    out_FragColor = v_Color;
}