
#include "ObjectComponentCreateModal.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUIDummy.h"
#include "IMGUIManager.h"
#include "Scene/SceneManager.h"
#include "ObjectHierarchyWindow.h"
#include "ObjectComponentWindow.h"
#include "Component/PaperBurnComponent.h"
#include "Component/NavAgent.h"
#include "../Component/GameDataComponent.h"
#include "../Component/PlayerDataComponent.h"
#include "../Component/GameStateComponent.h"
#include "../Window/InspectorWindow.h"
#include "../EditorUtil.h"
#include "Flag.h"

CObjectComponentCreateModal::CObjectComponentCreateModal()	:
	m_ComponentCombo(nullptr),
	m_NameTextInput(nullptr),
	m_ComponentCreateButton(nullptr)
{
}

CObjectComponentCreateModal::~CObjectComponentCreateModal()
{
}

bool CObjectComponentCreateModal::Init()
{
	CIMGUIPopUpModal::Init();

	m_ComponentCombo = AddWidget<CIMGUIComboBox>("");

	for (int i = (int)ObjectComponent3DType::PaperBurnComponent; i < (int)ObjectComponent3DType::Max; ++i)
	{
		ObjectComponent3DType foo = static_cast<ObjectComponent3DType>(i);
		std::string StrObjectComponent = CEditorUtil::ObjectComponent3DTypeToString(foo);
		m_ComponentCombo->AddItem(StrObjectComponent);
	}

	m_NameTextInput = AddWidget<CIMGUITextInput>("ObjectComponent Name");
	m_NameTextInput->SetHideName(true);
	m_NameTextInput->SetHintText("ObjectComponent Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentCreateButton = AddWidget<CIMGUIButton>("Create Button", 100.f, 20.f);
	m_ComponentCreateButton->SetClickCallback<CObjectComponentCreateModal>(this, &CObjectComponentCreateModal::OnCreateComponent);

	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 100.f, 50.f);

	return true;
}

void CObjectComponentCreateModal::Update(float DeltaTime)
{
	CIMGUIPopUpModal::Render();
}

std::string CObjectComponentCreateModal::GetComponentNameInput() const
{
	return m_NameTextInput->GetTextMultibyte();
}

void CObjectComponentCreateModal::OnCreateComponent()
{
	char Name[256] = {};
	strcpy_s(Name, m_NameTextInput->GetTextMultibyte());

	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (!Window)
		return;

	if (Window->GetSelectNode() == Window->GetRoot())
		return;

	CGameObject* SelectObject = Window->GetSelectObject();

	if (!SelectObject)
		return;

	int Index = m_ComponentCombo->GetSelectIndex();

	CObjectComponent* Com = nullptr;

	// TODO : Object Component 추가될때마다 추가
	size_t Typeid = CEditorUtil::ObjectComponentTypeIndexToTypeid(Index);

	if (Typeid == typeid(CPaperBurnComponent).hash_code())
		Com = SelectObject->CreateComponent<CPaperBurnComponent>(Name);
	else if (Typeid == typeid(CGameStateComponent).hash_code())
		Com = SelectObject->CreateComponent<CGameStateComponent>(Name);
	else if (Typeid == typeid(CPlayerDataComponent).hash_code())
		Com = SelectObject->CreateComponent<CPlayerDataComponent>(Name);
	else if (Typeid == typeid(CNavAgent).hash_code())
	{
		Com = SelectObject->CreateComponent<CNavAgent>(Name);
		SelectObject->SetNavAgent((CNavAgent*)Com);
		((CNavAgent*)Com)->SetUpdateComponent(SelectObject->GetRootComponent());
	}


	else if (Typeid == typeid(CGameDataComponent).hash_code())
		Com = SelectObject->CreateComponent<CGameDataComponent>(Name);

	CObjectComponentWindow* ComponentWindow = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);

	if (ComponentWindow)
	{
		int Index = ComponentWindow->OnCreateObjectComponent(Name);
	}

	// Insepctor Update
	CInspectorWindow* Inspector = (CInspectorWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(INSPECTOR);

	if (Inspector)
	{
		Inspector->OnCreateObjectComponent((CObjectComponent*)Com);
	}
}
