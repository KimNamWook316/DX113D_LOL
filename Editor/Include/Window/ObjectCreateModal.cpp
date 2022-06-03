
#include "ObjectCreateModal.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"
#include "IMGUIDummy.h"
#include "Scene/SceneManager.h"
#include "IMGUIManager.h"
#include "ObjectHierarchyWindow.h"
#include "SceneComponentHierarchyWindow.h"
#include "SceneComponentCreateModal.h"
#include "IMGUITree.h"
#include "GameObject/Champion.h"
#include "GameObject/Minion.h"
#include "GameObject/MapObject.h"

CObjectCreateModal::CObjectCreateModal()	:
	//m_ObjectCreatePopUp(nullptr),
	m_ObjectCombo(nullptr),
	m_NameTextInput(nullptr),
	m_ObjectCreateButton(nullptr)
{
}

CObjectCreateModal::~CObjectCreateModal()
{
}

bool CObjectCreateModal::Init()
{
	CIMGUIPopUpModal::Init();

	//m_ObjectCreatePopUp = AddWidget<CIMGUIPopUpModal>("CreateObjectPopUp");

	m_ObjectCombo = AddWidget<CIMGUIComboBox>("");

	for (int i = (int)LoLObject::GameObject; i < (int)LoLObject::Max; ++i)
	{
		LoLObject foo = static_cast<LoLObject>(i);
		std::string StrLoLObject = CEditorUtil::LoLObjectToString(foo);
		m_ObjectCombo->AddItem(StrLoLObject);
	}

	m_NameTextInput = AddWidget<CIMGUITextInput>("Object Name");
	m_NameTextInput->SetHideName(true);
	m_NameTextInput->SetHintText("Object Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectCreateButton = AddWidget<CIMGUIButton>("Create Button", 100.f, 20.f);
	m_ObjectCreateButton->SetClickCallback<CObjectCreateModal>(this, &CObjectCreateModal::OnCreateObject);

	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 100.f, 50.f);

	return true;
}

void CObjectCreateModal::Update(float DeltaTime)
{
	CIMGUIPopUpModal::Render();
}

std::string CObjectCreateModal::GetObjectNameInput() const
{
	std::string Name = m_NameTextInput->GetTextMultibyte();

	return Name;
}

void CObjectCreateModal::OnCreateObject()
{
	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	if (!CurrentScene)
		return;

	CIMGUITree* NewObjectTree = nullptr;

	CGameObject* NewObject = nullptr;

	char Name[256] = {};
	strcpy_s(Name, m_NameTextInput->GetTextMultibyte());

	int Index = m_ObjectCombo->GetSelectIndex();

	size_t Typeid = CEditorUtil::ObjectTypeIndexToTypeid(Index);

	if (Typeid == typeid(CGameObject).hash_code())
		NewObject = CurrentScene->CreateGameObject<CGameObject>(Name);

	else if (Typeid == typeid(CMovingObject).hash_code())
		NewObject = CurrentScene->CreateGameObject<CMovingObject>(Name);

	else if (Typeid == typeid(CMapObject).hash_code())
		NewObject = CurrentScene->CreateGameObject<CMapObject>(Name);

	else if (Typeid == typeid(CChampion).hash_code())
		NewObject = CurrentScene->CreateGameObject<CChampion>(Name);

	else if (Typeid == typeid(CMinion).hash_code())
		NewObject = CurrentScene->CreateGameObject<CMinion>(Name);

	// 차후, Loading 을 위해서 ObjectCombo Select Index 정보를 저장해준다.
	// NewObject->SetEditorObjectModalIndex(Index);

	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);
	CSceneComponentHierarchyWindow* SceneCompWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);

	if (Window)
	{
		CIMGUITree* NewNode = Window->GetRoot()->AddChild(Name);
		NewNode->AddSelectCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnSetSelectNode);
		NewNode->AddSelectCallback<CSceneComponentHierarchyWindow>(SceneCompWindow, &CSceneComponentHierarchyWindow::OnUpdateSceneComponetWindow);
		NewNode->SetDragDropSourceCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnDragDropSrc);
		NewNode->SetDragDropDestCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnDragDropDest);
	}
}

void CObjectCreateModal::OnCreateObject(const char* FullPathMultibyte)
{
	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	if (!CurrentScene)
		return;

	CGameObject* LoadedObject = CSceneManager::GetInst()->GetScene()->LoadGameObject<CGameObject>();

	LoadedObject->Load(FullPathMultibyte);

	CIMGUITree* NewObjectTree = nullptr;

	CGameObject* NewObject = nullptr;

	char Name[256] = {};
	strcpy_s(Name, LoadedObject->GetName().c_str());

	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);
	CSceneComponentHierarchyWindow* SceneCompWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);

	if (Window)
	{
		CIMGUITree* NewNode = Window->GetRoot()->AddChild(Name);
		NewNode->AddSelectCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnSetSelectNode);
		NewNode->AddSelectCallback<CSceneComponentHierarchyWindow>(SceneCompWindow, &CSceneComponentHierarchyWindow::OnUpdateSceneComponetWindow);
		NewNode->SetDragDropSourceCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnDragDropSrc);
		NewNode->SetDragDropDestCallback<CObjectHierarchyWindow>(Window, &CObjectHierarchyWindow::OnDragDropDest);

		// 해당 노드를 Select Node 로 세팅한다.
		Window->OnSetSelectNode(NewNode);

		// 해당 Object 의 Root Component 로 Widget 구성
		CSceneComponentHierarchyWindow* ComponentWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);
		ComponentWindow->GetSceneComponentCreateModal()->OnLoadComponent(LoadedObject);
	}
}
