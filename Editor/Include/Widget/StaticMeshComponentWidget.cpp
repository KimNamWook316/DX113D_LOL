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
#include "IMGUITree.h"
#include "Component/StaticMeshComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "ToonShaderWidget.h"
#include "IMGUITextInput.h"
#include "Resource/Shader/ToonShader.h"

CStaticMeshComponentWidget::CStaticMeshComponentWidget()	:
	m_MeshName(nullptr),
	m_LoadMeshButton(nullptr),
	m_MaterialSlotCombo(nullptr),
	m_BaseColorEdit(nullptr),
	m_AmbientColorEdit(nullptr),
	m_SpecularColorEdit(nullptr),
	m_SpecluarPowerEdit(nullptr),
	m_EmissiveColorEdit(nullptr),
	m_ShaderName(nullptr),
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
	m_ComponentTypeText->SetText("StaticMesh Component");

	// 최상위 트리
	CIMGUITree* m_RootTree = AddWidget<CIMGUITree>("Static Mesh Variables");

	m_MeshName = m_RootTree->AddWidget<CIMGUITextInput>("Mesh Name");
	m_RootTree->AddWidget<CIMGUISameLine>("Line");
	m_LoadMeshButton = m_RootTree->AddWidget<CIMGUIButton>("Load", 0.f, 0.f);

	m_MaterialSlotCombo = m_RootTree->AddWidget<CIMGUIComboBox>("Material Slot", 200.f);
	m_ShaderName = m_RootTree->AddWidget<CIMGUITextInput>("Shader", 200.f);
	m_ShaderWidgetTree = m_RootTree->AddWidget<CIMGUITree>("Shader Params", 200.f);
	m_BaseColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("BaseColor", 200.f);
	m_AmbientColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("Ambient", 200.f);
	m_SpecularColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("Specluar", 200.f);
	m_SpecluarPowerEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Specluar Power", 200.f);
	m_EmissiveColorEdit = m_RootTree->AddWidget<CIMGUIColor3>("Emissive", 200.f);
	m_TransparencyEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Enable Transparency", 200.f);
	m_OpacityEdit = m_RootTree->AddWidget<CIMGUISliderFloat>("Opacity", 200.f);
	m_OutlineEnable = m_RootTree->AddWidget<CIMGUICheckBox>("Outline", 200.f);
	m_OutlineThickness = m_RootTree->AddWidget<CIMGUISliderFloat>("Outline Thickness", 200.f);
	m_OutlineColor = m_RootTree->AddWidget<CIMGUIColor3>("Outline Color", 200.f);
	
	AddWidget<CIMGUISeperator>("Sep");

	// Initial Setting
	m_MeshName->ReadOnly(true);
	m_MeshName->SetHintText("Mesh Name");
	m_TransparencyEdit->AddCheckInfo("Transparency");
	m_OpacityEdit->SetMin(0.f);
	m_OpacityEdit->SetMax(1.f);
	m_OutlineThickness->SetMin(0.1f);
	m_OutlineThickness->SetMax(20.f);
	m_OutlineEnable->AddCheckInfo("Outline Enable");
	m_ShaderName->ReadOnly(true);

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
	m_OutlineEnable->SetCallBackIdx(this, &CStaticMeshComponentWidget::OnCheckOutlineEnable);
	m_OutlineThickness->SetCallBack(this, &CStaticMeshComponentWidget::OnEditOutlineThickness);
	m_OutlineColor->SetCallBack(this, &CStaticMeshComponentWidget::OnChangeOutlineColor);
	m_ShaderName->SetDropCallBack(this, &CStaticMeshComponentWidget::OnDropShaderName);

	return true;
}

void CStaticMeshComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		RefreshMeshWidget(MeshCom->GetMesh());
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
		std::string MeshName;
		if (!CSceneManager::GetInst()->GetScene()->GetResource()->LoadMeshFullPath(MeshName, Mesh_Type::Static, FilePath))
		{
			MessageBox(nullptr, TEXT("메쉬 로드 실패"), TEXT("실패"), MB_OK);
		}

		CMesh* Mesh = CSceneManager::GetInst()->GetScene()->GetResource()->FindMesh(MeshName);

		static_cast<CStaticMeshComponent*>(m_Component)->SetMesh((CStaticMesh*)Mesh);

		RefreshMeshWidget(Mesh);
	}
}

