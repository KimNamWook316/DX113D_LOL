
#include "BehaviorTreeWindow.h"
#include "IMGUIManager.h"
#include "IMGUIButton.h"
#include "Component/StateComponent.h"
#include "Component/Node/SequenceNode.h"
#include "Component/Node/SelectorNode.h"
#include "Component/Node/ActionNode.h"
#include "Component/Node/ConditionNode.h"
#include "Component/Node/InputQCheckNode.h"
#include "Component/Node/InputWCheckNode.h"
#include "Component/Node/SkillQNode.h"
#include "Component/Node/SkillWNode.h"
#include "Component/Node/SkillENode.h"
#include "Component/Node/SkillRNode.h"
#include "Component/Node/InputQCheckNode.h"
#include "Component/Node/InputWCheckNode.h"
#include "Component/Node/InputECheckNode.h"
#include "Component/Node/InputRCheckNode.h"
#include "Component/Node/IdleNode.h"
#include "Component/Node/MoveInputCheckNode.h"
#include "Component/Node/MovePickingNode.h"
#include "Component/Node/NoInterruptNode.h"

#include "ObjectComponentWindow.h"
#include "ObjectHierarchyWindow.h"
#include "../EditorInfo.h"

CBehaviorTreeWindow::CBehaviorTreeWindow()  :
    m_Select(false),
    m_TypeSelectIndex(-1),
    m_ActionSelectIndex(-1),
    m_StateComponent(nullptr)
{
    m_PrevViewName = "Select Node Type";
    m_LeafNodePrevViewName = "Select Node Action";

    m_vecNodeType.push_back("Sequence Node");
    m_vecNodeType.push_back("Selector Node");
    m_vecNodeType.push_back("Action Node");
    m_vecNodeType.push_back("Condition Node");

}

