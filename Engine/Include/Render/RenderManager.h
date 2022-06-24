#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture/RenderTarget.h"
#include "../Resource/Shader/InstancingCBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Shader/OutlineConstantBuffer.h"

struct RenderInstancingList
{
	std::list<class CSceneComponent*> RenderList;
	class CMesh* Mesh;
	CStructuredBuffer* Buffer;
	CStructuredBuffer* ShadowBuffer;
	int BufferCount; // 인스턴싱할 Scene Component의 개수
	CInstancingCBuffer* CBuffer;
	bool Animation;

	RenderInstancingList()
	{
		Mesh = nullptr;
		Animation = false;

		Buffer = new CStructuredBuffer;

		Buffer->Init("InstancingBuffer", sizeof(Instancing3DInfo), 100, 40, true, 
			(int)Buffer_Shader_Type::Vertex | (int)Buffer_Shader_Type::Pixel);

		BufferCount = 100;

		ShadowBuffer = new CStructuredBuffer;

		ShadowBuffer->Init("InstancingShadowBuffer", sizeof(Instancing3DInfo), 100, 40, true,
			(int)Buffer_Shader_Type::Vertex | (int)Buffer_Shader_Type::Pixel);

		CBuffer = new CInstancingCBuffer;
		CBuffer->Init();
	}

	~RenderInstancingList()
	{
		SAFE_DELETE(CBuffer);
		SAFE_DELETE(ShadowBuffer);
		SAFE_DELETE(Buffer);
	}
};

struct RenderLayer
{
	std::string		Name;
	int				LayerPriority;
	std::list<class CSceneComponent*> RenderList;
	std::vector<RenderInstancingList*> m_vecInstancing;
	int				InstancingIndex;
	int				RenderCount;

	RenderLayer()
	{
		LayerPriority = 0;
		RenderCount = 0;
		InstancingIndex = 0;
		m_vecInstancing.resize(30);

		for (int i = 0; i < 30; ++i)
		{
			m_vecInstancing[i] = new RenderInstancingList;
		}
	}

	~RenderLayer()
	{
		size_t Size = m_vecInstancing.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_vecInstancing[i]);
		}
	}
};

class CRenderManager
{
private:
	class CRenderStateManager* m_RenderStateManager;

private:
	const std::list<CSharedPtr<class CGameObject>>* m_ObjectList;
	std::vector<RenderLayer*>	m_RenderLayerList;
	class CStandard2DConstantBuffer* m_Standard2DCBuffer;
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaBlend;
	class CRenderState* m_AlphaBlendMRT;
	class CRenderState* m_LightAccBlend;
	CSharedPtr<class CShader> m_LightBlendShader;
	CSharedPtr<class CShader> m_LightBlendRenderShader;
	CSharedPtr<class CShader> m_Standard3DInstancingShader;
	CSharedPtr<class CShader> m_Transparent3DInstancingShader;

	// Final Target
	CSharedPtr<CRenderTarget> m_FinalTarget;
	
	// GBuffer
	std::vector<CSharedPtr<CRenderTarget>>	m_vecGBuffer;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecDecal;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecLightBuffer;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecTransparent;

	// Shadow
	bool m_Shadow;
	CSharedPtr<CRenderTarget> m_ShadowMapTarget;
	CSharedPtr<class CShader> m_ShadowMapShader;
	CSharedPtr<class CShader> m_ShadowMapInstancingShader;
	float m_ShadowLightDistance;
	class CShadowCBuffer* m_ShadowCBuffer;

	// Debug Render
	bool m_DebugRender;

	// Post Processing (HDR)
	bool m_PostProcessing;

 //	// Grayscale
 //	bool m_Gray;
 //	CSharedPtr<class CShader> m_GrayShader;

 //	// OutLine
 //	bool m_OutLine;
 //	CSharedPtr<class CShader> m_OutLineShader;
 //	class COutlineConstantBuffer* m_OutlineCBuffer;
 //	CSharedPtr<CRenderTarget> m_OutlineTarget;

	// Animation Editor 
	CSharedPtr<class CShader> m_Mesh3DNoLightRenderShader; // m_AnimEditorRenderTarget 에 그려내기 위한 Shader 
	CSharedPtr<CRenderTarget>	m_AnimEditorRenderTarget; // Skinning 처리 이후, 해당 출력을, 별도의 RenderTarget 에 그려낸다.

