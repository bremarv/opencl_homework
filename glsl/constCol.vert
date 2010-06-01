#version 130

in  vec3 in_v_Position;
out vec2 out_v_tex;

void
main()
{
  gl_Position = vec4(in_v_Position, 1.0);
  out_v_tex = (in_v_Position.xy+vec2(1))/2;
};
