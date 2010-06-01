#version 130

out vec4 res_Color;
in vec2 out_v_tex;
uniform sampler2D tex;

void
main()
{
  res_Color = texture(tex, out_v_tex);
};
