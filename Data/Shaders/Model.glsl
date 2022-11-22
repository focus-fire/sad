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

precision highp float;

struct Model {
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    vec4 Emissive;
};

out vec4 out_FragColor;

in vec2 v_TexCoord;
in vec3 v_Normals;
in vec4 v_FragPos;

uniform sampler2D u_Texture;

uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;
uniform Model u_Model;

void main()
{
    // Variables that could be uniforms
    float shininess = 2.0f;

    // Ambient
    float ambientStrength = 12.0f;
    vec4 ambient = u_Model.Ambient * texture(u_Texture, v_TexCoord) * ambientStrength;

    // Diffuse
    float diffuseStrength = 1.0f;
    vec3 norm = normalize(v_Normals);
    vec3 lightDir = normalize(u_LightPosition - vec3(v_FragPos));
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = u_Model.Diffuse * diff * diffuseStrength;

    // Specular
    float specularStrength = 1.5f;
    vec3 viewDir = normalize(u_ViewPosition - vec3(v_FragPos));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = u_Model.Specular * spec * specularStrength;
    
    if (spec < 0.0f) 
    {
        specular = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    vec3 result = vec3(ambient) + vec3(diffuse);
    out_FragColor = vec4(result, 1.0) * vec4(vec3(specular), 1.0);
}