void CStaticMeshComponentWidget::OnSelectMaterialSlotCombo(int Idx, const char* Label)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		CMaterial* Mat = MeshCom->GetMaterial(Idx);

		std::string ShaderName = Mat->GetShader()->GetName();

		MakeShaderWidget(Mat, ShaderName);

		m_ShaderName->SetText(ShaderName.c_str());
		m_BaseColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
		m_AmbientColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
		m_SpecularColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
		m_EmissiveColorEdit->SetRGB(Mat->GetBaseColor().x, Mat->GetBaseColor().y, Mat->GetBaseColor().z);
		m_SpecluarPowerEdit->SetVal(Mat->GetSpecularPower());
		m_TransparencyEdit->SetCheck(0, Mat->IsTransparent());
		m_OpacityEdit->SetValue(Mat->GetOpacity());
	}
}

void CStaticMeshComponentWidget::OnEditBaseColor(const Vector3& Color)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{ 
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetBaseColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
	}
}

void CStaticMeshComponentWidget::OnEditAmbientColor(const Vector3& Color)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetAmbientColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
	}
}

void CStaticMeshComponentWidget::OnEditSpecluarColor(const Vector3& Color)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetSpecularColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
	}
}

void CStaticMeshComponentWidget::OnEditSpecluarPower(float Power)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetSpecularPower(Power, m_MaterialSlotCombo->GetSelectIndex());
	}
}

void CStaticMeshComponentWidget::OnEditEmissiveColor(const Vector3& Color)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetEmissiveColor(Vector4(Color.x, Color.y, Color.z, 1.f), m_MaterialSlotCombo->GetSelectIndex());
	}
}

void CStaticMeshComponentWidget::OnCheckTransparency(int Idx, bool Check)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetTransparency(Check, Idx);
	}
}

void CStaticMeshComponentWidget::OnEditOpacity(float Opacity)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetOpacity(Opacity, m_MaterialSlotCombo->GetSelectIndex());
	}
}

void CStaticMeshComponentWidget::OnCheckOutlineEnable(int Idx, bool Enable)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->EnableOutline(Enable);
	}
}

void CStaticMeshComponentWidget::OnEditOutlineThickness(float Val)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetOutlineThickness(Val);
	}
}

void CStaticMeshComponentWidget::OnChangeOutlineColor(const Vector3& Color)
{
	if (m_MaterialSlotCombo->GetSelectIndex() == -1)
	{
		return;
	}

	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	if (MeshCom->GetMesh())
	{
		MeshCom->SetOutlineColor(Color);
	}
}

void CStaticMeshComponentWidget::OnDropShaderName(const std::string& Name)
{
	int Index = m_MaterialSlotCombo->GetSelectIndex();

	if (Index == -1)
	{
		return;
	}

	CMaterial* Mat = static_cast<CStaticMeshComponent*>(m_Component)->GetMaterial(Index);
	CShader* Shader = CSceneManager::GetInst()->GetScene()->GetResource()->FindShader(Name);

	if (!Shader || Shader->GetShaderType() != Shader_Type::Graphic)
	{
		return;
	}

	Mat->SetShader((CGraphicShader*)Shader);
	m_ShaderName->SetText(Name.c_str());
}

void CStaticMeshComponentWidget::RefreshMeshWidget(CMesh* Mesh)
{
	CStaticMeshComponent* MeshCom = (CStaticMeshComponent*)m_Component;

	m_MeshName->SetText(MeshCom->GetMesh()->GetName().c_str());

	std::string AutoName;

	int MatSlotSize = MeshCom->GetMaterialSlotCount();
	for (int i = 0; i < MatSlotSize; ++i)
	{
		// Material 이름이 없을 경우 자동으로 이름 지정
		if (MeshCom->GetMaterial(i)->GetName().empty())
		{
			AutoName = "Material" + std::to_string(i);
			m_MaterialSlotCombo->AddItem(AutoName);
			AutoName.clear();
		}
		else
		{
			m_MaterialSlotCombo->AddItem(MeshCom->GetMaterial(i)->GetName());
		}
	}
}

bool CStaticMeshComponentWidget::MakeShaderWidget(class CMaterial* Mat, const std::string& ShaderName)
{
	SAFE_DELETE(m_ShaderWidget);

	CShader* Shader = CResourceManager::GetInst()->FindShader(ShaderName);
	size_t TypeID = Shader->GetTypeID();

	if (TypeID == typeid(CToonShader).hash_code())
	{
		m_ShaderWidget = m_ShaderWidgetTree->AddWidget<CToonShaderWidget>("ToonShaderWidget", 200.f, 200.f);
	}
	else
	{
		return false;
	}

	ShaderParams MatShaderParams = Mat->GetShaderParams();
	m_ShaderWidget->SetShaderParams(MatShaderParams);
	m_ShaderWidget->SetMaterial(Mat);
	return true;
}
