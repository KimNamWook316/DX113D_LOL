#include "ShaderInfo.fx"

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
	float3 WorldPos : POSITION2;
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
	int		g_MtrlOutlineEnable;
	float	g_MtrlOutlineThickness; 
	float3  g_MtrlOutlineColor;
	float3  g_MtrlInstancingEmpty;
};

StructuredBuffer<InstancingInfo> g_InstancingInfoArray : register(t40);
StructuredBuffer<matrix> g_SkinningBoneMatrixArray : register(t106);

struct SkinningInfo
{
    float3 Pos;
    float3 Normal;
    float3 Tangent;
    float3 Binormal;
};

SkinningInfo Skinning(float3 Pos, float3 Normal, float3 Tangent, float3 Binormal,
    float4 BlendWeight, float4 BlendIndex)
{
    SkinningInfo Info = (SkinningInfo)0;
    
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
        
        matrix matBone = g_SkinningBoneMatrixArray[(int)BlendIndex[i]];
        
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

/*
11 12 13 14
21 22 23 24
31 32 33 34
41 42 43 44

원근투영
34 : 뷰공간의 z값

이 말은 투영변환까지 거치면 나오는
x, y, z, w 에서 w에는
뷰공간의 z값이 들어간다.

내부적으로 최종 화면에서의 위치를
구할때
투영의 x, y, z, w가 있으면 이 값을 모두 w로 나누어준다.

x / w, y / w, z / w, w / w
*/
