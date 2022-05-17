#pragma once

#include "IMGUIWindow.h"
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
	std::string					m_InitialPath;


	void SetUpdatePath(bool Update)
	{
		m_UpdatePath = Update;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void SetInitialPath(const std::string& Path);
	void ClearWidget();
	// 파일(또는 디렉토리) 이미지를 클릭하면 호출되는 콜백
	void FileClickCallback(class CIMGUIImage* Image);
};