CBehaviorTreeWindow::~CBehaviorTreeWindow()
{
    if (m_Delegate.m_IsCleared)
        return;

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

   m_Delegate.m_IsCleared = true;
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
    if (!m_Open)
        return;

    CObjectComponentWindow* ObjCompWindow = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);
    CObjectHierarchyWindow* ObjWindow = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);


    if (ObjWindow && ObjCompWindow)
    {
        CGameObject* SelectObj = ObjWindow->GetSelectObject();
        CStateComponent* State = SelectObj->FindComponentFromType<CStateComponent>();

        if (State)
        {
            // 만약 Object가 Load되고 처음 Behavior Tree를 열었다면 State Component가 갖고 있는 Behavior Tree안에 노드들이 Graph Editor에 하나도 나오지 않을테니
            // Behavior Tree안에 노드들을 Graph Editor에 출력되게 해야한다
            if (State->GetBehaviorTree()->GetRootNode() && m_Delegate.GetNodeCount() == 0)
            {
                UpdateLoadNode((CCompositeNode*)(State->GetBehaviorTree()->GetRootNode()));
            }
        }
    }


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
    if (ImGui::BeginCombo("", m_PrevViewName.c_str(), m_ComboFlag))
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
        m_vecNodeAction.clear();

        if (m_TypeSelectIndex == 2)
        {
            m_vecNodeAction.push_back("SkillQ");
            m_vecNodeAction.push_back("SkillW");
            m_vecNodeAction.push_back("SkillE");
            m_vecNodeAction.push_back("SkillR");
            m_vecNodeAction.push_back("SpellD");
            m_vecNodeAction.push_back("SpellF");
            m_vecNodeAction.push_back("Move");
            m_vecNodeAction.push_back("MovePicking");
            m_vecNodeAction.push_back("Idle");
        }

        else if (m_TypeSelectIndex == 3)
        {
            m_vecNodeAction.push_back("InputQCheck");
            m_vecNodeAction.push_back("InputWCheck");
            m_vecNodeAction.push_back("InputECheck");
            m_vecNodeAction.push_back("InputRCheck");
            m_vecNodeAction.push_back("InputDCheck");
            m_vecNodeAction.push_back("InputFCheck");
            m_vecNodeAction.push_back("MouseRightInputCheck");
            m_vecNodeAction.push_back("NoInterruptCheck");
        }

        ImGui::PushID(m_WidgetID + 1);
        ImGui::PushItemWidth(150.f);

        if (ImGui::BeginCombo("", m_LeafNodePrevViewName.c_str(), m_ComboFlag))
        {
            size_t  ItemCount = m_vecNodeAction.size();

            for (size_t i = 0; i < ItemCount; ++i)
            {
                m_Select = m_ActionSelectIndex == i;

                if (ImGui::Selectable(m_vecNodeAction[i].c_str(), m_Select))
                {
                    //if (m_SelectIndex != i && m_SelectCallback)
                    //    m_SelectCallback((int)i, m_vecItem[i].c_str());

                    m_LeafNodePrevViewName = m_vecNodeAction[i];

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
    char NameInput[256] = "";

    Input = ImGui::InputTextWithHint("##", HintText, m_Text, 1024, m_TextInputFlag);

    std::string NameInputStr = m_Text;

    if (Input)
    {
        memset(m_wText, 0, sizeof(wchar_t) * 1024);
        memset(m_TextUTF8, 0, sizeof(char) * 1024);

        int	Length = MultiByteToWideChar(CP_ACP, 0, m_Text, -1, 0, 0);
        MultiByteToWideChar(CP_ACP, 0, m_Text, -1, m_wText, Length);

        // UTF8로 변환한다.
        Length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, Length, 0, 0);

        if (NameInputStr.empty())
        {
            m_InputNodeName = m_TextUTF8;
        }
    }

    ImGui::PopID();

    ImGui::SameLine();

    // 노드 추가 버튼
    if (ImGui::Button("Add Node"))
    {
        OnAddNodeButton(m_Text, m_TypeSelectIndex, m_ActionSelectIndex);
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete Node"))
    {
        GraphEditorDelegate::Node node;
        bool Result = m_Delegate.GetSelectedDelegateNode(node);
        if (Result)
        {
            OnDeleteNodeButton(node.name);
        }
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

    ImGui::SameLine();

    if (ImGui::Button("Close"))
    {
        m_Open = false;
        ImGui::End();
        return;
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

void CBehaviorTreeWindow::OnAddNodeButton(const char* Name, int TypeIndex, int ActionIndex)
{
    GraphEditor::TemplateIndex idx = TypeIndex;

    char NodeName[256] = {};
    strcpy_s(NodeName, Name);

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
        enum ActionNode NodeActionClass;
        NodeActionClass = static_cast<ActionNode>(ActionIndex);

        switch (NodeActionClass)
        {
        case ActionNode::SkillQ:
            NewTreeNode = m_StateComponent->CreateTreeNode<CSkillQNode>(Name);
            break;
        case ActionNode::SkillW:
            NewTreeNode = m_StateComponent->CreateTreeNode<CSkillWNode>(Name);
            break;
        case ActionNode::SkillE:
            NewTreeNode = m_StateComponent->CreateTreeNode<CSkillENode>(Name);
            break;
        case ActionNode::SkillR:
            NewTreeNode = m_StateComponent->CreateTreeNode<CSkillRNode>(Name);
            break;
        case ActionNode::SpellD:
            break;
        case ActionNode::SpellF:
            break;
        case ActionNode::Move:
            break;
        case ActionNode::MovePicking:
            NewTreeNode = m_StateComponent->CreateTreeNode<CMovePickingNode>(Name);
            break;
        case ActionNode::Idle:
            NewTreeNode = m_StateComponent->CreateTreeNode<CIdleNode>(Name);
            break;
        }

        break;
    }

    case 3:
    {
        enum ConditionNode NodeConditionlass;
        NodeConditionlass = static_cast<ConditionNode>(ActionIndex);

        switch (NodeConditionlass)
        {
        case ConditionNode::InputQCheck:
            NewTreeNode = m_StateComponent->CreateTreeNode<CInputQCheckNode>(Name);
            break;
        case ConditionNode::InputWCheck:
            NewTreeNode = m_StateComponent->CreateTreeNode<CInputWCheckNode>(Name);
            break;
        case ConditionNode::InputECheck:
            NewTreeNode = m_StateComponent->CreateTreeNode<CInputECheckNode>(Name);
            break;
        case ConditionNode::InputRCheck:
            NewTreeNode = m_StateComponent->CreateTreeNode<CInputRCheckNode>(Name);
            break;
        case ConditionNode::InputDCheck:
            break;
        case ConditionNode::InputFCheck:
            break;
        case ConditionNode::MoveInputCheckNode:
            NewTreeNode = m_StateComponent->CreateTreeNode<CMoveInputCheckNode>(Name);
            break;
        case ConditionNode::NoInterruptNode:
            NewTreeNode = m_StateComponent->CreateTreeNode<CNoInterruptNode>(Name);
            break;
        }

    }

    }

    NewTreeNode->SetOwner(m_StateComponent->GetBehaviorTree());
    NewTreeNode->SetObject(m_StateComponent->GetGameObject());

    m_Delegate.AddNode(NodeName, idx, 0.f, 0.f, false, NewTreeNode);

    if (!m_StateComponent->GetBehaviorTree()->GetRootNode())
    {
        m_StateComponent->GetBehaviorTree()->SetRoot(NewTreeNode);
    }

    m_PrevViewName = "Select Node Type";
    m_LeafNodePrevViewName = "Select Node Action";
}

void CBehaviorTreeWindow::OnDeleteNodeButton(const char* Name)
{
    CBehaviorTree* Tree = m_StateComponent->GetBehaviorTree();

    if (Name == Tree->GetRootNode()->GetName())
    {
        Tree->SetRoot(nullptr);
    }

	CNode* Node = Tree->FindNode(Name);

	GraphEditorDelegate::Node node = m_Delegate.FindNode(Node->GetName());

	int NodeIdx = node.NodeIndex;

	// GUI상에서도 Node랑 (연결된 Link가 있다면)Link 지우기
	m_Delegate.DelAllLink(NodeIdx);
	m_Delegate.DeleteNode(NodeIdx);

	// Node가 지워지면 Node의 NodeIndex랑 Link에 적힌 Input/Output Node Index도 바뀌어야한다
	size_t NodeCount = m_Delegate.mNodes.size();

	for (int i = 0; i < NodeCount; ++i)
	{
		if (m_Delegate.mNodes[i].NodeIndex > NodeIdx)
			--m_Delegate.mNodes[i].NodeIndex;
	}


	size_t Count = m_Delegate.mLinks.size();
	for (int i = 0; i < Count; ++i)
	{
		if (m_Delegate.mLinks[i].mInputNodeIndex > NodeIdx)
			--m_Delegate.mLinks[i].mInputNodeIndex;

		if (m_Delegate.mLinks[i].mOutputNodeIndex > NodeIdx)
			--m_Delegate.mLinks[i].mOutputNodeIndex;
	}

	// Engine상에서도 지우기
	if (Node)
	{
		Tree->DeleteNode(Node);
	}

}


void CBehaviorTreeWindow::UpdateLoadNode(CCompositeNode* RootNode)
{
    int TypeIndex = -1;

    if (RootNode->GetTypeID() == typeid(CSequenceNode).hash_code())
    {
        TypeIndex = 0;
    }

    else if (RootNode->GetTypeID() == typeid(CSelectorNode).hash_code())
    {
        TypeIndex = 1;
    }

    else if (RootNode->GetNodeType() == Node_Type::Action)
    {
        TypeIndex = 2;
    }

    else if (RootNode->GetNodeType() == Node_Type::Condition)
    {
        TypeIndex = 3;
    }

    m_Delegate.AddNode(RootNode->GetName(), TypeIndex, 0.f, 0.f, false, RootNode);
    RootNode->SetOwner(m_StateComponent->GetBehaviorTree());
    RootNode->SetObject(m_StateComponent->GetGameObject());

    const std::list<CNode*> NodeList = RootNode->GetChildrenList();
    auto iter = NodeList.begin();
    auto iterEnd = NodeList.end();

    int Idx = 0;
    for (; iter != iterEnd; ++iter, ++Idx)
    {
        (*iter)->SetOwner(m_StateComponent->GetBehaviorTree());
        (*iter)->SetObject(m_StateComponent->GetGameObject());
        UpdateLoadNodeRecursive(*iter, 1, Idx);
    }

    UpdateLoadNodeLink(RootNode->GetOwner());
}

void CBehaviorTreeWindow::UpdateLoadNodeRecursive(CNode* Node, int Depth, int Height)
{
    int TypeIndex = -1;
    std::list<CNode*> NodeList;

    if (Node->GetTypeID() == typeid(CSequenceNode).hash_code())
    {
        TypeIndex = 0;
        NodeList = ((CSequenceNode*)Node)->GetChildrenList();

        auto iter = NodeList.begin();
        auto iterEnd = NodeList.end();

        int Idx = 0;
        for (; iter != iterEnd; ++iter, ++Idx)
        {
            UpdateLoadNodeRecursive(*iter, Depth + 1, Idx);
        }

        m_Delegate.AddNode(Node->GetName(), TypeIndex, Depth * 100.f, Height * 100.f, false, Node);
    }

    else if (Node->GetTypeID() == typeid(CSelectorNode).hash_code())
    {
        TypeIndex = 1;
        NodeList = ((CSelectorNode*)Node)->GetChildrenList();

        auto iter = NodeList.begin();
        auto iterEnd = NodeList.end();

        int Idx = 0;
        for (; iter != iterEnd; ++iter, ++Idx)
        {
            UpdateLoadNodeRecursive(*iter, Depth + 1, Idx);
        }

        m_Delegate.AddNode(Node->GetName(), TypeIndex, Depth * 100.f, Height * 100.f, false, Node);
    }

    else if (Node->GetNodeType() == Node_Type::Action)
    {
        TypeIndex = 2;
        m_Delegate.AddNode(Node->GetName(), TypeIndex, Depth * 100.f, Height * 100.f, false, Node);
    }

    else if (Node->GetNodeType() == Node_Type::Condition)
    {
        TypeIndex = 3;
        m_Delegate.AddNode(Node->GetName(), TypeIndex, Depth * 100.f, Height * 100.f, false, Node);
    }
}

void CBehaviorTreeWindow::UpdateLoadNodeLink(CBehaviorTree* Tree)
{
    size_t Count = Tree->GetNodeCount();
    size_t DelegateNodeCount = m_Delegate.GetNodeCount();
    size_t LinkCount = m_Delegate.mLinks.size();

    for (size_t i = 0; i < Count; ++i)
    {
        CNode* Node = Tree->GetNode(i);

        if (Node->GetTypeID() == typeid(CSelectorNode).hash_code() || Node->GetTypeID() == typeid(CSequenceNode).hash_code())
        {

            for (size_t j = 0; j < DelegateNodeCount; ++j)
            {
                if (m_Delegate.GetDelegateNode(j).BehaviorTreeNode == Node)
                {
                    GraphEditorDelegate::Node SrcNode = m_Delegate.GetDelegateNode(j);

                    const std::list<CNode*> ChildrenList = ((CCompositeNode*)Node)->GetChildrenList();

                    auto iter = ChildrenList.begin();
                    auto iterEnd = ChildrenList.end();

                    int ChildIdx = 0;
                    for (; iter != iterEnd; ++iter, ++ChildIdx)
                    {
                        GraphEditorDelegate::Node DestNode = m_Delegate.FindNode((*iter)->GetName());
                        m_Delegate.LoadLink(SrcNode.NodeIndex, ChildIdx, DestNode.NodeIndex, 0);
                    }
                }
            }
        }

    }
}

void CBehaviorTreeWindow::OnAddLink(CNode* ParentNode, CNode* ChildNode)
{
    int a = 3;
}
