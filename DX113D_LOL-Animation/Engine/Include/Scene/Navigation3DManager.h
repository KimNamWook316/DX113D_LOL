#pragma once

#include "../GameInfo.h"
#include "../Component/LandScape.h"
//#include "../ThreadQueue.h"
//#include "NavigationThread.h"

class CNavigation3DManager
{
	friend class CScene;

private:
	CNavigation3DManager();
	~CNavigation3DManager();

private:
	class CScene* m_Scene;
	//std::vector<CNavigationThread*>	m_vecNavigationThread;
	CSharedPtr<CLandScape>				m_NavData;
	//CThreadQueue<NavResultData>			m_ResultQueue;

public:
	void SetNavData(CLandScape* NavData);
	void AddNavResult(const NavResultData& NavData);

public:
	float GetY(const Vector3& Pos);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
};


