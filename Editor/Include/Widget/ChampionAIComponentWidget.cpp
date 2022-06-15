
#include "ChampionAIComponentWidget.h"
#include "../Component/ChampionAIComponent.h"
#include "IMGUIComboBox.h"
#include "IMGUIInputInt.h"
#include "IMGUIText.h"
#include "IMGUITree.h"

CChampionAIComponentWidget::CChampionAIComponentWidget()
{
}

CChampionAIComponentWidget::~CChampionAIComponentWidget()
{
}

bool CChampionAIComponentWidget::Init()
{
	CObjectComponentWidget::Init();

	// AddWidget
	m_ComponentTypeText->SetText("Champion AI Component");

	// 최상위 트리
	m_RootTree = AddWidget<CIMGUITree>("Champion AI Component Variables");
	m_LineComboBox = m_RootTree->AddWidget<CIMGUIComboBox>("Champion Line");
	m_DetectRangeInput = m_RootTree->AddWidget<CIMGUIInputInt>("AI Detect Range");
	m_InHome = m_RootTree->AddWidget<CIMGUIText>("AI Champion In Home?");

	m_LineComboBox->AddItem("Top");
	m_LineComboBox->AddItem("Mid");
	m_LineComboBox->AddItem("Bottom");

	m_LineComboBox->SetSelectCallback<CChampionAIComponentWidget>(this, &CChampionAIComponentWidget::OnChangeChampionLine);
	//m_DetectRangeInput->SetSelectCallback<CChampionAIComponentWidget>(this, &CChampionAIComponentWidget::ChangeChampionLine);
	//m_LineComboBox->SetSelectCallback<CChampionAIComponentWidget>(this, &CChampionAIComponentWidget::ChangeChampionLine);

	return true;
}

void CChampionAIComponentWidget::SetObjectComponent(CObjectComponent* Com)
{
	CObjectComponentWidget::SetObjectComponent(Com);

	CChampionAIComponent* ChampionAIComp = (CChampionAIComponent*)m_Component.Get();

	m_Line = ChampionAIComp->GetLine();
	
}

void CChampionAIComponentWidget::OnChangeChampionLine(int Index, const char* Label)
{
}

void CChampionAIComponentWidget::OnChangeDetectRange(int Index, const char* Label)
{
}

void CChampionAIComponentWidget::OnChangeInHome(int Index, const char* Label)
{
}
