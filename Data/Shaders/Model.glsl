#shader vertex
#version 410 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec3 in_Color;

out vec2 v_TexCoord;
out vec3 v_Normals;
out vec4 v_FragPos;
out vec3 v_Color;

uniform mat3 u_NormalMatrix;
uniform mat4 u_ModelViewMatrix;
uniform mat4 u_MvpMatrix;

varying float intensity;

void main()
{
    v_TexCoord = in_TexCoord;

    v_Normals = u_NormalMatrix * in_Normal;

    v_FragPos = u_ModelViewMatrix * in_Position;

    v_Color = in_Color;
    
    gl_Position = u_MvpMatrix * in_Position;
}

#shader fragment
#version 410 core

precision highp float;

struct Model {
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    vec4 Emissive;
};

uniform sampler2D u_Texture;
uniform vec3 u_LightDirection;
uniform vec3 u_ViewPosition;
uniform Model u_Model;

in vec2 v_TexCoord;
in vec3 v_Normals;
in vec4 v_FragPos;
in vec3 v_Color;

out vec4 out_FragColor;

void main()
{
    // Variables that could be uniforms
    float shininess = 2.0;

    // Ambient
    float ambientStrength = 1.0;
    vec4 ambient = u_Model.Ambient * texture(u_Texture, v_TexCoord) * ambientStrength;

    // Diffuse
    float diffuseStrength = 2.0;
    vec3 norm = normalize(u_ViewPosition - v_Normals);
    vec3 lightDir = normalize(u_ViewPosition - vec3(v_FragPos));
    float diff = dot(norm, lightDir);
    vec4 diffuse = u_Model.Diffuse * diffuseStrength * diff;

    // Basic cel-shading portion
    if (diff > 0.95)
        diffuse = diffuse;
    else if (diff > 0.5)
        diffuse = diffuse - vec4(0.2, 0.1, 0.1, 1.0);
    else if (diff > 0.25)
        diffuse = diffuse - vec4(0.3, 0.15, 0.15, 1.0);
    else
        diffuse = diffuse - vec4(0.4, 0.2, 0.2, 1.0);
        
    // Specular
    float specularStrength = 2.0;
    vec3 viewDir = normalize(u_ViewPosition - vec3(v_FragPos));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(dot(viewDir, reflectDir), shininess);
    vec4 specular = u_Model.Specular  * spec * specularStrength;

    vec3 result = vec3(ambient) + vec3(diffuse);
    out_FragColor = vec4(result, 1.0) * vec4(vec3(specular), 1.0);
}