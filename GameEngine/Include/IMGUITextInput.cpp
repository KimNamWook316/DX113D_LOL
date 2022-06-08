#include "IMGUITextInput.h"

CIMGUITextInput::CIMGUITextInput() :
	m_TextType(ImGuiText_Type::String),
	m_Text{},
	m_wText{},
	m_TextUTF8{},
	m_HintText{},
	m_wHintText{},
	m_HintTextUTF8{},
	m_MultiLine(false),
	m_Flag(0),
	m_ValueInt(0),
	m_ValueFloat(0.f)
{
}

CIMGUITextInput::~CIMGUITextInput()
{
}

bool CIMGUITextInput::Init()
{
	CIMGUIWidget::Init();

	return true;
}

void CIMGUITextInput::Render()
{
	RenderText();

	ApplyDropEffect();

	ImGui::PopID();
}

void CIMGUITextInput::RenderText()
{
	bool	Input = false;

	ImGui::PushID(m_WidgetID);
	ImGui::PushItemWidth(m_Size.x);

	if (m_MultiLine)
	{
		Input = ImGui::InputTextMultiline(m_Name.c_str(), m_Text, 1024, m_Size, m_Flag);
	}

	else
	{
		switch (m_TextType)
		{
		case ImGuiText_Type::String:
			if (strlen(m_HintText) > 0)
			{
				Input = ImGui::InputTextWithHint(m_Name.c_str(), m_HintText, m_Text, 1024, m_Flag);
			}

			else
			{
				Input = ImGui::InputText(m_Name.c_str(), m_Text, 1024, m_Flag);
			}
			break;
		case ImGuiText_Type::Int:
			Input = ImGui::InputInt(m_Name.c_str(), &m_ValueInt, 1, 10, m_Flag);
			break;
		case ImGuiText_Type::Float:
			Input = ImGui::InputFloat(m_Name.c_str(), &m_ValueFloat, 0.f, 0.f, "%.3f", m_Flag);
			break;
		}
	}

	if (Input)
	{
		if (m_TextType == ImGuiText_Type::String)
		{
			memset(m_wText, 0, sizeof(wchar_t) * 1024);
			memset(m_TextUTF8, 0, sizeof(char) * 1024);

			int	Length = MultiByteToWideChar(CP_ACP, 0, m_Text, -1, 0, 0);
			MultiByteToWideChar(CP_ACP, 0, m_Text, -1, m_wText, Length);

			// UTF8로 변환한다.
			Length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, Length, 0, 0);

		}

		if (m_InputCallback)
			m_InputCallback();
	}
}

void CIMGUITextInput::ApplyDropEffect()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
		{
			// 일반적으로 IMGUIText Widget을 Drop 할 것이다.
			// 그리고 IMGUIText 는 char[] 이기 때문에, char type 의 배수 크기인지 확인한다.
			IM_ASSERT(payload->DataSize % sizeof(char) == 0);
			char* payload_n = (char*)payload->Data;

			char AcceptedName[MAX_PATH] = {};
			strcpy_s(AcceptedName, payload_n);
			bool True = false;

			// 들어온 Input 으로, Name 을 세팅

		}
		ImGui::EndDragDropTarget();
	}
}

