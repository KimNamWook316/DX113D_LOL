
#include "ComponentHierarchyWindow.h"
#include "IMGUITree.h"
#include "IMGUIPopUpModal.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUITextInput.h"
#include "IMGUISameLine.h"
#include "IMGUIDummy.h"
#include "../EditorUtil.h"
#include "Flag.h"
#include "ComponentCreateModal.h"

CComponentHierarchyWindow::CComponentHierarchyWindow()	:
	m_Root(nullptr),
	m_ComponentCreatePopUpButton(nullptr),
	m_ComponentCreateModal(nullptr)
{
}

CComponentHierarchyWindow::~CComponentHierarchyWindow()
{
	//size_t Count = m_vecComponentTree.size();

	//for (size_t i = 0; i < Count; ++i)
	//{
	//	SAFE_DELETE(m_vecComponentTree[i]);
	//}
}

CIMGUITree* CComponentHierarchyWindow::GetRoot() const
{
	return m_Root;
}

bool CComponentHierarchyWindow::Init()
{
	CIMGUIWindow::Init();

	m_Root = AddWidget<CIMGUITree>("Components");
	m_Root->SetSelected(true);
	m_SelectNode = m_Root;

	m_ComponentCreatePopUpButton = AddWidget<CIMGUIButton>("Create", 50.f, 20.f);
	m_ComponentCreatePopUpButton->SetClickCallback<CComponentHierarchyWindow>(this, &CComponentHierarchyWindow::OnCreateComponentPopUp);

	m_vecComponentTree.push_back(m_Root);

	return true;
}

void CComponentHierarchyWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	// 매 프레임마다 Tree를 순회하면서 m_SelectNode에 지금 선택된 노드를 갱신해줌
	FindSelectNode(m_Root);
}

void CComponentHierarchyWindow::OnCreateComponentPopUp()
{
	if (!m_ComponentCreateModal)
		m_ComponentCreateModal = AddWidget<CComponentCreateModal>(COMPONENTCREATE_POPUPMODAL);

	else
	{
		PopUpModalState State = m_ComponentCreateModal->GetPopUpModalState();

		if (State == PopUpModalState::Closed)
			m_ComponentCreateModal->SetPopUpModalState(PopUpModalState::Open);
	}
}


void CComponentHierarchyWindow::FindSelectNode(CIMGUITree* RootNode)
{
	if (!RootNode)
		return;

	if (RootNode->IsSelected())
	{
		m_SelectNode = RootNode;
		return;
	}

	else
	{
		size_t Count = RootNode->GetChildCount();

		if (Count > 0)
		{
			for (size_t i = 0; i < Count; ++i)
				FindSelectNode(RootNode->GetNode((int)i));
		}
	}
}
