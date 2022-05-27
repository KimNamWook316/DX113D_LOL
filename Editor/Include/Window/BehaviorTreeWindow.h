#pragma once

#include "IMGUIWindow.h"
#include "imgui_internal.h"
#include "imgui.h"
#include "../GraphEditor.h"
#include "Component/Node/Node.h"

static ImVec2 operator+(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x + b.x, a.y + b.y);
}

static ImVec2 operator-(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x - b.x, a.y - b.y);
}

static ImVec2 operator*(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x * b.x, a.y * b.y);
}

static ImVec2 operator/(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x / b.x, a.y / b.y);
}

static ImVec2 operator/(const ImVec2& a, float b)
{
    return ImVec2(a.x / b, a.y / b);
}


static ImVec2 operator*(const ImVec2& a, const float b)
{
    return ImVec2(a.x * b, a.y * b);
}


template <typename T, std::size_t N>
struct Array
{
    T data[N];
    const size_t size() const { return N; }

    const T operator [] (size_t index) const { return data[index]; }
    operator T* () {
        T* p = new T[N];
        memcpy(p, data, sizeof(data));
        return p;
    }
};


template <typename T, typename ... U> Array(T, U...)->Array<T, 1 + sizeof...(U)>;

struct GraphEditorDelegate : public GraphEditor::Delegate
{
    struct Node
    {
        const char* name;
        GraphEditor::TemplateIndex templateIndex;
        float x, y;
        bool mSelected;
        CNode* BehaviorTreeNode;
    };


    bool AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) override
    {
        return true;
    }

    void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override
    {
        mNodes[nodeIndex].mSelected = selected;
    }

    void MoveSelectedNodes(const ImVec2 delta) override
    {
        for (auto& node : mNodes)
        {
            if (!node.mSelected)
            {
                continue;
            }
            node.x += delta.x;
            node.y += delta.y;
        }
    }

    virtual void RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) override
    {
    }

    void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override
    {
        mLinks.push_back({ inputNodeIndex, inputSlotIndex, outputNodeIndex, outputSlotIndex });
    }

    void DelLink(GraphEditor::LinkIndex linkIndex) override
    {
        mLinks.erase(mLinks.begin() + linkIndex);
    }

    void CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex) override
    {
        drawList->AddLine(rectangle.Min, rectangle.Max, IM_COL32(0, 0, 0, 255));
        drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), "Draw");
    }

    void CustomDraw(ImDrawList* drawList, ImRect rectangle, GraphEditor::NodeIndex nodeIndex, const std::string& Name) override
    {
        drawList->AddLine(rectangle.Min, rectangle.Max, IM_COL32(0, 0, 0, 255));
        drawList->AddText(rectangle.Min, IM_COL32(255, 128, 64, 255), Name.c_str());
    }

    const size_t GetTemplateCount() override
    {
        return sizeof(mTemplates) / sizeof(GraphEditor::Template);
    }

    const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override
    {
        return mTemplates[index];
    }

    const size_t GetNodeCount() override
    {
        return mNodes.size();
    }

    const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override
    {
        const auto& myNode = mNodes[index];
        return GraphEditor::Node
        {
            myNode.name,
            myNode.templateIndex,
            ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + 100, myNode.y + 100)), // 노드별 사이즈
            myNode.mSelected
        };
    }

    const size_t GetLinkCount() override
    {
        return mLinks.size();
    }

    const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override
    {
        return mLinks[index];
    }

    void AddNode(const char* Name, GraphEditor::TemplateIndex TemplateIndex, float PosX, float PosY, bool mSelected, CNode* TreeNode)
    {
        Node NewNode;

        NewNode.name = Name;
        NewNode.templateIndex = TemplateIndex;
        NewNode.x = PosX;
        NewNode.y = PosY;
        NewNode.mSelected = mSelected;
        NewNode.BehaviorTreeNode = TreeNode;

        mNodes.push_back(NewNode);
    }

    Node& FindNode(const std::string& Name)
    {
        size_t Count = mNodes.size();

        for (size_t i = 0; i < Count; ++i)
        {
            if (mNodes[i].name == Name)
                return mNodes[i];
        }
    }

    // Graph datas
    static const inline GraphEditor::Template mTemplates[] =
    {

        // Sequence Node Template
        {
            IM_COL32(140, 140, 160, 255),
            IM_COL32(140, 100, 140, 255),
            IM_COL32(110, 110, 150, 255),
            1,
            Array{"Parent"},
            nullptr,
            2,
            Array{"Child0", "Child1"},
            nullptr
        },

        // Selector Node Template
        {
            IM_COL32(100, 140, 190, 255),
            IM_COL32(100, 100, 180, 255),
            IM_COL32(110, 110, 150, 255),
            1,
            Array{"Parent"},
            nullptr,
            2,
            Array{"Child0", "Child1"},
            nullptr
        },

        // Action Node Template
        {
            IM_COL32(100, 160, 100, 255),
            IM_COL32(100, 100, 140, 255),
            IM_COL32(110, 110, 150, 255),
            1,
            Array{"Parent"},
            nullptr,
            0,
            nullptr,
            nullptr
        },

        // Condition Node Templte
        {
            IM_COL32(180, 160, 160, 255),
            IM_COL32(140, 100, 100, 255),
            IM_COL32(150, 110, 110, 255),
            1,
            Array{"Parent"},
            nullptr,
            0,
            nullptr,
            nullptr,
        }
    };

    std::vector<Node> mNodes = 
    {
        //{
        //    "Sequence",
        //    0,
        //    50, 50,
        //    false
        //},

        //{
        //    "Selector",
        //    1,
        //    50, 50,
        //    false
        //},

        //{
        //    "Action",
        //    2,
        //    50, 50,
        //    false
        //},

        //{
        //    "Condition",
        //    3,
        //    50, 50,
        //    false
        //},
        //{
        //    "My Node 1",
        //    0,
        //    400, 100,
        //    false
        //},

        //{
        //    "My Node 2",
        //    1,
        //    400, 400,
        //    false
        //}
    };

    std::vector<GraphEditor::Link> mLinks = { };

};

