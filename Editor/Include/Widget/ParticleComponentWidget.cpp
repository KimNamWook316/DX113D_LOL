#include "ParticleComponentWidget.h"
#include "Component/ParticleComponent.h"
#include "IMGUITextInput.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIImage.h"
#include "IMGUISliderFloat.h"
#include "IMGUIColor3.h"
#include "IMGUIColor4.h"
#include "IMGUIInputFloat.h"
#include "IMGUIComboBox.h"
#include "IMGUIDummy.h"
#include "IMGUITree.h"
#include "Component/StaticMeshComponent.h"
#include "Resource/Particle/Particle.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"

CParticleComponentWidget::CParticleComponentWidget()
{
}

CParticleComponentWidget::~CParticleComponentWidget()
{
}

bool CParticleComponentWidget::Init()
{
    CSceneComponentWidget::Init();

    // AddWidget
    m_ComponentTypeText->SetText("Particle Component");

    // 최상위 트리
    CIMGUITree* m_RootTree = AddWidget<CIMGUITree>("Particle Variables");

    // m_DurationEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Duration");
    // m_DurationEdit->SetCallBack(this, &CParticleComponentWidget::OnDurationEdit);

    m_StartMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("StartMin", 100.f);
    m_StartMinEdit->SetCallBack(this, &CParticleComponentWidget::OnStartMinEdit);
    m_StartMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("StartMax", 100.f);
    m_StartMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnStartMaxEdit);

    m_SpawnCountMaxEdit = m_RootTree->AddWidget<CIMGUIInputInt>("Spawn Cnt Max", 100.f);

    m_ScaleMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("ScaleMin", 100.f);
    m_ScaleMinEdit->SetCallBack(this, &CParticleComponentWidget::OnScaleMinEdit);
    m_ScaleMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("ScaleMax", 100.f);
    m_ScaleMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnScaleMaxEdit);

    m_LifeTimeMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Life T Min", 100.f);
    m_LifeTimeMinEdit->SetCallBack(this, &CParticleComponentWidget::OnLifeTimeMinEdit);
    m_LifeTimeMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Life T Max", 100.f);
    m_LifeTimeMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnLifeTimeMaxEdit);

    m_SpeedMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Speed Min", 100.f);
    m_SpeedMinEdit->SetCallBack(this, &CParticleComponentWidget::OnSpeedMinEdit);
    m_SpeedMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Speed Max", 100.f);
    m_SpeedMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnSpeedMaxEdit);

    m_ColorMinEdit = m_RootTree->AddWidget<CIMGUIColor4>("Color Min", 100.f);
    m_ColorMinEdit->SetCallBack(this, &CParticleComponentWidget::OnColorMinEdit);

    m_ColorMaxEdit = m_RootTree->AddWidget<CIMGUIColor4>("Color Max", 100.f);
    m_ColorMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnColorMaxEdit);

    m_IsMoveEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Move", 100.f);
    m_IsMoveEdit->AddCheckInfo("Move");
    m_IsMoveEdit->SetCallBackLabel<CParticleComponentWidget>(this, &CParticleComponentWidget::OnIsMoveEdit);

    m_IsGravityEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Gravity", 100.f);
    m_IsGravityEdit->AddCheckInfo("Gravity");
    m_IsGravityEdit->SetCallBackLabel<CParticleComponentWidget>(this, &CParticleComponentWidget::OnIsGravityEdit);
    // m_Is3DEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Load", 200.f);

    m_MoveDirEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("Move Dir", 100.f);
    m_MoveDirEdit->SetCallBack<CParticleComponentWidget>(this, &CParticleComponentWidget::OnMoveDirEdit);
    m_MoveAngleEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("Move Angle", 100.f);
    m_MoveAngleEdit->SetCallBack<CParticleComponentWidget>(this, &CParticleComponentWidget::OnMoveAngleEdit);

    // m_GravityAccelEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Gravity Accel", 100.f);
    // m_StartDelayEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Start Delay T// ", 100.f);

    m_SetMaterialTextureButton = m_RootTree->AddWidget<CIMGUIButton>("Set Texture", 100.f);
    // m_SetMaterialTextureButton->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayScale);

    // Render Target
    m_ParticleRenderTarget = m_RootTree->AddWidget<CIMGUIImage>("Render Target", 100.f);
    m_ParticleRenderTarget = AddWidget<CIMGUIImage>("Render Target", 500.f, 500.f);
    m_ParticleRenderTarget->SetRenderTargetImage(true);
    m_ParticleRenderTarget->SetBorderColor(10.f, 10.f, 255.f);
    m_ParticleRenderTarget->SetTexture(CRenderManager::GetInst()->GetParticleEffectRenderTarget());

	return true;
}

void CParticleComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CParticleComponent* MeshCom = (CParticleComponent*)m_Component.Get();

}

// void CParticleComponentWidget::OnDurationEdit(float Num)
// {
   
// }

void CParticleComponentWidget::OnStartMinEdit(const Vector3& Pos)
{
   dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetStartMin(Pos);
}

void CParticleComponentWidget::OnStartMaxEdit(const Vector3& Pos)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetStartMax(Pos);
}

void CParticleComponentWidget::OnSpawnCountMaxEdit(int Num)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetSpawnCountMax(Num);
}

void CParticleComponentWidget::OnScaleMinEdit(const Vector3& Scale)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetScaleMin(Scale);
}

void CParticleComponentWidget::OnScaleMaxEdit(const Vector3& Scale)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetScaleMax(Scale);
}

void CParticleComponentWidget::OnLifeTimeMinEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetLifeTimeMin(Num);
}

void CParticleComponentWidget::OnLifeTimeMaxEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetLifeTimeMax(Num);
}

void CParticleComponentWidget::OnSpeedMinEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetSpeedMin(Num);
}

void CParticleComponentWidget::OnSpeedMaxEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetSpeedMax(Num);
}

void CParticleComponentWidget::OnColorMinEdit(const Vector4& Color)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetColorMin(Color);
}

void CParticleComponentWidget::OnColorMaxEdit(const Vector4& Color)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetColorMax(Color);
}


void CParticleComponentWidget::OnIsMoveEdit(const char*, bool Enable)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetMove(Enable);
}

void CParticleComponentWidget::OnIsGravityEdit(const char*, bool Enable)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetGravity(Enable);
}

void CParticleComponentWidget::OnMoveDirEdit(const Vector3& Dir)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetMoveDir(Dir);
}

void CParticleComponentWidget::OnMoveAngleEdit(const Vector3& Angle)
{
    dynamic_cast<CParticleComponent*>(m_Component.Get())->GetParticle()->SetMoveAngle(Angle);
}
