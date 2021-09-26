#version 430 core
layout(triangles) in;
layout(points, max_vertices = 3) out;

in VS_OUT {
    vec3 normal;
    vec2 tex;
} gs_in[];

uniform mat4 P;
uniform float exp_gloss;

out vec2 TexCoords;
out vec4 color;


void explode(){
    vec4 nor = (vec4(gs_in[0].normal, 0.0f) +  vec4(gs_in[1].normal, 0.0f)+ vec4(gs_in[2].normal, 0.0f)/3);
    gl_Position = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) /3 + nor*(exp_gloss + 0.5*2.0f*pow(exp_gloss,2));
    color = vec4(exp_gloss, exp_gloss/2, exp_gloss/2,0.0f);
    EmitVertex();
    gl_Position = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) /3 + nor*(exp_gloss + 0.5*2.0f*pow(exp_gloss,2))+ vec4(0.25f,0.0f,0.0f,0.0f);
    color = vec4(exp_gloss, exp_gloss/2.5, exp_gloss/2.5,0.0f);
    EmitVertex();
    gl_Position = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) /3 + nor*(exp_gloss + 0.5*2.0f*pow(exp_gloss,2))+ vec4(-0.25f,0.0f,0.0f,0.0f);
    color = vec4(exp_gloss, exp_gloss/1.5, exp_gloss/1.5,0.0f);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    explode();   
}
