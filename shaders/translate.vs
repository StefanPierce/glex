#version 130


in vec3 position;
in vec3 color;


out vec3 frag_color;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;


void main() {
   
       gl_Position = (Projection * View * Model) * vec4(position, 1);
      frag_color = color;
}
