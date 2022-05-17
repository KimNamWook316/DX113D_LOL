
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
#include "Scene/SceneManager.h"

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

void CSceneComponentHierarchyWindow::OnCreateComponentPopUp()
{
	if (!m_ComponentCreateModal)
		m_ComponentCreateModal = AddWidget<CSceneComponentCreateModal>(SCENECOMPONENT_CREATE_POPUPMODAL);

	else
	{
		PopUpModalState State = m_ComponentCreateModal->GetPopUpModalState();

		if (State == PopUpModalState::Closed)
			m_ComponentCreateModal->SetPopUpModalState(PopUpModalState::Open);
	}
}


//void CSceneComponentHierarchyWindow::FindSelectNode(CIMGUITree* RootNode)
//{
//	if (!RootNode)
//		return;
//
//	if (RootNode->IsSelected())
//	{
//		m_SelectNode = RootNode;
//		return;
//	}
//
//	else
//	{
//		size_t Count = RootNode->GetChildCount();
//
//		if (Count > 0)
//		{
//			for (size_t i = 0; i < Count; ++i)
//				FindSelectNode(RootNode->GetNode((int)i));
//		}
//	}
//}

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

void CSceneComponentHierarchyWindow::OnUpdateSceneComponetWindow(CIMGUITree* SelectObjectNode)
{
	// 지금 Component Hierarchy Window에 출력되고 있는것들을 모두(최상위에 있는 "Components" TreeNode제외하고) Disable 처리
	size_t Count = m_Root->GetChildCount();

	for (size_t i = 0; i < Count; ++i)
	{
		m_Root->GetNode(i)->DisableAll();
	}

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(SelectObjectNode->GetName());

	if (!Obj)
		return;
	
	CSceneComponent* RootComp = Obj->GetRootComponent();

	if (!RootComp)
		return;

	std::string Name = RootComp->GetName();

	CIMGUITree* RootTreeNode = m_Root->GetNode(Name);

	RootTreeNode->EnableAll();

}

void CSceneComponentHierarchyWindow::OnSetSelectNode(CIMGUITree* Tree)
{
	m_SelectNode = Tree;
}

void CSceneComponentHierarchyWindow::OnDeleteComponent()
{
	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	std::string SelectObjName = Window->GetSelectNode()->GetName();

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(SelectObjName);

	CSceneComponent* DeleteComp = (CSceneComponent*)Obj->FindComponent(m_SelectNode->GetName());

	// 루트 노드면 전부 삭제, 중간에 있는 노드면 그 노드 삭제하고 첫번째 자식 Component를 올리는 동작을 GUI상에서도 구현
	m_SelectNode->Delete();

	if (DeleteComp)
		DeleteComp->DeleteComponent();

	
}
