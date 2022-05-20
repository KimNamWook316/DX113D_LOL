
#include "ShaderInfo.fx"

struct VS_OUTPUT_DECAL
{
	float4 Pos : SV_POSITION;
	float4 ProjPos : POSITION;
};

struct VS_OUTPUT_DEBUGDECAL
{
	float4 Pos : SV_POSITION;
	float4 ProjPos : POSITION;
	float3 Normal : NORMAL;
};

struct PS_OUTPUT_DECAL
{
	float4 Diffuse : SV_Target;
	float4 Normal : SV_Target1;
	float4 Material : SV_Target2;
};

Texture2D g_GBufferDepth : register(t10);

VS_OUTPUT_DECAL DecalVS(float3 Pos : POSITION)
{
	VS_OUTPUT_DECAL output = (VS_OUTPUT_DECAL) 0;
    
	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
    
	return output;
}

PS_OUTPUT_DECAL DecalPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL) 0;
    
	float2 ScreenUV = input.ProjPos.xy / input.ProjPos.w;
	ScreenUV = ScreenUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    
	float4 Depth = g_GBufferDepth.Sample(g_BaseSmp, ScreenUV);
    
	// a == 0 이면, 물체가 출력되지 않은 지점이라는 것 -> 데칼이 입혀질 일이 없음
	if (Depth.a == 0.f)
		clip(-1);
    
	float4 Pos;
	// 원근 나누기 전의 투영공간의 좌표로 변환
	Pos.x = (ScreenUV.x * 2.f - 1.f) * Depth.g;
	Pos.y = (ScreenUV.y * -2.f + 1.f) * Depth.g;
	Pos.z = Depth.r * Depth.g;
	Pos.w = Depth.g;
    
    // 출력되어 있는 물체의 데칼 기준 로컬 공간에서의 좌표를 얻는다.
	float3 LocalPos = mul(Pos, g_matInvWVP).xyz;
    
    // DecalMesh의 로컬 크기는 -0.5 ~ 0.5 사이이므로 이 영역을 벗어나면
    // Decal 영역이 아닌걸로 판단한다.
	clip(0.5f - abs(LocalPos));
    
	return output;
}
