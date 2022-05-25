
#include "IMGUIImage.h"
#include "Resource/Texture/Texture.h"
#include "Resource/ResourceManager.h"
#include "Render\RenderManager.h"
#include "Input.h"
#include "Timer.h"
#include "Device.h"

CIMGUIImage::CIMGUIImage()	:
	m_ImageStart{},
	m_ImageEnd{},
	m_Tint{ 1.f, 1.f, 1.f, 1.f },
	m_BorderColor{},
	m_IsRenderTargetImage(false)
{
}

CIMGUIImage::~CIMGUIImage()
{
}

void CIMGUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTexture(const std::string& Name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::CreateTexture(unsigned int Width, unsigned int Height)
{
	// Target용 Texture 생성
	D3D11_TEXTURE2D_DESC	Desc = {};

	Desc.Width = Width;
	Desc.Height = Height;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET;
	// Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&Desc, nullptr, &m_TexResource)))
		return ;

	m_TexResource->QueryInterface(__uuidof(IDXGISurface), (void**)&m_Surface);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(
		m_TexResource, nullptr, &m_ShaderResourceView)))
		return ;

	// m_ClearColor[0] = 1.f;
	// m_ClearColor[1] = 1.f;
	// m_ClearColor[2] = 1.f;
	// m_ClearColor[3] = 1.f;
}

void CIMGUIImage::SetCopyTargetTexture(ID3D11Texture2D* CopyTargetTexture)
{
}

bool CIMGUIImage::Init()
{
	SetTexture("DefaultUI", TEXT("DefaultUI.png"));

	SetImageStart(0.f, 0.f);
	SetImageEnd(100.f, 100.f);

	return true;
}

void CIMGUIImage::Render()
{
	if (ImGui::CollapsingHeader("See Rendered Image"))
	{
		if (m_Texture)
		{
			unsigned int	Width = m_Texture->GetWidth();
			unsigned int	Height = m_Texture->GetHeight();

			ImVec2	StartUV = ImVec2(m_ImageStart.x / Width, m_ImageStart.y / Height);
			ImVec2	EndUV = ImVec2(m_ImageEnd.x / Width, m_ImageEnd.y / Height);

			ImGui::Image(m_Texture->GetResource(), m_Size, StartUV, EndUV, m_Tint, m_BorderColor);
			/*
			// if (!m_IsRenderTargetImage)
			if (!m_IsRenderTargetImage)
			{
				ImGui::Image(m_Texture->GetResource(), m_Size, StartUV, EndUV, m_Tint, m_BorderColor);
			}
			else
			{
				m_CopyTargetTexResource = m_Texture->GetTextureResource();

				if (!m_CopyTargetTexResource)
					return;

				if (!m_TexResource)
				{
					CreateTexture(Width, Height);
				}

				// Target Texture 복사 
				CDevice::GetInst()->GetContext()->CopyResource(m_TexResource, m_CopyTargetTexResource);

				// ImGui::Image(m_Texture->GetResource(), m_Size, StartUV, EndUV, m_Tint, m_BorderColor);
				ImGui::Image(m_ShaderResourceView, m_Size, StartUV, EndUV, m_Tint, m_BorderColor);
			}
			*/
		}
	}
}
