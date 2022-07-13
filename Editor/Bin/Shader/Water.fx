
#include "TransparentInfo.fx"

Texture2DMS<float4> g_Depth : register(t11);

cbuffer WaterCBuffer : register(b13)
{
	float g_WaterSpeed;
	float g_WaterFoamDepthThreshold;
	float2 g_WaterEmpty;
}

Vertex3DOutput WaterVS(Vertex3D Input)
{
    Vertex3DOutput Output = (Vertex3DOutput) 0;
    
    float3 Pos = Input.Pos;

	Output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	Output.Pos = Output.ProjPos;
    
    // 뷰공간의 위치를 만들어준다.
	Output.ViewPos = mul(float4(Pos, 1.f), g_matWV).xyz;
	Output.WorldPos = Pos;

	Output.Normal = normalize(mul(float4(Input.Normal, 0.f), g_matWV).xyz);
	Output.Tangent = normalize(mul(float4(Input.Tangent, 0.f), g_matWV).xyz);
	Output.Binormal = normalize(mul(float4(Input.Binormal, 0.f), g_matWV).xyz);

    Output.UV = Input.UV;
	
	return Output;
}

PSOutput_Single WaterPS(Vertex3DOutput Input)
{
	PSOutput_Single Output = (PSOutput_Single) 0;

	if (g_MtrlOpacity == 0.f)
	{
		clip(-1);
	}

	float4 BaseColor = g_MtrlBaseColor;

	uint2 ScreenPos = (uint2) 0;
	ScreenPos.x = g_Resolution.x * ((Input.ProjPos.x / Input.ProjPos.w) * 0.5f + 0.5f);
	ScreenPos.y = g_Resolution.y * ((Input.ProjPos.y / Input.ProjPos.w) * -0.5f + 0.5f);

	float4 PrevDepth = g_Depth.Load(ScreenPos, 0);

	float2 MoveUV = (Input.UV * 8.f) + float2(g_AccTime, 0.f) * g_WaterSpeed * 0.2f;

	float DepthDiff = PrevDepth.g - Input.ViewPos.z;

	float3 NoiseColor = g_BaseTexture.Sample(g_BaseSmp, MoveUV).rgb;

	MoveUV = (Input.UV * 6.f) + float2(g_AccTime, g_AccTime) * g_WaterSpeed * 0.2f;
	float NoiseColor2 = g_BaseTexture.Sample(g_BaseSmp, MoveUV).rgb;

	NoiseColor += NoiseColor2;
	NoiseColor *= 0.5f;

	if (PrevDepth.r < 0.95f)
	{
		Output.Color.rgb = BaseColor.rgb;
	}
	else
	{
		if (DepthDiff < g_WaterFoamDepthThreshold)
		{
			DepthDiff = 1.f - (DepthDiff / g_WaterFoamDepthThreshold);

			float3 AddColor = float3(0.3f, 0.3f, 0.3f);

			if (DepthDiff <= (0.3f + NoiseColor.r))
			{
				if (NoiseColor.r < 0.35f)
				{
					AddColor = 0.f;
				}
			}
			BaseColor.rgb += AddColor;
		}
	}

	Output.Color.rgb = BaseColor.rgb;

	// 픽셀 색상
	Output.Color.a = 1.f;

	return Output;
}