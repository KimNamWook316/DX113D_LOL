
#include "MinionAIComponentWidget.h"
#include "../Component/MinionAIComponent.h"
#include "IMGUIComboBox.h"
#include "IMGUIText.h"
#include "IMGUITree.h"


CMinionAIComponentWidget::CMinionAIComponentWidget()
{
}

CMinionAIComponentWidget::~CMinionAIComponentWidget()
{
}

bool CMinionAIComponentWidget::Init()
{
	CObjectComponentWidget::Init();

	// AddWidget
	m_ComponentTypeText->SetText("Minion AI Component");

	// 최상위 트리
	m_RootTree = AddWidget<CIMGUITree>("Minion AI Component Variables");
	m_MinionTypeCombo = m_RootTree->AddWidget<CIMGUIComboBox>("MinionTypeCombo");
	m_MinionTypeCombo->SetHideName(true);

	m_MinionTypeCombo->AddItem("Melee");
	m_MinionTypeCombo->AddItem("Ranged");
	m_MinionTypeCombo->AddItem("Super");
	m_MinionTypeCombo->AddItem("Tank");
	m_MinionTypeCombo->AddItem("None");

	m_MinionTypeCombo->SetSelectCallback<CMinionAIComponentWidget>(this, &CMinionAIComponentWidget::OnChangeMinionType);

	return true;
}

void CMinionAIComponentWidget::SetObjectComponent(CObjectComponent* Com)
{
	CObjectComponentWidget::SetObjectComponent(Com);

	CMinionAIComponent* MinionAIComp = (CMinionAIComponent*)m_Component.Get();

	m_MinionType = MinionAIComp->GetMinionType();
}

void CMinionAIComponentWidget::OnChangeMinionType(int Index, const char* Label)
{

	if (Index == 0)
		((CMinionAIComponent*)m_Component.Get())->SetMinionType(Minion_Type::Melee);
	else if (Index == 1)
		((CMinionAIComponent*)m_Component.Get())->SetMinionType(Minion_Type::Ranged);
	else if (Index == 2)
		((CMinionAIComponent*)m_Component.Get())->SetMinionType(Minion_Type::Super);
	else if (Index == 3)
		((CMinionAIComponent*)m_Component.Get())->SetMinionType(Minion_Type::Tank);
	else if (Index == 4)
		((CMinionAIComponent*)m_Component.Get())->SetMinionType(Minion_Type::None);
}
