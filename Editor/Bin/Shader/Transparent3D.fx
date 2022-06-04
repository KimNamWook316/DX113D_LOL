
#include "ShaderInfo.fx"

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

struct Vertex3D
{
	// 변수 뒤에 : 레지스터이름 + 번호 로 지정한다.
	// 번호를 안붙이면 0으로 지정된다.
	float3 Pos : POSITION; // Vector3타입.
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float4 BlendWeight : BLENDWEIGHT;
	float4 BlendIndex : BLENDINDEX;
};

struct Vertex3DOutput
{
	// SV가 붙으면 System Value이다. 이 값은 레지스터에 저장만 하고
	// 가져다 사용하면 안된다.
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float4 ProjPos : POSITION;
	float3 ViewPos : POSITION1;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};

// Instancing
struct Vertex3DInstancing
{
	// 변수 뒤에 : 레지스터이름 + 번호 로 지정한다.
	// 번호를 안붙이면 0으로 지정된다.
	float3 Pos : POSITION; // Vector3타입.
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float4 BlendWeight : BLENDWEIGHT;
	float4 BlendIndex : BLENDINDEX;
	uint InstanceID : SV_InstanceID;
};


struct Vertex3DOutputInstancing
{
	// SV가 붙으면 System Value이다. 이 값은 레지스터에 저장만 하고
	// 가져다 사용하면 안된다.
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float4 ProjPos : POSITION;
	float3 ViewPos : POSITION1;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	uint InstanceID : TEXCOORD1;
};

struct SkinningInfo
{
	float3 Pos;
	float3 Normal;
	float3 Tangent;
	float3 Binormal;
};

struct InstancingInfo
{
	matrix matWV;
	matrix matWVP;
	float4 g_MtrlBaseColor;
	float4 g_MtrlAmbientColor;
	float4 g_MtrlSpecularColor;
	float4 g_MtrlEmissiveColor;
	float g_MtrlOpacity;
	int g_MtrlPaperBurnEnable;
	int g_MtrlBumpEnable;
	int g_MtrlAnimation3DEnable;
	int g_MtrlSpecularTex;
	int g_MtrlEmissiveTex;
	int g_MtrlReceiveDecal;
	float g_PaperBurnFilter;
	int g_PaperBurnInverse;
	float g_PaperBurnInFilter;
	float g_PaperBurnOutFilter;
	float g_PaperBurnCenterFilter;
	float4 g_PaperBurnInLineColor;
	float4 g_PaperBurnOutLineColor;
	float4 g_PaperBurnCenterLineColor;
};


StructuredBuffer<InstancingInfo> g_InstancingInfoArray : register(t40);
StructuredBuffer<LightInfo> g_LightInfoArray : register(t13);
StructuredBuffer<matrix> g_SkinningBoneMatrixArray : register(t106);

SkinningInfo Skinning(float3 Pos, float3 Normal, float3 Tangent, float3 Binormal,
    float4 BlendWeight, float4 BlendIndex)
{
	SkinningInfo Info = (SkinningInfo) 0;
    
	if (g_MtrlAnimation3DEnable == 0)
	{
		Info.Pos = Pos;
		Info.Normal = Normal;
		Info.Tangent = Tangent;
		Info.Binormal = Binormal;
        
		return Info;
	}
    
	for (int i = 0; i < 4; ++i)
	{
		if (BlendWeight[i] == 0.f)
			continue;
        
		matrix matBone = g_SkinningBoneMatrixArray[(int) BlendIndex[i]];
        
		Info.Pos += (mul(float4(Pos, 1.f), matBone) * BlendWeight[i]).xyz;
		Info.Normal += (mul(float4(Normal, 0.f), matBone) * BlendWeight[i]).xyz;
		Info.Tangent += (mul(float4(Tangent, 0.f), matBone) * BlendWeight[i]).xyz;
		Info.Binormal += (mul(float4(Binormal, 0.f), matBone) * BlendWeight[i]).xyz;
	}
    
	Info.Normal = normalize(Info.Normal);
	Info.Tangent = normalize(Info.Tangent);
	Info.Binormal = normalize(Info.Binormal);
    
	return Info;
}

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
	MtrlSpc = float4(0.5f, 0.5f, 0.5f, 1.f);
	
	float3 MtrlEmv = g_MtrlEmissiveColor.rgb;
	if (g_MtrlEmissiveTex)
	{
		MtrlEmv = g_EmissiveTexture.Sample(g_BaseSmp, UV).rgb;
	}


	result.Dif = Info.LightColor.xyz * MtrlDif * Intensity * Attn;
	result.Amb = Info.LightColor.xyz * 0.2f * MtrlAmb * Attn;
	
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
    
    // 뷰 공간의 Normal을 만들어준다.
	output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_matWV).xyz);
    // 뷰 공간의 Tangent을 만들어준다.
	output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_matWV).xyz);
    // 뷰 공간의 Binormal을 만들어준다.
	output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_matWV).xyz);
	output.UV = input.UV;

	return output;
}

