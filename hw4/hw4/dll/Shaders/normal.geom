#version 430 core
layout(triangles) in;
layout(line_strip, max_vertices = 12) out;
in VS_OUT {
    vec3 normal;
    vec2 tex;
} gs_in[];

uniform mat4 P;
uniform float gloss;
uniform float b_gloss;

out vec4 color;
out vec2 textest;

void GenerateLine(int index)
{
    if(max(dot(gs_in[index].normal,vec3(1.0f,0.0f,0.0f)),0)>0.99){
        gl_Position = gl_in[index].gl_Position;
        color=vec4(1.0f,1.0f,1.0f,0.0f);
        textest = gs_in[index].tex;
        EmitVertex();
        gl_Position = gl_in[index].gl_Position + vec4(1.0f*0.5,0.0f,-1.0*0.5,0.5f) * 0.5;
        color=vec4(1.0f,1.0f,1.0f,0.0f);
        EmitVertex();
    }
    if(max(dot(gs_in[index].normal,vec3(1.0f,0.0f,0.0f)),0)>0.5){
        gl_Position = gl_in[index].gl_Position+0.1;
        color=vec4(0.2f,0.2f,0.2f,0.0f);
        textest = gs_in[index].tex;
        EmitVertex();
        gl_Position = gl_in[index].gl_Position+0.1 + vec4(1.0f*0.5,0.0f,-1.0*0.5,0.5f) * 0.5;
        EmitVertex();
    }
    EndPrimitive();
}

void main()
{
   for(int i=0;i<gl_in.length();i++){
        GenerateLine(i);
   }
}
