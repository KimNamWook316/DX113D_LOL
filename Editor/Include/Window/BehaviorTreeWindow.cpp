
#include "BehaviorTreeWindow.h"
#include "IMGUIManager.h"
#include "IMGUIButton.h"
#include "Component/StateComponent.h"
#include "Component/Node/SequenceNode.h"
#include "Component/Node/SelectorNode.h"
#include "Component/Node/ActionNode.h"
#include "Component/Node/ConditionNode.h"

CBehaviorTreeWindow::CBehaviorTreeWindow()  :
    m_Select(false),
    m_TypeSelectIndex(-1),
    m_ActionSelectIndex(-1),
    m_StateComponent(nullptr)
{
    m_vecNodeType.push_back("Sequence Node");
    m_vecNodeType.push_back("Selector Node");
    m_vecNodeType.push_back("Action Node");
    m_vecNodeType.push_back("Condition Node");

    m_vecNodeAction.push_back("InputQCheck");
    m_vecNodeAction.push_back("InputWCheck");
    m_vecNodeAction.push_back("InputECheck");
    m_vecNodeAction.push_back("InputRCheck");
    m_vecNodeAction.push_back("InputDCheck");
    m_vecNodeAction.push_back("InputFCheck");
    m_vecNodeAction.push_back("MouseRightInputCheck");

    m_vecNodeAction.push_back("SkillQ");
    m_vecNodeAction.push_back("SkillW");
    m_vecNodeAction.push_back("SkillE");
    m_vecNodeAction.push_back("SkillR");
    m_vecNodeAction.push_back("SpellD");
    m_vecNodeAction.push_back("SpellF");
    m_vecNodeAction.push_back("Move");

}

CBehaviorTreeWindow::~CBehaviorTreeWindow()
{
    size_t Count = m_Delegate.GetTemplateCount();

    for (size_t i = 0; i < Count; ++i)
    {
        GraphEditor::Template tmp = m_Delegate.GetTemplate(i);
        if (tmp.mInputNames)
        {
            delete[] tmp.mInputNames;
        }

        if (tmp.mOutputNames)
        {
            delete[] tmp.mOutputNames;
        }
    }
}

bool CBehaviorTreeWindow::Init()
{
	CIMGUIWindow::Init();

    if (m_StateComponent)
        m_StateComponent->SetTreeUpdate(false);

	return true;
}

