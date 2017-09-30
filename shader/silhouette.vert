#version 150

uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;
uniform float thickness;

in vec3 vertex;
in vec3 direction;
in vec3 leftNormal;
in vec3 rightNormal;

void main() {

    vec3 displacedVertex = vertex;

    // TODO: Compute the eye vector pointing towards the camera, and check
    // if dot(leftNormal, eye) and dot(rightNormal, eye) have opposite signs.
    // If so, displace the vertex by thickness*direction.
    
    vec3 eyeVector = (modelViewMatrix * vec4(vertex, 1)).xyz;
    vec3 leftNormalEye = (normalMatrix * vec4(leftNormal, 0)).xyz;
    vec3 rightNormalEye = (normalMatrix * vec4(rightNormal, 0)).xyz;
    float ldot = dot(eyeVector, leftNormalEye);
    float rdot = dot(eyeVector, rightNormalEye);
    
    if ((ldot>0 && rdot<0) || (ldot<0 && rdot>0)){
        //vertex += direction * thickness;
        displacedVertex = vertex + direction * thickness;
    }

    gl_Position = projectionMatrix * modelViewMatrix * vec4(displacedVertex,1);

}
