#include "IMGUIMenuItem.h"

CIMGUIMenuItem::CIMGUIMenuItem()    :
    mbSelect(false),
    mbEnable(true),
    m_EnableCheck(false)
{
}

CIMGUIMenuItem::~CIMGUIMenuItem()
{
}

bool CIMGUIMenuItem::Init()
{
    return true;
}

void CIMGUIMenuItem::Render()
{
    if (mShortCut.empty())
    {
        ImGui::MenuItem(m_Name.c_str(), NULL, &mbSelect, mbEnable);
    }
    else
    {
        ImGui::MenuItem(m_Name.c_str(), mShortCut.c_str(), &mbSelect, mbEnable);
    }

    // MenuItem 중에서 Check 표시를 사용할 것인지 아닌지를 선별하기 위한 변수
    if (!m_EnableCheck)
        mbSelect = false;

    if (mbSelect)
    {
        if (mCallBack)
        {
            mCallBack();
            mbSelect = false;
        }
    }
}
