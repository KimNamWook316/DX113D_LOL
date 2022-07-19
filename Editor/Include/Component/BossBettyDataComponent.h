#pragma once
#include "MonsterDataComponent.h"

enum class BossBettyFarAttackType
{
    Spin,
    JumpSmash
};

enum class BossBettyHPState
{
    None,
    Below60,
    Below30
};

class CBossBettyDataComponent :
    public CMonsterDataComponent
{
    friend class CGameObject;

private:
    CBossBettyDataComponent();
    CBossBettyDataComponent(const CBossBettyDataComponent& com);
    virtual ~CBossBettyDataComponent();

private :
    // 근거리 연속 공격 횟수
    int m_CloseSequentialAttackNum;
    // 연속 2번 근거리 공격 시에, Close Attack 이후, AngryAttack Node 중에서 2번째 (FirstSlam + Roar)
    bool m_CloseAngryAttackEnable;
    // 원거리 공격 횟수 -> 3번 마다, Spin , Jump 를 번갈아가면서 실행할 것이다.
    int m_FarAttackAttackNum;
    // 6번 원거리 공격 Gauge 가 다 차면, Throw 를 할 것이다.
    bool m_ThrowFarAttackEnable;
    // 원거리 공격 Type
    BossBettyFarAttackType m_FarAttackType;
    // 60%, 30% 가 될 때마다, Angry Attack 중에서 첫번째 Attack 설정하기 
    BossBettyHPState m_BettyHPState;
public :
    bool IsCloseAngryAttackEnable() const
    {
        return m_CloseAngryAttackEnable;
    }
    int GetCloseSequentialAttackNum() const
    {
        return m_CloseSequentialAttackNum;
    }
    BossBettyHPState GetBettyHPState() const
    {
        return m_BettyHPState;
    }
    BossBettyFarAttackType GetBettyFarAttackType() const
    {
        return m_FarAttackType;
    }
public  :
    void SetCloseAngryAttackEnable(bool Enable)
    {
        m_ThrowFarAttackEnable = Enable;
    }
    void SetFarThrowAttackEnable(bool Enable)
    {
        m_CloseAngryAttackEnable = Enable;
    }
    void IncCloseSequentialAttackNum() 
    {
        ++m_CloseSequentialAttackNum;

        if (m_CloseSequentialAttackNum == 2)
        {
            m_CloseSequentialAttackNum = 0;
            m_CloseAngryAttackEnable = true;
        }
    }
    void IncFarAttackNum()
    {
        ++m_FarAttackAttackNum;

        // 3번 마자, Far Attack Type 을 다르게 해줄 것이다.
        if (m_FarAttackAttackNum % 3 == 0)
        {
            if (m_FarAttackType == BossBettyFarAttackType::Spin)
                m_FarAttackType = BossBettyFarAttackType::JumpSmash;
            else
                m_FarAttackType = BossBettyFarAttackType::Spin;
        }

        if (m_FarAttackAttackNum == 6)
        {
            m_ThrowFarAttackEnable = true;
            m_FarAttackAttackNum = 0;
        }
    }
public:
    virtual void Start() override;
};

