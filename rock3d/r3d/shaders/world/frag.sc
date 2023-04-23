$input v_atlasinfo, v_texcoord, v_bright

#include <bgfx_shader.sh>

uniform sampler2D u_texture;

float Z_NEAR = 1.0;
float Z_FAR = 10000.0;

float linearDepth(float coord) {
    float ndcCoord = coord * 2.0 - 1.0;
    return (2.0 * Z_NEAR * Z_FAR) / (Z_FAR + Z_NEAR - ndcCoord * (Z_FAR - Z_NEAR));
}

float wrap(float coord, float origin, float len) {
    return mod(coord - origin, len) + origin;
}

void main() {
    float uAtOrigin = v_atlasinfo.x;
    float vAtOrigin = v_atlasinfo.y;
    float uAtLen = v_atlasinfo.z;
    float vAtLen = v_atlasinfo.w;

    vec2 texCord;
    texCord.x = wrap(v_texcoord.x, uAtOrigin, uAtLen);
    texCord.y = wrap(v_texcoord.y, vAtOrigin, vAtLen);

    vec4 color = texture2D(u_texture, texCord);
    color.x *= v_bright.x;
    color.y *= v_bright.y;
    color.z *= v_bright.z;
    // [AM] Uncomment this to show texture coordinates as red and blue gradient.
    // color.x = texCord.x;
    // color.y = 0.0;
    // color.z = texCord.y;
    gl_FragColor = color;
}
