#version 150

uniform vec4 lightInEyeSpace;

uniform vec4 Ia;
uniform vec4 Id;
uniform vec4 Is;  // lighting
uniform vec4 Ka;
uniform vec4 Kd;
uniform vec4 Ks;
uniform float ss;  // material

uniform sampler2D diffuseRamp;
uniform sampler2D specularRamp; // texture

//in vec3 n;
//in vec3 l;
//in vec3 v;

in vec3 vertexEye;
in vec3 normalEye;

out vec4 color;

void main() {

    // We'll start with black, then add various lighting terms to it
    // as we calculate them.
    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    // TODO: Calculate ambient, diffuse, and specular lighting for this pixel
    // based on its position, normal, etc.
    vec3 n = normalize(normalEye);
    vec3 l = normalize(lightInEyeSpace.xyz - vertexEye);
    vec3 v = normalize(-vertexEye);
    vec3 h = normalize(l + v);
    
    vec2 diffuseCord = vec2(max(0.5 * dot(n, l) + 0.5, 0.0), 0.5);
    vec2 specCord = vec2(pow(max(dot(n, h), 0.0), ss), 0.5);
    
    vec3 ambient = (Ka * Ia).xyz;
    vec3 diffuse = (Kd * Id * texture(diffuseRamp, diffuseCord)).xyz;
    vec3 spec = (Ks * Is * texture(specularRamp, specCord)).xyz;
    
//    vec3 diffuse = (Kd * Id * max(dot(n, l), 0.0)).xyz;
//    vec3 spec = (Ks * Is * pow(max(dot(n, h), 0.0), ss)).xyz;

    finalColor += ambient + diffuse + spec;

    // Use the R,G,B components of finalColor for the output color of this
    // fragment, and set the alpha component to 1.0 (completely opaque).
    color.rgb = finalColor;
    color.a = 1.0;

}
