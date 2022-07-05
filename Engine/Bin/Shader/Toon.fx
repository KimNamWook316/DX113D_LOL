#include "Standard3DInfo.fx"

cbuffer ToonCBuffer : register(b11)
{
	int g_ToonType;
	float3 g_ToonEmpty;
}

Vertex3DOutput ToonVS(Vertex3D input)
{
    Vertex3DOutput output = (Vertex3DOutput) 0;

    SkinningInfo Info = Skinning(input.Pos, input.Normal, input.Tangent,
                input.Binormal, input.BlendWeight, input.BlendIndex);
    
    float3 Pos = Info.Pos;
    output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
    output.Pos = output.ProjPos;
    output.ViewPos = mul(float4(Pos, 1.f), g_matWV).xyz;
    output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_matWV).xyz);
    output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_matWV).xyz);
    output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_matWV).xyz);
    output.UV = input.UV;

    return output;
}

PSOutput_GBuffer ToonPS(Vertex3DOutput input)
{
	PSOutput_GBuffer output = (PSOutput_GBuffer) 0;

    float4 BaseTextureColor = g_BaseTexture.Sample(g_BaseSmp, input.UV);

    if (BaseTextureColor.a == 0.f || g_MtrlOpacity == 0.f)
        clip(-1);
    
    output.Diffuse.rgb = BaseTextureColor.rgb;
    output.Diffuse.a = BaseTextureColor.a * g_MtrlOpacity;
    
    output.GBuffer1.rgb = ComputeBumpNormal(input.Normal, input.Tangent, input.Binormal, input.UV);
    output.GBuffer1.a = 1.f;
    
    output.GBuffer2.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer2.g = input.ProjPos.w;
    output.GBuffer2.b = g_MtrlSpecularColor.w;
    output.GBuffer2.a = 1.f;

    output.GBuffer3.r = ConvertColor(g_MtrlBaseColor);
    output.GBuffer3.g = ConvertColor(g_MtrlAmbientColor);

    // r : 어떤 Toon Ramp Texture 샘플링 해야 하는지
    // a : Toon Shader를 거쳐서 나온 픽셀이라는 것을 기록
	output.GBuffer4.r = (float)g_ToonType;
	output.GBuffer4.a = (float)Pixel_Type_Toon;
    
    output.GBuffer5.rgb = input.Binormal.xyz;
    output.GBuffer5.a = 1.f;
    
    float4 SpecularColor = g_MtrlSpecularColor.xyzw;
	
    if (g_MtrlSpecularTex)
        SpecularColor = g_SpecularTexture.Sample(g_BaseSmp, input.UV).xxxx;
    
    output.GBuffer3.b = ConvertColor(SpecularColor);
    
    float4 EmissiveColor = g_MtrlEmissiveColor.xyzw;
	
    if (g_MtrlEmissiveTex)
        EmissiveColor = g_EmissiveTexture.Sample(g_BaseSmp, input.UV).xxxx;

    output.GBuffer3.a = ConvertColor(EmissiveColor);

	return output;
}