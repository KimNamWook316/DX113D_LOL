#include "SceneComponentWidget.h"
#include "IMGUITextInput.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIDummy.h"
#include "Component/SceneComponent.h"
#include "../Widget/TransformWidget.h"

CSceneComponentWidget::CSceneComponentWidget()	:
	m_NameInput(nullptr),
	m_RenameButton(nullptr),
	m_EnableCheckBox(nullptr),
	m_TransformWidget(nullptr)
{
}

CSceneComponentWidget::~CSceneComponentWidget()
{
}

bool CSceneComponentWidget::Init()
{
	CIMGUIWidgetList::Init();
	
	// AddWidget
	CIMGUIText* Text = AddWidget<CIMGUIText>("Text");
	Text->SetText("Component Name");

	AddWidget<CIMGUIDummy>("Dummy", 50.f);
	AddWidget<CIMGUISameLine>("Line");

	m_EnableCheckBox = AddWidget<CIMGUICheckBox>("Enable");
	m_EnableCheckBox->AddCheckInfo("Enable");

	m_NameInput = AddWidget<CIMGUITextInput>("Component Name Input");
	AddWidget<CIMGUISameLine>("Line");
	m_RenameButton = AddWidget<CIMGUIButton>("Rename", 0.f, 0.f);

	m_TransformWidget = AddWidget<CTransformWidget>("Transform");

	// CallBack
	m_RenameButton->SetClickCallback(this, &CSceneComponentWidget::OnClickRenameButton);
	m_EnableCheckBox->SetCallBackIdx(this, &CSceneComponentWidget::OnCheckEnableCheckBox);

	return true;
}


void CSceneComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	m_Component = Com;

	m_EnableCheckBox->SetCheck(0, m_Component->IsEnable());
	m_NameInput->SetText(m_Component->GetName().c_str());
	m_TransformWidget->SetTransform(m_Component->GetTransform());
}

void CSceneComponentWidget::OnClickRenameButton()
{
	m_Component->SetName(m_NameInput->GetTextMultibyte());
}

void CSceneComponentWidget::OnCheckEnableCheckBox(int Idx, bool Check)
{
	m_Component->Enable(Check);
}
