// IMGUI
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUICheckBox.h"
#include "IMGUIDummy.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIProgressBar.h"
#include "IMGUIChild.h"
#include "IMGUIText.h"
#include "IMGUITable.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIColor3.h"
#include "IMGUITextInput.h"
#include "IMGUISliderFloat.h"
#include "IMGUISliderInt.h"
#include "IMGUITableElemList.h"
// Window
#include "MaterialEditor.h"
// Engine
#include "Resource/Material/Material.h"

CMaterialEditor::CMaterialEditor()
{
}

CMaterialEditor::~CMaterialEditor()
{
}

bool CMaterialEditor::Init()
{
	// class CIMGUIText* m_SelectedMaterialName;  // 현재 선택된 Material 이름
	// class CIMGUITextInput* m_NewMaterialName; // 동시에 파일 이름으로 저장될 것이다.
	
	// New Material
	m_NewMaterialName = AddWidget<CIMGUITextInput>("New Mtrl Name", 150.f, 20.f);
	m_NewMaterialName->SetHideName(true);
	m_NewMaterialName->SetHintText("New Mtrl Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_CreateMaterialBtn = AddWidget<CIMGUIButton>("Create", 90.f, 20.f);
	m_CreateMaterialBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnCreateMaterialCallback);

	// 현재 선택된 Material 정보
	m_SelectedMaterialName = AddWidget<CIMGUIText>("Current Mtrl Name", 150.f, 20.f);

	m_TextureInfoTable = AddWidget<CIMGUITableElemList>("Texture Info", 400.f, 150.f);
	m_TextureInfoTable->SetTableTitle("Texture Info");

	m_MtrlInfoTable = AddWidget<CIMGUITable>("Mtrl Info", 200.f, 150.f);
	m_MtrlInfoTable->SetTableTitle("Mtrl Info");

	m_OutLineCheck = AddWidget<CIMGUICheckBox>("Outline", 80.f);
	m_OutLineCheck->AddCheckInfo("Outline");
	m_OutLineCheck->SetCallBackLabel<CMaterialEditor>(this, &CMaterialEditor::OnIsOutLineEdit);

	m_OutLineColor = AddWidget<CIMGUIColor3>("Color", 150.f);
	m_OutLineColor->SetCallBack<CMaterialEditor>(this, &CMaterialEditor::OnSetOutLineColor);

	m_OutLineThickNess = AddWidget<CIMGUIInputFloat>("ThickNess", 150.f);
	m_OutLineThickNess->SetCallBack<CMaterialEditor>(this, &CMaterialEditor::OnSetOutLineThickNess);

	// Add Texture
	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 150.f, 20.f);

	// Set Texture
	m_TextureRegister = AddWidget<CIMGUIInputInt>("Register", 150.f, 20.f);

	m_SetTextureInput = AddWidget<CIMGUITextInput>("Set Texture", 150.f, 20.f);
	m_SetTextureInput->SetHideName(true);
	m_SetTextureInput->SetHintText("Key Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_SetTextureBtn = AddWidget<CIMGUIButton>("Set Texture", 90.f, 20.f);
	m_SetTextureBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnSetTextureBtn);

	m_AddTextureInput = AddWidget<CIMGUITextInput>("Key Name", 150.f, 20.f);
	m_AddTextureInput->SetHideName(true);
	m_AddTextureInput->SetHintText("Key Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	m_AddTextureBtn = AddWidget<CIMGUIButton>("Add Texture", 90.f, 20.f);
	m_AddTextureBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnAddTextureBtn);

	// Load & Save Btn
	Dummy = AddWidget<CIMGUIDummy>("Dummy", 150.f, 20.f);

	m_SaveMaterialBtn = AddWidget<CIMGUIButton>("Save Mtrl", 90.f, 20.f);
	m_SaveMaterialBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnSaveMaterial);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_LoadMaterialBtn = AddWidget<CIMGUIButton>("Load Mtrl", 90.f, 20.f);
	m_LoadMaterialBtn->SetClickCallback<CMaterialEditor>(this, &CMaterialEditor::OnLoadMaterial);


	// class CIMGUIButton* m_CreateMaterialBtn;
	// class CIMGUIButton* m_SaveMaterialBtn;
	// class CIMGUIButton* m_LoadMaterialBtn;
	// 
	// class CIMGUIListBox* m_MadeMaterialList;
	// 
	// // Shader 를 Drag, Drop 할 수 있는 장소
	// 
	// // Texture 목록을 보여주는 장소
	// class CIMGUITableElemList* m_TextureInfoTable;
	// 
	// // Drop 해서 Set Texture 하기
	// 
	// // Drop 해서 Add Texture 하기 
	// 
	// // Drop 해서 Image 보여주는 Image Widget
	// 
	// 
	// // 남욱씨가 말한 내용들 Setting 하는 장소
	// class CIMGUICheckBox* m_OutLineCheck;
	// class CIMGUIColor4* m_OutLineColor;
	// class CIMGUIInputFloat* m_OutLineThickNess;


	return true;
}

void CMaterialEditor::OnCreateMaterialCallback()
{
}

void CMaterialEditor::OnIsOutLineEdit(const char*, bool Enable)
{
	if (!m_SelectedMaterial)
		return;
	m_SelectedMaterial->SetOutLineEnable(Enable);
}

void CMaterialEditor::OnSetOutLineColor(const Vector3& Color)
{
	if (!m_SelectedMaterial)
		return;
	m_SelectedMaterial->SetOutLineColor(Color);
}

void CMaterialEditor::OnSetOutLineThickNess(float ThickNess)
{
	if (!m_SelectedMaterial)
		return;
	m_SelectedMaterial->SetOutLineThickNess(ThickNess);
}

void CMaterialEditor::OnSetTextureBtn()
{
	if (!m_SelectedMaterial)
		return;
	// m_SelectedMaterial->SetOutLineThickNess(ThickNess);
}

void CMaterialEditor::OnAddTextureBtn()
{
	if (!m_SelectedMaterial)
		return;
	// m_SelectedMaterial->SetOutLineThickNess(ThickNess);
}

void CMaterialEditor::OnSaveMaterial()
{
}

void CMaterialEditor::OnLoadMaterial()
{
}

void CMaterialEditor::RefreshMaterialDisplayInfo()
{
}
