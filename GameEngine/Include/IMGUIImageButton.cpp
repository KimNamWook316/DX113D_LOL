
#include "IMGUIImageButton.h"
#include "Resource/Texture/Texture.h"
#include "PathManager.h"

CIMGUIImageButton::CIMGUIImageButton()  :
	m_BackgroundColor(1.f, 1.f, 1.f, 1.f),
	m_TintColor(1.f, 0.f, 0.f, 1.f),
	m_Size(100.f, 100.f)
{
}

CIMGUIImageButton::~CIMGUIImageButton()
{
}

bool CIMGUIImageButton::SetTexture(CTexture* Texture)
{
	if (!m_Texture)
		return false;

	m_Texture = Texture;

	return true;
}

bool CIMGUIImageButton::SetTexture(TCHAR* FileName, const std::string& PathName)
{
	TCHAR* FullPath = new TCHAR[MAX_PATH];
	memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	lstrcpy(FullPath, Info->Path);

	return false;
}

bool CIMGUIImageButton::SetTextureFullPath(const TCHAR* FullPath)
{
	return false;
}

bool CIMGUIImageButton::Init()
{
	CIMGUIWidget::Init();

    return true;
}

void CIMGUIImageButton::Render()
{
	if (!m_Texture)
		return;

	float Width = m_Texture->GetWidth();
	float Height = m_Texture->GetHeight();

	//ImGui::PushID(m_WidgetID);

	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(m_Size.x / Width, m_Size.y / Height);// UV coordinates for (32,32) in our texture


	if (ImGui::ImageButton(m_Texture->GetResource(), m_Size, uv0, uv1, -1, m_BackgroundColor, m_TintColor))
	{
		m_PressCount += 1;

		if (m_PressCount == 2)
		{
			m_PressCount = 0;
			m_DoubleClickCallback(this);
		}
	}

	//ImGui::PopID();

}
