
#include "FileBrowser.h"
#include "PathManager.h"
#include "IMGUIText.h"
#include "IMGUIImage.h"
#include "IMGUISameline.h"
#include "../EditorUtil.h"
#include "../EditorInfo.h"

#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

CFileBrowser::CFileBrowser()	:
	m_UpdatePath(true)
{
	m_InitialPath = ROOT_PATH;
}

CFileBrowser::~CFileBrowser()
{
	//ClearWidget();
}

bool CFileBrowser::Init()
{
	CIMGUIWindow::Init();

	return true;
}

void CFileBrowser::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	if (m_UpdatePath)
	{
		ClearWidget();

		CEditorUtil::GetAllFilenames(m_InitialPath, m_vecFileName, m_vecDirName);

		size_t Count = m_vecDirName.size();

		ImVec2 Size = ImGui::GetWindowSize();
		
		int TotalCount = 0;

		for (size_t i = 0; i < Count; ++i, ++TotalCount)
		{
			std::string Name = m_vecDirName[i] + "ICon";
			CIMGUIChild* Child = AddWidget<CIMGUIChild>(Name);

			CIMGUIImage* DirImage = Child->AddWidget<CIMGUIImage>("DirectoryImage", 80.f, 80.f);
			DirImage->SetTexture("DirectoryImage", TEXT("Directory.png"));
			DirImage->SetClickCallback<CFileBrowser>(this, &CFileBrowser::FileClickCallback);

			CIMGUIText* DirName = Child->AddWidget<CIMGUIText>("DirName");
			DirName->SetText(m_vecDirName[i].c_str());

			if (TotalCount < (int)SINGLELINE_NUMICON - 1)
				Child->SetSameLine(true);
			else
				TotalCount = -1;

			m_vecChild.push_back(Child);
		}

		Count = m_vecFileName.size();

		for (size_t i = 0; i < Count; ++i, ++TotalCount)
		{
			std::string Name = m_vecFileName[i] + "ICon";
			CIMGUIChild* Child = AddWidget<CIMGUIChild>(Name);

			CIMGUIImage* FileImage = Child->AddWidget<CIMGUIImage>("FileImage", 80.f, 80.f);
			FileImage->SetTexture("FileImage", TEXT("FileImage.png"));

			CIMGUIText* FileName = Child->AddWidget<CIMGUIText>("FileName");
			FileName->SetText(m_vecFileName[i].c_str());

			if (TotalCount < (int)SINGLELINE_NUMICON - 1)
				Child->SetSameLine(true);
			else
				TotalCount = -1;

			m_vecChild.push_back(Child);
		}

		m_UpdatePath = false;
	}

	//m_vecWidget.clear();
}

void CFileBrowser::SetInitialPath(const std::string& Name)
{
	m_InitialPath = Name;
}

//std::string CFileBrowser::FilterFileName(const std::string& FullPath)
//{
//	size_t len = FullPath.length();
//
//	std::string FileName;
//
//	for (size_t i = len - 1; i >= 0; --i)
//	{
//		if (FullPath[i] == '\\' || FullPath[i] == '/')
//		{
//			FileName = FullPath.substr(i + 1);
//			return FileName;
//		}
//	}
//
//	return FileName;
//}

void CFileBrowser::ClearWidget()
{
	size_t size = m_vecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
}

void CFileBrowser::FileClickCallback(CIMGUIImage* Image)
{

}
