// IMGUI
#include "IMGUIWidgetList.h"
#include "IMGUIText.h"
// Window
#include "ResourceDisplayWindow.h"
// Engine
#include "Resource/Texture/TextureManager.h"
#include "Resource/ResourceManager.h"

CResourceDisplayWindow::CResourceDisplayWindow()
{
}

CResourceDisplayWindow::~CResourceDisplayWindow()
{
}

bool CResourceDisplayWindow::Init()
{
	// Texture List
	m_TextureList = AddWidget<CIMGUIWidgetList>("Texture", 100.f, 20.f);
	m_TextureList->SetApplyHideEffect(true);

	// -- Sample
	CIMGUIText* AddedText = m_TextureList->AddWidget<CIMGUIText>("Sample Text");
	AddedText->SetText("Hello");

	m_vecResourceTapList.push_back(m_TextureList);

	// Material List
	m_MaterialList = AddWidget<CIMGUIWidgetList>("Material", 100.f, 20.f);
	m_MaterialList->SetApplyHideEffect(true);
	m_vecResourceTapList.push_back(m_MaterialList);

	RefreshLoadedTextureResources();
	RefreshLoadedMaterialResources();

	return true;
}

void CResourceDisplayWindow::RefreshLoadedTextureResources()
{
	m_TextureList->ClearWidget();

	const std::unordered_map<std::string, CSharedPtr<class CTexture>>& MapTexture = CResourceManager::GetInst()->GetTextureManager()->GetMapTexture();

	auto iter = MapTexture.begin();
	auto iterEnd = MapTexture.end();

	for (; iter != iterEnd; ++iter)
	{
		CIMGUIText* AddedText = m_TextureList->AddWidget<CIMGUIText>(iter->first);
		AddedText->SetText(iter->first.c_str());
	}
}

void CResourceDisplayWindow::RefreshLoadedMaterialResources()
{
	m_MaterialList->ClearWidget();

	const std::unordered_map<std::string, CSharedPtr<class CMaterial>>& MapTexture = CResourceManager::GetInst()->GetMaterialManager()->GetMapMaterial();
	
	auto iter = MapTexture.begin();
	auto iterEnd = MapTexture.end();
	
	for (; iter != iterEnd; ++iter)
	{
		CIMGUIText* AddedText = m_MaterialList->AddWidget<CIMGUIText>(iter->first);
		AddedText->SetText(iter->first.c_str());
	}
}

