
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
		Com = SelectObject->CreateComponent<CAnimationMeshComponent>(Name);

	else if (Typeid == typeid(CStaticMeshComponent).hash_code())
		Com = SelectObject->CreateComponent<CStaticMeshComponent>(Name);

	else if (Typeid == typeid(CLandScape).hash_code())
		Com = SelectObject->CreateComponent<CLandScape>(Name);

	else if (Typeid == typeid(CArm).hash_code())
		Com = SelectObject->CreateComponent<CArm>(Name);

	else if (Typeid == typeid(CLightComponent).hash_code())
		Com = SelectObject->CreateComponent<CLightComponent>(Name);

	else if (Typeid == typeid(CSceneComponent).hash_code())
		Com = SelectObject->CreateComponent<CSceneComponent>(Name);

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

	// 오브젝트에 속한 SceneComponent를 2개 이상 생성할 때 반드시 2번째 Component부터는 1번째 생성한 SceneComponent(=RootNode) 밑으로 넣어줘야한다
	// 그렇게 하지 않으면 지금 Engine코드상 2번째 추가된 SceneComponent는 계층 구조에 속하지 못하고 그냥 CGameObject::m_SceneComponentList에만 들어가있다
	if (ComponentWindow)
	{
		CIMGUITree* Child = ComponentWindow->GetRoot()->AddChild(Name);
		Child->AddSelectCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnSetSelectNode);
		Child->SetDragDropSourceCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnDragDropSrc);
		Child->SetDragDropDestCallback<CSceneComponentHierarchyWindow>(ComponentWindow, &CSceneComponentHierarchyWindow::OnDragDropDest);
	}
}


