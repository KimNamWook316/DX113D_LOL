#pragma once
#pragma once

#include "GameInfo.h"

class CObjectPool
{
private:
	std::unordered_multimap <std::string, class CGameObject*> m_mapProjectile;
	std::unordered_multimap <std::string, class CGameObject*> m_mapMonster;
	std::unordered_multimap <std::string, class CGameObject*> m_mapParticle;

	std::string m_DataName;

public:
	virtual void Init();

	class CGameObject* GetProjectile(const std::string& Name);
	class CGameObject* GetMonster(const std::string& Name);
	class CGameObject* GetParticle(const std::string& Name);

	void ReturnToPool(class CGameObject* Object);
	void LoadPoolData(const char* FileName);
	void CreatePoolObject(const std::string& PathName);

	DECLARE_SINGLE(CObjectPool)
};


