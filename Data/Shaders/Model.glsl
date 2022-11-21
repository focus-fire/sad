#shader vertex
#version 410 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;

out vec2 v_TexCoord;
out vec3 v_Normals;
out vec4 v_FragPos;

uniform mat3 u_NormalMatrix;
uniform mat4 u_ModelViewMatrix;
uniform mat4 u_MvpMatrix;

void main()
{
    v_TexCoord = in_TexCoord;

    v_Normals = u_NormalMatrix * in_Normal;

    v_FragPos = u_ModelViewMatrix * in_Position;
    
    gl_Position = u_MvpMatrix * in_Position;
}

#shader fragment
#version 410 core

out vec4 out_FragColor;

in vec2 v_TexCoord;
in vec3 v_Normals;
in vec4 v_FragPos;

uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);

    vec3 normals = normalize(v_Normals);
    vec3 fragPos = vec3(v_FragPos);

    out_FragColor = vec4(1, 1, 1, 1);
}