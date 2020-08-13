
#define CT_FULL_SCREEN_VERTEX_OUT \
    VertexOut                     \
    {                             \
        vec2 pos;                 \
        vec2 uv;                  \
    }

#define CT_FULL_SCREEN_VS                                     \
    layout(location = 0) in vec2 inPos;                       \
    layout(location = 1) in vec2 inUV;                        \
    layout(location = 0) out CT_FULL_SCREEN_VERTEX_OUT vOut;  \
    void main()                                               \
    {                                                         \
        vOut.pos = vec2(inPos.x, -inPos.y);                   \
        vOut.uv = inUV;                                       \
        gl_Position = vec4(vOut.pos.x, vOut.pos.y, 0.0, 1.0); \
    }

#define CT_FULL_SCREEN_PS \
    layout(location = 0) in CT_FULL_SCREEN_VERTEX_OUT vOut;