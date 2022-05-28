#include "ParticleComponentWidget.h"
#include "Component/ParticleComponent.h"
#include "IMGUITextInput.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIImage.h"
#include "IMGUISliderFloat.h"
#include "../Window/ObjectHierarchyWindow.h"
#include "IMGUIManager.h"
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
#include "../Object/3DParticleCamera.h"
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

    // Material , Particle 세팅
    OnSetParticleMaterialSetting(m_Component);

    // Camera 세팅
    OnSetCameraSetting();

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

void CParticleComponentWidget::OnSetParticleMaterialSetting(CSceneComponent* Com)
{
    // 기본 Particle Setting, 현재 Component 에 Particle Setting 하기
    // 1) Particle Material 세팅
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("BasicParticleMaterial");
    CMaterial* Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");
    Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));
    Material->SetShader("ParticleRenderShader");
    Material->SetRenderState("AlphaBlend");
    Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));

    // 2) Particle 제작
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateParticle("BasicParticle");
    CParticle* Particle = CSceneManager::GetInst()->GetScene()->GetResource()->FindParticle("BasicParticle");
    Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");
    Particle->SetMaterial(Material);

    // 반드시 3D 로 세팅한다.
    Particle->Set2D(false);
    Particle->SetSpawnCountMax(1000);
    Particle->SetLifeTimeMin(70.f);
    Particle->SetLifeTimeMax(80.f);
    Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
    Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
    Particle->SetSpeedMin(100.f);
    Particle->SetSpeedMax(300.f);
    Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
    Particle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
    Particle->SetStartMax(Vector3(30.f, 30.f, 0.f));
    Particle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    Particle->SetColorMax(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    Particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));
    Particle->SetGravity(true);
    // Particle->SetMove(true);

    // Layer 를 세팅한다.
    Com->SetLayerName("ParticleEditorLayer");

    dynamic_cast<CParticleComponent*>(Com)->SetParticle("BasicParticle");
}

void CParticleComponentWidget::OnSetCameraSetting()
{
    // Camera Object 생성
    m_ParticleCamera = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleCamera>("ParticleCamera");

    CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

    if (!Window)
        return;

    if (Window->GetSelectNode() == Window->GetRoot())
        return;

    std::string ObjName = Window->GetSelectNode()->GetName();

    CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindObject(ObjName);

    if (!SelectObject)
        return;

    // Particle Component Object 위치에 Sample Object 생성
    // 실제 Camera 는 이 Sample Object 를 바라보는 형태가 될 것이다.
    m_ParticleCameraTargetObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>("CameraTarget");
    m_ParticleCameraTargetObj->SetWorldPos(SelectObject->GetWorldPos());


    // 해당 Object 를 Target 으로 세팅하기
    m_ParticleCamera->SetTargetObject(m_ParticleCameraTargetObj);
    m_ParticleCamera->SetTargetDistance(10.f);

    // Particle Effect Camera 로 세팅
    CSceneManager::GetInst()->GetScene()->GetCameraManager()->SetParticleEditorCamera(m_ParticleCamera->GetCamera());
}
