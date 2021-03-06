const char *phong_frag = R"foo(
#version 430 core

#define LIGHTS_N 1

#define LIGHT_COLOR_I 0
#define LIGHT_POSITION_I 1

/** Material */
struct Material_t {
    vec3 diffuse;
    vec3 specular;
    float roughness;
};
layout( location = 0) uniform Material_t material = { vec3( 1.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0), 0.0 };

/** Lights */
struct Light {
    vec4 position;
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
};
layout( location = 4) uniform Light lights[ LIGHTS_N ];

uniform vec3 viewPos;

/** Default: 1 white light at origo */

vec3 saturate( vec3 color ) {
    return clamp( color, 0.0, 1.0f);
}

in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosition;   /** world space */

out vec4 FragColor;


/** Main */
void main()
{
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos - FragPosition.xyz);

    for( int i=0; i < LIGHTS_N; i++ ) {
        ambient += material.diffuse.rgb * lights[i].ambient.rgb;
        vec3 lightDir = normalize( (lights[i].position - FragPosition).xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse += lights[0].diffuse.rgb * diff * material.diffuse.rgb;
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.roughness);
        specular += lights[0].specular.rgb * (spec * material.specular.rgb);
    }

    vec3 result = saturate(ambient + diffuse + specular);
    FragColor =  vec4( result, 1.0f);
}

)foo";
