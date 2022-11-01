#version 130

in vec3 positionIn;
in vec4 colorIn;
out vec4 fragmentColor;
uniform mat4 projection = mat4(1.0);
uniform mat4 model = mat4(1.0);

void main() {
   gl_Position = projection * model * vec4(positionIn, 1.0f);
   fragmentColor = colorIn;
}
