#version 130

out vec4 res_Color;
uniform vec4 color;


void
main()
{
  res_Color = color;
  res_Color = vec4(1.0, 0.0, 0.0, 0.0);
};
