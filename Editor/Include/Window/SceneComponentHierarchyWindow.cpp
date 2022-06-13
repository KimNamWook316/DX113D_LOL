
#include "SceneComponentHierarchyWindow.h"
#include "IMGUITree.h"
#include "IMGUIPopUpModal.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUITextInput.h"
#include "IMGUISameLine.h"
#include "IMGUIDummy.h"
#include "../EditorUtil.h"
#include "Flag.h"
#include "SceneComponentCreateModal.h"
#include "IMGUIManager.h"
#include "ObjectHierarchyWindow.h"
#include "ObjectComponentWindow.h"
#include "ToolWindow.h"
#include "Scene/SceneManager.h"
#include "InspectorWindow.h"
#include "Engine.h"
#include "PathManager.h"

CSceneComponentHierarchyWindow::CSceneComponentHierarchyWindow() :
	m_Root(nullptr),
	m_ComponentCreatePopUpButton(nullptr),
	m_ComponentCreateModal(nullptr),
	m_DragSrc(nullptr),
	m_DragDest(nullptr),
	m_ComponentDeleteButton(nullptr)
{
}

CSceneComponentHierarchyWindow::~CSceneComponentHierarchyWindow()
{
	//size_t Count = m_vecSceneComponentTree.size();

	//for (size_t i = 0; i < Count; ++i)
	//{
	//	SAFE_DELETE(m_vecSceneComponentTree[i]);
	//}
}

CIMGUITree* CSceneComponentHierarchyWindow::GetRoot() const
{
	return m_Root;
}

CIMGUITree* CSceneComponentHierarchyWindow::GetDragSrc() const
{
	return m_DragSrc;
}

CIMGUITree* CSceneComponentHierarchyWindow::GetDragDest() const
{
	return m_DragDest;
}

void CSceneComponentHierarchyWindow::SetDragSrc(CIMGUITree* DragSrc)
{
	m_DragSrc = DragSrc;
}

void CSceneComponentHierarchyWindow::SetDragDest(CIMGUITree* DragDest)
{
	m_DragDest = DragDest;
}

void CSceneComponentHierarchyWindow::ClearExistingHierarchy()
{
	m_Root->DeleteHierarchy();
}

bool CSceneComponentHierarchyWindow::Init()
{
	CIMGUIWindow::Init();

	m_Root = AddWidget<CIMGUITree>("SceneComponents");
	m_Root->SetSelected(true);
	m_SelectNode = m_Root;

	m_ComponentCreatePopUpButton = AddWidget<CIMGUIButton>("Create", 50.f, 20.f);
	m_ComponentCreatePopUpButton->SetClickCallback<CSceneComponentHierarchyWindow>(this, &CSceneComponentHierarchyWindow::OnCreateComponentPopUp);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentDeleteButton = AddWidget<CIMGUIButton>("Delete", 50.f, 20.f);
	m_ComponentDeleteButton->SetClickCallback<CSceneComponentHierarchyWindow>(this, &CSceneComponentHierarchyWindow::OnDeleteComponent);

	m_vecComponentTree.push_back(m_Root);

	return true;
}

void CSceneComponentHierarchyWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	// 매 프레임마다 Tree를 순회하면서 m_SelectNode에 지금 선택된 노드를 갱신해줌
	//FindSelectNode(m_Root);
}

void CSceneComponentHierarchyWindow::OnRenameComponent(const std::string& NewName, const std::string& PrevName)
{
	CIMGUITree* Node = m_Root->FindChild(PrevName);
	Node->SetName(NewName);
}

void CSceneComponentHierarchyWindow::OnCreateComponentPopUp()
{
	if (!m_ComponentCreateModal)
		m_ComponentCreateModal = AddWidget<CSceneComponentCreateModal>(SCENECOMPONENT_CREATE_POPUPMODAL);

	else
	{
		PopUpModalState State = m_ComponentCreateModal->GetPopUpModalState();

		if (State == PopUpModalState::Closed)
			m_ComponentCreateModal->SetPopUpModalState(PopUpModalState::Open);

		m_ComponentCreateModal->SetRender(true);
	}
}

void CSceneComponentHierarchyWindow::OnDragDropSrc(CIMGUITree* SrcTree)
{
	if (m_SelectNode == m_Root || m_DragSrc == m_Root)
		return;

	m_DragSrc = SrcTree;

	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (Window)
	{
		std::string ObjName = Window->GetSelectNode()->GetName();

		CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjName);

		if (Obj)
		{
			// ObjectHierarchy Window에서 찾은 Object내에서 SceneComponent Hierarchy Window에서 Drag Drop의 Src에 해당하는 Component를 찾기
			CSceneComponent * Comp = (CSceneComponent*)Obj->FindComponent(m_DragSrc->GetName());

			Comp->ClearParent();
		}
	}
}

