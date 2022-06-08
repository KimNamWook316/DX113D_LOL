#include "IMGUIText.h"

CIMGUIText::CIMGUIText() :
    m_Text{},
    m_wText{},
    m_IsHelpMode(false),
    m_TextUTF8{}
{
}

CIMGUIText::~CIMGUIText()
{
}

bool CIMGUIText::Init()
{
    return true;
}

void CIMGUIText::Render()
{
    if (m_IsHelpMode) // Ό³Έν±Ϋ 
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(m_TextUTF8);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
    else
    {
        ImGui::TextColored(m_Color.Value, m_TextUTF8);
    }

    ApplyDragEffect();
}

void CIMGUIText::ApplyDragEffect()
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
        ImGui::SetDragDropPayload("DND_DEMO_CELL", m_TextUTF8, sizeof(char) * strlen(m_TextUTF8));

        ImGui::EndDragDropSource();
    }
}
