#pragma once

#include "AnimationSequenceData.h"
#include "../Resource/Animation/AnimationSequence.h"

class CAnimationSequenceInstance
{
	friend class CAnimationEditor;
public:
	CAnimationSequenceInstance();
	CAnimationSequenceInstance(const CAnimationSequenceInstance& Anim);
	virtual ~CAnimationSequenceInstance();

protected:
	size_t		m_TypeID;
	class CAnimationMeshComponent* m_Owner;
	std::string m_AnimInstanceName;
	class CScene* m_Scene;
	std::unordered_map<std::string, CAnimationSequenceData*>	m_mapAnimation;
	CAnimationSequenceData* m_CurrentAnimation;
	CAnimationSequenceData* m_ChangeAnimation;
	bool		m_PlayAnimation;
	bool		m_AnimEnd;

	CSharedPtr<class CComputeShader>	m_AnimationUpdateShader;
	class CAnimationUpdateConstantBuffer* m_AnimationUpdateCBuffer;
	class CStructuredBuffer* m_OutputBuffer;	// 애니메이션 결과 저장용 버퍼
	class CStructuredBuffer* m_BoneBuffer;
	class CStructuredBuffer* m_InstancingBoneBuffer;
	std::vector<Matrix>	m_vecBoneMatrix;
	CSharedPtr<class CSkeleton>	m_Skeleton;
	// class CSkeleton*	m_Skeleton;

	Vector3	m_BlendScale;
	Vector3	m_BlendPos;
	Vector4	m_BlendRot;

	float	m_GlobalTime;
	float	m_SequenceProgress;
	float	m_ChangeTimeAcc;
	float	m_ChangeTime;

	// Animation Editor 조작
	bool m_EditorStopAnimation;
	int m_EditorStopTargetFrame;

	// Saved FileName
	char m_SavedFileName[MAX_PATH];

public:
	void SetNotifyParam(const std::string& SequenceName, const std::string& NotifyName, const NotifyParameter& Param)
	{
		CAnimationSequenceData* Data = FindAnimation(SequenceName);

		if (Data)
		{
			auto iter = Data->m_vecNotify.begin();
			auto iterEnd = Data->m_vecNotify.end();

			for (; iter != iterEnd; ++iter)
			{
				if ((*iter)->Name == NotifyName)
				{
					(*iter)->Param = Param;
					return;
				}
			}

		}
	}

	void SetNotifyParamRange(const std::string& SequenceName, const std::string& NotifyName, float Range)
	{
		CAnimationSequenceData* Data = FindAnimation(SequenceName);

		if (Data)
		{
			auto iter = Data->m_vecNotify.begin();
			auto iterEnd = Data->m_vecNotify.end();

			for (; iter != iterEnd; ++iter)
			{
				if ((*iter)->Name == NotifyName)
				{
					(*iter)->Param.Range = Range;
					return;
				}
			}

		}
	}

	void SetNotifyParamAmount(const std::string& SequenceName, const std::string& NotifyName, int Amount)
	{
		CAnimationSequenceData* Data = FindAnimation(SequenceName);

		if (Data)
		{
			auto iter = Data->m_vecNotify.begin();
			auto iterEnd = Data->m_vecNotify.end();

			for (; iter != iterEnd; ++iter)
			{
				if ((*iter)->Name == NotifyName)
				{
					(*iter)->Param.Amount = Amount;
					return;
				}
			}

		}
	}

	void SetNotifyParamTargetObject(const std::string& SequenceName, const std::string& NotifyName, CGameObject* TargetObject)
	{
		CAnimationSequenceData* Data = FindAnimation(SequenceName);

		if (Data)
		{
			auto iter = Data->m_vecNotify.begin();
			auto iterEnd = Data->m_vecNotify.end();

			for (; iter != iterEnd; ++iter)
			{
				if ((*iter)->Name == NotifyName)
				{
					(*iter)->Param.TargetObject = TargetObject;
					return;
				}
			}

		}
	}

	std::unordered_map<std::string, CAnimationSequenceData*>& GetAnimationSequenceMap()
	{
		return m_mapAnimation;
	}

	bool IsCurrentAnimLoop()	const
	{
		return m_CurrentAnimation->m_Loop;
	}

	bool IsCurrentAnimEnd()	const	// Loop아닌 시퀀스가 끝났는지
	{
		return m_AnimEnd;
	}

	void SetInstancingBoneBuffer(class CStructuredBuffer* Buffer)
	{
		m_InstancingBoneBuffer = Buffer;
	}

	size_t GetTypeID()	const
	{
		return m_TypeID;
	}
	template <typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}

	void SetSavedFileName(const char* FileName)
	{
		strcpy_s(m_SavedFileName, FileName);
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetOwner(class CAnimationMeshComponent* Owner)
	{
		m_Owner = Owner;
	}

	const char* GetSavedFileName() const
	{
		return m_SavedFileName;
	}

	int GetAnimationCount()	const
	{
		return (int)m_mapAnimation.size();
	}

	void Play()
	{
		m_PlayAnimation = true;

		if (m_EditorStopAnimation)
			m_EditorStopAnimation = false;
	}

	void Stop()
	{
		m_PlayAnimation = false;
	}

	bool IsPlay()	const
	{
		return m_PlayAnimation;
	}

	CAnimationSequenceData* GetCurrentAnimation()	const
	{
		return m_CurrentAnimation;
	}

	CAnimationSequenceData* GetChangeAnimation()	const
	{
		return m_ChangeAnimation;
	}

	void SetEditorStopTargetFrame(int Frame);
	void SetSkeleton(class CSkeleton* Skeleton);
	void GetAnimationSequenceNames(std::vector<std::string>& VecSequenceNames);
	void SetCurrentAnimationFrameIdx(int Idx);
	bool EditCurrentSequenceKeyName(const std::string& NewKey, const std::string& PrevKey);
	void DeleteCurrentAnimation();
	const std::string& GetCurrentAnimationKeyName();

public:
	void AddAnimation(const std::string& SequenceName, const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f);
	void AddAnimation(const TCHAR* FileName, const std::string& PathName, const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetLoop(const std::string& Name, bool Loop);
	// 현재 Current Animation Loop 세팅
	void SetLoop(bool Loop);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	void KeepCurrentAnimation();
	bool CheckCurrentAnimation(const std::string& Name);
	void ClearAnimationSequenceFromAnimationEditor();
	void GatherSequenceNames(std::vector<std::string>& vecString);
	void AddAnimationSequenceToSceneResource();
	int GetCurrentAnimationOrder();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetShader();
	void ResetShader();
	virtual CAnimationSequenceInstance* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	bool SaveAnimationFullPath(const char* FullPath);
	bool LoadAnimationFullPath(const char* FullPath);
	bool LoadAnimation(const char* FileName);
private:
	CAnimationSequenceData* FindAnimation(const std::string& Name);

public:
	

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void (T::* Func)())
	{
		CAnimationSequenceData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, const std::string& NotifyName, int Frame,
		T* Obj, void (T::* Func)())
	{
		CAnimationSequenceData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->AddNotify<T>(NotifyName, Frame, Obj, Func);
	}

	template <typename T>
	void AddNotifyParam(const std::string& Name, const std::string& NotifyName, int Frame,
		T* Obj, void (T::* Func)(const NotifyParameter&))
	{
		CAnimationSequenceData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->AddNotifyParam<T>(NotifyName, Frame, Obj, Func);
	}
};

