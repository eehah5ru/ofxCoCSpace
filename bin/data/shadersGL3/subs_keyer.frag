OF_GLSL_SHADER_HEADER

uniform sampler2DRect tex0;

// uniform sampler2D tex0;

// in vec2 texCoordVarying;

// out vec4 outputColor;

void main()
{
  // vec4 texel0 = texture(tex0, texCoordVarying);
  // vec3 s = step(0.5, texel0.rgb);
  // float blackness = dot(s, s);
  // if (blackness < 1) {
  //    texel0.a = 0.0;
  // } else {
  //   texel0.a = 1.0;
  // }
  
  // float gray0 = dot(texel0.rgb, vec3(0.299, 0.587, 0.114));
  // if(gray0 < 0.0) {
  //   texel0.a = 0.0;
  // } else {
  //   texel0.a = 1.0;
  // }

  vec4 texel0 = texture(tex0,  gl_FragCoord.xy);

  if (texel0.r < 0.1 && texel0.g < 0.1 && texel0.b < 0.1) {
     discard;
     // texel0.a = 0.0;
  } else {
    gl_FragColor = vec4(texel0.r, texel0.g, texel0.b, 1.0);
     // texel0.a = 1.0;
  }

  // outputColor = vec4(texel0.r, texel0.g, texel0.b, texel0.a);
}