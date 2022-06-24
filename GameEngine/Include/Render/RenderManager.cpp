
#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh/AnimationMesh.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Viewport.h"
#include "../Engine.h"
#include "../Device.h"
#include "../Resource/Shader/Shader.h"
#include "../Component/PaperBurnComponent.h"
#include "../Resource/Shader/ShadowCBuffer.h"
#include "../Resource/Shader/OutlineConstantBuffer.h"
#include "PostFXRenderer.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr),
	m_Shadow(true),
	m_ShadowLightDistance(20.f),
	m_ShadowCBuffer(nullptr),
	//m_OutlineCBuffer(nullptr),
	//m_Gray(false),
	m_AlphaBlendMRT(nullptr),
	m_DebugRender(false),
	m_PostFXRenderer(nullptr),
	m_PostProcessing(false)
{
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_ShadowCBuffer);
	//SAFE_DELETE(m_OutlineCBuffer);

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_RenderLayerList.clear();

	SAFE_DELETE(m_Standard2DCBuffer);
	SAFE_DELETE(m_RenderStateManager);
	SAFE_DELETE(m_PostFXRenderer);
}

float CRenderManager::GetMiddleGray() const
{
	return m_PostFXRenderer->GetMiddleGray();
}

float CRenderManager::GetLumWhite() const
{
	return m_PostFXRenderer->GetLumWhite();
}

void CRenderManager::SetMiddleGray(float Gray)
{
	m_PostFXRenderer->SetMiddleGray(Gray);
}

void CRenderManager::SetLumWhite(float White)
{
	m_PostFXRenderer->SetLumWhite(White);
}

void CRenderManager::SetAdaptationTime(float Time)
{
	m_PostFXRenderer->SetAdaptationTime(Time);
}

