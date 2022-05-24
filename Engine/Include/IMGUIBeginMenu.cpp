#include "IMGUIBeginMenu.h"

CIMGUIBeginMenu::CIMGUIBeginMenu()
{
}

CIMGUIBeginMenu::~CIMGUIBeginMenu()
{
	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
}

bool CIMGUIBeginMenu::Init()
{
	return true;
}

void CIMGUIBeginMenu::Render()
{
	if (ImGui::BeginMenu(m_Name.c_str()))
	{
		mSize = m_vecChild.size();
		for (size_t i = 0; i < mSize; ++i)
		{
			m_vecChild[i]->Render();
		}

		ImGui::EndMenu();
	}
}
