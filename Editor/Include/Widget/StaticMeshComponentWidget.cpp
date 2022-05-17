#include "StaticMeshComponentWidget.h"
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
#include "Component/StaticMeshComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CStaticMeshComponentWidget::CStaticMeshComponentWidget()	:
	m_MeshName(nullptr),
	m_LoadMeshButton(nullptr),
	m_MaterialSlotCombo(nullptr),
	m_BaseColorEdit(nullptr),
	m_AmbientColorEdit(nullptr),
	m_SpecularColorEdit(nullptr),
	m_SpecluarPowerEdit(nullptr),
	m_EmissiveColorEdit(nullptr),
	m_TransparencyEdit(nullptr),
	m_OpacityEdit(nullptr)
{
}

CStaticMeshComponentWidget::~CStaticMeshComponentWidget()
{
}

bool CStaticMeshComponentWidget::Init()
{
	CSceneComponentWidget::Init();

	// AddWidget
	CIMGUIText* Text = AddWidget<CIMGUIText>("Text");
	Text->SetText("Static Mesh Component");

	m_MeshName = AddWidget<CIMGUITextInput>("Mesh Name");
	AddWidget<CIMGUISameLine>("Line");
	m_LoadMeshButton = AddWidget<CIMGUIButton>("Load", 0.f, 0.f);

	m_MaterialSlotCombo = AddWidget<CIMGUIComboBox>("Material Slot");
	m_BaseColorEdit = AddWidget<CIMGUIColor3>("BaseColor");
	m_AmbientColorEdit = AddWidget<CIMGUIColor3>("Ambient");
	m_SpecularColorEdit = AddWidget<CIMGUIColor3>("Specluar");
	m_SpecluarPowerEdit = AddWidget<CIMGUIInputFloat>("Specluar Power");
	m_EmissiveColorEdit = AddWidget<CIMGUIColor3>("Emissive");
	m_TransparencyEdit = AddWidget<CIMGUICheckBox>("Enable Transparency");
	m_OpacityEdit = AddWidget<CIMGUISliderFloat>("Opacity");

	// Initial Setting
	m_MeshName->ReadOnly(true);
	m_MeshName->SetHintText("Mesh Name");
	m_TransparencyEdit->AddCheckInfo("Enable");
	m_OpacityEdit->SetMin(0.f);
	m_OpacityEdit->SetMax(1.f);

	// CallBack
	m_LoadMeshButton->SetClickCallback(this, &CStaticMeshComponentWidget::OnClickLoadMesh);
	m_MaterialSlotCombo->SetSelectCallback(this, &CStaticMeshComponentWidget::OnSelectMaterialSlotCombo);
	m_BaseColorEdit->SetCallBack(this, &CStaticMeshComponentWidget::OnEditBaseColor);
	m_AmbientColorEdit->SetCallBack(this, &CStaticMeshComponentWidget::OnEditAmbientColor);
	m_SpecularColorEdit->SetCallBack(this, &CStaticMeshComponentWidget::OnEditSpecluarColor);
	m_EmissiveColorEdit->SetCallBack(this, &CStaticMeshComponentWidget::OnEditEmissiveColor);
	m_SpecluarPowerEdit->SetCallBack(this, &CStaticMeshComponentWidget::OnEditSpecluarPower);
	m_TransparencyEdit->SetCallBackIdx(this, &CStaticMeshComponentWidget::OnCheckTransparency);
	m_OpacityEdit->SetCallBack(this, &CStaticMeshComponentWidget::OnEditOpacity);

	return true;
}

void CStaticMeshComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();
	m_MeshName->SetText(MeshCom->GetMesh()->GetName().c_str());

	int MatSlotSize = MeshCom->GetMaterialSlotSize();
	for (int i = 0; i < MatSlotSize; ++i)
	{
		m_MaterialSlotCombo->AddItem(MeshCom->GetMaterial(i)->GetName());
	}
}

void CStaticMeshComponentWidget::OnClickLoadMesh()
{
	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("Mesh File\0*.msh\0");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MESH_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		TCHAR   FileName[MAX_PATH] = {};

		_wsplitpath_s(FilePath, 0, 0, 0, 0, FileName, MAX_PATH, 0, 0);

		char    ConvertFileName[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, 0, 0);

		// TODO : 메쉬 로드
	}
}

void CStaticMeshComponentWidget::OnSelectMaterialSlotCombo(int Idx, const char* Label)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	CMaterial* Mat = MeshCom->GetMaterial(Idx);
	
	m_BaseColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
	m_AmbientColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
	m_SpecularColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
	m_EmissiveColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
	m_SpecluarPowerEdit->SetVal(Mat->GetSpecularPower());
	m_OpacityEdit->SetValue(Mat->GetOpacity());
}

void CStaticMeshComponentWidget::OnEditBaseColor(const Vector3& Color)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	MeshCom->SetBaseColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
}

void CStaticMeshComponentWidget::OnEditAmbientColor(const Vector3& Color)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	MeshCom->SetAmbientColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
}

void CStaticMeshComponentWidget::OnEditSpecluarColor(const Vector3& Color)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	MeshCom->SetSpecularColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
}

void CStaticMeshComponentWidget::OnEditSpecluarPower(float Power)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	MeshCom->SetSpecularPower(Power, m_MaterialSlotCombo->GetSelectIndex());
}

void CStaticMeshComponentWidget::OnEditEmissiveColor(const Vector3& Color)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	MeshCom->SetEmissiveColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
}

void CStaticMeshComponentWidget::OnCheckTransparency(int Idx, bool Check)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	MeshCom->SetTransparency(Check, m_MaterialSlotCombo->GetSelectIndex());
}

void CStaticMeshComponentWidget::OnEditOpacity(float Opacity)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component.Get();

	MeshCom->SetOpacity(Opacity, m_MaterialSlotCombo->GetSelectIndex());
}
