#pragma once

#include "../../GameInfo.h"
#include "Material.h"

class CMaterialManager
{
	friend class CResourceManager;

private:
	CMaterialManager();
	~CMaterialManager();

private:
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;
	class CMaterialConstantBuffer* m_CBuffer;
public :
	const std::unordered_map<std::string, CSharedPtr<CMaterial>>& GetMapMaterial() const
	{
		return m_mapMaterial;
	}
public:
	bool Init();
	CMaterial* FindMaterial(const std::string& Name);
	CMaterialConstantBuffer* GetMaterialConstantBuffer()	const
	{
		return m_CBuffer;
	}
	void ReleaseMaterial(const std::string& Name);
	CMaterial* LoadMaterialFullPathMultibyte(const char* FullPath);
	void AddMaterial(CMaterial* Materal);
public:
	template <typename T>
	T* CreateMaterial(const std::string& Name)
	{
		T* Material = (T*)FindMaterial(Name);

		if (Material)
			return Material;

		Material = new T;

		Material->SetConstantBuffer(m_CBuffer);

		Material->SetName(Name);

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return Material;
	}

	template <typename T>
	T* CreateMaterialEmpty()
	{
		T* Material = new T;

		Material->SetConstantBuffer(m_CBuffer);

		return Material;
	}
};

