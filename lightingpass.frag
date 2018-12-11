#version 400 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 lightPosition_camera;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// Light data
uniform int lightType;
uniform vec3 lightAttenuation;
uniform vec3 lightColor;

void main()
{
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec4 vertexToLight = vec4(0);
    // Point Light
    if (lightType == 2) {
        //ambient component
        FragColor = lightColor;
    } else {
        if (lightType == 0) {
            vertexToLight = normalize(vec4(lightPosition_camera, 1) - FragPos);
        } else if (lightType == 1) {
            // Dir Light
            vertexToLight = normalize(vec4(-lightPosition_camera, 0));
        }

        float length = length(lightPosition_camera - FragPos);
        float attenuation = 1.0 / (lightAttenuation.x + lightAttenuation.y * length +
                                             lightAttenuation.z * (length * length));

         // Add diffuse component
        float diffuseIntensity = max(0.0, dot(vertexToLight, gNormal));
        FragColor += max(vec3(0), attenuation * lightColor * Albedo * diffuseIntensity);

        // Add specular component- not adding it, since it will slow down vr and is not needed for
        // the cartoony texture, but will keep as an option
//        vec4 lightReflection = normalize(-reflect(vertexToLight, FragPos));
//        vec4 eyeDirection = normalize(vec4(0,0,0,1) - FragPos);
//        float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), shininess);
//         color += max (vec3(0), attenuation * lightColors[i] * specular_color * specIntensity);
    }


    FragColor = clamp(FragColor, 0.0, 1.0);
}