void CBehaviorTreeWindow::Update(float DeltaTime)
{
    GraphEditor::FitOnScreen fit = GraphEditor::FitOnScreen::Fit_None;

    ImGui::Begin("Graph Editor", NULL, 0);

    if (ImGui::Button("Fit all nodes"))
    {
        fit = GraphEditor::FitOnScreen::Fit_AllNodes;
    }
    ImGui::SameLine();
    if (ImGui::Button("Fit selected nodes"))
    {
        fit = GraphEditor::FitOnScreen::Fit_SelectedNodes;
    }

    ImGui::PushID(m_WidgetID);
    ImGui::PushItemWidth(150.f);

    // 노드 타입 결정하는 콤보
    if (ImGui::BeginCombo("", "Select Node Type", m_ComboFlag))
    {
        size_t  ItemCount = m_vecNodeType.size();

        for (size_t i = 0; i < ItemCount; ++i)
        {
            m_Select = m_TypeSelectIndex == i;

            if (ImGui::Selectable(m_vecNodeType[i].c_str(), m_Select))
            {
                //if (m_SelectIndex != i && m_SelectCallback)
                //    m_SelectCallback((int)i, m_vecItem[i].c_str());

                m_PrevViewName = m_vecNodeType[i];

                m_TypeSelectIndex = (int)i;
            }

            if (m_Select)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::PopID();

    ImGui::SameLine();


    // 노드가 ActionNode이거나 Condition Node일 때 Invoke할 동작을 정하고 만들어야한다
    if (m_TypeSelectIndex == 2 || m_TypeSelectIndex == 3)
    {
        ImGui::PushID(m_WidgetID + 1);
        ImGui::PushItemWidth(150.f);

        if (ImGui::BeginCombo("", "Select Node Action", m_ComboFlag))
        {
            size_t  ItemCount = m_vecNodeAction.size();

            for (size_t i = 0; i < ItemCount; ++i)
            {
                m_Select = m_ActionSelectIndex == i;

                if (ImGui::Selectable(m_vecNodeAction[i].c_str(), m_Select))
                {
                    //if (m_SelectIndex != i && m_SelectCallback)
                    //    m_SelectCallback((int)i, m_vecItem[i].c_str());

                    m_PrevViewName = m_vecNodeAction[i];

                    m_ActionSelectIndex = (int)i;
                }

                if (m_Select)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::PopID();

        ImGui::SameLine();
    }

    // 노드 이름
    bool	Input = false;

    ImGui::PushID(m_WidgetID + 1);
    ImGui::PushItemWidth(100.f);

    char HintText[1024] = "Enter Node Name";
    char Tmp[10] = "";

    Input = ImGui::InputTextWithHint("##", HintText, Tmp, 1024, m_TextInputFlag);

    if (Input)
    {
        memset(m_wText, 0, sizeof(wchar_t) * 1024);
        memset(m_TextUTF8, 0, sizeof(char) * 1024);

        int	Length = MultiByteToWideChar(CP_ACP, 0, m_Text, -1, 0, 0);
        MultiByteToWideChar(CP_ACP, 0, m_Text, -1, m_wText, Length);

        // UTF8로 변환한다.
        Length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, Length, 0, 0);
    }
    ImGui::PopID();

    ImGui::SameLine();

    // 노드 추가 버튼
    if (ImGui::Button("Add Node"))
    {
        //OnAddNodeButton(m_TextUTF8, m_TypeSelectIndex, m_ActionSelectIndex);
    }

    ImGui::SameLine();

    ImGui::Dummy(ImVec2(100.f, 20.f));

    ImGui::SameLine();

    if (ImGui::Button("Run"))
    {
        if (m_StateComponent)
            m_StateComponent->SetTreeUpdate(true);
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop"))
    {
        if (m_StateComponent)
            m_StateComponent->SetTreeUpdate(false);
    }

    GraphEditor::Show(m_Delegate, m_Option, m_ViewState, true, &fit);

    ImGui::End();


    //CIMGUIWindow::Update(DeltaTime);
}

void CBehaviorTreeWindow::SetStateComponent(CStateComponent* Com)
{
    m_StateComponent = Com;
}

CStateComponent* CBehaviorTreeWindow::GetStateComponent() const
{
    return m_StateComponent;
}

void CBehaviorTreeWindow::OnAddNodeButton(const char* Name, int TypeIndex, int ActionIndex, CNode* TreeNode)
{
    GraphEditor::TemplateIndex idx = TypeIndex;

    m_Delegate.AddNode(Name, idx, 0.f, 0.f, false, TreeNode);

    CNode* NewTreeNode = nullptr;

    switch (TypeIndex)
    {
    case 0:
        NewTreeNode = m_StateComponent->CreateTreeNode<CSequenceNode>(Name);
        break;
    case 1:
        NewTreeNode = m_StateComponent->CreateTreeNode<CSelectorNode>(Name);
        break;
    case 2:
    {
        enum LeafNode NodeActionClass;
        NodeActionClass = static_cast<LeafNode>(ActionIndex);

        switch (NodeActionClass)
        {
        case LeafNode::InputQCheck:
            break;
        case LeafNode::InputWCheck:
            break;
        case LeafNode::InputECheck:
            break;
        case LeafNode::InputRCheck:
            break;
        case LeafNode::InputDCheck:
            break;
        case LeafNode::InputFCheck:
            break;
        case LeafNode::MouseRightInputCheck:
            break;
        case LeafNode::SkillQ:
            break;
        case LeafNode::SkillW:
            break;
        case LeafNode::SkillE:
            break;
        case LeafNode::SkillR:
            break;
        case LeafNode::SpellD:
            break;
        case LeafNode::SpellF:
            break;
        case LeafNode::Move:
            break;
        }

        break;
    }

    case 3:
    {

        break;
    }

    }

    if (!m_StateComponent->GetBehaviorTree()->GetRootNode())
    {
        m_StateComponent->GetBehaviorTree()->SetRoot(NewTreeNode);
    }
}
