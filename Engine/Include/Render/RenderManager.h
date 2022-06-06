#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture/RenderTarget.h"
#include "../Resource/Shader/InstancingCBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"

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
	class CRenderState* m_LightAccBlend;
	CSharedPtr<class CShader> m_LightBlendShader;
	CSharedPtr<class CShader> m_LightBlendRenderShader;
	CSharedPtr<class CShader> m_Standard3DInstancingShader;
	CSharedPtr<class CShader> m_Transparent3DInstancingShader;
	
	// GBuffer
	std::vector<CSharedPtr<CRenderTarget>>	m_vecGBuffer;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecDecal;
	std::vector<CSharedPtr<CRenderTarget>>	m_vecLightBuffer;

	// Shadow
	bool m_Shadow;
	CSharedPtr<CRenderTarget> m_ShadowMapTarget;
	CSharedPtr<class CShader> m_ShadowMapShader;
	CSharedPtr<class CShader> m_ShadowMapInstancingShader;
	float m_ShadowLightDistance;
	class CShadowCBuffer* m_ShadowCBuffer;

	// Animation Editor 
	CSharedPtr<class CShader> m_Mesh3DNoLightRenderShader; // m_AnimEditorRenderTarget 에 그려내기 위한 Shader 
	CSharedPtr<CRenderTarget>	m_AnimEditorRenderTarget; // Skinning 처리 이후, 해당 출력을, 별도의 RenderTarget 에 그려낸다.

	// Particle Editor
	CSharedPtr<class CShader> m_ParticleShader; // m_AnimEditorRenderTarget 에 그려내기 위한 Shader 
	CSharedPtr<CRenderTarget>	m_ParticleEffectEditorRenderTarget; // Skinning 처리 이후, 해당 출력을, 별도의 RenderTarget 에 그려낸다.

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
	void Render();

private:
	void RenderShadowMap();
	void RenderGBuffer();
	void RenderDecal();
	void RenderLightAcc();
	void RenderLightBlend();
	void RenderTransparent();
	void RenderFinalScreen();
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

