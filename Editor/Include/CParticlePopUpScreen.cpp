// IMGUI
#include "IMGUITextInput.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "EditorInfo.h"
#include "EditorUtil.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIImage.h"
#include "IMGUISliderFloat.h"
#include "Window/ObjectHierarchyWindow.h"
#include "IMGUIManager.h"
#include "IMGUIColor3.h"
#include "IMGUIColor4.h"
#include "IMGUIInputFloat.h"
#include "IMGUIComboBox.h"
#include "IMGUIDummy.h"
#include "IMGUITree.h"
// Engine
#include "CParticlePopUpScreen.h"
#include "Component/ParticleComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Render/RenderManager.h"
#include "Scene/SceneResource.h"

CParticlePopUpScreen::CParticlePopUpScreen()
{
}

CParticlePopUpScreen::~CParticlePopUpScreen()
{
}

bool CParticlePopUpScreen::Init()
{
    m_ParticleRenderTarget = AddWidget<CIMGUIImage>("Render Target", 500.f, 500.f);
    // m_ParticleRenderTarget->SetRenderTargetImage(true);
    m_ParticleRenderTarget->SetTexture(CRenderManager::GetInst()->GetParticleEffectRenderTarget());
    m_ParticleRenderTarget->SetBorderColor(10, 10, 255);
    m_ParticleRenderTarget->SetTableTitle("Render Target");

	return true;
}

void CParticlePopUpScreen::Update(float DeltaTime)
{
}
