
#include "ObjectComponentWindow.h"
#include "IMGUIListBox.h"
#include "IMGUIButton.h"
#include "ObjectComponentCreateModal.h"
#include "../EditorInfo.h"

CObjectComponentWindow::CObjectComponentWindow()	:
	m_ComponentCreatePopUpButton(nullptr),
	m_ComponentCreateModal(nullptr)
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

	return true;
}

void CObjectComponentWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
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
	}
}

std::string CObjectComponentWindow::GetComponentNameInput() const
{
	return m_ComponentCreateModal->GetComponentNameInput();
}

//void CObjectComponentWindow::FindSelectComponent(CIMGUITree* RootNode)
//{
//}
