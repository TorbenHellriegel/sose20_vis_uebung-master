#version 330
uniform sampler2D imgtexture;
smooth in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    // Set the texture un the specified coordinates.
    fragColor = texture(imgtexture,texCoord);
}