float CRenderManager::GetAdaptationTime() const
{
	return m_PostFXRenderer->GetAdaptationTime();
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	RenderLayer* Layer = nullptr;

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetLayerName())
		{
			Layer = *iter;
			break;
		}
	}

	if (!Layer)
		return;

	Layer->RenderList.push_back(Component);
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	m_RenderStateManager->Init();

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;

	m_Standard2DCBuffer->Init();

	// 기본 레이어 생성
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = "Back";
	Layer->LayerPriority = 0;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Default";
	Layer->LayerPriority = 1;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Transparency";
	Layer->LayerPriority = 2;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Decal";
	Layer->LayerPriority = 3;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Particle";
	Layer->LayerPriority = 4;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ScreenWidgetComponent";
	Layer->LayerPriority = 5;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "AnimationEditorLayer";
	Layer->LayerPriority = 6;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ParticleEditorLayer";
	Layer->LayerPriority = 7;

	m_RenderLayerList.push_back(Layer);

	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");
	m_AlphaBlendMRT = m_RenderStateManager->FindRenderState("AlphaBlend_MRT"); // Transparent Render 패스에서 사용
	m_LightAccBlend = m_RenderStateManager->FindRenderState("LightAcc");

	// 디퍼드 렌더링용 타겟 생성
	Resolution RS = CDevice::GetInst()->GetResolution();

	if (!CResourceManager::GetInst()->CreateTarget("Diffuse",
		RS.Width, RS.Height, DXGI_FORMAT_R8G8B8A8_UNORM))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer1",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer2",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer3",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer4",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer5",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

 //	if (!CResourceManager::GetInst()->CreateTarget("GBufferOutlineInfo",
 //		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
 //		return false;

	CRenderTarget* GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("Diffuse");
	GBufferTarget->SetPos(Vector3(0.f, 0.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	GBufferTarget->SetPos(Vector3(0.f, 100.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");
	GBufferTarget->SetPos(Vector3(0.f, 200.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	GBufferTarget->SetPos(Vector3(0.f, 300.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer4");
	GBufferTarget->SetPos(Vector3(0.f, 400.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer5");
	GBufferTarget->SetPos(Vector3(0.f, 500.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

 //	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBufferOutlineInfo");
 //	GBufferTarget->SetPos(Vector3(0.f, 600.f, 0.f));
 //	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
 //	GBufferTarget->SetDebugRender(true);
 //	m_vecGBuffer.push_back(GBufferTarget);

	// Decal은 이미 생성된 렌더타겟에 렌더한다.
	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("Diffuse");
	m_vecDecal.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	m_vecDecal.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	m_vecDecal.push_back(GBufferTarget);

	if (!CResourceManager::GetInst()->CreateTarget("LightDif",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightSpc",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightEmv",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightDif");
	LightTarget->SetPos(Vector3(100.f, 0.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightSpc");
	LightTarget->SetPos(Vector3(100.f, 100.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightEmv");
	LightTarget->SetPos(Vector3(100.f, 200.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	if (!CResourceManager::GetInst()->CreateTarget("FinalScreen",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
	FinalScreenTarget->SetPos(Vector3(200.f, 0.f, 0.f));
	FinalScreenTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	FinalScreenTarget->SetDebugRender(true);

	// Animation Editor 용 Render Target 
	if (!CResourceManager::GetInst()->CreateTarget("AnimationEditorRenderTarget",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	m_AnimEditorRenderTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("AnimationEditorRenderTarget");
	m_AnimEditorRenderTarget->SetPos(Vector3(450.f, 100.f, 0.f));
	m_AnimEditorRenderTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	m_AnimEditorRenderTarget->SetDebugRender(false);

	// Particle Editor 용 Render Target
	if (!CResourceManager::GetInst()->CreateTarget("ParticleEffectRenderTarget",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	m_ParticleEffectEditorRenderTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("ParticleEffectRenderTarget");
	m_ParticleEffectEditorRenderTarget->SetPos(Vector3(600.f, 100.f, 0.f));
	m_ParticleEffectEditorRenderTarget->SetScale(Vector3(150.f, 150.f, 1.f));
	m_ParticleEffectEditorRenderTarget->SetDebugRender(false);
	m_ParticleEffectEditorRenderTarget->SetClearColor(0.6f, 0.6f, 0.6f, 0.6f);

 //	// Outline 용 Render Target
 //	if (!CResourceManager::GetInst()->CreateTarget("OutlineRenderTarget",
 //		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
 //		return false;

 //	// Outline Target
 //	m_OutlineTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("OutlineRenderTarget");
 //	m_OutlineTarget->SetPos(Vector3(800.f, 0.f, 0.f));
 //	m_OutlineTarget->SetScale(Vector3(300.f, 300.f, 1.f));
 //	m_OutlineTarget->SetDebugRender(true);

	// Transparent Target : FinalScreen, Normal, Depth, Outline
	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
	m_vecTransparent.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	m_vecTransparent.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");
	m_vecTransparent.push_back(GBufferTarget);

 //	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBufferOutlineInfo");
 //	m_vecTransparent.push_back(GBufferTarget);

	// Map 출력용 변수들

	// Shadow
	if (!CResourceManager::GetInst()->CreateTarget("ShadowMap", (unsigned int)SHADOWMAP_WIDTH, (unsigned int)SHADOWMAP_HEIGHT,
		DXGI_FORMAT_R32G32B32A32_FLOAT, true, DXGI_FORMAT_D24_UNORM_S8_UINT))
	{
		assert(false);
		return false;
	}

	m_ShadowMapTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("ShadowMap");
	m_ShadowMapTarget->SetPos(Vector3(300.f, 0.f, 0.f));
	m_ShadowMapTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	m_ShadowMapTarget->SetDebugRender(true);

	m_LightBlendShader = CResourceManager::GetInst()->FindShader("LightBlendShader");
	m_LightBlendRenderShader = CResourceManager::GetInst()->FindShader("LightBlendRenderShader");
	m_Mesh3DNoLightRenderShader = CResourceManager::GetInst()->FindShader("Mesh3DNoLightShader");
	m_Standard3DInstancingShader = CResourceManager::GetInst()->FindShader("Standard3DInstancingShader");
	m_ShadowMapShader = CResourceManager::GetInst()->FindShader("ShadowMapShader");
	m_ShadowMapInstancingShader = CResourceManager::GetInst()->FindShader("ShadowMapInstancingShader");
	m_Transparent3DInstancingShader = CResourceManager::GetInst()->FindShader("TransparentInstancing3DShader");
	// m_OutLineShader = CResourceManager::GetInst()->FindShader("OutlineShader");
	// m_GrayShader = CResourceManager::GetInst()->FindShader("GrayShader");

	m_ShadowCBuffer = new CShadowCBuffer;
	m_ShadowCBuffer->Init();

 //	m_OutlineCBuffer = new COutlineConstantBuffer;
 //	m_OutlineCBuffer->Init();

	// Post FX Renderer
	m_PostFXRenderer = new CPostFXRenderer;
	m_PostFXRenderer->Init();

	return true;
}

void CRenderManager::Render(float DeltaTime)
{
	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderList.clear();
			(*iter)->InstancingIndex = 0;

			auto iter1 = (*iter)->m_vecInstancing.begin();
			auto iter1End = (*iter)->m_vecInstancing.end();

			for (; iter1 != iter1End; ++iter1)
			{
				(*iter1)->RenderList.clear();
			}
		}
	}

	{
		auto	iter = m_ObjectList->begin();
		auto	iterEnd = m_ObjectList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}	
	}

	{
		const std::list<InstancingCheckCount*>* InstancingList = CSceneComponent::GetInstancingCheckList();

		auto	iter = InstancingList->begin();
		auto	iterEnd = InstancingList->end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->InstancingList.size() >= 10)
			{
				RenderLayer* Layer = nullptr;

				size_t	Size = m_RenderLayerList.size();

				for (size_t i = 0; i < Size; ++i)
				{
					if (m_RenderLayerList[i]->Name == (*iter)->LayerName)
					{
						Layer = m_RenderLayerList[i];
						break;
					}
				}

				if (Layer)
				{
					if (Layer->m_vecInstancing.size() == Layer->InstancingIndex)
					{
						Layer->m_vecInstancing.resize(Layer->InstancingIndex * 2);

						for (int i = 0; i < Layer->InstancingIndex; ++i)
						{
							Layer->m_vecInstancing[Layer->InstancingIndex + i] = new RenderInstancingList;
						}
					}

					if ((*iter)->InstancingList.size() > Layer->m_vecInstancing[Layer->InstancingIndex]->BufferCount)
					{
						int	Count = (int)Layer->m_vecInstancing[Layer->InstancingIndex]->BufferCount * 1.5f;

						if ((*iter)->InstancingList.size() > Count)
							Count = (int)(*iter)->InstancingList.size();

						SAFE_DELETE(Layer->m_vecInstancing[Layer->InstancingIndex]->Buffer);
						SAFE_DELETE(Layer->m_vecInstancing[Layer->InstancingIndex]->ShadowBuffer);

						Layer->m_vecInstancing[Layer->InstancingIndex]->Buffer = new CStructuredBuffer;
						Layer->m_vecInstancing[Layer->InstancingIndex]->Buffer->Init("InstancingBuffer", sizeof(Instancing3DInfo),
							Count, 40, true,
							(int)Buffer_Shader_Type::Vertex | (int)Buffer_Shader_Type::Pixel);

						Layer->m_vecInstancing[Layer->InstancingIndex]->ShadowBuffer = new CStructuredBuffer;
						Layer->m_vecInstancing[Layer->InstancingIndex]->Buffer->Init("InstancingShadowBuffer", sizeof(Instancing3DInfo),
							Count, 40, true,
							(int)Buffer_Shader_Type::Vertex | (int)Buffer_Shader_Type::Pixel);
					}

					auto	iter1 = (*iter)->InstancingList.begin();
					auto	iter1End = (*iter)->InstancingList.end();

					for (; iter1 != iter1End; ++iter1)
					{
						Layer->m_vecInstancing[Layer->InstancingIndex]->RenderList.push_back(*iter1);
					}

					Layer->m_vecInstancing[Layer->InstancingIndex]->Mesh = (*iter)->Mesh;

					if ((*iter)->Mesh->CheckType<CAnimationMesh>())
					{
						Layer->m_vecInstancing[Layer->InstancingIndex]->Animation = true;
						Layer->m_vecInstancing[Layer->InstancingIndex]->CBuffer->SetBoneCount(((CAnimationMesh*)(*iter)->Mesh)->GetBoneCount());
					}

					++Layer->InstancingIndex;
				}
			}
		}
	}

	// 환경맵 출력
	RenderSkyBox();
	
	// 인스턴싱 정보를 만든다.
	RenderDefaultInstancingInfo();

	// 그림자 맵을 만든다.
	RenderShadowMap();

	// GBuffer를 만들어낸다.
	RenderGBuffer();

	// Decal을 그려낸다.
	RenderDecal();

	// 조명 누적버퍼를 만들어낸다.
	RenderLightAcc();

	// 조명 누적버퍼와 GBuffer를 이용하여 최종화면을 만들어낸다.
	RenderLightBlend();

	// 반투명한 오브젝트들의 인스턴싱 정보 생성
	RenderTransparentInstancingInfo();

	// 반투명한 오브젝트를 그린다.
	RenderTransparent();

 //	// 외곽선을 그린다.
 //	RenderOutLine();

	if (m_PostProcessing)
	{
		// 적응 계수 계산
		m_PostFXRenderer->Adaptation(DeltaTime);

		CRenderTarget* FinalTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
		
		// 평균 휘도 계산
		m_PostFXRenderer->ExcuteDownScale(FinalTarget);

		// 백버퍼에 렌더
		m_PostFXRenderer->Render(FinalTarget, m_DepthDisable);
	}
	else
	{
		// 반투명 오브젝트 + 조명처리 + 외곽선 처리된 최종 화면을 백버퍼에 그려낸다.
		RenderFinalScreen();
	}


 //	// 흑백 효과
 //	if (m_Gray)
 //	{
 //		RenderGray();
 //	}

	// Animation Editor Animation Instance 제작용 Render Target
	RenderAnimationEditor();

	// Particle Effect Editor 제작용 Render Target
	RenderParticleEffectEditor();

	m_vecGBuffer[2]->SetShader(10, (int)Buffer_Shader_Type::Pixel, 0);

	m_AlphaBlend->SetState();

	// 파티클 레이어 출력
	auto	iter = m_RenderLayerList[4]->RenderList.begin();
	auto	iterEnd = m_RenderLayerList[4]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	m_AlphaBlend->ResetState();

	m_vecGBuffer[2]->ResetShader(10, (int)Buffer_Shader_Type::Pixel, 0);

	// Screen Widget 출력
	iter = m_RenderLayerList[5]->RenderList.begin();
	iterEnd = m_RenderLayerList[5]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	// 조명 정보를 Shader로 넘겨준다.
	//CSceneManager::GetInst()->GetScene()->GetLightManager()->SetShader();

	/*{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->Render();
			}
		}
	}*/

	{
		auto	iter1 = m_RenderLayerList.begin();
		auto	iter1End = m_RenderLayerList.end();

		for (; iter1 != iter1End; ++iter1)
		{
			auto	iter2 = (*iter1)->RenderList.begin();
			auto	iter2End = (*iter1)->RenderList.end();

			for (; iter2 != iter2End; ++iter2)
			{
				(*iter2)->PostRender();
			}
		}
	}


	// Widget 출력
	m_DepthDisable->SetState();

	m_AlphaBlend->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	if (m_DebugRender)
	{
		// 디버깅용 렌더타겟을 출력한다.
		CResourceManager::GetInst()->RenderTarget();
	}

	// 마우스 출력
	CWidgetWindow* MouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (MouseWidget)
		MouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::RenderSkyBox()
{
	CSharedPtr<CGameObject> SkyObj = CSceneManager::GetInst()->GetScene()->GetSkyObject();

	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");

	FinalScreenTarget->ClearTarget();
 //	FinalScreenTarget->SetTarget(nullptr);

	SkyObj->Render();

 //	FinalScreenTarget->ResetTarget();
}

void CRenderManager::RenderShadowMap()
{
	// 뷰포트 Shadowmap Textre와 일치하도록
	D3D11_VIEWPORT VP = {};

	VP.Width = SHADOWMAP_WIDTH;
	VP.Height = SHADOWMAP_HEIGHT;
	VP.MaxDepth = 1.f;

	CDevice::GetInst()->GetContext()->RSSetViewports(1, &VP);

	m_ShadowMapTarget->ClearTarget();
	m_ShadowMapTarget->SetTarget();
	
	m_ShadowMapShader->SetShader();

	for (size_t i = 0; i <= 1; ++i)
	{
		auto iter = m_RenderLayerList[i]->RenderList.begin();
		auto iterEnd = m_RenderLayerList[i]->RenderList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderShadowMap();
		}
	}

	RenderDefaultInstancingShadow();

	m_ShadowMapTarget->ResetTarget();

	D3D11_VIEWPORT PrevVP = {};

	PrevVP.Width = (float)CDevice::GetInst()->GetResolution().Width;
	PrevVP.Height = (float)CDevice::GetInst()->GetResolution().Height;
	PrevVP.MaxDepth = 1.f;

	CDevice::GetInst()->GetContext()->RSSetViewports(1, &PrevVP);
}

void CRenderManager::RenderGBuffer()
{
	// 현재 백버퍼로 렌더타겟이 지정되어 있다.
	// 이를 GBuffer 렌더타겟으로 교체해야 한다.
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	GBufferSize = m_vecGBuffer.size();

	vecPrevTarget.resize(GBufferSize);

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		m_vecGBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	// 현재 지정되어 있는 렌더타겟과 깊이타겟을 얻어온다.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecTarget[0], PrevDepthTarget);

	for (size_t i = 0; i <= 1; ++i)
	{
		auto	iter = m_RenderLayerList[i]->RenderList.begin();
		auto	iterEnd = m_RenderLayerList[i]->RenderList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render();

		}
	}

	RenderDefaultInstancing();

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < GBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderDecal()
{
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	DecalSize = m_vecDecal.size();

	vecPrevTarget.resize(DecalSize);

	for (size_t i = 0; i < DecalSize; ++i)
	{
		vecTarget.push_back(m_vecDecal[i]->GetTargetView());
	}

	// 현재 지정되어 있는 렌더타겟과 깊이타겟을 얻어온다.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)DecalSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)DecalSize,
		&vecTarget[0], PrevDepthTarget);

	m_vecGBuffer[2]->SetTargetShader(10);
	m_vecGBuffer[4]->SetTargetShader(11);
	m_vecGBuffer[5]->SetTargetShader(12);

	auto	iter = m_RenderLayerList[3]->RenderList.begin();
	auto	iterEnd = m_RenderLayerList[3]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	m_vecGBuffer[2]->ResetTargetShader(10);
	m_vecGBuffer[4]->ResetTargetShader(11);
	m_vecGBuffer[5]->ResetTargetShader(12);


	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)DecalSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < DecalSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}

	// 디버깅 출력
#ifdef _DEBUG

	std::vector<ID3D11RenderTargetView*>	vecTarget1;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget1;
	ID3D11DepthStencilView* PrevDepthTarget1 = nullptr;

	size_t	GBufferSize = m_vecGBuffer.size();

	vecPrevTarget1.resize(GBufferSize);

	for (size_t i = 0; i < GBufferSize; ++i)
	{
		vecTarget1.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	// 현재 지정되어 있는 렌더타겟과 깊이타겟을 얻어온다.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget1[0], &PrevDepthTarget1);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecTarget1[0], PrevDepthTarget1);

	iter = m_RenderLayerList[3]->RenderList.begin();
	iterEnd = m_RenderLayerList[3]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderDebug();
	}

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize,
		&vecPrevTarget1[0], PrevDepthTarget1);

	SAFE_RELEASE(PrevDepthTarget1);
	for (size_t i = 0; i < GBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget1[i]);
	}

#endif // _DEBUG
}

void CRenderManager::RenderLightAcc()
{
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	LightBufferSize = m_vecLightBuffer.size();

	vecPrevTarget.resize(LightBufferSize);

	for (size_t i = 0; i < LightBufferSize; ++i)
	{
		m_vecLightBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecLightBuffer[i]->GetTargetView());
	}

	// 현재 지정되어 있는 렌더타겟과 깊이타겟을 얻어온다.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)LightBufferSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)LightBufferSize,
		&vecTarget[0], PrevDepthTarget);


	m_LightAccBlend->SetState();

	m_DepthDisable->SetState();


	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[1]->SetTargetShader(15);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[3]->SetTargetShader(17);

	CSceneManager::GetInst()->GetScene()->GetLightManager()->Render();

	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecGBuffer[1]->ResetTargetShader(15);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[3]->ResetTargetShader(17);

	m_DepthDisable->ResetState();

	m_LightAccBlend->ResetState();


	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)LightBufferSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < LightBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderLightBlend()
{
	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");

	FinalScreenTarget->SetTarget(nullptr);

	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecLightBuffer[0]->SetTargetShader(18);
	m_vecLightBuffer[1]->SetTargetShader(19);
	m_vecLightBuffer[2]->SetTargetShader(20);
	m_ShadowMapTarget->SetTargetShader(22);

	m_LightBlendShader->SetShader();

	m_DepthDisable->SetState();

	CScene* Scene = CSceneManager::GetInst()->GetScene();
	Matrix matView = Scene->GetCameraManager()->GetCurrentCamera()->GetShadowViewMatrix();
	Matrix matProj = Scene->GetCameraManager()->GetCurrentCamera()->GetShadowViewMatrix();

	m_ShadowCBuffer->SetShadowVP(matView * matProj);
	m_ShadowCBuffer->UpdateCBuffer();

	// 아래코드를 사용한 이유는, Null Buffer 를 사용하기 때문이다.
	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CDevice::GetInst()->GetContext()->Draw(4, 0);


	m_DepthDisable->ResetState();

	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecLightBuffer[0]->ResetTargetShader(18);
	m_vecLightBuffer[1]->ResetTargetShader(19);
	m_vecLightBuffer[2]->ResetTargetShader(20);
	m_ShadowMapTarget->ResetTargetShader(22);

	FinalScreenTarget->ResetTarget();
}

 //void CRenderManager::RenderOutLine()
 //{
 //	m_OutlineTarget->ClearTarget();
 //	m_OutlineTarget->SetTarget(nullptr);
 //
 //	// ScreenColor, Normal, Depth, OutlineInfo
 //	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
 //
 //	FinalScreenTarget->SetTargetShader(13);
 //	m_vecGBuffer[1]->SetTargetShader(14);
 //	m_vecGBuffer[2]->SetTargetShader(15);
 //	m_vecGBuffer[6]->SetTargetShader(16);
 //	
 //	m_DepthDisable->SetState();
 //
 //	m_OutlineCBuffer->UpdateCBuffer();
 //	m_OutLineShader->SetShader();
 //
 //	UINT Offset = 0;
 //	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
 //	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
 //	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
 //
 //	CDevice::GetInst()->GetContext()->Draw(4, 0);
 //
 //	FinalScreenTarget->ResetTargetShader(13);
 //	m_vecGBuffer[1]->ResetTargetShader(14);
 //	m_vecGBuffer[2]->ResetTargetShader(15);
 //	m_vecGBuffer[6]->ResetTargetShader(16);
 //
 //	m_OutlineTarget->ResetTarget();
 //
 //	m_DepthDisable->ResetState();
 //}

void CRenderManager::RenderTransparent()
{
	// z소팅
	if (m_RenderLayerList[2]->RenderList.size() >= 2)
	{
		m_RenderLayerList[2]->RenderList.sort(CRenderManager::SortZ);
	}

	// 렌더타겟 설정
	std::vector<ID3D11RenderTargetView*>	vecTarget;
	std::vector<ID3D11RenderTargetView*>	vecPrevTarget;
	ID3D11DepthStencilView* PrevDepthTarget = nullptr;

	size_t	TransparentBufferSize = m_vecTransparent.size();

	vecPrevTarget.resize(TransparentBufferSize);

	for (size_t i = 0; i < TransparentBufferSize; ++i)
	{
		vecTarget.push_back(m_vecTransparent[i]->GetTargetView());
	}

	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)TransparentBufferSize,
		&vecPrevTarget[0], &PrevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)TransparentBufferSize,
		&vecTarget[0], PrevDepthTarget);

	// 모든 Light정보 구조화 버퍼에 담아서 넘긴다.
	CSceneManager::GetInst()->GetScene()->GetLightManager()->SetForwardRenderShader();

	// Forward Rendering
	auto iter = m_RenderLayerList[2]->RenderList.begin();
	auto iterEnd = m_RenderLayerList[2]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}

	RenderTransparentInstancing();

	CSceneManager::GetInst()->GetScene()->GetLightManager()->ResetForwardRenderShader();

	// 타겟 초기화
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)TransparentBufferSize,
		&vecPrevTarget[0], PrevDepthTarget);

	SAFE_RELEASE(PrevDepthTarget);
	for (size_t i = 0; i < TransparentBufferSize; ++i)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderFinalScreen()
{
	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");

	FinalScreenTarget->SetTargetShader(21);
	// m_OutlineTarget->SetTargetShader(23);

	m_LightBlendRenderShader->SetShader();

	m_DepthDisable->SetState();

	// 마지막 그리기 또한 Null Buffer 를 사용하여 그려낸다.
	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_DepthDisable->ResetState();

	// m_OutlineTarget->ResetTargetShader(23);
	FinalScreenTarget->ResetTargetShader(21);
}

void CRenderManager::RenderAnimationEditor()
{
	int AnimationEditorLayerIdx = GetRenderLayerIndex("AnimationEditorLayer");

	// 만~약에 해당 Layer 의 Idx 가 정해져 있지 않다면
	if (AnimationEditorLayerIdx == -1)
		return;

	// Animation Edtior 상에서 Animation Editor 제작 중이지 않다면
	if (m_RenderLayerList[AnimationEditorLayerIdx]->RenderList.size() <= 0)
		return;

	// Render Target 교체
	m_AnimEditorRenderTarget->ClearTarget();

	m_AnimEditorRenderTarget->SetTarget(nullptr);

	m_Mesh3DNoLightRenderShader->SetShader();

	// m_DepthDisable->SetState();

	// m_AnimationRenderTarget->SetTargetShader(55);

	auto iter = m_RenderLayerList[AnimationEditorLayerIdx]->RenderList.begin();
	auto iterEnd = m_RenderLayerList[AnimationEditorLayerIdx]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderAnimationEditor();
	}

	m_AnimEditorRenderTarget->ResetTarget();
 }

void CRenderManager::RenderParticleEffectEditor()
{
	int ParticleEffectEditorLayerIdx = GetRenderLayerIndex("ParticleEditorLayer");

	// 만~약에 해당 Layer 의 Idx 가 정해져 있지 않다면
	if (ParticleEffectEditorLayerIdx == -1)
		return;

	// Animation Edtior 상에서 Animation Editor 제작 중이지 않다면
	if (m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.size() <= 0)
		return;

	// Render Target 교체
	m_ParticleEffectEditorRenderTarget->ClearTarget();

	m_ParticleEffectEditorRenderTarget->SetTarget(nullptr);

	auto iter = m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.begin();
	auto iterEnd = m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->RenderParticleEffectEditor();
	}

	m_ParticleEffectEditorRenderTarget->ResetTarget();
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g,
	float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable,
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, 
	D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, 
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

int CRenderManager::GetRenderLayerIndex(const std::string& TargetName)
{
	for (size_t j = 0; j < m_RenderLayerList.size(); ++j)
	{
		if (m_RenderLayerList[j]->Name == TargetName)
			return j;
	}

	return -1;
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}

void CRenderManager::RenderDefaultInstancingInfo()
{
	for (int i = 0; i < m_RenderLayerList[1]->InstancingIndex; ++i)
	{
		// Material Slot 수만큼 반복한다.
		int	SlotCount = 0;
		if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
		{
			SlotCount = ((CStaticMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		else if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
		{
			SlotCount = ((CAnimationMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		for (int j = 0; j < SlotCount; ++j)
		{
			auto	iter = m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.begin();
			auto	iterEnd = m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.end();

			std::vector<Instancing3DInfo>	vecInfo;
			vecInfo.reserve(m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.size());

			// ShadowBuffer
			std::vector<Instancing3DInfo>	vecShadowInfo;
			vecShadowInfo.reserve(m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.size());

			CMaterial* Material = nullptr;

			for (; iter != iterEnd; ++iter)
			{
				if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
				{
					Material = ((CStaticMeshComponent*)(*iter))->GetMaterial(j);
				}

				else if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
				{
					Material = ((CAnimationMeshComponent*)(*iter))->GetMaterial(j);
				}

				Instancing3DInfo	Info = {};

				(*iter)->SetInstancingInfo(&Info);
				Material->GetCBuffer()->SetInstancingInfo(&Info);

				// 페이퍼번 정보
				if (Info.MtrlPaperBurnEnable == 1)
				{
					CPaperBurnComponent* PaperBurn = (*iter)->GetGameObject()->FindComponentFromType<CPaperBurnComponent>();
					if (PaperBurn)
					{
						PaperBurn->SetInstancingInfo(&Info);
					}
				}

				vecInfo.push_back(Info);

				Instancing3DInfo	ShadowInfo = {};

				(*iter)->SetInstancingShadowInfo(&ShadowInfo);
				Material->GetCBuffer()->SetInstancingInfo(&ShadowInfo);

				vecShadowInfo.push_back(ShadowInfo);
			}

			m_RenderLayerList[1]->m_vecInstancing[i]->Buffer->UpdateBuffer(&vecInfo[0],
				(int)vecInfo.size());
			m_RenderLayerList[1]->m_vecInstancing[i]->ShadowBuffer->UpdateBuffer(&vecShadowInfo[0],
				(int)vecShadowInfo.size());
		}
	}
}

void CRenderManager::RenderDefaultInstancing()
{
	// 이제 Default Layer 에 있는 Instancing집단. 들을 순회할 것이다.
	for (int i = 0; i < m_RenderLayerList[1]->InstancingIndex; ++i)
	{
		// Material Slot 수만큼 반복한다.
		int	SlotCount = 0;

		if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
		{
			SlotCount = ((CStaticMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		else if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
		{
			SlotCount = ((CAnimationMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		for (int j = 0; j < SlotCount; ++j)
		{
			CMaterial* Material = nullptr;

			if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
			{
				Material = ((CStaticMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterial(j);
			}

			else if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
			{
				Material = ((CAnimationMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterial(j);
			}

			if (Material)
				Material->RenderTexture();

			if (m_RenderLayerList[1]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[1]->m_vecInstancing[i]->Mesh)->SetBoneShader();
			}

			m_Standard3DInstancingShader->SetShader();

			m_RenderLayerList[1]->m_vecInstancing[i]->CBuffer->UpdateCBuffer();

			m_RenderLayerList[1]->m_vecInstancing[i]->Buffer->SetShader();

			m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->RenderInstancing((int)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.size(), j);

			m_RenderLayerList[1]->m_vecInstancing[i]->Buffer->ResetShader();


			if (m_RenderLayerList[1]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[1]->m_vecInstancing[i]->Mesh)->ResetBoneShader();
			}


			if (Material)
				Material->Reset();

		}
	}
}

void CRenderManager::RenderDefaultInstancingShadow()
{
	for (int i = 0; i < m_RenderLayerList[1]->InstancingIndex; ++i)
	{
		// Material Slot 수만큼 반복한다.
		int	SlotCount = 0;
		if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
		{
			SlotCount = ((CStaticMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		else if (m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
		{
			SlotCount = ((CAnimationMeshComponent*)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		for (int j = 0; j < SlotCount; ++j)
		{
			if (m_RenderLayerList[1]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[1]->m_vecInstancing[i]->Mesh)->SetBoneShader();
			}

			m_ShadowMapInstancingShader->SetShader();

			m_RenderLayerList[1]->m_vecInstancing[i]->CBuffer->UpdateCBuffer();

			m_RenderLayerList[1]->m_vecInstancing[i]->ShadowBuffer->SetShader();

			m_RenderLayerList[1]->m_vecInstancing[i]->Mesh->RenderInstancing((int)m_RenderLayerList[1]->m_vecInstancing[i]->RenderList.size(), j);


			m_RenderLayerList[1]->m_vecInstancing[i]->ShadowBuffer->ResetShader();



			if (m_RenderLayerList[1]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[1]->m_vecInstancing[i]->Mesh)->ResetBoneShader();
			}
		}
	}
}

void CRenderManager::RenderTransparentInstancingInfo()
{
	for (int i = 0; i < m_RenderLayerList[2]->InstancingIndex; ++i)
	{
		// Material Slot 수만큼 반복한다.
		int	SlotCount = 0;
		if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
		{
			SlotCount = ((CStaticMeshComponent*)m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		else if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
		{
			SlotCount = ((CAnimationMeshComponent*)m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		for (int j = 0; j < SlotCount; ++j)
		{
			auto	iter = m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.begin();
			auto	iterEnd = m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.end();

			std::vector<Instancing3DInfo>	vecInfo;
			vecInfo.reserve(m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.size());

			CMaterial* Material = nullptr;

			for (; iter != iterEnd; ++iter)
			{
				if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
				{
					Material = ((CStaticMeshComponent*)(*iter))->GetMaterial(j);
				}

				else if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
				{
					Material = ((CAnimationMeshComponent*)(*iter))->GetMaterial(j);
				}

				Instancing3DInfo	Info = {};

				(*iter)->SetInstancingInfo(&Info);
				Material->GetCBuffer()->SetInstancingInfo(&Info);

				// 페이퍼번 정보
				if (Info.MtrlPaperBurnEnable == 2)
				{
					CPaperBurnComponent* PaperBurn = (*iter)->GetGameObject()->FindComponentFromType<CPaperBurnComponent>();
					if (PaperBurn)
					{
						PaperBurn->SetInstancingInfo(&Info);
					}
				}

				vecInfo.push_back(Info);

				Instancing3DInfo	ShadowInfo = {};
			}

			m_RenderLayerList[2]->m_vecInstancing[i]->Buffer->UpdateBuffer(&vecInfo[0],
				(int)vecInfo.size());
		}
	}
}

void CRenderManager::RenderTransparentInstancing()
{
	for (int i = 0; i < m_RenderLayerList[2]->InstancingIndex; ++i)
	{
		// Material Slot 수만큼 반복한다.
		int	SlotCount = 0;

		if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
		{
			SlotCount = ((CStaticMeshComponent*)m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		else if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
		{
			SlotCount = ((CAnimationMeshComponent*)m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.back())->GetMaterialSlotCount();
		}

		for (int j = 0; j < SlotCount; ++j)
		{
			CMaterial* Material = nullptr;

			if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Static)
			{
				Material = ((CStaticMeshComponent*)m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.back())->GetMaterial(j);
			}

			else if (m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->GetMeshType() == Mesh_Type::Animation)
			{
				Material = ((CAnimationMeshComponent*)m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.back())->GetMaterial(j);
			}

			if (Material)
				Material->RenderTexture();

			if (m_RenderLayerList[2]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[2]->m_vecInstancing[i]->Mesh)->SetBoneShader();
			}

			m_AlphaBlendMRT->SetState();

			m_Transparent3DInstancingShader->SetShader();

			m_RenderLayerList[2]->m_vecInstancing[i]->CBuffer->UpdateCBuffer();

			m_RenderLayerList[2]->m_vecInstancing[i]->Buffer->SetShader();

			m_RenderLayerList[2]->m_vecInstancing[i]->Mesh->RenderInstancing((int)m_RenderLayerList[2]->m_vecInstancing[i]->RenderList.size(), j);

			m_RenderLayerList[2]->m_vecInstancing[i]->Buffer->ResetShader();

			m_AlphaBlendMRT->ResetState();

			if (m_RenderLayerList[2]->m_vecInstancing[i]->Animation)
			{
				((CAnimationMesh*)m_RenderLayerList[2]->m_vecInstancing[i]->Mesh)->ResetBoneShader();
			}


			if (Material)
				Material->Reset();

		}
	}
}

bool CRenderManager::Sortlayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}

bool CRenderManager::SortZ(CSceneComponent* Src, CSceneComponent* Dest)
{
	return Src->GetViewZ() > Dest->GetViewZ();
}

 //void CRenderManager::RenderGray()
 //{
 //	CRenderTarget* FinalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
 //	FinalScreenTarget->SetTargetShader(21);
 //
 //	m_DepthDisable->SetState();
 //	m_GrayShader->SetShader();
 //
 //	UINT Offset = 0;
 //	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
 //	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
 //	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
 //
 //	CDevice::GetInst()->GetContext()->Draw(4, 0);
 //
 //	m_DepthDisable->ResetState();
 //	FinalScreenTarget->ResetTargetShader(21);
 //}

/*
void CRenderManager::RenderParticleEffectEditor()
{
	int ParticleEffectEditorLayerIdx = GetRenderLayerIndex("ParticleEditorLayer");

	// 만~약에 해당 Layer 의 Idx 가 정해져 있지 않다면
	if (ParticleEffectEditorLayerIdx == -1)
		return;

	// Animation Edtior 상에서 Animation Editor 제작 중이지 않다면
	if (m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.size() <= 0)
		return;

	// Render Target 교체
	m_ParticleEffectEditorRenderTarget->ClearTarget();

	m_ParticleEffectEditorRenderTarget->SetTarget(nullptr);

	auto iter = m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.begin();
	auto iterEnd = m_RenderLayerList[ParticleEffectEditorLayerIdx]->RenderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderParticleEffectEditor();
	}

	CSharedPtr<CGameObject> SkyObj = CSceneManager::GetInst()->GetScene()->GetSkyObject();
	SkyObj->GetRootComponent()->RenderParticleEffectEditor();

	// Sample Sky 그리기

	m_ParticleEffectEditorRenderTarget->ResetTarget();
}
*/