void CSceneComponentHierarchyWindow::OnDragDropDest(CIMGUITree* DestTree, const std::string& ParentName, const std::string& NewChildName)
{
	if (m_SelectNode == m_Root)
		return;

	m_DragDest = DestTree;

	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (Window)
	{
		std::string ObjName = Window->GetSelectNode()->GetName();

		CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjName);

		if (Obj)
		{
			CSceneComponent* ParentComponent = (CSceneComponent*)Obj->FindComponent(ParentName);
			CSceneComponent* ChildComponent = (CSceneComponent*)Obj->FindComponent(NewChildName);

			ParentComponent->AddChild(ChildComponent);
		}
	}
}

void CSceneComponentHierarchyWindow::OnUpdateSceneComponentWindow(CGameObject* Object)
{
	if (!Object)
		return;

	DisableCurrent();
	
	CSceneComponent* RootComp = Object->GetRootComponent();

	if (!RootComp)
		return;

	std::string Name = RootComp->GetName();
	CIMGUITree* RootTreeNode = m_Root->FindChild(Name);
	RootTreeNode->EnableAll();
}

void CSceneComponentHierarchyWindow::OnCreateComponent(bool IsRoot, CSceneComponent* Component)
{
	if (!Component)
	{
		return;
	}

	std::string Name = Component->GetName();

	CIMGUITree* NewNode = nullptr;

	if (IsRoot)
	{
		NewNode = m_Root->AddChild(Name);
	}
	else
	{
		CIMGUITree* RootCompNode = m_Root->GetNode(0);
		NewNode = RootCompNode->AddChild(Name);
	}

	NewNode->AddSelectCallback(this, &CSceneComponentHierarchyWindow::OnSetSelectNode);
	NewNode->SetDragDropSourceCallback(this, &CSceneComponentHierarchyWindow::OnDragDropSrc);
	NewNode->SetDragDropDestCallback(this, &CSceneComponentHierarchyWindow::OnDragDropDest);
}

void CSceneComponentHierarchyWindow::OnLoadGameObject(CGameObject* Object)
{
	if (!Object || !Object->GetRootComponent())
	{
		return;
	}

	// 기존 Hierachy 비활성화
	DisableCurrent();

	CSceneComponent* Root = Object->GetRootComponent();
	std::string RootName = Root->GetName();

	// Hierachy 생성
	CIMGUITree* RootComponentNode = m_Root->AddChild(RootName);
	MakeHierachyRecursive(Root, RootComponentNode);
}

void CSceneComponentHierarchyWindow::OnClearComponents(const std::string& RootComponentName)
{
	size_t Count = m_Root->GetChildCount();

	for (size_t i = 0; i < Count; ++i)
	{
		CIMGUITree* RootComponent = m_Root->FindChild(RootComponentName);

		if (RootComponent && RootComponent->IsEnable())
		{
			RootComponent->Delete();
			break;
		}
	}

	//CObjectHierarchyWindow* ObjWindow = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	//CGameObject* SelectObj = ObjWindow->GetSelectObject();

	//for (size_t i = 0; i < Count;)
	//{
	//	CIMGUITree* Node = m_Root->GetNode(i);

	//	if (Node)
	//	{
	//		CComponent* Comp = SelectObj->FindComponent(Node->GetName());

	//		if (Comp)
	//		{
	//			Node->Delete();
	//			Count = m_Root->GetChildCount();
	//		}

	//		else
	//			++i;
	//	}

	//	else
	//		++i;
	//}

	CObjectComponentWindow* ObjCompWindow = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);

	if (ObjCompWindow)
		ObjCompWindow->ClearListBox();
}

