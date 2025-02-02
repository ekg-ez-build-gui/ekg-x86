#include "ekg/math/geometry.hpp"

void ekg::ortho(
  float *p_mat4x4,
  float left,
  float right,
  float bottom,
  float top
) {
  const float depth_near = -1.0f;
  const float depth_far = 1.0f;
  const float depth_inv = 1.0f / (depth_far - depth_near);
  const float y_inv = 1.0f / (top - bottom);
  const float x_inv = 1.0f / (right - left);

  p_mat4x4[0] = 2.0f * x_inv;
  p_mat4x4[1] = 0.0f;
  p_mat4x4[2] = 0.0f;
  p_mat4x4[3] = 0.0f;

  p_mat4x4[4] = 0.0f;
  p_mat4x4[5] = 2.0f * y_inv;
  p_mat4x4[6] = 0.0f;
  p_mat4x4[7] = 0.0f;

  p_mat4x4[8] = 0.0f;
  p_mat4x4[9] = 0.0f;
  p_mat4x4[10] = -2.0f * depth_inv;
  p_mat4x4[11] = 0.0f;

  p_mat4x4[12] = -(right + left) * x_inv;
  p_mat4x4[13] = -(top + bottom) * y_inv;
  p_mat4x4[14] = -(depth_far + depth_near) * depth_inv;
  p_mat4x4[15] = 1.0f;
}