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

varying float intensity;

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

uniform sampler2D u_Texture;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;
uniform Model u_Model;

in vec2 v_TexCoord;
in vec3 v_Normals;
in vec4 v_FragPos;

out vec4 out_FragColor;

void main()
{
//    float shininess = 2.0f;
   
//
//    float znear = 1.0;
//    float zfar = 50000.0;
//    float depthm = texture2D(u_Texture, v_TexCoord.xy).r;
//
//    float lineAmp = mix( 0.001, 0.0, clamp( (500.0 / (zfar + znear - ( 2.0 * depthm - 1.0 ) * (zfar - znear) )/2.0), 0.0, 1.0 ) );// make the lines thicker at close range
//
//    float depthn = texture2D(u_Texture, v_TexCoord.xy + vec2( (0.002 + lineAmp)*0.625 , 0.0) ).r;
//    depthn = depthn / depthm;
//
//    float depths = texture2D(u_Texture, v_TexCoord.xy - vec2( (0.002 + lineAmp)*0.625 , 0.0) ).r;
//    depths = depths / depthm;
//
//    float depthw = texture2D(u_Texture, v_TexCoord.xy + vec2(0.0 , 0.002 + lineAmp) ).r;
//    depthw = depthw / depthm;
//
//    float depthe = texture2D(u_Texture, v_TexCoord.xy - vec2(0.0 , 0.002 + lineAmp) ).r;
//    depthe = depthe / depthm;
//
//    float Contour = -4.0 + depthn + depths + depthw + depthe;
//
//    float lineAmp2 = 100.0 * clamp( depthm - 0.99, 0.0, 1.0);
//    lineAmp2 = lineAmp2 * lineAmp2;
//    Contour = (512.0 + lineAmp2 * 204800.0 ) * Contour;
//
//    if(Contour > 0.15){
//        Contour = (0.15 - Contour) / 1.5 + 0.5;
//    } else
//        Contour = 1.0;

    // Variables that could be uniforms
    float shininess = 1.0f;

    // Ambient
    float ambientStrength = 20.0f;
    vec4 ambient = u_Model.Ambient * texture(u_Texture, v_TexCoord) * ambientStrength;

    // Diffuse
    float diffuseStrength = 3.0f;
    vec3 norm = normalize(v_Normals);
    vec3 lightDir = normalize(u_ViewPosition);
    float diff = dot(norm, lightDir);
    vec4 diffuse = u_Model.Diffuse * diffuseStrength * diff;

    // Basic cel-shading portion
    if (diff > 0.95)
        diffuse = diffuse;
    else if (diff > 0.5)
        diffuse = diffuse - vec4(0.4, 0.2, 0.2, 1.0);
    else if (diff > 0.25)
        diffuse = diffuse - vec4(0.6, 0.3, 0.3, 1.0);
    else
        diffuse = diffuse - vec4(0.8, 0.4, 0.4, 1.0);
        

    // Specular
    float specularStrength = 2.0f;
    vec3 viewDir = normalize(u_ViewPosition - vec3(v_FragPos));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = u_Model.Specular  * spec * specularStrength;
    
    if (spec < 0.0f) 
    {
        specular = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    vec3 result = vec3(ambient) + vec3(diffuse);
    out_FragColor = vec4(result, 1.0);
}