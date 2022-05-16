#pragma once

#include "IMGUIWidget.h"

class CIMGUIImage :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUIWidgetList;
	friend class CIMGUIChild;

protected:
	CIMGUIImage();
	virtual ~CIMGUIImage();

protected:
	CSharedPtr<class CTexture>	m_Texture;
	ImVec2			m_ImageStart;
	ImVec2			m_ImageEnd;
	ImVec4			m_Tint;
	ImVec4			m_BorderColor;
	int				m_ClickStack;
	float			m_AccTime;
	class CTimer*	m_Timer;

	std::function<void(CIMGUIImage* Image)> m_ClickCallback;
	std::function<void(CIMGUIImage* Image)> m_DoubleClickCallback;

public:
	void SetImageStart(float x, float y)
	{
		m_ImageStart = ImVec2(x, y);
	}

	void SetImageEnd(float x, float y)
	{
		m_ImageEnd = ImVec2(x, y);
	}

	void SetTint(unsigned char r, unsigned char g, unsigned char b)
	{
		m_Tint = ImVec4(r / 255.f, g / 255.f, b / 255.f, 1.f);
	}

	void SetBorderColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_BorderColor = ImVec4(r / 255.f, g / 255.f, b / 255.f, 1.f);
	}

public:
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(class CTexture* Texture);

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetClickCallback(T* Obj, void (T::* Func)(T* Obj))
	{
		m_ClickCallback = std::bind(Func, Obj);
	}

	template <typename T>
	void SetDoubleClickCallback(T* Obj, void(T::* Func)(T* Obj))
	{
		m_DoubleClickCallback = std::bind(Func, Obj, std::placeholders_1);
	}
};

