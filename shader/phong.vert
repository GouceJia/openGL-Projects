#version 150

uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;

in vec3 vertex;
in vec3 normal;

//out vec3 n;
//out vec3 l;
//out vec3 v;

out vec3 vertexEye;
out vec3 normalEye;

void main() {

    // TODO: Transform the vertex position and normal to eye space
    // and pass them on to the fragment shader so that it can
    // calculate the lighting correctly.
    
    vertexEye = (modelViewMatrix * vec4(vertex, 1)).xyz;
    normalEye = (normalMatrix * vec4(normal, 0)).xyz;
    
//    vec3 vertexInEyeSpace = (modelViewMatrix*vec4(vertex,1)).xyz;
//    n = normalize((normalMatrix * vec4(normal,0)).xyz);
//    l = normalize(lightInEyeSpace.xyz - vertexInEyeSpace);
//    v = normalize(-vertexInEyeSpace);

    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex,1);
}
