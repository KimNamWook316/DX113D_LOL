#pragma once

#include "IMGUIWindow.h"
#include "IMGUIMenuItem.h"
#include "IMGUIChild.h"

class CFileBrowser :
	public CIMGUIWindow
{
public:
	CFileBrowser();
	virtual ~CFileBrowser();

private:
	std::vector<std::string>	m_vecFileName;
	std::vector<std::string>	m_vecDirName;
	std::vector<CIMGUIChild*>	m_vecChild;

	bool						m_UpdatePath;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	std::string m_InitialPath;

public:
	void SetInitialPath(const std::string& Name);
	void ClearWidget();
	// 파일(또는 디렉토리) 이미지를 클릭하면 호출되는 콜백
	void FileClickCallback(class CIMGUIImage* Image);

};
