struct VSInput
{
    float3 position : POSITION; // 入力：頂点座標
    float4 color : COLOR; // 入力：頂点色
};

cbuffer O_ConstantBuffer : register(b0)
{
    matrix view;
    matrix projection;
}

cbuffer O_ConstanBuffer : register(b1)
{
    matrix world;
    float4 color;
}

struct VSOutput
{
    float4 position : SV_POSITION; // 出力：変換後座標
    float4 color : COLOR; // 出力：頂点色
};


VSOutput vs(VSInput input)
{
    VSOutput output;

    // 3D座標を同次座標に変換
    float4 pos = float4(input.position, 1.0f);
    
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.position = pos;

    // 色情報をそのまま次の段階に渡す
    output.color = input.color;

    return output;
}

float4 ps(VSOutput input) : SV_TARGET
{
    // 補間された頂点色をそのまま出力
    return input.color * color;

    // より複雑な処理の例：
    // return input.color * 0.8f;           // 暗くする
    // return float4(1.0f, 0.0f, 0.0f, 1.0f); // 全て赤色
}