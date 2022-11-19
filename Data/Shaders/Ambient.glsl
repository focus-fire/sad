#shader vertex
#version 410 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 in_TexCoord;
layout (location = 2) in vec3 aNormal; // df

out vec4 v_Color; // vector color
out vec2 v_TexCoord; // vector texture coordinates

out vec3 FragPos; // df
out vec3 Normal; // df

uniform vec3 u_Color; // light color
uniform mat4 u_MvpMatrix; // model view projection matrix
uniform mat4 u_ModelMatrix; // transform aka model;

void main()
{
    v_TexCoord = in_TexCoord;

    v_Color = vec4(u_Color, 1.0);
    
    gl_Position = u_MvpMatrix * in_Position;
    FragPos = vec3(u_ModelMatrix * vec4(in_Position)); // df
    Normal = aNormal; // df
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec4 v_Color; // link into shader from previous
in vec2 v_TexCoord;

in vec3 FragPos; // diffuse frag
in vec3 Normal; // diffuse normal

uniform vec3 u_Color; // light uniform
uniform sampler2D u_Texture; // texture uniform

uniform vec3 lightPos; // diffuse

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);

    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * u_Color;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_Color;

    // vec4 result = vec4(ambient, 1.0) * texColor;
    vec3 result = (ambient + diffuse) * vec3(texColor);
    color = vec4(result, 1.0);
    // color = result;
}