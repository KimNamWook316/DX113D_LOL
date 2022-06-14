
#include "BuildingComponentWidget.h"
#include "../Component/BuildingComponent.h"
#include "IMGUIText.h"
#include "IMGUITree.h"
#include "IMGUIRadioButton.h"
#include "IMGUIInputInt.h"
#include "IMGUISeperator.h"

CBuildingComponentWidget::CBuildingComponentWidget()
{
}

CBuildingComponentWidget::~CBuildingComponentWidget()
{
}

bool CBuildingComponentWidget::Init()
{
	CSceneComponentWidget::Init();

	// AddWidget
	m_ComponentTypeText->SetText("Building Component");

	// 최상위 트리
	CIMGUITree* m_RootTree = AddWidget<CIMGUITree>("Building Component Variables");

	m_BuildingLineRadioButton = m_RootTree->AddWidget<CIMGUIRadioButton>("Building Line");
	CIMGUISeperator* Sep = m_RootTree->AddWidget<CIMGUISeperator>("Sep");
	m_BuildingTypeRadioButton = m_RootTree->AddWidget<CIMGUIRadioButton>("Building Type");
	m_TowerOrderInput = m_RootTree->AddWidget<CIMGUIInputInt>("Tower Order");

	m_BuildingTypeRadioButton->AddCheckInfo("Tower");
	m_BuildingTypeRadioButton->AddCheckInfo("Inhibitor");
	m_BuildingTypeRadioButton->AddCheckInfo("Nexus");


	m_BuildingLineRadioButton->AddCheckInfo("Top");
	m_BuildingLineRadioButton->AddCheckInfo("Mid");
	m_BuildingLineRadioButton->AddCheckInfo("Bottom");
	m_BuildingLineRadioButton->AddCheckInfo("LeftTwin");
	m_BuildingLineRadioButton->AddCheckInfo("RightTwin");
	m_BuildingLineRadioButton->AddCheckInfo("None");

	//m_BuildingLineRadioButton->SetCheck(0, true);
	m_BuildingLineRadioButton->AlwaysCheck(true);
	m_BuildingTypeRadioButton->AlwaysCheck(true);


	m_BuildingLineRadioButton->SetCallBack<CBuildingComponentWidget>(this, &CBuildingComponentWidget::OnChangeBuildingLine);
	m_TowerOrderInput->SetCallBack<CBuildingComponentWidget>(this, &CBuildingComponentWidget::OnChangeTowerOrder);
	m_BuildingTypeRadioButton->SetCallBack<CBuildingComponentWidget>(this, &CBuildingComponentWidget::OnChangeBuildingType);

	return true;
}

void CBuildingComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CBuildingComponent* BuildingCom = (CBuildingComponent*)m_Component.Get();

	m_BuildingLine = BuildingCom->GetBuildingLine();
	m_TowerOrder = BuildingCom->GetTowerOrder();
	m_BuildingType = BuildingCom->GetBuildingType();

	switch (m_BuildingLine)
	{
	case LoLLine::Top:
		m_BuildingLineRadioButton->SetCheck(0, true);
		break;
	case LoLLine::Mid:
		m_BuildingLineRadioButton->SetCheck(1, true);
		break;
	case LoLLine::Bottom:
		m_BuildingLineRadioButton->SetCheck(2, true);
		break;
	case LoLLine::LeftTwin:
		m_BuildingLineRadioButton->SetCheck(3, true);
		break;
	case LoLLine::RightTwin:
		m_BuildingLineRadioButton->SetCheck(4, true);
		break;
	case LoLLine::None:
		m_BuildingLineRadioButton->SetCheck(5, true);
		break;
	}

	switch (m_BuildingType)
	{
	case Building_Type::Tower:
		m_BuildingTypeRadioButton->SetCheck(0, true);
		break;
	case Building_Type::Inhibitor:
		m_BuildingTypeRadioButton->SetCheck(1, true);
		break;
	case Building_Type::Nexus:
		m_BuildingTypeRadioButton->SetCheck(2, true);
		break;
	}

	m_TowerOrderInput->SetVal(m_TowerOrder);
}

void CBuildingComponentWidget::OnChangeTowerOrder(int Order)
{
	CBuildingComponent* BuildingComp = (CBuildingComponent*)m_Component.Get();

	BuildingComp->SetTowerOrder(Order);
}

void CBuildingComponentWidget::OnChangeBuildingLine(const char* Label, bool Check)
{
	std::string strLabel = Label;

	CBuildingComponent* BuildingComp = (CBuildingComponent*)m_Component.Get();

	if (strLabel == "Top")
	{
		BuildingComp->SetBuildingLine(LoLLine::Top);
		m_BuildingLineRadioButton->SetCheck(0, true);
	}

	else if (strLabel == "Mid")
	{
		BuildingComp->SetBuildingLine(LoLLine::Mid);
		m_BuildingLineRadioButton->SetCheck(1, true);
	}

	else if (strLabel == "Bottom")
	{
		BuildingComp->SetBuildingLine(LoLLine::Bottom);
		m_BuildingLineRadioButton->SetCheck(2, true);
	}

	else if (strLabel == "LeftTwin")
	{
		BuildingComp->SetBuildingLine(LoLLine::LeftTwin);
		m_BuildingLineRadioButton->SetCheck(3, true);
	}

	else if (strLabel == "RightTwin")
	{
		BuildingComp->SetBuildingLine(LoLLine::RightTwin);
		m_BuildingLineRadioButton->SetCheck(4, true);
	}

	else if (strLabel == "None")
	{
		BuildingComp->SetBuildingLine(LoLLine::None);
		m_BuildingLineRadioButton->SetCheck(5, true);
	}
}

void CBuildingComponentWidget::OnChangeBuildingType(const char* Label, bool Check)
{
	std::string strLabel = Label;

	CBuildingComponent* BuildingComp = (CBuildingComponent*)m_Component.Get();

	if (strLabel == "Tower")
	{
		BuildingComp->SetBuildingType(Building_Type::Tower);
		m_BuildingTypeRadioButton->SetCheck(0, true);
	}

	else if (strLabel == "Inhibitor")
	{
		BuildingComp->SetBuildingType(Building_Type::Inhibitor);
		m_BuildingTypeRadioButton->SetCheck(1, true);
	}

	else if (strLabel == "Nexus")
	{
		BuildingComp->SetBuildingType(Building_Type::Nexus);
		m_BuildingTypeRadioButton->SetCheck(2, true);
	}

}
