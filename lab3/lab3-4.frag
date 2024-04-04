#version 150

out vec4 out_Color;
in vec3 color;
in vec3 normal_world;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

uniform mat4 world2view;
uniform vec3 cameraPos;

in vec3 SurfPos;


void main(void)
{	
    vec4 color = vec4(0, 0, 0, 0);
    mat3 world2view_3 = mat3(world2view);
    vec3 normal_view = normalize(world2view_3 * normal_world);


    for(int i = 0; i < 4; i++) {
        vec3 lightDirection;
        if (isDirectional[i]) {
            // Directional light
            lightDirection = normalize(world2view_3 * lightSourcesDirPosArr[i]);
            } 
        else {
            // Positional light
            lightDirection = normalize(world2view_3 *  (lightSourcesDirPosArr[i] - SurfPos));
        }
        // Diffuse light
        float diffuse = max(dot(normal_view, lightDirection), 0.0);
        vec3 diffuseLight = diffuse * lightSourcesColorArr[i];

        // Specular light
        vec3 viewDirection = normalize(world2view_3 * (cameraPos - SurfPos));
        vec3 reflectDirection = reflect(-lightDirection, normal_view);
        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
        vec3 specularLight = specular * lightSourcesColorArr[i];

        color = color + vec4(diffuseLight * 0.3 + specularLight * 0.5, 1);
    }

    out_Color = color;
}
