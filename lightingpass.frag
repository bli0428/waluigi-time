#version 400 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


// Light data
const int MAX_LIGHTS = 200;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

// Material data
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;
uniform vec2 repeatUV;

uniform bool useLighting;     // Whether to calculate lighting using lighting equation

uniform vec3 viewPos;

void main()
{
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    if (useLighting) {
        color = ambient_color.xyz; // Add ambient component

        for (int i = 0; i < MAX_LIGHTS; i++) {
            vec4 vertexToLight = vec4(0);
            // Point Light
            if (lightTypes[i] == 0) {
                vertexToLight = normalize(v * vec4(lightPositions[i], 1) - position_cameraSpace);
            } else if (lightTypes[i] == 1) {
                // Dir Light
                vertexToLight = normalize(v * vec4(-lightDirections[i], 0));
            }

            float length = length(lightPositions[i] - vec3(position_worldSpace));
            float attenuation = 1.0 / (lightAttenuations[i].x + lightAttenuations[i].y * length +
                                         lightAttenuations[i].z * (length * length));

            // Add diffuse component
            float diffuseIntensity = max(0.0, dot(vertexToLight, normal_cameraSpace));
            color += max(vec3(0), attenuation * lightColors[i] * diffuse_color * diffuseIntensity);

            // Add specular component
            vec4 lightReflection = normalize(-reflect(vertexToLight, normal_cameraSpace));
            vec4 eyeDirection = normalize(vec4(0,0,0,1) - position_cameraSpace);
            float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), shininess);
            color += max (vec3(0), attenuation * lightColors[i] * specular_color * specIntensity);
        }
    } else {
        color = ambient_color + diffuse_color;
    }
    color = clamp(color, 0.0, 1.0);
}
