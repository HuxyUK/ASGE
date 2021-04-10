const std::string fs_text =
R"(
#version 330 core
#define FRAG_COLOUR     0
in VertexData
{
    vec2    uvs;
    vec4    rgba;
} fs_in;

uniform sampler2D image;
layout  (location = FRAG_COLOUR, index = 0) out vec4 FragColor;

void main()
{
    vec4 atlas_sample = vec4(1.0, 1.0, 1.0, texture(image, fs_in.uvs).r);
    if (atlas_sample.a == 0.0) {
        discard;
    }

    FragColor = vec4(atlas_sample) * fs_in.rgba;
}
)";

const std::string fs_instancing =
R"(
#version 330 core
#define FRAG_COLOUR     0
in VertexData
{
    vec2    uvs;
    vec4    rgba;
} fs_in;

uniform sampler2D image;
layout  (location = FRAG_COLOUR, index = 0) out vec4 FragColor;

void main()
{
    FragColor = fs_in.rgba * texture(image, fs_in.uvs);
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
)";