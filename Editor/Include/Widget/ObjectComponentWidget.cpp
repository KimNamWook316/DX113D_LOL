#include "ObjectComponentWidget.h"
#include "IMGUITextInput.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIDummy.h"
#include "Component/ObjectComponent.h"

CObjectComponentWidget::CObjectComponentWidget() :
	m_NameInput(nullptr),
	m_RenameButton(nullptr),
	m_EnableCheckBox(nullptr)
{
}

CObjectComponentWidget::~CObjectComponentWidget()
{
}

bool CObjectComponentWidget::Init()
{
	CIMGUIWidgetList::Init();
	
	// AddWidget
	CIMGUIText* Text = AddWidget<CIMGUIText>("Text");
	Text->SetText("GameObject Name");

	AddWidget<CIMGUIDummy>("Dummy", 50.f);
	AddWidget<CIMGUISameLine>("Line");

	m_EnableCheckBox = AddWidget<CIMGUICheckBox>("ObjEnable");
	m_EnableCheckBox->AddCheckInfo("Enable");

	m_NameInput = AddWidget<CIMGUITextInput>("GameObjNameInput");
	AddWidget<CIMGUISameLine>("Line");
	m_RenameButton = AddWidget<CIMGUIButton>("Obj Rename", 0.f, 0.f);

	// CallBack
	m_RenameButton->SetClickCallback(this, &CObjectComponentWidget::OnClickRenameButton);
	m_EnableCheckBox->SetCallBackIdx(this, &CObjectComponentWidget::OnCheckEnableCheckBox);

	return true;
}

void CObjectComponentWidget::SetObjectComponent(CObjectComponent* Com)
{
	m_Component = Com;

	m_EnableCheckBox->SetCheck(0, m_Component->IsEnable());
	m_NameInput->SetText(m_Component->GetName().c_str());
}

void CObjectComponentWidget::OnClickRenameButton()
{
	m_Component->SetName(m_NameInput->GetTextMultibyte());
}

void CObjectComponentWidget::OnCheckEnableCheckBox(int Idx, bool Check)
{
	m_Component->Enable(Check);
}
