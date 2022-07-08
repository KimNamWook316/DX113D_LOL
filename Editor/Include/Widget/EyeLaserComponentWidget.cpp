
#include "EyeLaserComponentWidget.h"
#include "../Component/EyeLaserComponent.h"
#include "IMGUITextInput.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUISliderFloat.h"
#include "IMGUIColor3.h"
#include "IMGUIInputFloat.h"
#include "IMGUIComboBox.h"
#include "IMGUIDummy.h"
#include "IMGUITree.h"

CEyeLaserComponentWidget::CEyeLaserComponentWidget()
{
}

CEyeLaserComponentWidget::~CEyeLaserComponentWidget()
{
}

bool CEyeLaserComponentWidget::Init()
{
	CSceneComponentWidget::Init();

	m_ComponentTypeText->SetText("EyeLaser Component");

	// 최상위 트리
	CIMGUITree* m_RootTree = AddWidget<CIMGUITree>("EyeLaser Variables");

	m_MeshName = m_RootTree->AddWidget<CIMGUITextInput>("Mesh Name");
	m_RootTree->AddWidget<CIMGUISameLine>("Line");

	// Transparent는 모든 Material에 일괄 적용
	m_TransparencyEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Enable Transparency", 200.f);

	m_MaterialSlotCombo = m_RootTree->AddWidget<CIMGUIComboBox>("Material Slot", 200.f);
	m_BaseColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("BaseColor", 200.f);
	m_AmbientColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("Ambient", 200.f);
	m_SpecularColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("Specluar", 200.f);
	m_SpecluarPowerEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Specluar Power", 200.f);
	m_EmissiveColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("Emissive", 200.f);
	m_OpacityEdit = m_RootTree->AddWidget<CIMGUISliderFloat>("Opacity", 200.f);

	AddWidget<CIMGUISeperator>("Sep");

	// Initial Setting

	m_TransparencyEdit->AddCheckInfo("Transparency");
	m_OpacityEdit->SetMin(0.f);
	m_OpacityEdit->SetMax(1.f);

	// CallBack
	m_BaseColorEdit->SetCallBack(this, &CEyeLaserComponentWidget::OnEditBaseColor);
	m_AmbientColorEdit->SetCallBack(this, &CEyeLaserComponentWidget::OnEditAmbientColor);
	m_SpecularColorEdit->SetCallBack(this, &CEyeLaserComponentWidget::OnEditSpecluarColor);
	m_EmissiveColorEdit->SetCallBack(this, &CEyeLaserComponentWidget::OnEditEmissiveColor);
	m_SpecluarPowerEdit->SetCallBack(this, &CEyeLaserComponentWidget::OnEditSpecluarPower);
	m_TransparencyEdit->SetCallBackIdx(this, &CEyeLaserComponentWidget::OnCheckTransparency);
	m_OpacityEdit->SetCallBack(this, &CEyeLaserComponentWidget::OnEditOpacity);

	return true;
}

void CEyeLaserComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CEyeLaserComponent* MeshCom = (CEyeLaserComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		RefreshMeshWidget(MeshCom->GetMesh());
	}
}

void CEyeLaserComponentWidget::OnEditBaseColor(const Vector3 & Color)
{
	CEyeLaserComponent* MeshCom = (CEyeLaserComponent*)m_Component;

	//if (MeshCom->GetMesh())
	//{
	//	MeshCom->SetBaseColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
	//}
}

void CEyeLaserComponentWidget::OnEditAmbientColor(const Vector3& Color)
{
}

void CEyeLaserComponentWidget::OnEditSpecluarColor(const Vector3& Color)
{
}

void CEyeLaserComponentWidget::OnEditSpecluarPower(float Power)
{
}

void CEyeLaserComponentWidget::OnEditEmissiveColor(const Vector3& Color)
{
}

void CEyeLaserComponentWidget::OnCheckTransparency(int Idx, bool Check)
{
}

void CEyeLaserComponentWidget::OnEditOpacity(float Opacity)
{
}

void CEyeLaserComponentWidget::RefreshMeshWidget(CMesh* Mesh)
{
	CEyeLaserComponent* MeshCom = (CEyeLaserComponent*)m_Component;

	m_MeshName->SetText(MeshCom->GetMesh()->GetName().c_str());

	std::string AutoName;

	// Material 이름이 없을 경우 자동으로 이름 지정
	if (MeshCom->GetMaterial()->GetName().empty())
	{
		AutoName = "Material0";
		m_MaterialSlotCombo->AddItem(AutoName);
		AutoName.clear();
	}
	else
	{
		m_MaterialSlotCombo->AddItem(MeshCom->GetMaterial()->GetName());
	}


	m_TransparencyEdit->SetCheck(0, MeshCom->IsTransparent());
}