PSOutput_Single Transparent3DPS(Vertex3DOutput input)
{
	PSOutput_Single output = (PSOutput_Single) 0;

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

	output.Color.rgb = BaseTextureColor.rgb * (LAcc.Dif.rgb + LAcc.Amb.rgb) +
        LAcc.Spc.rgb + LAcc.Emv.rgb;

	output.Color.a = g_MtrlOpacity;

	return output;
}

SkinningInfo SkinningInstancing(float3 Pos, float3 Normal, float3 Tangent, float3 Binormal,
    float4 BlendWeight, float4 BlendIndex, uint InstanceID)
{
	SkinningInfo Info = (SkinningInfo) 0;
    
	if (g_InstancingInfoArray[InstanceID].g_MtrlAnimation3DEnable == 0)
	{
		Info.Pos = Pos;
		Info.Normal = Normal;
		Info.Tangent = Tangent;
		Info.Binormal = Binormal;
        
		return Info;
	}
    
	for (int i = 0; i < 4; ++i)
	{
		if (BlendWeight[i] == 0.f)
			continue;
        
		matrix matBone = g_SkinningBoneMatrixArray[(InstanceID * g_InstancingBoneCount) + (int) BlendIndex[i]];
        
		Info.Pos += (mul(float4(Pos, 1.f), matBone) * BlendWeight[i]).xyz;
		Info.Normal += (mul(float4(Normal, 0.f), matBone) * BlendWeight[i]).xyz;
		Info.Tangent += (mul(float4(Tangent, 0.f), matBone) * BlendWeight[i]).xyz;
		Info.Binormal += (mul(float4(Binormal, 0.f), matBone) * BlendWeight[i]).xyz;
	}
    
	Info.Normal = normalize(Info.Normal);
	Info.Tangent = normalize(Info.Tangent);
	Info.Binormal = normalize(Info.Binormal);
    
    
	return Info;

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
    
    // 뷰 공간의 Normal을 만들어준다.
	output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    // 뷰 공간의 Tangent을 만들어준다.
	output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    // 뷰 공간의 Binormal을 만들어준다.
	output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
	output.UV = input.UV;

	return output;
}

float3 ComputeBumpNormalInstancing(float3 Normal, float3 Tangent, float3 Binormal,
	float2 UV, uint InstanceID)
{
	float3 result = Normal;
	
	if (g_InstancingInfoArray[InstanceID].g_MtrlBumpEnable == 1)
	{
		float4 NormalColor = g_NormalTexture.Sample(g_BaseSmp, UV);
		
		// 색상은 0 ~ 1 사이이므로 -1 ~ 1 사이의 법선벡터로 만들어준다.
		float3 ConvertNormal = NormalColor.xyz * 2.f - 1.f;
		// z는 무조건 + 방향으로 만들어준다.
		ConvertNormal.z = 1.f;
		ConvertNormal = normalize(ConvertNormal);
		
		// Tangent, Binormal, Normal을 이용해서 위에서 만들어준 법선
		// 벡터를 뷰공간으로 변환해준다. 뷰공간이 되는 이유는 Tangent,
		// Binormal, Normal 3개 모두 뷰공간으로 변환된 벡터가 들어오기
		// 때문이다.
		float3x3 mat =
		{
			Tangent,
			Binormal,
			Normal
		};

		result = normalize(mul(ConvertNormal, mat));
	}
	
	return result;
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
	MtrlSpc = float4(0.5f, 0.5f, 0.5f, 1.f);
	
	float3 MtrlEmv = g_InstancingInfoArray[InstanceID].g_MtrlEmissiveColor.rgb;
	if (g_MtrlEmissiveTex)
	{
		MtrlEmv = g_EmissiveTexture.Sample(g_BaseSmp, UV).rgb;
	}


	result.Dif = Info.LightColor.xyz * MtrlDif * Intensity * Attn;
	result.Amb = Info.LightColor.xyz * 0.2f * MtrlAmb * Attn;
	
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

PSOutput_Single TransparentInstancingPS(Vertex3DOutputInstancing input)
{
	PSOutput_Single output = (PSOutput_Single) 0;

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
		LResult = ComputeLightInstancing(Info, input.ViewPos, ViewNormal, input.UV, input.InstanceID);
		LAcc.Dif += LResult.Dif;
		LAcc.Amb += LResult.Amb;
		LAcc.Spc += LResult.Spc;
		LAcc.Emv += LResult.Emv;
	}

	output.Color.rgb = BaseTextureColor.rgb * (LAcc.Dif.rgb + LAcc.Amb.rgb) +
        LAcc.Spc.rgb + LAcc.Emv.rgb;

	output.Color.a = g_InstancingInfoArray[input.InstanceID].g_MtrlOpacity;

	return output;
}
