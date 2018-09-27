#include "Defines.hlsl"
#include "Structs.hlsl"

VS_OUTPUT VShader(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.position, 1.0f);
    output.position = mul(pos, mul(_modelMatrix, mul(_viewMatrix, _projectionMatrix)));
    output.uv = input.uv;
    output.normal = input.normal;
    return output;
}