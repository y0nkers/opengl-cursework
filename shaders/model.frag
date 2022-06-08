#version 410 core

struct Light {
    int type;

    vec3 position;
    vec3 direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoord_FS_in;
in vec3 Normal_FS_in;
in vec3 WorldPos_FS_in;
in mat3 outTBN2;

out vec4 OutColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float shininess = 64.0f;

uniform vec4 COLOR;

uniform vec3 WorldPosition;
#define MAX_LIGHTS 4
uniform int lights_count;
uniform Light light[MAX_LIGHTS];

float getAtten(int i)
{
    float dist = distance(light[i].position, WorldPos_FS_in);
    float attenuation = 1.0 / (light[i].constant + light[i].linear*dist + light[i].quadratic * dist * dist);
    return attenuation;
}

vec3 CalcDiffusePlusSpecular(int i, vec3 lightDir)
{
    vec3 norm = texture(texture_normal1, TexCoord_FS_in).rgb; // диапазон значений [0;1]
    norm = normalize(norm * 2.0f - 1.0f); // диапазон значений [-1;1]
    norm = normalize(outTBN2 * norm);
    //vec3 norm = normalize(Normal_FS_in);
    float diff_koef = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light[i].diffuse * diff_koef * vec3(texture(texture_diffuse1, TexCoord_FS_in));

    // specular
    vec3 reflectDir = reflect(lightDir, norm);
    vec3 viewDir = normalize(WorldPos_FS_in-WorldPosition);
    float spec_koef = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    vec3 specular = light[i].specular * spec_koef * vec3(texture(texture_specular1, TexCoord_FS_in));

    return diffuse + specular;
}

vec3 calcFog(vec3 material) // Туман
{
    const vec3 fogColor = vec3(0.6, 0.7, 0.8);
    const float fogStart = 2;
    const float fogEnd = 50;
    float dist = length(WorldPosition - WorldPos_FS_in);
    float fogFactor = clamp(((fogEnd - dist)/(fogEnd - fogStart)), 0.125, 1.0);
    return mix(fogColor, material, fogFactor);
}

void main()
{    
    vec3 lresult;
    for (int i = 0; i<lights_count; i++)
    {
        if (light[i].type == 1) // Directional Light
        {
            vec3 lightDir = -light[i].direction;

            vec3 ambient = light[i].ambient * texture(texture_diffuse1, TexCoord_FS_in).rgb;
            vec3 diffspec = CalcDiffusePlusSpecular(i, lightDir);

            lresult = ambient + diffspec;
        }
        else 
        { 
            vec3 lightDir = normalize(light[i].position - WorldPos_FS_in);
            if (light[i].type == 2) // Point Light
            {
                float attenuation = getAtten(i);
                vec3 ambient = light[i].ambient * vec3(texture(texture_diffuse1, TexCoord_FS_in));
                vec3 diffspec = CalcDiffusePlusSpecular(i, lightDir);

                lresult = (ambient + diffspec) * attenuation;
            }
            else if (light[i].type == 3) // SpotLight
            {
                float angle = acos(dot(lightDir, normalize(-light[i].direction)));

                if (angle <= light[i].cutOff*2.0f)
                {
                    float koef  = 1.0f;
                    if (angle >= light[i].cutOff)
                    {
                        koef = (light[i].cutOff*2.0f - angle) / light[i].cutOff;
                    }

                    float attenuation = getAtten(i);
                    vec3 ambient = light[i].ambient * vec3(texture(texture_diffuse1, TexCoord_FS_in));
                    vec3 diffspec = CalcDiffusePlusSpecular(i, lightDir) * koef;

                    lresult = (ambient + diffspec) * attenuation;
                }
                else
                {
                    lresult =  vec3(texture(texture_diffuse1, TexCoord_FS_in)) * light[i].ambient;
                }
            }
        }
        OutColor += vec4(calcFog(lresult), 1.0f);
    } // end of for

    OutColor *= COLOR;
}