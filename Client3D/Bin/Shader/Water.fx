
#include "TransparentInfo.fx"

Texture2DMS<float4> g_Depth : register(t11);

cbuffer WaterCBuffer : register(b13)
{
	float g_WaterSpeed;
	float g_WaterDistortThreshold;
	int2 g_WaterDepthOffset;
}

int2 g_MultiplyArr[4] =
{
	int2(1, 1),
	int2(-1, 1),
	int2(1, -1),
	int2(-1, -1)
};

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

	float4 BaseTexColor = g_BaseTexture.Sample(g_BaseSmp, Input.UV);

	if (BaseTexColor.a == 0.f || g_MtrlOpacity == 0.f)
	{
		clip(-1);
	}

	uint2 ScreenPos = (uint2) 0;
	ScreenPos.x = g_Resolution.x * ((Input.ProjPos.x / Input.ProjPos.w) * 0.5f + 0.5f);
	ScreenPos.y = g_Resolution.y * ((Input.ProjPos.y / Input.ProjPos.w) * -0.5f + 0.5f);

	float WorldDepth = g_Depth.Load(ScreenPos, 0).g;

	// 사방의 Depth를 검사해 임계값 이상의 깊이 차이가 난다면 Normal Texture를 참조해 물결파를 만듬
	// 막힌 방향 반대로 이동하는 UV 텍스쳐를 참조함, 현재 Depth보다 얕은 쪽에서 물결파가 일어남
	float3 DistortNormal;
	float3 InputNormal = Input.Normal;
	
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		float DistortDepth = g_Depth.Load(ScreenPos + (g_WaterDepthOffset * g_MultiplyArr[i]), 0).g;

		if (DistortDepth - WorldDepth > g_WaterDistortThreshold)
		{
			DistortNormal = ComputeBumpNormal(InputNormal, Input.Tangent, Input.Binormal,
					Input.UV + (-1 * g_MultiplyArr[i] * g_AccTime * g_WaterSpeed * 0.002f));
			Input.Normal += DistortNormal;
			Input.Normal *= 0.f;
		}
	}

	// 조명 계산
	LightInfo Info;
	LightResult LResult;
	LightResult LAcc;

	int LightCount = g_LightCount;
	
	for (int j = 0; j < LightCount; ++j)
	{
		Info = g_LightInfoArray[j];
		LResult = ComputeLight(Info, Input.ViewPos, Input.Normal, Input.UV);
		LAcc.Dif += LResult.Dif;
		LAcc.Amb += LResult.Amb;
		LAcc.Spc += LResult.Spc;
		LAcc.Emv += LResult.Emv;
	}

	Output.Color.rgb = BaseTexColor.rgb * (LAcc.Dif.rgb + LAcc.Amb.rgb) +
        LAcc.Spc.rgb + LAcc.Emv.rgb;

	// 픽셀 색상
	Output.Color.a = BaseTexColor.a * g_MtrlOpacity;

	return Output;
}