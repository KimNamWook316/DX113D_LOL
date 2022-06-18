
#include "SceneComponentCreateModal.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"
#include "IMGUIDummy.h"
#include "Scene/SceneManager.h"
#include "IMGUIManager.h"
#include "SceneComponentHierarchyWindow.h"
#include "InspectorWindow.h"
#include "IMGUITree.h"
#include "ObjectHierarchyWindow.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Resource/Particle/Particle.h"
#include "Component/Arm.h"
#include "Component/LandScape.h"
#include "ToolWindow.h"

CSceneComponentCreateModal::CSceneComponentCreateModal() :
	//m_SceneComponentCreatePopUp(nullptr),
	m_ComponentCombo(nullptr),
	m_NameTextInput(nullptr),
	m_ComponentCreateButton(nullptr)
{
}

CSceneComponentCreateModal::~CSceneComponentCreateModal()
{
}

bool CSceneComponentCreateModal::Init()
{
	CIMGUIPopUpModal::Init();

	//m_SceneComponentCreatePopUp = AddWidget<CIMGUIPopUpModal>("CreateSceneComponentPopUp");

	m_ComponentCombo = AddWidget<CIMGUIComboBox>("");

	for (int i = (int)SceneComponent3DType::AnimationMeshComponent; i < (int)SceneComponent3DType::Max; ++i)
	{
		SceneComponent3DType foo = static_cast<SceneComponent3DType>(i);
		std::string StrSceneComponent = CEditorUtil::SceneComponent3DTypeToString(foo);
		m_ComponentCombo->AddItem(StrSceneComponent);
	}

	m_NameTextInput = AddWidget<CIMGUITextInput>("SceneComponent Name");
	m_NameTextInput->SetHideName(true);
	m_NameTextInput->SetHintText("SceneComponent Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentCreateButton = AddWidget<CIMGUIButton>("Create Button", 100.f, 20.f);
	m_ComponentCreateButton->SetClickCallback<CSceneComponentCreateModal>(this, &CSceneComponentCreateModal::OnCreateComponent);

	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 100.f, 50.f);

	return true;
}

void CSceneComponentCreateModal::Update(float DeltaTime)
{
	CIMGUIPopUpModal::Render();

}

std::string CSceneComponentCreateModal::GetComponentNameInput() const
{
	std::string Name = m_NameTextInput->GetTextMultibyte();

	return Name;
}

void CSceneComponentCreateModal::OnCreateComponent()
{
	char Name[256] = {};
	strcpy_s(Name, m_NameTextInput->GetTextMultibyte());

	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (!Window)
		return;

	if (Window->GetSelectNode() == Window->GetRoot())
		return;

	std::string ObjName = Window->GetSelectNode()->GetName();

	CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindObject(ObjName);

	if (!SelectObject)
		return;

	// Object의 루트로 들어가는지 확인
	// 아래 Gizmo에 Object 넣어주기 위해 필요
	bool HasRoot = SelectObject->GetRootComponent();

	int Index = m_ComponentCombo->GetSelectIndex();

	size_t Typeid = CEditorUtil::SceneComponentTypeIndexToTypeid(Index);

	CSceneComponent* Com = nullptr;

	// TODO : 컴포넌트 추가될때마다 추가
	if (Typeid == typeid(CAnimationMeshComponent).hash_code())
		Com = SelectObject->CreateComponentAddChild<CAnimationMeshComponent>(Name);

	else if (Typeid == typeid(CStaticMeshComponent).hash_code())
		Com = SelectObject->CreateComponentAddChild<CStaticMeshComponent>(Name);

	else if (Typeid == typeid(CLandScape).hash_code())
		Com = SelectObject->CreateComponentAddChild<CLandScape>(Name);

	else if (Typeid == typeid(CArm).hash_code())
		Com = SelectObject->CreateComponentAddChild<CArm>(Name);

	else if (Typeid == typeid(CLightComponent).hash_code())
		Com = SelectObject->CreateComponentAddChild<CLightComponent>(Name);

	else if (Typeid == typeid(CSceneComponent).hash_code())
		Com = SelectObject->CreateComponentAddChild<CSceneComponent>(Name);

	else if (Typeid == typeid(CParticleComponent).hash_code())
		Com = SelectObject->CreateComponentAddChild<CParticleComponent>(Name);

	else if (Typeid == typeid(CColliderBox3D).hash_code())
		Com = SelectObject->CreateComponentAddChild<CColliderBox3D>(Name);

	else if (Typeid == typeid(CColliderSphere).hash_code())
		Com = SelectObject->CreateComponentAddChild<CColliderSphere>(Name);


	// Window 갱신
	CSceneComponentHierarchyWindow* ComponentWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);
	CInspectorWindow* Inspector = (CInspectorWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(INSPECTOR);
	CToolWindow* Tool = (CToolWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(TOOL);

	bool IsRoot = SelectObject->GetRootComponent() == Com;

	if (ComponentWindow)
	{
		ComponentWindow->OnCreateComponent(IsRoot, Com);
	}

	if (Inspector)
	{
		if (Com->GetComponentType() == Component_Type::SceneComponent)
			Inspector->OnCreateSceneComponent((CSceneComponent*)Com);
		else
			Inspector->OnCreateObjectComponent((CObjectComponent*)Com);
	}

	if (Tool)
	{
		Tool->SetGizmoComponent(Com);
	}
}

