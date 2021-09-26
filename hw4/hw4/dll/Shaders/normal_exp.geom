#version 430 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
in VS_OUT {
    vec3 normal;
    vec2 tex;
} gs_in[];

uniform mat4 P;
uniform float gloss;

out vec4 color;
out vec2 textest;

void GenerateLine(int index)
{
    vec3 nor =( gs_in[0].normal + gs_in[1].normal + gs_in[2].normal )/3;
    gl_Position = gl_in[0].gl_Position + vec4(nor,0.0f)*(gloss + 0.5*2.0f*pow(gloss,2));
    textest = gs_in[0].tex;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position + vec4(nor,0.0f)*(gloss + 0.5*2.0f*pow(gloss,2));
    textest = gs_in[1].tex;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position + vec4(nor,0.0f)*(gloss + 0.5*2.0f*pow(gloss,2));
    textest = gs_in[2].tex;
    EmitVertex();
    
    EndPrimitive();
}

void main()
{
   for(int i=0;i<3;i++){
        GenerateLine(i);
   }
}
