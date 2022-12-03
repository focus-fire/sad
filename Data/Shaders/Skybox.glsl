#shader vertex
#version 410 core

layout(location = 0) in vec3 in_Position;

out vec3 v_TexCoord;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    v_TexCoord = vec3(in_Position.x, in_Position.y, -in_Position.z);
    vec4 SkyboxPosition = u_ProjectionMatrix * u_ViewMatrix * vec4(in_Position, 1.0);
    gl_Position = SkyboxPosition.xyww;
}

#shader fragment
#version 410 core

out vec4 color;
in vec3 v_TexCoord;

uniform samplerCube u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoord);
}