
#include "ShaderInfo.fx"

TextureCube g_SkyTex : register(t20);

struct VS_SKY_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 UV : TEXCOORD;
};

VS_SKY_OUTPUT SkyVS(float3 Pos : POSITION)
{
	VS_SKY_OUTPUT output = (VS_SKY_OUTPUT) 0;

	output.Pos = mul(float4(Pos, 1.f), g_matWVP).xyww; // z를 무조건 1로 만들어준다.
	output.UV = Pos;

	return output;
}

float4 SkyPS(VS_SKY_OUTPUT input) : SV_TARGET
{
	float4 Color = g_SkyTex.Sample(g_BaseSmp, input.UV);
	return Color;
}
