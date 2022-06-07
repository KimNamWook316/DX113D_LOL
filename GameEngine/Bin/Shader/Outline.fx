#include "ShaderInfo.fx"

Texture2DMS<float4> g_GBufferDiffuse : register(t13);
Texture2DMS<float4> g_GBufferNormal : register(t14);
Texture2DMS<float4> g_GBufferDepth : register(t15);
//Texture2DMS<float4> g_LightBlendTex : register(t16);

cbuffer OutlineCBuffer : register (b10)
{
	float g_OutlineThickness;
	float g_OutlineDepthMultiplier;
	float g_OutlineDepthBias;
	float g_OutlineNormalMultiplier;
	float g_OutlineNormalBias;
	float3 g_OutlineColor;
};

struct VS_OUTPUT_OUTLINE
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
};

float3 SobelSample(int2 TargetPos, int3 Offset)
{
	float3 NormalCenter = g_GBufferNormal.Load(TargetPos, 0).rgb;
	float3 NormalLeft = g_GBufferNormal.Load(TargetPos - Offset.xz, 0).rgb;
	float3 NormalRight = g_GBufferNormal.Load(TargetPos + Offset.xz, 0).rgb;
	float3 NormalUp = g_GBufferNormal.Load(TargetPos - Offset.yz, 0).rgb;
	float3 NormalDown = g_GBufferNormal.Load(TargetPos + Offset.yz, 0).rgb;

	return abs(NormalLeft - NormalCenter) + abs(NormalRight - NormalCenter) + 
			abs(NormalUp - NormalCenter) + abs(NormalDown - NormalCenter);
}

float SobelDepth(float ldc, float ldl, float ldr, float ldu, float ldd)
{
	return abs(ldl - ldc) + abs(ldr - ldc) + abs(ldu - ldc) + abs(ldd - ldc);
}

float SobelSampleDepth(int2 TargetPos, int3 Offset)
{
	float DepthCenter = g_GBufferDepth.Load(TargetPos, 0).r;
	float DepthLeft = g_GBufferDepth.Load(TargetPos - Offset.xz, 0).r;
	float DepthRight = g_GBufferDepth.Load(TargetPos + Offset.xz, 0).r;
	float DepthUp = g_GBufferDepth.Load(TargetPos - Offset.yz, 0).r;
	float DepthDown = g_GBufferDepth.Load(TargetPos + Offset.yz, 0).r;

	return SobelDepth(DepthCenter, DepthLeft, DepthRight, DepthUp, DepthDown);
}

VS_OUTPUT_OUTLINE OutlineVS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT_OUTLINE output = (VS_OUTPUT_OUTLINE) 0;
    
    output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
    output.Pos = output.ProjPos;
    
    return output;
}

PSOutput_Single OutlinePS(VS_OUTPUT_OUTLINE Input)
{
	PSOutput_Single Output = (PSOutput_Single) 0;

	int3 Offset = int3(1,1,0) * g_OutlineThickness;

	float2 UV = (float2) 0;
	UV.x = Input.ProjPos.x / Input.ProjPos.w * 0.5f + 0.5f;
	UV.y = Input.ProjPos.y / Input.ProjPos.w * -0.5f + 0.5f;

	int2 TargetPos = (int2) 0;
	TargetPos.x = (int) (UV.x * g_Resolution.x);
	TargetPos.y = (int) (UV.y * g_Resolution.y);

	float4 ScreenColor = g_GBufferDiffuse.Load(TargetPos, 0);
	
	float4 DepthColor = g_GBufferDepth.Load(TargetPos, 0);

	if (DepthColor.a == 0.f)
	{
		clip(-1);
	}

	float SobelDepth = SobelSampleDepth(TargetPos, Offset);
	SobelDepth = pow(saturate(SobelDepth) * g_OutlineDepthMultiplier, g_OutlineDepthBias);

 //	float3 SobelNormalVec = SobelSample(TargetPos, Offset);
 //	float SobelNormal = SobelNormalVec.x + SobelNormalVec.y + SobelNormalVec.z;
 //	SobelNormal = pow(SobelNormal * g_OutlineNormalMultiplier, g_OutlineNormalBias);

	float SobelOutline = saturate(SobelDepth);
	//float SobelOutline = saturate(max(SobelDepth, SobelNormal));
	//float SobelOutline = saturate(SobelDepth);

	float3 OutlineColor = lerp(ScreenColor.rgb, g_OutlineColor.rgb, 0.f);
	float3 Color = lerp(ScreenColor.rgb, g_OutlineColor.rgb, SobelOutline);

	Output.Color.rgb = Color;

	Output.Color.a = SobelOutline;

	return Output;
}
