#pragma once
#include "IMGUIWidget.h"

class CIMGUIImageButton :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUIWidgetList;
	friend class CIMGUIChild;

protected:
	CIMGUIImageButton();
	virtual ~CIMGUIImageButton();

protected:
	CSharedPtr<class CTexture>	m_Texture;
	ImVec4			m_BackgroundColor;
	ImVec4			m_TintColor;
	ImVec2			m_Size;
	int				m_PressCount;

	std::function<void(CIMGUIImageButton*)> m_DoubleClickCallback;

	void SetBackgroundColor(const ImVec4& Color)
	{
		m_BackgroundColor = Color;
	}

	void SetTintColor(const ImVec4& Color)
	{
		m_TintColor = Color;
	}

	void SetSize(const ImVec2& Size)
	{
		m_Size = Size;
	}

	int GetPressCount()	const
	{
		return m_PressCount;
	}

	bool SetTexture(class CTexture* Texture);
	bool SetTexture(TCHAR* FileName, const std::string& PathName);
	bool SetTextureFullPath(const TCHAR* FullPath);

public:
	virtual bool Init();
	virtual void Render();
	
	template <typename T>
	void SetImageButtonDoubleClickCallback(T* Obj, void(T::*Func)(T* Obj))
	{
		m_DoubleClickCallback = std::bind(Func, Obj, std::placeholders::_1);
	}

};

