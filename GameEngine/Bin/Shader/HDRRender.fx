#include "ShaderInfo.fx"

cbuffer HDRRenderCBuffer : register(b10)
{
	float g_HDRMiddleGray;
	float g_HDRLumWhiteSqr;
	float g_HDRBloomScale;
	float g_HDRDOFMin;
	float g_HDRDOFMax;
	float3 g_HDRDOFEmpty;
}

Texture2DMS<float4> g_HDRTex : register(t10);
StructuredBuffer<float> g_AvgLum : register(t11);
Texture2D<float4> g_BloomTex : register(t12);
Texture2DMS<float4> g_GBufferDepth : register(t13);
Texture2D<float4> g_DownScaleTex : register(t14);

float3 DepthDOF(float3 ColorFocus, float3 ColorBlurred, float Depth)
{
	// 1. Depth <= DOFMin : Always ColorFocus
	// 2. Depth > DOFMin && Depth < DOFMax : 0 ~ 1
	// 3. Depth > DOFMax : Always Blurred
	float BlurFactor = saturate((Depth - g_HDRDOFMin) / (g_HDRDOFMax - g_HDRDOFMin));
	return lerp(ColorFocus, ColorBlurred, BlurFactor);
}

float3 ToneMapping(float3 HDRColor)
{
	// 현재 픽셀에 대한 휘도 스케일 계산
	float LScale = dot(float4(HDRColor, 0.f), LUM_FACTOR);
	LScale *= g_HDRMiddleGray / g_AvgLum[0];
	LScale = (LScale + ((LScale * LScale) / g_HDRLumWhiteSqr)) / (1.0 + LScale);

	return HDRColor * LScale;
}

struct VS_OUTPUT_HDR
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
};

VS_OUTPUT_HDR HDRRenderVS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT_HDR Output = (VS_OUTPUT_HDR) 0;

	Output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
	Output.Pos = Output.ProjPos;
    
	return Output;
}

PSOutput_Single HDRRenderPS(VS_OUTPUT_HDR Input)
{
	PSOutput_Single Output = (PSOutput_Single)0;

	float2 UV = (float2) 0;
	UV.x = Input.ProjPos.x / Input.ProjPos.w * 0.5f + 0.5f;
	UV.y = Input.ProjPos.y / Input.ProjPos.w * -0.5f + 0.5f;

	int2 TargetPos = (int2) 0;
	TargetPos.x = (int) (UV.x * g_Resolution.x);
	TargetPos.y = (int) (UV.y * g_Resolution.y);

	// 색상 샘플 계산
	float4 Color = g_HDRTex.Load(TargetPos, 0);

	// Bloom 분포 색상
	float4 BloomColor = g_BloomTex.Sample(g_LinearSmp, UV);
	BloomColor *= g_HDRBloomScale * BloomColor;

	float Alpha = Color.a + ((BloomColor.r + BloomColor.g + BloomColor.b) / 3.f);

	if (Alpha == 0.f)
	{
		clip(-1);
	}

	float4 Depth = g_GBufferDepth.Load(TargetPos, 0);

	// 가장 먼 평면 제외
	if (Depth.r < 1.f)
	{
		// DownScale된 Texture를 업스케일 하면서 블러 효과
		float3 ColorBlurred = g_DownScaleTex.Sample(g_LinearSmp, UV).rgb;

		// 깊이에 따라 원래 픽셀 색상과 블러 색상을 보간함
		Color.rgb = DepthDOF(Color.rgb, ColorBlurred, Depth.g);
	}

	// 원래 픽셀에 Bloom 추가
	Color += BloomColor;

	// 톤 매핑
	Color = float4(ToneMapping(Color.rgb), 1.f);

	Output.Color = Color;
	Output.Color.a = Alpha;

	return Output;
}
