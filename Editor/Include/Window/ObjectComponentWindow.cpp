
#include "ObjectComponentWindow.h"
#include "IMGUIListBox.h"
#include "IMGUIButton.h"
#include "ObjectComponentCreateModal.h"
#include "../EditorInfo.h"
#include "ObjectHierarchyWindow.h"
#include "IMGUIManager.h"
#include "GameObject/GameObject.h"
#include "IMGUISameLine.h"

CObjectComponentWindow::CObjectComponentWindow()	:
	m_ComponentCreatePopUpButton(nullptr),
	m_ComponentCreateModal(nullptr),
	m_SelectIndex(-1)
{
}

CObjectComponentWindow::~CObjectComponentWindow()
{
}

bool CObjectComponentWindow::Init()
{
	CIMGUIWindow::Init();

	// 숫자 하드 코딩하지 말고, Window크기 알아서 Window크기에 Offset줘서 맞추기
	m_ComponentListBox = AddWidget<CIMGUIListBox>("ObjectComponentList", 130.f, 200.f);
	m_ComponentListBox->SetHideName(true);

	m_ComponentCreatePopUpButton = AddWidget<CIMGUIButton>("Create", 50.f, 20.f);
	m_ComponentCreatePopUpButton->SetClickCallback<CObjectComponentWindow>(this, &CObjectComponentWindow::OnCreateComponentPopUp);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentDeleteButton = AddWidget<CIMGUIButton>("Delete", 50.f, 20.f);
	m_ComponentDeleteButton->SetClickCallback<CObjectComponentWindow>(this, &CObjectComponentWindow::OnDeleteComponent);

	return true;
}

void CObjectComponentWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CObjectComponentWindow::OnRenameComponent(const std::string& NewName, const std::string& PrevName)
{
	m_ComponentListBox->ChangeItem(NewName, PrevName);
}

void CObjectComponentWindow::OnCreateComponentPopUp()
{
	if (!m_ComponentCreateModal)
		m_ComponentCreateModal = AddWidget<CObjectComponentCreateModal>(OBJECTCOMPONENT_CREATE_POPUPMODAL);

	else
	{
		PopUpModalState State = m_ComponentCreateModal->GetPopUpModalState();

		if (State == PopUpModalState::Closed)
			m_ComponentCreateModal->SetPopUpModalState(PopUpModalState::Open);

		m_ComponentCreateModal->SetRender(true);
	}
}

std::string CObjectComponentWindow::GetComponentNameInput() const
{
	return m_ComponentCreateModal->GetComponentNameInput();
}

int CObjectComponentWindow::AddObjectComponent(const std::string& Name)
{
	int Count = m_ComponentListBox->GetItemCount();
	m_ComponentListBox->AddItem(Name);

	return Count;
}

void CObjectComponentWindow::OnSelectComponent(int Index, const char* Label)
{
	m_SelectIndex = Index;
	m_SelectLabel = Label;
}

void CObjectComponentWindow::OnDeleteComponent()
{
	CObjectHierarchyWindow* Window = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	CGameObject* Obj = Window->GetSelectObject();

	std::string SelectItem = m_ComponentListBox->GetSelectItem();
	int Index = m_ComponentListBox->GetSelectIndex();

	CObjectComponent* DeleteComp = (CObjectComponent*)Obj->FindComponent(SelectItem);

	if (!DeleteComp)
		return;

	Obj->DeleteObjectComponent(SelectItem);

	m_ComponentListBox->DeleteItem(Index);
}

