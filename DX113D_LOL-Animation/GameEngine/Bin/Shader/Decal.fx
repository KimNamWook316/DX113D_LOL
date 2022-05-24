
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

Texture2DMS<float4> g_GBufferDepth : register(t10);
Texture2DMS<float4> g_GBufferTangent : register(t11);
Texture2DMS<float4> g_GBufferBinormal : register(t12);

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
    
	// 픽셀의 UV 값을, 이용해서, 
	// (0에서 1사이의 값) Depth Buffer 에서 UV 에 해당하는 깊이값을 가져온다.
	// 즉, 원래 대로 라면, 해당 UV 좌표값 위치에 현재 Decal 내 특정 pixel 이 기록되어야 하는데
	// 거기에 이미 기록되어 있는 대상의 정보를 가져와서 확인하는 것이다. 
	
	// input.ProjPos.w; 에는, 뷰공간에서의 Z 값이 있다. 
	// 즉, 결과적으로 아래의 식은, Z 로 나눈, 원근 나누기를 적용한 것
	// -1 에서 1 사이의 투영공간에서의 x,y 위치를 얻어낸 것이다.
	float2 ScreenUV = input.ProjPos.xy / input.ProjPos.w;

	// 0에서 1 사이의 UV 값으로 변경
	ScreenUV = ScreenUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	// g_GBufferDepth 는 Standard3D.fx 에서 GBuffer2 로 만들어준 Render Target 정보이다
	// rgb 중에서 g 에 투영공간에서의 w 값, 즉 뷰공간에서의 z 값을 저장해두었었다.

	int2 TargetPos = (int2) 0;
	
	TargetPos.x = (int) (ScreenUV.x * g_Resolution.x);
	TargetPos.y = (int) (ScreenUV.y * g_Resolution.y);
    
	float4 Depth = g_GBufferDepth.Load(TargetPos, 0);
    
	// a == 0 이면, 물체가 출력되지 않은 지점이라는 것 -> 데칼이 입혀질 일이 없음
	if (Depth.a == 0.f)
		clip(-1);
    
	// 최종적으로 Local 공간의 위치값으로 변경시켜야 한다.
	// Depth.g --> 뷰공간에서의 깊이값
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
       
	float2 UV = LocalPos.xz + float2(0.5f, 0.5f);
	UV.y = 1.f - UV.y;
    
	float4 Color = g_BaseTexture.Sample(g_BaseSmp, UV);
    
	if (Color.a == 0.f)
		clip(-1);
    
	float4 TangentColor = g_GBufferTangent.Load(TargetPos, 0);
	int ReceiveDecal = TangentColor.a;

	// 현재 버퍼에 그려진 픽셀이 데칼을 적용받지 않는 물체의 픽셀이라면
	if (ReceiveDecal == 0)
	{
		clip(-1);
	}
	
	float3 Tangent = TangentColor.rgb;
	float3 Binormal = g_GBufferBinormal.Load(TargetPos, 0).rgb;
	float3 Normal = normalize(cross(Tangent, Binormal));
    
	output.Normal.rgb = ComputeBumpNormal(Normal, Tangent, Binormal, UV);
    //output.Normal.rgb = g_NormalTexture.Sample(g_BaseSmp, UV).rgb;
	output.Normal.a = 1.f;
    
	output.Diffuse.rgb = Color.rgb;
	output.Diffuse.a = Color.a * g_MtrlOpacity;
    
	output.Material.r = ConvertColor(g_MtrlBaseColor);
	output.Material.g = ConvertColor(g_MtrlAmbientColor);
    
	float4 SpecularColor = g_MtrlSpecularColor.xyzw;
	
	if (g_MtrlSpecularTex)
		SpecularColor = g_SpecularTexture.Sample(g_BaseSmp, UV).rgba;
	
    //SpecularColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    
	output.Material.b = ConvertColor(SpecularColor);
    
	float4 EmissiveColor = g_MtrlEmissiveColor.xyzw;
	
	if (g_MtrlEmissiveTex)
		EmissiveColor = g_EmissiveTexture.Sample(g_BaseSmp, UV).rgba;
    
	output.Material.a = ConvertColor(EmissiveColor);
     
	return output;
}

VS_OUTPUT_DEBUGDECAL DecalDebugVS(float3 Pos : POSITION)
{
	VS_OUTPUT_DEBUGDECAL output = (VS_OUTPUT_DEBUGDECAL) 0;
    
	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
	output.Normal = normalize(mul(float4(Pos, 0.f), g_matWV).xyz);
    
	return output;
}

PSOutput_GBuffer DecalDebugPS(VS_OUTPUT_DEBUGDECAL input)
{
	PSOutput_GBuffer output = (PSOutput_GBuffer) 0;
    
	output.Diffuse = float4(0.f, 1.f, 0.f, 1.f);
    
	output.GBuffer1.rgb = input.Normal;
	output.GBuffer1.a = 1.f;
    
	output.GBuffer2.r = input.ProjPos.z / input.ProjPos.w;
	output.GBuffer2.g = input.ProjPos.w;
	output.GBuffer2.b = 0.f;
	output.GBuffer2.a = 1.f;
    
	output.GBuffer3.r = ConvertColor(float4(1.f, 1.f, 1.f, 1.f));
	output.GBuffer3.g = ConvertColor(float4(0.f, 0.f, 0.f, 0.f));
	output.GBuffer3.z = ConvertColor(float4(0.f, 0.f, 0.f, 0.f));
	output.GBuffer3.w = ConvertColor(float4(0.f, 0.f, 0.f, 0.f));
    
	return output;
}