class CBehaviorTreeWindow :
    public CIMGUIWindow
{
public:
	CBehaviorTreeWindow();
	virtual ~CBehaviorTreeWindow();

private:
    GraphEditor::Options m_Option;
    GraphEditor::ViewState m_ViewState;
    GraphEditorDelegate m_Delegate;
    ImGuiComboFlags m_ComboFlag;
    bool m_Select;
    int m_TypeSelectIndex;
    int m_ActionSelectIndex;
    std::vector<std::string>	m_vecNodeType;
    std::vector<std::string>	m_vecNodeAction;
    std::string m_PrevViewName;

    class CStateComponent* m_StateComponent;

    char		m_Text[1024];
    wchar_t		m_wText[1024];
    char		m_TextUTF8[1024];

    char		m_HintText[1024];
    wchar_t		m_wHintText[1024];
    char		m_HintTextUTF8[1024];
    ImGuiInputTextFlags	m_TextInputFlag;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
    void SetStateComponent(class CStateComponent* Com);
    class CStateComponent* GetStateComponent() const;

private:
    void OnAddNodeButton(const char* Name, int TypeIndex, int ActionIndex, class CNode* Node);

    void SetHintText(const char* Text)
    {
        memset(m_wHintText, 0, sizeof(wchar_t) * 1024);
        memset(m_HintText, 0, sizeof(char) * 1024);
        memset(m_HintTextUTF8, 0, sizeof(char) * 1024);
        int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
        MultiByteToWideChar(CP_ACP, 0, Text, -1, m_wHintText, Length);

        strcpy_s(m_HintText, Text);

        // UTF8로 변환한다.
        Length = WideCharToMultiByte(CP_UTF8, 0, m_wHintText, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, m_wHintText, -1, m_HintTextUTF8, Length, 0, 0);
    }
};

