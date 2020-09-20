
struct VSOut
{
    float3 color : Color;
    float4 pos : SV_Position;
};

cbuffer cBuf
{
    row_major matrix transform;
};

VSOut main(float2 pos : Position, float3 color : Color)
{
    VSOut vsout;
    vsout.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vsout.color = color;
    return vsout;
}