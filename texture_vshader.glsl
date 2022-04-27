#version 330
uniform mat4 mvpMatrix;
in vec4 texcoords;

smooth out vec2 texCoord;

void main(void)
{
    // Calculate vertex position in screen space.
    gl_Position = mvpMatrix * texcoords;
    // Use x and y coordinates as texture coordinates.
    texCoord.xy = texcoords.xy;
}
