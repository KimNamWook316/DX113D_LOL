
#include "TowerComponentWidget.h"
#include "../Component/TowerComponent.h"
#include "IMGUIText.h"
#include "IMGUITree.h"
#include "IMGUIRadioButton.h"
#include "IMGUIInputInt.h"

CTowerComponentWidget::CTowerComponentWidget()
{
}

CTowerComponentWidget::~CTowerComponentWidget()
{
}

bool CTowerComponentWidget::Init()
{
	CSceneComponentWidget::Init();

	// AddWidget
	m_ComponentTypeText->SetText("Tower Component");

	// 최상위 트리
	CIMGUITree* m_RootTree = AddWidget<CIMGUITree>("Tower Component Variables");

	m_TowerLineRadioButton = m_RootTree->AddWidget<CIMGUIRadioButton>("Tower Line");
	m_TowerOrderInput = m_RootTree->AddWidget<CIMGUIInputInt>("Tower Order");

	m_TowerLineRadioButton->AddCheckInfo("Top");
	m_TowerLineRadioButton->AddCheckInfo("Mid");
	m_TowerLineRadioButton->AddCheckInfo("Botton");
	m_TowerLineRadioButton->AddCheckInfo("LeftTwin");
	m_TowerLineRadioButton->AddCheckInfo("RightTwin");
	
	//m_TowerLineRadioButton->SetCheck(0, true);
	m_TowerLineRadioButton->AlwaysCheck(true);

	m_TowerLineRadioButton->SetCallBack<CTowerComponentWidget>(this, &CTowerComponentWidget::OnChangeTowerLine);
	m_TowerOrderInput->SetCallBack<CTowerComponentWidget>(this, &CTowerComponentWidget::OnChangeTowerOrder);

	return true;
}

void CTowerComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CTowerComponent* TowerCom = (CTowerComponent*)m_Component.Get();

	m_TowerLine = TowerCom->GetTowerLine();
	m_TowerOrder = TowerCom->GetTowerOrder();

	switch (m_TowerLine)
	{
	case LoLLine::Top:
		m_TowerLineRadioButton->SetCheck(0, true);
		break;
	case LoLLine::Mid:
		m_TowerLineRadioButton->SetCheck(1, true);
		break;
	case LoLLine::Bottom:
		m_TowerLineRadioButton->SetCheck(2, true);
		break;
	case LoLLine::LeftTwin:
		m_TowerLineRadioButton->SetCheck(3, true);
		break;
	case LoLLine::RightTwin:
		m_TowerLineRadioButton->SetCheck(4, true);
		break;
	}

	m_TowerOrderInput->SetVal(m_TowerOrder);
}

void CTowerComponentWidget::OnChangeTowerOrder(int Order)
{
	CTowerComponent* TowerComp = (CTowerComponent*)m_Component.Get();

	TowerComp->SetTowerOrder(Order);
}

void CTowerComponentWidget::OnChangeTowerLine(const char* Label, bool Check)
{
	std::string strLabel = Label;

	CTowerComponent* TowerComp = (CTowerComponent*)m_Component.Get();

	if (strLabel == "Top")
	{
		TowerComp->SetTowerLine(LoLLine::Top);
		m_TowerLineRadioButton->SetCheck(0, true);
	}

	else if (strLabel == "Mid")
	{
		TowerComp->SetTowerLine(LoLLine::Mid);
		m_TowerLineRadioButton->SetCheck(1, true);
	}

	else if (strLabel == "Bottom")
	{
		TowerComp->SetTowerLine(LoLLine::Bottom);
		m_TowerLineRadioButton->SetCheck(2, true);
	}

	else if (strLabel == "LeftTwin")
	{
		TowerComp->SetTowerLine(LoLLine::LeftTwin);
		m_TowerLineRadioButton->SetCheck(3, true);
	}

	else if (strLabel == "RightTwin")
	{
		TowerComp->SetTowerLine(LoLLine::RightTwin);
		m_TowerLineRadioButton->SetCheck(4, true);
	}
}
