
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

// TODO : DeathDoor Object Component 추가될 때마다 업데이트 
#include "../DeathDoor/Component/ObjectDataComponent.h"
#include "../DeathDoor/Component/PlayerDataComponent.h"
#include "../DeathDoor/Component/GameStateComponent.h"
#include "../DeathDoor/Component/EyeLaserComponent.h"
#include "../DeathDoor/Component/MonsterDataComponent.h"
#include "../DeathDoor/Component/KnightDataComponent.h"

#include "../Window/InspectorWindow.h"
#include "../EditorUtil.h"
#include "../EditorManager.h"
#include "../DeathDoor/DataManager.h"
#include "../DeathDoor/DDUtil.h"
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

	for (int i = 0; i < (int)DDObjectComponentType::Max; ++i)
	{
		DDObjectComponentType DDType = (DDObjectComponentType)i;
		std::string TypeName = CDDUtil::DDObjectComponentTypeToString(DDType);
		m_ComponentCombo->AddItem(TypeName);
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
	else if (Typeid == typeid(CPlayerDataComponent).hash_code())
		Com = SelectObject->CreateComponent<CPlayerDataComponent>(Name);
	else if (Typeid == typeid(CNavAgent).hash_code())
	{
		Com = SelectObject->CreateComponent<CNavAgent>(Name);
		SelectObject->SetNavAgent((CNavAgent*)Com);
		((CNavAgent*)Com)->SetUpdateComponent(SelectObject->GetRootComponent());
	}

	else if (Typeid == typeid(CObjectDataComponent).hash_code())
	{
		Com = SelectObject->CreateComponent<CObjectDataComponent>(Name);
 //		// Editor에서는 EditorManager에서 DataManager 클래스를 갖고 있도록 함
 //		if (CEngine::GetInst()->GetEditMode())
 //		{
 //			CEditorManager::GetInst()->GetDataManager()->SetObjectData(SelectObject);
 //		}
	}

	// 위에서 생성되지 않았다면 클라이언트 단에서 정의된 컴포넌트임
	// TODO : Death Door Object Component 추가될 때마다 업데이트
	if (!Com)
	{
		Index -= (int)ObjectComponent3DType::Max;
		DDObjectComponentType DDObjType = (DDObjectComponentType)Index;

		Typeid = CDDUtil::DDObjectComponentTypeToTypeID(DDObjType);

		if (Typeid == typeid(CObjectDataComponent).hash_code())
		{
			Com = SelectObject->CreateComponent<CObjectDataComponent>(Name);
		}
		else if (Typeid == typeid(CPlayerDataComponent).hash_code())
		{
			Com = SelectObject->CreateComponent<CPlayerDataComponent>(Name);
		}
		else if (Typeid == typeid(CGameStateComponent).hash_code())
		{
			Com = SelectObject->CreateComponent<CGameStateComponent>(Name);
		}
		else if (Typeid == typeid(CMonsterDataComponent).hash_code())
		{
			Com = SelectObject->CreateComponent<CMonsterDataComponent>(Name);
		}
		else if (Typeid == typeid(CKnightDataComponent).hash_code())
		{
			Com = SelectObject->CreateComponent<CKnightDataComponent>(Name);
		}
	}

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
