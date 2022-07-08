
#include "Standard3DInfo.fx"

struct PS_OUTPUT_TRANSPARENT
{
	float4 ScreenColor : SV_Target;
	float4 Normal : SV_Target1;
	float4 Depth : SV_Target2;
};

cbuffer LightForwardCBuffer : register(b12)
{
	int g_LightCount;
	float3 g_LightForwardEmpty;
}

struct LightInfo
{
	float4 LightColor;
	int LightType;
	float3 LightPos;
	float3 LightDir;
	float LightDistance;
	float LightAngleIn;
	float LightAngleOut;
	float LightAtt1;
	float LightAtt2;
	float LightAtt3;
	float3 LightEmpty;
};

StructuredBuffer<LightInfo> g_LightInfoArray : register(t13);

LightResult ComputeLight(LightInfo Info, float3 Pos, float3 Normal, float2 UV)
{
	LightResult result = (LightResult) 0;
	
	float3 LightDir = (float3) 0.f;
	float Attn = 1.f;
	
	if (Info.LightType == LightTypeDir)
	{
		LightDir = -Info.LightDir;
		LightDir = normalize(LightDir);
	}
	
	
	if (Info.LightType == LightTypePoint)
	{
		LightDir = Info.LightPos - Pos;
		LightDir = normalize(LightDir);
		
		float Dist = distance(Info.LightPos, Pos);
		
		if (Dist > Info.LightDistance)
			Attn = 0.f;
		else
			Attn = 1.f / (Info.LightAtt1 + Info.LightAtt2 * Dist + Info.LightAtt3 * (Dist * Dist));
	}
	
	
	if (Info.LightType == LightTypeSpot)
	{
	}
	
	float3 ViewNormal = Normal;
	
	// 내적값이 음수가 나오면 0이 반환되고 양수가 나오면 해당 값이 그대로 반환된다.
	float Intensity = max(0.f, dot(ViewNormal, LightDir));
    
	float3 MtrlDif = g_MtrlBaseColor.rgb;
	float3 MtrlAmb = g_MtrlAmbientColor.rgb;

	float3 MtrlSpc = g_MtrlSpecularColor.rgb;
	if (g_MtrlSpecularTex)
	{
		MtrlSpc = g_SpecularTexture.Sample(g_BaseSmp, UV).rgb;
	}
	
	float3 MtrlEmv = g_MtrlEmissiveColor.rgb;
	if (g_MtrlEmissiveTex)
	{
		MtrlEmv = g_EmissiveTexture.Sample(g_BaseSmp, UV).rgb;
	}

    // Gamma Space To Linear Space
	MtrlDif = pow(MtrlDif, 2.2f);

	result.Dif = Info.LightColor.xyz * MtrlDif * Intensity * Attn;
	result.Amb = Info.LightColor.xyz * g_GLightAmbIntensity * MtrlAmb * Attn;

    // Linear Space To Gamma Space 
    // 다시 Gamma Space로 되돌림
	result.Dif = pow(result.Dif, 1 / 2.2f);
	
	float3 View = -Pos;
	View = normalize(View);
	
	// 퐁 쉐이딩
	float3 Reflect = 2.f * ViewNormal * dot(ViewNormal, LightDir) - LightDir;
	Reflect = normalize(Reflect);
	float SpcIntensity = max(0.f, dot(View, Reflect));
	
	// 블린-퐁 쉐이딩
    //float3 Reflect = normalize(View + LightDir);
    //float SpcIntensity = max(0.f, dot(ViewNormal, Reflect));
	
	result.Spc = Info.LightColor.xyz * MtrlSpc *
		pow(SpcIntensity, g_MtrlSpecularColor.w) * Attn;
	
	result.Emv = MtrlEmv;
	
	return result;
}

Vertex3DOutput Transparent3DVS(Vertex3D input)
{
	Vertex3DOutput output = (Vertex3DOutput) 0;

	SkinningInfo Info = Skinning(input.Pos, input.Normal, input.Tangent,
                input.Binormal, input.BlendWeight, input.BlendIndex);
    
	float3 Pos = Info.Pos;

	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;
    
    // 뷰공간의 위치를 만들어준다.
	output.ViewPos = mul(float4(Pos, 1.f), g_matWV).xyz;

	// 월드 공간 위치
	output.WorldPos = input.Pos;
    
    // 뷰 공간의 Normal을 만들어준다.
	output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_matWV).xyz);
    // 뷰 공간의 Tangent을 만들어준다.
	output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_matWV).xyz);
    // 뷰 공간의 Binormal을 만들어준다.
	output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_matWV).xyz);
	output.UV = input.UV;

	return output;
}

