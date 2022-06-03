
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
		std::string StrLoLSceneComponent = CEditorUtil::SceneComponent3DTypeToString(foo);
		m_ComponentCombo->AddItem(StrLoLSceneComponent);
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
	bool IsRoot = !(SelectObject->GetRootComponent());

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
		Com = SelectObject->CreateComponent<CParticleComponent>(Name);
	
	CSceneComponentHierarchyWindow* ComponentWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);

	// Root Node로 들어가는 경우, Gizmo에 Object갱신
	if (IsRoot)
	{
		CToolWindow* ToolWindow = (CToolWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(TOOL);
		ToolWindow->SetGizmoObject(SelectObject);
	}

	// Inspector Window 갱신
	CInspectorWindow* Inspector = (CInspectorWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(INSPECTOR);
	Inspector->OnCreateSceneComponent(Com);

	if (ComponentWindow)
	{
		// TODO : 이미 Root Component가 있는 상태에서 SceneComponent가 추가됐을때 RootComponent의 자식 Component로 들어갈테니
		// GUI상에서도 Root의 자식으로 들어가는것 반영하기

		CIMGUITree* Child = nullptr; 

		// Control flow가 여기 들어올때면 엔진 상에서 이미 GameObject의 Component가 추가된 하고 여기로 들어오므로
		// GameObject의 SceneComponent 개수가 2개 이상이면서 Root Component가 존재할때가 Root의 자식으로 지금 추가하려는 Component를 넣어줘야 할 때 이다
		if (SelectObject->GetRootComponent() && SelectObject->GetSceneComponentCount() > 1)
		{
			const std::string& RootName = SelectObject->GetRootComponent()->GetName();
			CIMGUITree* RootComponent = ComponentWindow->GetRoot()->FindChild(RootName);

			Child = RootComponent->AddChild(Name);
		}

		else
			Child = ComponentWindow->GetRoot()->AddChild(Name);

		Child->AddSelectCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnSetSelectNode);
		Child->SetDragDropSourceCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnDragDropSrc);
		Child->SetDragDropDestCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnDragDropDest);
	}
}

void CSceneComponentCreateModal::OnLoadComponent(CGameObject* SelectObject)
{
	if (!SelectObject)
		return;

	// Object의 루트로 들어가는지 확인
	// 아래 Gizmo에 Object 넣어주기 위해 필요
	bool IsRoot = !(SelectObject->GetRootComponent());

	CSceneComponent* Com = SelectObject->GetRootComponent();

	CSceneComponentHierarchyWindow* ComponentWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);

	// Root Node로 들어가는 경우, Gizmo에 Object갱신
	if (IsRoot)
	{
		CToolWindow* ToolWindow = (CToolWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(TOOL);
		ToolWindow->SetGizmoObject(SelectObject);
	}

	// Inspector Window 갱신
	CInspectorWindow* Inspector = (CInspectorWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(INSPECTOR);
	Inspector->OnCreateSceneComponent(Com);

	if (ComponentWindow)
	{
		char Name[256] = {};
		strcpy_s(Name, Com->GetName().c_str());


		// TODO : 이미 Root Component가 있는 상태에서 SceneComponent가 추가됐을때 RootComponent의 자식 Component로 들어갈테니
		// GUI상에서도 Root의 자식으로 들어가는것 반영하기
		CIMGUITree* Child = nullptr;

		// Control flow가 여기 들어올때면 엔진 상에서 이미 GameObject의 Component가 추가된 하고 여기로 들어오므로
		// GameObject의 SceneComponent 개수가 2개 이상이면서 Root Component가 존재할때가 Root의 자식으로 지금 추가하려는 Component를 넣어줘야 할 때 이다
		if (SelectObject->GetRootComponent() && SelectObject->GetSceneComponentCount() > 1)
		{
			const std::string& RootName = SelectObject->GetRootComponent()->GetName();
			CIMGUITree* RootComponent = ComponentWindow->GetRoot()->FindChild(RootName);

			Child = RootComponent->AddChild(Name);
		}

		else
			Child = ComponentWindow->GetRoot()->AddChild(Name);

		Child->AddSelectCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnSetSelectNode);
		Child->SetDragDropSourceCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnDragDropSrc);
		Child->SetDragDropDestCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnDragDropDest);
	}
}


