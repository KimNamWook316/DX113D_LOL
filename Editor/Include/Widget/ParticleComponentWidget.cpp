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
#include "IMGUISliderFloat.h"
#include "IMGUIColor3.h"
#include "IMGUIInputFloat.h"
#include "IMGUIComboBox.h"
#include "IMGUIDummy.h"
#include "IMGUITree.h"
#include "Component/StaticMeshComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

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

    m_DurationEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Duration");

    m_StartMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("StartMin", 100.f);
    m_StartMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("StartMax", 100.f);

    m_SpawnCountMaxEdit = m_RootTree->AddWidget<CIMGUIInputInt>("Spawn Cnt Max", 100.f);

    m_ScaleMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("ScaleMin", 100.f);
    m_ScaleMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("ScaleMax", 100.f);

    m_LifeTimeMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("LifeT Min", 100.f);
    m_LifeTimeMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("LifeT Max", 100.f);

    m_SpeedMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Speed Min", 100.f);
    m_SpeedMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Speed Max", 100.f);

    m_ColorMinEdit = m_RootTree->AddWidget<CIMGUIColor3>("Color Min", 100.f);
    m_ColorMaxEdit = m_RootTree->AddWidget<CIMGUIColor3>("Color Max", 100.f);

    m_IsLoopEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Loop", 100.f);
    m_IsMoveEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Move", 100.f);
    m_IsGravityEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Gravity", 100.f);
    // m_Is3DEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Load", 200.f);

    m_MoveDirEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("Move Dir", 100.f);
    m_MoveAngleEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("Move Angle", 100.f);

    m_GravityAccelEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Move Dir", 100.f);
    m_StartDelayEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Move Angle", 100.f);

    m_SetMaterialTextureButton = m_RootTree->AddWidget<CIMGUIButton>("Set Texture", 100.f);
    // m_SetMaterialTextureButton->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayScale);

	return true;
}

void CParticleComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CParticleComponent* MeshCom = (CParticleComponent*)m_Component.Get();

}
