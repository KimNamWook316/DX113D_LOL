#pragma once

#include "IMGUIWidget.h"

// MainMenuBar 클래스의 하위 메뉴, 툴바에서 바로 보이는 메뉴들을 생성하는 위젯
// MenuItem을 하위 위젯으로 가지고 있음

class CIMGUIBeginMenu :
    public CIMGUIWidget
{
public:
    CIMGUIBeginMenu();
    virtual ~CIMGUIBeginMenu();

public:
    virtual bool Init() override;
    virtual void Render() override;

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
        m_vecChild.push_back(widget);
        return widget;
    }

protected:
    std::vector<class CIMGUIWidget*> m_vecChild;
    size_t mSize;
};

