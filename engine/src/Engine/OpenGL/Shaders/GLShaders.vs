const std::string vs_instancing =
R"(
#version 430 core

struct Quad {
  mat4 model_matrix;
  vec4 color;
  vec4 uv_data[4];
};

layout (location = 0) in vec2 position;
layout (location = 2) uniform int quad_buffer_offset;

layout (std140, binding=1) uniform global_shader_data
{
    mat4 projection;
};

layout (std140, binding=10) buffer ssbo_buffer
{
    Quad quads[];
};

out VertexData
{
    vec2    uvs;
    vec4    rgba;
}  vs_out;

void main()
{
    // Calculate the offset into the SSBO
    int instance_offset = gl_InstanceID+quad_buffer_offset;

    // Calculate the final pixel position
    gl_Position  = projection * (quads[instance_offset].model_matrix * vec4(position.xy, 0.0, 1.0));

    // Pass the per-instance color through to the fragment shader.
    vs_out.rgba = quads[instance_offset].color;

    // Pass on the texture coordinate mappings
    vs_out.uvs[0] = quads[instance_offset].uv_data[gl_VertexID][0];
    vs_out.uvs[1] = quads[instance_offset].uv_data[gl_VertexID][1];
}
)";


const std::string vs_instancing_legacy =
R"(
  #version 330 core

  #define MAX_NUM_TOTAL_QUADS     400
  struct Quad {
      mat4 model_matrix;   //     64B
      vec4 color;          //    +32B
      vec4 uv_data[4];     //    +64B
                           // =======
                           //    160B
  };

  layout (location = 0) in vec2 position;
  uniform int quad_buffer_offset;

  layout (std140) uniform global_shader_data
  {
      mat4 projection;
  };

  layout (std140) uniform render_quads
  {
      Quad quads[MAX_NUM_TOTAL_QUADS];
  };

  out VertexData
  {
      vec2    uvs;
      vec4    rgba;
  }  vs_out;

  void main()
  {
    // Calculate the offset into the UBO
    int instance_offset = gl_InstanceID + quad_buffer_offset;

    // Final position
    gl_Position  = projection * (quads[instance_offset].model_matrix * vec4(position.xy, 0.0, 1.0));

    // Pass the per-instance color through to the fragment shader.
    vs_out.rgba = quads[instance_offset].color;

    // Pass on the texture coordinate mappings
    vs_out.uvs[0] = quads[instance_offset].uv_data[gl_VertexID][0];
    vs_out.uvs[1] = quads[instance_offset].uv_data[gl_VertexID][1];
  }
)";
