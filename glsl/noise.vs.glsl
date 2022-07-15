// input vertex coordinates
attribute vec4 vPosition;

// input vertex normal
attribute vec4 vNormal;

// output vertex coordinates in eye space
varying vec4 oPosition;

// output vertex normal in eye space
varying vec4 oNormal;

// Model view matrix M 
uniform mat4 MVMatrix;

// (M^{-1})^T
uniform mat4 NormalMatrix; 

// Projection matrix
uniform mat4 PMatrix; 


void main()
{
    vec4 Normal;
    // Complete
    // compute the vertex position in clip space
    // (i.e. multiply vPosition by the projection matrix
    // and the model-view matrix).
    // Save the result in gl_Position
    gl_Position = PMatrix * MVMatrix * vPosition;
    
    // Complete
    // compute the vertex in eye space (i.e. multiply vPosition by the
    // model view matrix).
    // Save the result in oPosition
    Normal = NormalMatrix * vNormal;
    Normal.w = 0.0;
    oPosition = MVMatrix * vPosition;
    
    // Complete
    // compute the normal in eye space (i.e. multiply vNormal by the
    // normal matrix).
    // Save the result in oNormal
    oNormal = normalize(Normal);
    
}
