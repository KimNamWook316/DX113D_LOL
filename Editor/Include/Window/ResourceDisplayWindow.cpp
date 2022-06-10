// IMGUI
#include "IMGUIWidgetList.h"
#include "IMGUIText.h"
// Window
#include "ResourceDisplayWindow.h"
// Engine
#include "Resource/Texture/TextureManager.h"
#include "Resource/ResourceManager.h"
#include "Render/RenderManager.h"
#include "Render/RenderStateManager.h"
#include "Resource/Shader/ShaderManager.h"

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

	// Shader List
	m_ShaderList = AddWidget<CIMGUIWidgetList>("Shader", 100.f, 20.f);
	m_ShaderList->SetApplyHideEffect(true);
	m_vecResourceTapList.push_back(m_ShaderList);

	// Sampler List
	m_SamplerList = AddWidget<CIMGUIWidgetList>("Sampler", 100.f, 20.f);
	m_SamplerList->SetApplyHideEffect(true);
	m_vecResourceTapList.push_back(m_SamplerList);

	RefreshLoadedTextureResources();
	RefreshLoadedMaterialResources();
	RefreshLoadedSamplerResources();
	RefreshLoadedShaderResources();

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

void CResourceDisplayWindow::RefreshLoadedSamplerResources()
{
	m_SamplerList->ClearWidget();

	const std::unordered_map<std::string, CSharedPtr<class CRenderState>>& MapSampler = CRenderManager::GetInst()->GetRenderStateManager()->GetMapRenderState();
	
	auto iter = MapSampler.begin();
	auto iterEnd = MapSampler.end();
	
	for (; iter != iterEnd; ++iter)
	{
		CIMGUIText* AddedText = m_SamplerList->AddWidget<CIMGUIText>(iter->first);
		AddedText->SetText(iter->first.c_str());
	}
}

void CResourceDisplayWindow::RefreshLoadedShaderResources()
{
	m_ShaderList->ClearWidget();

	const std::unordered_map<std::string, CSharedPtr<class CShader>>& MapShader = CResourceManager::GetInst()->GetShaderManager()->GetMapShader();

	auto iter = MapShader.begin();
	auto iterEnd = MapShader.end();

	for (; iter != iterEnd; ++iter)
	{
		CIMGUIText* AddedText = m_ShaderList->AddWidget<CIMGUIText>(iter->first);
		AddedText->SetText(iter->first.c_str());
	}
}