PS_OUTPUT_TRANSPARENT Transparent3DPS(Vertex3DOutput input)
{
	PS_OUTPUT_TRANSPARENT output = (PS_OUTPUT_TRANSPARENT) 0;

	float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, input.UV);

	if (BaseTextureColor.a == 0.f || g_MtrlOpacity == 0.f)
		clip(-1);

	float3 ViewNormal = ComputeBumpNormal(input.Normal, input.Tangent, input.Binormal, input.UV);

	LightInfo Info;
	LightResult LResult;
	LightResult LAcc;

	int LightCount = g_LightCount;
	
	for (int i = 0; i < LightCount; ++i)
	{
		Info = g_LightInfoArray[i];
		LResult = ComputeLight(Info, input.ViewPos, ViewNormal, input.UV);
		LAcc.Dif += LResult.Dif;
		LAcc.Amb += LResult.Amb;
		LAcc.Spc += LResult.Spc;
		LAcc.Emv += LResult.Emv;
	}

	output.ScreenColor.rgb = BaseTextureColor.rgb * (LAcc.Dif.rgb + LAcc.Amb.rgb) +
        LAcc.Spc.rgb + LAcc.Emv.rgb;

	// 픽셀 색상
	output.ScreenColor.a = BaseTextureColor.a * g_MtrlOpacity;

	// 노말
	output.Normal.rgb = ViewNormal;
	output.Normal.a = 1.f;

	// 깊이
	output.Depth.r = input.ProjPos.z / input.ProjPos.w;
	output.Depth.g = input.ProjPos.w;
	output.Depth.b = g_MtrlSpecularColor.w;
	output.Depth.a = 1.f;

	return output;
}

