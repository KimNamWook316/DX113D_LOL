
#include "IMGUIImage.h"
#include "Resource/Texture/Texture.h"
#include "Resource/ResourceManager.h"
#include "Input.h"
#include "Timer.h"

CIMGUIImage::CIMGUIImage()	:
	m_ImageStart{},
	m_ImageEnd{},
	m_Tint{ 1.f, 1.f, 1.f, 1.f },
	m_BorderColor{},
	m_ClickCallback(nullptr),
	m_DoubleClickCallback(nullptr),
	m_ClickStack(0),
	m_AccTime(0.f),
	m_Timer(nullptr)
{
}

CIMGUIImage::~CIMGUIImage()
{
	SAFE_DELETE(m_Timer);
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

bool CIMGUIImage::Init()
{
	SetTexture("DefaultUI", TEXT("DefaultUI.png"));

	SetImageStart(0.f, 0.f);
	SetImageEnd(100.f, 100.f);

	m_Timer = new CTimer;

	return true;
}

void CIMGUIImage::Render()
{
	if (m_Texture)
	{
		unsigned int	Width = m_Texture->GetWidth();
		unsigned int	Height = m_Texture->GetHeight();

		ImVec2	StartUV = ImVec2(m_ImageStart.x / Width, m_ImageStart.y / Height);
		ImVec2	EndUV = ImVec2(m_ImageEnd.x / Width, m_ImageEnd.y / Height);

		if (CInput::GetInst()->GetMouseLButtonClick())
		{
			m_ClickCallback();
			++m_ClickStack = 0;
		}

		m_AccTime += m_Timer->GetDeltaTime();

		if (m_AccTime > 2.f)
		{
			m_AccTime = 0.f;
			m_ClickStack = 0;
		}

		if (m_ClickStack == 2)
		{
			m_DoubleClickCallback();
		}
		

		ImGui::Image(m_Texture->GetResource(), m_Size, StartUV, EndUV, m_Tint, m_BorderColor);
	}
}
