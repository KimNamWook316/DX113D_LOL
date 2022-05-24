#pragma once

#include "IMGUIWidget.h"

class CIMGUITree :
    public CIMGUIWidget
{
public:
    CIMGUITree();
    virtual ~CIMGUITree();

public:
    void SetParent(CIMGUITree* parent)
    {
        mParent = parent;
    }

    bool IsRootNode()
    {
        return mParent == nullptr;
    }

    bool IsEndNode()
    {
        return mVecChild.size() == 0;
    }

    CIMGUITree* GetParent() const
    {
        return mParent;
    }

public:
    CIMGUITree* GetNode(const int idx);

public:
    CIMGUITree* FindChild(const std::string& Name);
    CIMGUITree* AddChild(const std::string& name, const float width = 100.f, const float height = 100.f);
    CIMGUITree* AddChild(CIMGUITree* Tree, const float width = 100.f, const float height = 100.f);
    CIMGUITree* AddNewChild(CIMGUITree* Tree);
    void DeleteChild(const std::string& Name);
    CIMGUITree* DeleteHierarchy(); // 자식 Tree 구조 전부 삭제
    CIMGUITree* Delete();
    size_t GetChildCount() const;
    CIMGUITree* GetRoot();

    // 자신부터 시작해서 아래로 내려가면서 m_Selected = false로 해주는 함수
    void DeselectAll();


public:
    void SetFlag(ImGuiTreeNodeFlags Flag)
    {
        m_Flag = Flag;
    }

    void AddFlag(ImGuiTreeNodeFlags Flag)
    {
        m_Flag |= Flag;
    }

    void SetSelected(bool Selected)
    {
        m_Selected = Selected;
    }

    bool IsSelected() const
    {
        return m_Selected;
    }


public:
    template <typename T>
    T* AddWidget(const std::string& name, const float width = 100.f, const float height = 100.f)
    {
		T* widget = new T;

		widget->SetName(name);
		widget->SetOwner(m_Owner);
		widget->SetSize(width, height);

		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			assert(false);
			return nullptr;
		}
		mVecWidget.push_back(widget);
        mVecRender.push_back(widget);
        return widget;
    }


public:
    virtual bool Init();
    virtual void Render();

protected:
    CIMGUITree* mParent;
    std::vector<CIMGUITree*> mVecChild;
    std::vector<CIMGUIWidget*> mVecWidget;
    std::vector<CIMGUIWidget*> mVecRender; 

    int m_GlobalID;
    bool m_Selected;
    bool m_Open;
    // false면 Window에서 출력 안함
    bool m_Enable;
    ImGuiTreeNodeFlags  m_Flag;
    ImGuiTreeNodeFlags  m_DefaultFlag;

    std::function<void(class CIMGUITree*)> m_DragDropSourceCallback;
    std::function<void(class CIMGUITree*, const std::string&, const std::string&)> m_DragDropDestCallback;

    std::list<std::function<void(class CIMGUITree*)>> m_SelectCallbackList;
    std::list<std::function<void(class CIMGUITree*)>> m_OpenCallbackList;

public:
    bool IsOpen()   const
    {
        return m_Open;
    }

    void SetEnable(bool Enable)
    {
        m_Enable = Enable;
    }

    // 자신을 포함해서 밑에 자식까지 전부 m_Enable을 true
    void EnableAll()
    {
        m_Enable = true;

        size_t Count = mVecChild.size();

        for (size_t i = 0; i < Count; ++i)
        {
            mVecChild[i]->EnableAll();
        }
    }

    // 자신을 포함해서 밑에 자식까지 전부 m_Enable을 false
    void DisableAll()
    {
        m_Enable = false;

        size_t Count = mVecChild.size();

        for (size_t i = 0; i < Count; ++i)
        {
            mVecChild[i]->DisableAll();
        }
    }

    bool IsEnable() const
    {
        return m_Enable;
    }

public:
    template <typename T>
    void SetDragDropSourceCallback(T* Obj, void(T::* Func)(class CIMGUITree*))
    {
        m_DragDropSourceCallback = std::bind(Func, Obj, std::placeholders::_1);
    }

    template <typename T>
    void SetDragDropDestCallback(T* Obj, void(T::*Func)(class CIMGUITree*, const std::string&, const std::string&))
    {
        m_DragDropDestCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }

    template <typename T>
    void AddSelectCallback(T* Obj, void(T::* Func)(class CIMGUITree*))
    {
        m_SelectCallbackList.push_back(std::bind(Func, Obj, std::placeholders::_1));
    }

    template <typename T>
    void AddOpenCallback(T* Obj, void(T::* Func)(class CIMGUITree*))
    {
        m_OpenCallbackList.push_back(std::bind(Func, Obj, std::placeholders::_1));
    }
};