// Instancing
Vertex3DOutputInstancing TransparentInstancingVS(Vertex3DInstancing input)
{
	Vertex3DOutputInstancing output = (Vertex3DOutputInstancing) 0;
    
	SkinningInfo Info = SkinningInstancing(input.Pos, input.Normal, input.Tangent,
                input.Binormal, input.BlendWeight, input.BlendIndex, input.InstanceID);
    
	output.InstanceID = input.InstanceID;
    
	float3 Pos = Info.Pos;

	output.ProjPos = mul(float4(Pos, 1.f), g_InstancingInfoArray[input.InstanceID].matWVP);
	output.Pos = output.ProjPos;
    
    // 뷰공간의 위치를 만들어준다.
	output.ViewPos = mul(float4(Pos, 1.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz;

	// 월드 공간 위치
	output.WorldPos = input.Pos;
    
    // 뷰 공간의 Normal을 만들어준다.
	output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    // 뷰 공간의 Tangent을 만들어준다.
	output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    // 뷰 공간의 Binormal을 만들어준다.
	output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
	output.UV = input.UV;

	return output;
}

LightResult ComputeLightInstancing(LightInfo Info, float3 Pos, float3 Normal, float2 UV, uint InstanceID)
{
	LightResult result = (LightResult) 0;
	
	float3 LightDir = (float3) 0.f;
	float Attn = 1.f;
	
	if (Info.LightType == LightTypeDir)
	{
		LightDir = -Info.LightDir;
		LightDir = normalize(LightDir);
	}
	
	
	if (Info.LightType == LightTypePoint)
	{
		LightDir = Info.LightPos - Pos;
		LightDir = normalize(LightDir);
		
		float Dist = distance(Info.LightPos, Pos);
		
		if (Dist > Info.LightDistance)
			Attn = 0.f;
		else
			Attn = 1.f / (Info.LightAtt1 + Info.LightAtt2 * Dist + Info.LightAtt3 * (Dist * Dist));
	}
	
	
	if (Info.LightType == LightTypeSpot)
	{
	}
	
	float3 ViewNormal = Normal;
	
	// 내적값이 음수가 나오면 0이 반환되고 양수가 나오면 해당 값이 그대로 반환된다.
	float Intensity = max(0.f, dot(ViewNormal, LightDir));
    
	float3 MtrlDif = g_InstancingInfoArray[InstanceID].g_MtrlBaseColor.rgb;
	float3 MtrlAmb =  g_InstancingInfoArray[InstanceID].g_MtrlAmbientColor.rgb;

	float3 MtrlSpc = g_InstancingInfoArray[InstanceID].g_MtrlSpecularColor.rgb;
	if (g_MtrlSpecularTex)
	{
		MtrlSpc = g_SpecularTexture.Sample(g_BaseSmp, UV).rgb;
	}
	
	float3 MtrlEmv = g_InstancingInfoArray[InstanceID].g_MtrlEmissiveColor.rgb;
	if (g_MtrlEmissiveTex)
	{
		MtrlEmv = g_EmissiveTexture.Sample(g_BaseSmp, UV).rgb;
	}

    // Gamma Space To Linear Space
	MtrlDif = pow(MtrlDif, 2.2f);

	result.Dif = Info.LightColor.xyz * MtrlDif * Intensity * Attn;
	result.Amb = Info.LightColor.xyz * g_GLightAmbIntensity * MtrlAmb * Attn;

    // Linear Space To Gamma Space 
    // 다시 Gamma Space로 되돌림
	result.Dif = pow(result.Dif, 1 / 2.2f);
	
	float3 View = -Pos;
	View = normalize(View);
	
	// 퐁 쉐이딩
	float3 Reflect = 2.f * ViewNormal * dot(ViewNormal, LightDir) - LightDir;
	Reflect = normalize(Reflect);
	float SpcIntensity = max(0.f, dot(View, Reflect));
	
	// 블린-퐁 쉐이딩
    //float3 Reflect = normalize(View + LightDir);
    //float SpcIntensity = max(0.f, dot(ViewNormal, Reflect));
	
	result.Spc = Info.LightColor.xyz * MtrlSpc *
		pow(SpcIntensity, g_InstancingInfoArray[InstanceID].g_MtrlSpecularColor.w) * Attn;
	
	result.Emv = MtrlEmv;
	
	return result;
}

PS_OUTPUT_TRANSPARENT TransparentInstancingPS(Vertex3DOutputInstancing input)
{
	PS_OUTPUT_TRANSPARENT output = (PS_OUTPUT_TRANSPARENT) 0;

	int InstancingIndex = input.InstanceID + (g_InstancingObjecCount * g_InstancingMaterialIndex);

	float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, input.UV);

	if (BaseTextureColor.a == 0.f || g_MtrlOpacity == 0.f)
		clip(-1);

	float3 ViewNormal = ComputeBumpNormalInstancing(input.Normal, input.Tangent, input.Binormal, input.UV, input.InstanceID);

	LightInfo Info;
	LightResult LResult;
	LightResult LAcc;

	int LightCount = g_LightCount;
	
	for (int i = 0; i < LightCount; ++i)
	{
		Info = g_LightInfoArray[i];
		LResult = ComputeLightInstancing(Info, input.ViewPos, ViewNormal, input.UV, InstancingIndex);
		LAcc.Dif += LResult.Dif;
		LAcc.Amb += LResult.Amb;
		LAcc.Spc += LResult.Spc;
		LAcc.Emv += LResult.Emv;
	}

	output.ScreenColor.rgb = BaseTextureColor.rgb * (LAcc.Dif.rgb + LAcc.Amb.rgb) +
        LAcc.Spc.rgb + LAcc.Emv.rgb;

	// 픽셀 색상
	output.ScreenColor.a = BaseTextureColor.a * g_InstancingInfoArray[InstancingIndex].g_MtrlOpacity;

	// 노말
	output.Normal.rgb = ViewNormal;
	output.Normal.a = 1.f;

	// 깊이
	output.Depth.r = input.ProjPos.z / input.ProjPos.w;
	output.Depth.g = input.ProjPos.w;
	output.Depth.b = g_InstancingInfoArray[InstancingIndex].g_MtrlSpecularColor;
	output.Depth.a = 1.f;

	return output;
}
