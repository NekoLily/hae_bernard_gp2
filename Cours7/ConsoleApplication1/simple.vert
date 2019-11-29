#version 120
uniform float time;

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position.x += 0.1 + cos(time)/1.2f;
	gl_Position.y += 0.1 + sin(time)/1.2f;
    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}