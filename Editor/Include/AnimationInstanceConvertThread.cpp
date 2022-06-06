#include "AnimationInstanceConvertThread.h"
#include "Resource/Mesh/AnimationMesh.h"
#include "Resource/ResourceManager.h"

CAnimationInstanceConvertThread::CAnimationInstanceConvertThread()
{
	SetLoop(true);

	m_ExitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CAnimationInstanceConvertThread::~CAnimationInstanceConvertThread()
{
	m_Loop = false;

	WaitForSingleObject(m_ExitEvent, INFINITE);

	CloseHandle(m_ExitEvent);
}

bool CAnimationInstanceConvertThread::Init()
{
	if (!CThread::Init())
	{
		assert(false);
		return false;
	}

	return true;
}

void CAnimationInstanceConvertThread::Run()
{
	while (m_Loop)
	{
		if (!m_WorkQueue.empty())
		{
			// FBX -> msh, bne, sqc파일로 변환한다.
			// bool result = CResourceManager::GetInst()->ConvertFBXLocalFormatFullPathMultiByte(m_WorkQueue.front().FBXFullPath.c_str());
			// 
			// if (!result)
			// {
			// 	// 실패한경우 콜백
			// 	if (m_FailCallBack)
			// 	{
			// 		m_StartSize = 0;
			// 		m_FailCallBack(m_WorkQueue.front().AnimSequenceFullPath;
			// 		);
			// 	}
			// 	m_WorkQueue.empty();
			// 	continue;
			// }
			// 
			// LoadingMessage msg;
			// msg.Percent = 1.f - (float)((float)(m_WorkQueue.size() - 1) / (float)m_StartSize);
			// msg.Complete = m_WorkQueue.size() == 1 ? true : false;
			// msg.Message = m_WorkQueue.front().AnimSequenceFullPath;
			// 
			// m_WorkQueue.pop();
			// 
			// if (m_LoadingCallBack)
			// {
			// 	if (msg.Complete)
			// 	{
			// 		m_StartSize = 0;
			// 	}
			// 
			// 	m_LoadingCallBack(msg);
			// }
		}
	}

	SetEvent(m_ExitEvent);
}

void CAnimationInstanceConvertThread::AddWork(const std::string& AnimationSequenceFullPath)
{
	AnimInstanceWorkData data;
	data.AnimSequenceFullPath = AnimationSequenceFullPath;

	char CopyAnimSequenceFullPath[MAX_PATH] = {};
	strcpy_s(CopyAnimSequenceFullPath, AnimationSequenceFullPath.c_str());

	// _splitpath_s();
	// 
	// data.FBXFullPath = FBXFullPath;
	// m_WorkQueue.push(data);

	++m_StartSize;
}