	// Particle Editor
	CSharedPtr<class CShader> m_ParticleShader; // m_AnimEditorRenderTarget 에 그려내기 위한 Shader 
	CSharedPtr<CRenderTarget>	m_ParticleEffectEditorRenderTarget; // Skinning 처리 이후, 해당 출력을, 별도의 RenderTarget 에 그려낸다.

	// Post Processing Renderer
	class CPostFXRenderer* m_PostFXRenderer;

public :
	CRenderStateManager* GetRenderStateManager() const
	{
		return m_RenderStateManager;
	}

public:
	float GetShadowLightDistance() const
	{
		return m_ShadowLightDistance;
	}

	class CStandard2DConstantBuffer* GetStandard2DCBuffer()	const
	{
		return m_Standard2DCBuffer;
	}

	CRenderTarget* GetAnimationRenderTarget() const
	{
		return m_AnimEditorRenderTarget;
	}

	CRenderTarget* GetParticleEffectRenderTarget() const
	{
		return m_ParticleEffectEditorRenderTarget;
	}

 //	void SetOutlineDepthMultiplier(float Val)
 //	{
 //		m_OutlineCBuffer->SetDepthMultiplier(Val);
 //	}
 //
 //	void SetOutlineDepthBias(float Val)
 //	{
 //		m_OutlineCBuffer->SetDepthBias(Val);
 //	}
 //
 //	void SetOutlineNormalMultiplier(float Val)
 //	{
 //		m_OutlineCBuffer->SetNormalMultiplier(Val);
 //	}
 //
 //	void SetOutlineNormalBias(float Val)
 //	{
 //		m_OutlineCBuffer->SetNormalBias(Val);
 //	}

 //	void GrayEnable(bool Enable)
 //	{
 //		m_Gray = Enable;
 //	}

	void SetDebugRender(bool DebugRender)
	{
		m_DebugRender = DebugRender;
	}

 //	float GetOutlineDepthMultiplier()
 //	{
 //		return m_OutlineCBuffer->GetDepthMultiplier();
 //	}
 //
 //	float GetOutlineDepthBias()
 //	{
 //		return m_OutlineCBuffer->GetDepthBias();
 //	}
 //
 //	float GetOutlineNormalMultiplier()
 //	{
 //		return m_OutlineCBuffer->GetNormalMultiplier();
 //	}
 //
 //	float GetOutlineNormalBias()
 //	{
 //		return m_OutlineCBuffer->GetNormalBias();
 //	}

	bool IsPostProcessingEnable()
	{
		return m_PostProcessing;
	}

	void EnablePostProcessing(bool Enable)
	{
		m_PostProcessing = Enable;
	}

	float GetMiddleGray() const;
	float GetLumWhite() const;
	float GetBloomThreshold() const;
	float GetBloomScale() const;

	void SetMiddleGray(float Gray);
	void SetLumWhite(float White);
	void SetBloomThreshold(float Threshold);
	void SetBloomScale(float Scale);

	void SetAdaptationTime(float Time);
	float GetAdaptationTime() const;

 //	bool IsGray()
 //	{
 //		return m_Gray;
 //	}

	bool IsDebugRender()
	{
		return m_DebugRender;
	}

public:
	void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* List)
	{
		m_ObjectList = List;
	}

	void AddRenderList(class CSceneComponent* Component);
	void CreateLayer(const std::string& Name, int Priority);
	void SetLayerPriority(const std::string& Name, int Priority);

public:
	bool Init();
	void Render(float DeltaTime);

private:
	void RenderSkyBox();
	void RenderShadowMap();
	void RenderGBuffer();
	void RenderDecal();
	void RenderLightAcc();
	void RenderLightBlend();
	// void RenderOutLine();
	void RenderTransparent();
	void RenderFinalScreen();
	// void RenderGray();
	void RenderAnimationEditor();
	void RenderParticleEffectEditor();

	// Render State
public:
	void SetBlendFactor(const std::string& Name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& Name, bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);
public:
	class CRenderState* FindRenderState(const std::string& Name);

private:
	void RenderDefaultInstancingInfo();
	void RenderDefaultInstancing();
	void RenderDefaultInstancingShadow();
	void RenderTransparentInstancingInfo();
	void RenderTransparentInstancing();

private :
	int GetRenderLayerIndex(const std::string& Name);

private:
	static bool Sortlayer(RenderLayer* Src, RenderLayer* Dest);
	static bool SortZ(CSceneComponent* Src, CSceneComponent* Dest);

	DECLARE_SINGLE(CRenderManager)
};

