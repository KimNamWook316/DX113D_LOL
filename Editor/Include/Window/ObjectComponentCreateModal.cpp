
#include "ObjectComponentCreateModal.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUIDummy.h"
#include "Scene/SceneManager.h"
#include "../EditorUtil.h"
#include "Flag.h"
#include "ObjectHierarchyWindow.h"
#include "IMGUIManager.h"
#include "Component/PaperBurnComponent.h"
#include "Component/StateComponent.h"
#include "Component/NavAgent.h"
#include "ObjectComponentWindow.h"

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
		std::string StrLoLObjectComponent = CEditorUtil::ObjectComponent3DTypeToString(foo);
		m_ComponentCombo->AddItem(StrLoLObjectComponent);
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

	size_t Typeid = CEditorUtil::ObjectComponentTypeIndexToTypeid(Index);

	size_t tmp = typeid(CNavAgent).hash_code();

	if (Typeid == typeid(CPaperBurnComponent).hash_code())
		CObjectComponent* Com = SelectObject->CreateComponent<CPaperBurnComponent>(Name);
	else if (Typeid == typeid(CStateComponent).hash_code())
		CObjectComponent* Com = SelectObject->CreateComponent<CStateComponent>(Name);
	else if (Typeid == typeid(CNavAgent).hash_code())
	{
		CNavAgent* Com = SelectObject->CreateComponent<CNavAgent>(Name);
		SelectObject->SetNavAgent(Com);
		Com->SetUpdateComponent(SelectObject->GetRootComponent());
	}

	CObjectComponentWindow* ComponentWindow = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);

	// 오브젝트에 속한 SceneComponent를 2개 이상 생성할 때 반드시 2번째 Component부터는 1번째 생성한 SceneComponent(=RootNode) 밑으로 넣어줘야한다
	// 그렇게 하지 않으면 지금 Engine코드상 2번째 추가된 SceneComponent는 계층 구조에 속하지 못하고 그냥 CGameObject::m_SceneComponentList에만 들어가있다
	if (ComponentWindow)
	{
		int Index = ComponentWindow->AddObjectComponent(Name);
		ComponentWindow->SetSelectCallback(Index, &CObjectComponentWindow::OnSelectComponent);
	}

}
