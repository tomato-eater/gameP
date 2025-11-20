struct VSInput
{
    float3 position : POSITIONT;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VSOutput vs(VSInput input)
{
    VSOutput output;
    
    // 3D座標を同次座標に変換
    output.position = float4(input.position, 1.0f);
    
    // 色情報をそのまま次の段階に渡す
    output.color = input.color;
    
    return output;
}


struct PSInput
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

float4 ps(PSInput input)
{
    return input.color;
}