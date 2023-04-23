$input a_position, a_texcoord0, a_texcoord1, a_color0
$output v_atlasinfo, v_texcoord, v_bright

#include <bgfx_shader.sh>

void main() {
    v_atlasinfo = a_texcoord0;
    v_bright = a_color0;

    gl_Position = mul(u_viewProj, vec4(a_position, 1.0));

    float uAtOrigin = a_texcoord0.x;
    float vAtOrigin = a_texcoord0.y;
    float uAtLen = a_texcoord0.z;
    float vAtLen = a_texcoord0.w;

    v_texcoord.x = (a_texcoord1.x * uAtLen) + uAtOrigin;
    v_texcoord.y = (a_texcoord1.y * vAtLen) + vAtOrigin;
}