void CSceneComponentHierarchyWindow::OnSetSelectNode(CIMGUITree* Tree)
{
	m_SelectNode = Tree;

	// Gizmo 현재 선택된 컴포넌트 업데이트
	CSceneComponent* Comp = (CSceneComponent*)FindSelectComponent();

	((CToolWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(TOOL))->SetGizmoComponent(Comp);
}

void CSceneComponentHierarchyWindow::OnDeleteComponent()
{
	CSceneComponent* DeleteComp = (CSceneComponent*)FindSelectComponent();
	CGameObject* Object = DeleteComp->GetGameObject();

	// Inspector에서 Widget삭제
	((CInspectorWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(INSPECTOR))->OnDeleteSceneComponent(DeleteComp);

	// 루트 노드면 전부 삭제, 중간에 있는 노드면 그 노드 삭제하고 첫번째 자식 Component를 올리는 동작을 GUI상에서도 구현
	m_SelectNode->Delete();

	CSceneComponent* NewRoot = nullptr;
	if (DeleteComp)
	{
		NewRoot = DeleteComp->GetChild(0);
		DeleteComp->DeleteComponent();
	}

	((CToolWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(TOOL))->SetGizmoComponent(NewRoot);
}

void CSceneComponentHierarchyWindow::OnSaveComponent()
{
	CComponent* SaveComponent = FindSelectComponent();

	if (!SaveComponent)
	{
		MessageBox(nullptr, TEXT("선택된 컴포넌트 없음"), TEXT("Error"), MB_OK);
		return;
	}

	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All File\0*.*\0");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(COMPONENT_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, Length, 0, 0);

		CGameObject* Object = SaveComponent->GetGameObject();
		bool Ret = Object->SaveOnly(SaveComponent, FullPath);

		if (!Ret)
		{
			MessageBox(nullptr, TEXT("컴포넌트 저장 실패"), TEXT("Error"), MB_OK);
			return;
		}

		MessageBox(nullptr, TEXT("컴포넌트 저장 성공"), TEXT("Success"), MB_OK);
	}
}

void CSceneComponentHierarchyWindow::OnLoadComponent()
{
	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (!Window)
	{
		return;
	}

	CGameObject* SelectObject = Window->GetSelectGameObject();

	if (!SelectObject)
	{
		return;
	}

	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All File\0*.*\0");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(COMPONENT_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, Length, 0, 0);

		CComponent* LoadComp = nullptr;
		bool Ret = SelectObject->LoadOnly(FullPath, LoadComp);

		if (!Ret)
		{
			MessageBox(nullptr, TEXT("컴포넌트 로드 실패"), TEXT("Error"), MB_OK);
			return;
		}

		// Hierachy Update
		OnAddComponent(SelectObject, (CSceneComponent*)LoadComp);

		// Insepctor Update
		CInspectorWindow* Inspector = (CInspectorWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(INSPECTOR);

		if (Inspector)
		{
			Inspector->OnCreateSceneComponent((CSceneComponent*)LoadComp);
		}

		MessageBox(nullptr, TEXT("컴포넌트 로드 성공"), TEXT("Success"), MB_OK);
	}
}

CComponent* CSceneComponentHierarchyWindow::FindSelectComponent()
{
	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (!Window)
	{
		return nullptr;
	}

	std::string SelectObjName = Window->GetSelectNode()->GetName();

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(SelectObjName);

	if (!Obj)
	{
		return nullptr;
	}

	CComponent* Comp = Obj->FindComponent(m_SelectNode->GetName());

	return Comp;
}

void CSceneComponentHierarchyWindow::OnAddComponent(class CGameObject* Object, CSceneComponent* Component)
{
	if (!Object)
	{
		return;
	}

	if (Object->GetRootComponent() == Component)
	{
		m_Root->AddChild(Component->GetName());
	}
	else
	{
		m_Root->GetNode(0)->AddChild(Component->GetName());
	}
}

void CSceneComponentHierarchyWindow::MakeHierachyRecursive(CSceneComponent* Parent, CIMGUITree* ParentTree)
{
	size_t Count = Parent->GetChildCount();

	// CallBack 연결
	ParentTree->AddSelectCallback(this, &CSceneComponentHierarchyWindow::OnSetSelectNode);
	ParentTree->SetDragDropSourceCallback(this, &CSceneComponentHierarchyWindow::OnDragDropSrc);
	ParentTree->SetDragDropDestCallback(this, &CSceneComponentHierarchyWindow::OnDragDropDest);

	// Tree 생성
	for (size_t i = 0; i < Count; ++i)
	{
		ParentTree->AddChild(Parent->GetChild(i)->GetName());
	}

	// 재귀적 생성
	for (size_t i = 0; i < Count; ++i)
	{
		CSceneComponent* Child = Parent->GetChild(i);
		CIMGUITree* ChildTree = ParentTree->GetNode(i);
		MakeHierachyRecursive(Child, ChildTree);
	}
}

void CSceneComponentHierarchyWindow::DisableCurrent()
{
	// 지금 Component Hierarchy Window에 출력되고 있는것들을 모두(최상위에 있는 "Components" TreeNode제외하고) Disable 처리
	size_t Count = m_Root->GetChildCount();

	for (size_t i = 0; i < Count; ++i)
	{
		m_Root->GetNode(i)->DisableAll();
	}
}
