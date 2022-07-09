
#include "ShaderInfo.fx"

#define PARTICLE_INIT_ANGLE 1000

cbuffer	ParticleCBuffer : register(b11)
{
	uint	g_ParticleSpawnEnable;	// 파티클 생성여부
	float3	g_ParticleStartMin;		// 파티클이 생성될 영역의 Min
	float3	g_ParticleStartMax;		// 파티클이 생성될 영역의 Max
	uint	g_ParticleSpawnCountMax;	// 생성될 파티클의 최대

	float3	g_ParticleScaleMin;		// 생성될 파티클 크기의 Min
	float	g_ParticleLifeTimeMin;	// 생성될 파티클이 살아있을 최소시간

	float3	g_ParticleScaleMax;		// 새성될 파티클 크기의 Max
	float	g_ParticleLifeTimeMax;	// 생성될 파티클이 살아있을 최대시간

	float4	g_ParticleColorMin;		// 생성될 파티클의 색상 Min
	float4	g_ParticleColorMax;		// 생성될 파티클의 색상 Max

	float	g_ParticleSpeedMin;		// 파티클의 최소 이동속도
	float	g_ParticleSpeedMax;		// 파티클의 최대 이동속도
	int		g_ParticleMove;			// 이동을 하는지 안하는지
	int		g_ParticleGravity;		// 중력 적용을 받는지 안받는지

	float3	g_ParticleMoveDir;		// 이동을 한다면 기준이 될 이동 방향
	int		g_Particle2D;			// 2D용 파티클인지

	float3	g_ParticleMoveAngle;	// 이동을 한다면 기준이 될 방향으로부터 x, y, z 에 저장된 각도만큼 틀어진 랜덤한 방향을 구한다.
	int      g_ParticleBounce;

	float3 g_ParticleRotationAngle;
	float   g_ParticleBounceResist;

	int g_ParticleGenerateShapeType;
	float g_ParcticleGenerateRadius;
	int g_LoopGenerateRing;
	int g_ParticleSpecialMoveDirType;

	float g_ParticleAlphaEnd;
	float g_ParticleAlphaStart;
	int g_ParticleLifeTimeLinear;
	int g_ParticleRotateAccordingToDir; // 자신의 진행 방향 대로 회전시킬 것인가. g_ParticleSpecialMoveDirType 을 지정해야 한다. 회전 중심 축을 잡아야 하므로

	int g_ParticleUVMoveEnable;
	int g_ParticleUVRowN;
	int g_ParticleUVColN;
	int g_ParticleResetSharedInfoSpawnCntSum;

	// 각 Particle 별로 다르게 Rotation Angle을 주는 경우
	float3 g_ParticleSeperateRotAngleMin;
	int g_ParticleDisableNewAlive;

	float3 g_ParticleSeperateRotAngleMax;
	int g_ParticleAlphaLinearFromCenter;

	// Particle Component Relative Scale
	float3 g_ParticleCommonRelativeScale;
	int ParticleEmpty5;

	// Particle Component WorldPos
	float3 g_ParticleComponentWorldPos;
	int ParticleEmpty6;
};

/*
if (g_ParticleBounce == 1)
{
	if (g_ParticleArray[ThreadID.x].InitWorldPosY >= g_ParticleArray[ThreadID.x].WorldPos.y)
	{
		g_ParticleArray[ThreadID.x].FallTime = 0.f;
		// g_ParticleArray[ThreadID.x].Speed *= 0.98f;
		g_ParticleArray[ThreadID.x].Speed *= g_ParticleBounceResist;
	}
}
*/

#define	GRAVITY	9.8f

struct ParticleInfo
{
	float3	WorldPos;
	float3	Dir;
	float	Speed;
	float	LifeTime;

	float	LifeTimeMax;
	int		Alive;
	float	FallTime;
	float	FallStartY;

	// Alpha
	float AlphaDistinctStart; 

	float CurrentParticleAngle;

	float3 SeperateRotAngleOffset;
	float3 FinalSeperateRotAngle;

	float  InitWorldPosY;
};

struct ParticleInfoShared
{
	uint	SpawnEnable;
	uint	SpawnCountMax;

	uint CurrentSpawnCountSum;
	int DisableNewAlive;

	float3	ScaleMin;
	float3	ScaleMax;

	float4	ColorStart;
	float4	ColorEnd;

	float3 CommonRelativeScale;

	int		GravityEnable;

	float3 RotationAngle;
	float3  SeperateMinRotAngle;
	float3  SeperateMaxRotAngle;

	float3  ParticleComponentWorldPos;

	float MaxDistFromCenter;

	float PrevRingAngle;

	int UVMoveEnable;
	int UVRowN;
	int UVColN;
};


RWStructuredBuffer<ParticleInfo>		g_ParticleArray	: register(u0);
RWStructuredBuffer<ParticleInfoShared> g_ParticleShare : register(u1);

StructuredBuffer<float>		g_ParticleNormalDistValArray	: register(t20);

Texture2DMS<float4> g_GBufferDepth : register(t10);


float3x3 ComputeRotationMatrix(float3 Angle)
{
	float3	ConvertAngle;
	ConvertAngle.x = DegreeToRadian(Angle.x);
	ConvertAngle.y = DegreeToRadian(Angle.y);
	ConvertAngle.z = DegreeToRadian(Angle.z);

	float3x3	matRotX, matRotY, matRotZ, matRot;

	// 11, 12, 13
	// 21, 22, 23
	// 31, 32, 33
	matRotX._11 = 1.f;
	matRotX._21 = 0.f;
	matRotX._31 = 0.f;

	matRotX._12 = 0.f;
	matRotX._22 = cos(ConvertAngle.x);
	matRotX._32 = -sin(ConvertAngle.x);

	matRotX._13 = 0.f;
	matRotX._23 = sin(ConvertAngle.x);
	matRotX._33 = cos(ConvertAngle.x);


	matRotY._11 = cos(ConvertAngle.y);
	matRotY._21 = 0.f;
	matRotY._31 = -sin(ConvertAngle.y);

	matRotY._12 = 0.f;
	matRotY._22 = 1.f;
	matRotY._32 = 0.f;

	matRotY._13 = sin(ConvertAngle.y);
	matRotY._23 = 0.f;
	matRotY._33 = cos(ConvertAngle.y);


	matRotZ._11 = cos(ConvertAngle.z);
	matRotZ._21 = -sin(ConvertAngle.z);
	matRotZ._31 = 0.f;

	matRotZ._12 = sin(ConvertAngle.z);
	matRotZ._22 = cos(ConvertAngle.z);
	matRotZ._32 = 0.f;

	matRotZ._13 = 0.f;
	matRotZ._23 = 0.f;
	matRotZ._33 = 1.f;

	matRot = mul(matRotX, matRotY);
	matRot = mul(matRot, matRotZ);

	return matRot;
}


void ApplySpecialParticleGenerateShape(float RandomAngle, int ThreadID, float FinalAppliedRadius,
	float Rand)
{
	switch (g_ParticleGenerateShapeType)
	{
		// Ring 모양 생성 => Generate Circle 을 하게 되면, Start Min, Max 가 무의미해지게 되는 것이다.
		// 0.f, 0.f, 0.f 중심으로 원에 생성하기
	case 0:
	{
		float CurrentRingAngle = RandomAngle;

		float3 RingPos = float3(0.f, 0.f, 0.f) + float3(
			cos(CurrentRingAngle) * FinalAppliedRadius,
			0.f,
			sin(CurrentRingAngle) * FinalAppliedRadius);

		g_ParticleArray[ThreadID].WorldPos = RingPos;

		// Loop 을 설정하게 되면, 차례대로 만들어지게 한다.
		if (g_LoopGenerateRing == 1)
		{
			CurrentRingAngle = g_ParticleShare[0].PrevRingAngle + (360.f / g_ParticleSpawnCountMax);

			if (CurrentRingAngle > 360.f)
				CurrentRingAngle -= 360.f;

			RingPos = float3(0.f, 0.f, 0.f) + float3(cos(CurrentRingAngle) * FinalAppliedRadius,
				0.f,
				sin(CurrentRingAngle) * FinalAppliedRadius);

			g_ParticleArray[ThreadID].WorldPos = RingPos;

			g_ParticleShare[0].PrevRingAngle = CurrentRingAngle;

			// 현재 Frame 에서 해당 Particle 이 놓인 각도 (중심 기준)
			g_ParticleArray[ThreadID].CurrentParticleAngle = CurrentRingAngle;
		}
	}
	break;
	// Circle
	// 일정 범위 원 안에서 생성되게 하기 (Ring 과 같이 일정 간격에만 생성 X. 원 안에 랜덤 위치에 생성)
	case 1:
	{
		// 생성 각도도 Random
		// 생성 반지름 크기도 Random
		float RandomRadius = FinalAppliedRadius * Rand;

		float3 CirclePos = float3(0.f, 0.f, 0.f) + float3(
			cos(RandomAngle) * RandomRadius,
			0.f,
			sin(RandomAngle) * RandomRadius);

		g_ParticleArray[ThreadID].WorldPos = CirclePos;
	}
	break;
	// 사용자 정면을 바라보는 형태로 Ring 생성하게 하기 
	case 2:
	{
		float CurrentRingAngle = RandomAngle;

		float3 RingPos = float3(0.f, 0.f, 0.f) + float3(
			cos(RandomAngle) * FinalAppliedRadius,
			sin(RandomAngle) * FinalAppliedRadius,
			0.f);

		g_ParticleArray[ThreadID].WorldPos = RingPos;

		// Loop 을 설정하게 되면, 차례대로 만들어지게 한다.
		if (g_LoopGenerateRing == 1)
		{
			CurrentRingAngle = g_ParticleShare[0].PrevRingAngle + (360.f / g_ParticleSpawnCountMax);

			if (CurrentRingAngle > 360.f)
				CurrentRingAngle -= 360.f;

			RingPos = float3(0.f, 0.f, 0.f) + float3(
				cos(CurrentRingAngle) * FinalAppliedRadius,
				sin(CurrentRingAngle) * FinalAppliedRadius,
				0.f);

			g_ParticleArray[ThreadID].WorldPos = RingPos;

			g_ParticleShare[0].PrevRingAngle = CurrentRingAngle;

			// 현재 Frame 에서 해당 Particle 이 놓인 각도 (중심 기준)
			g_ParticleArray[ThreadID].CurrentParticleAngle = CurrentRingAngle;
		}
	}
	break;
	}
}

void ApplySpecialMoveDirType(int ThreadID, float RandomAngle, float3 OriginDir, float Rand)
{
	float MoveTowardAngle = RandomAngle;

	// 만약 Generate Loop 이라면, 순차적으로 만들어지는 것
	// 그렇다면, 중심 기준 -> 자신이 생성된 위치
	// 해당 각도 방향으로 회전해야 한다.
	if (g_LoopGenerateRing == 1)
	{
		MoveTowardAngle = g_ParticleArray[ThreadID].CurrentParticleAngle;
	}

	switch (g_ParticleSpecialMoveDirType)
	{
	case 0:
	{
		// y 는 0 -> XZ 평면으로, 사방으로 뻗어나가게 하기 
		float3 RandDir = float3(
			// cos(RandomAngle) * Rand, 어차피 normalize 를 아래에서 진행해주는데, 굳이 Rand 를 곱해줘야 하는가
			cos(MoveTowardAngle),
			g_ParticleMoveDir.y,
			// OriginDir.y, 
			sin(MoveTowardAngle));

		normalize(RandDir);

		g_ParticleArray[ThreadID].Dir = RandDir;
	}
	break;

	case 1:
	{
		// z 는 0, xy 평면 방향으로 뻗어나가게 하기 
		float3 RandDir = float3(
			cos(MoveTowardAngle),
			sin(MoveTowardAngle),
			g_ParticleMoveDir.z);
			// OriginDir.z);

		normalize(RandDir);

		g_ParticleArray[ThreadID].Dir = RandDir;
	}
	break;
	}
}

void ApplyParticleMove(float3 RandomPos, int ThreadID, float RandomAngle, float Rand)
{
	// Rand는 0 에서 1 사이의 값
	// Move 하는 Particle 이라면, Speed 와 Dir 를 세팅한다.
	if (g_ParticleMove == 1)
	{
		// RandomPos.xyz 는 각각 0 에서 1 사이의 값 --> -1 에서 1 사이의 값으로 바꾼다.
		// ex) 360도 => -180 ~ 180 도 사이의 랜덤한 각도로 회전을 한다고 생각하면 된다.
		float3	ConvertAngle = (RandomPos.xyz * 2.f - 1.f) * g_ParticleMoveAngle;

		// Particle Component 의 Rotation Angle 도 적용한다.
		ConvertAngle += g_ParticleRotationAngle;

		// float3x3 matRot = ComputeRotationMatrix(ConvertAngle);
		float3x3 matRot = ComputeRotationMatrix(ConvertAngle);

		// 현재 Rot 상으로는, Particle Rotation 기능만 수행중이다.
		float3 OriginDir = normalize(mul(g_ParticleMoveDir, matRot));

		float3	Dir = normalize(mul(g_ParticleMoveDir, matRot));

		g_ParticleArray[ThreadID.x].Speed = Rand * (g_ParticleSpeedMax - g_ParticleSpeedMin) + g_ParticleSpeedMin;
		g_ParticleArray[ThreadID.x].Dir = Dir;

		// 단순 Dir 이동이 아니라, 특정 방향으로 Special 하게 이동하는 설정을 했다면
		ApplySpecialMoveDirType(ThreadID, RandomAngle, OriginDir, Rand);
	}
}

// 중력 적용하기 
void ApplyGravity(int ThreadID, float3 MovePos)
{
	// 중력 적용
	if (g_ParticleShare[0].GravityEnable == 1)
	{
		g_ParticleArray[ThreadID].FallTime += g_DeltaTime;

		float	Velocity = 0.f;

		if (g_ParticleArray[ThreadID].Dir.y > 0.f)
			Velocity = g_ParticleArray[ThreadID].Speed * g_ParticleArray[ThreadID].FallTime;

		g_ParticleArray[ThreadID].WorldPos.y = g_ParticleArray[ThreadID].FallStartY +
			(Velocity - 0.5f * GRAVITY * g_ParticleArray[ThreadID].FallTime * g_ParticleArray[ThreadID].FallTime * 10.f);

		// Bounce 효과를 낸다면 
		if (g_ParticleBounce == 1)
		{
			if (g_ParticleArray[ThreadID].InitWorldPosY >= g_ParticleArray[ThreadID].WorldPos.y)
			{
				g_ParticleArray[ThreadID].FallTime = 0.f;
				// g_ParticleArray[ThreadID.x].Speed *= 0.98f;
				g_ParticleArray[ThreadID].Speed *= g_ParticleBounceResist;
			}
		}

		g_ParticleArray[ThreadID].WorldPos.x += MovePos.x;
		g_ParticleArray[ThreadID].WorldPos.z += MovePos.z;
	}
	else
	{
		g_ParticleArray[ThreadID.x].WorldPos += MovePos;
	}
}

void ApplyRotationAccordingToDir(int ThreadID)
{
	// HLSL 좌표계를 무엇을 사용하는가 ?
	//  y 축 20도 회전 ? -> 반시계방향으로 20도 돌린 것
	// 그런데 왼손좌표계에서는 20도 회전이 곧, 시계방향인 것으로 알고 있는데 ..?
	// g_ParticleArray[ThreadID.x].FinalSeperateRotAngle += float3(0.f, 20.f , 0.f);

	if (g_ParticleRotateAccordingToDir == 1)
	{
		// float3x3 matComponentRot = ComputeRotationMatrix(g_ParticleShareSRV[0].RotationAngle);
		float RotAngle = 0.f;
		float3 DirVector = g_ParticleArray[ThreadID.x].Dir;

		switch (g_ParticleSpecialMoveDirType)
		{
		// y 는 0 -> XZ 평면으로, 사방으로 뻗어나가게 하기 
		case 0:
		{

			// 아래는 Y + 방향을 바라본 상태로 회전하는 것
			float3 CenterVector = float3(0.f, 0.f, 1.f);
			DirVector.y = 0.f;

			normalize(DirVector);
			// CenterVector = mul(matComponentRot, CenterVector);

			RotAngle = AngleBetweenTwoVector(CenterVector, DirVector);

			if (DirVector.x > 0.f)
			{
				RotAngle = (180 - RotAngle);
			}

			g_ParticleArray[ThreadID.x].FinalSeperateRotAngle += float3(0.f, RotAngle, 0.f);
		}
		break;

		case 1:
		{
			// z 는 0, xy 평면 방향으로 뻗어나가게 하기 (z 는 - 방향을 향하고 있게 된다.)
			float3 CenterVector = float3(0.f, 1.f, 0.f);
			DirVector.z = 0.f;

			normalize(DirVector);
			float RotAngle = AngleBetweenTwoVector(CenterVector, DirVector);

			if (DirVector.x > 0.f)
			{
				RotAngle = (180 - RotAngle);
			}

			g_ParticleArray[ThreadID.x].FinalSeperateRotAngle += float3(0.f, 0.f, RotAngle );

		}
		break;
		}
	}
}

// 해당 함수로 들어올 때는, 이미 동기화 작업을 거쳐서
// 특정 쓰레드가, 자기 혼자만 공유 정보에 접근하고 있는 상태
// 따라서 별도의 추가적인 동기화는 필요없다. (필요하다면 넣어야 하지만)
void ApplyLinearEffect(int ThreadID)
{
	float DistFromCenter = distance(g_ParticleArray[ThreadID].WorldPos, float3(0.f, 0.f, 0.f));
	float MaxDistFromCenter = g_ParticleShare[0].MaxDistFromCenter;

	if (MaxDistFromCenter == 0)
		MaxDistFromCenter = DistFromCenter;

	float SpawnPosRatio = (float)(DistFromCenter / MaxDistFromCenter);

	if (SpawnPosRatio >= 1.f)
		SpawnPosRatio = 0.999f;

	// Life Time
	if (g_ParticleLifeTimeLinear == 1)
	{
		// 이렇게만 하면 LifeTime 에서 큰 차이가 없다.
		// float LifeTimeRatio = (1 - SpawnPosRatio);
		float LifeTimeRatio = pow(1 - SpawnPosRatio, 2);

		g_ParticleArray[ThreadID].LifeTimeMax = LifeTimeRatio * (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;
	}

	// Alpha
	if (g_ParticleAlphaLinearFromCenter == 1)
	{
		// 이렇게만 하면 LifeTime 에서 큰 차이가 없다.
		// float LifeTimeRatio = (1 - SpawnPosRatio);
		float AlphaTimeRatio = pow(1 - SpawnPosRatio, 2);
		float AlphaDiff = g_ParticleAlphaEnd - g_ParticleAlphaStart;

		g_ParticleArray[ThreadID].AlphaDistinctStart = AlphaTimeRatio * (g_ParticleAlphaEnd - g_ParticleAlphaStart) + g_ParticleAlphaStart;
	}
}

[numthreads(64, 1, 1)]	// 스레드 그룹 스레드 수를 지정한다.
void ParticleUpdate(uint3 ThreadID : SV_DispatchThreadID)
{
	g_ParticleShare[0].SpawnEnable = g_ParticleSpawnEnable;
	g_ParticleShare[0].SpawnCountMax = g_ParticleSpawnCountMax;

	g_ParticleShare[0].ScaleMin = g_ParticleScaleMin;
	g_ParticleShare[0].ScaleMax = g_ParticleScaleMax;

	g_ParticleShare[0].ColorStart = g_ParticleColorMin;
	g_ParticleShare[0].ColorStart.a = g_ParticleAlphaStart;

	g_ParticleShare[0].ColorEnd = g_ParticleColorMax;
	g_ParticleShare[0].ColorEnd.a = g_ParticleAlphaEnd;

	g_ParticleShare[0].CommonRelativeScale = g_ParticleCommonRelativeScale;
	g_ParticleShare[0].ParticleComponentWorldPos = g_ParticleComponentWorldPos;

	g_ParticleShare[0].GravityEnable = g_ParticleGravity;
	g_ParticleShare[0].RotationAngle = g_ParticleRotationAngle;

	g_ParticleShare[0].DisableNewAlive = g_ParticleDisableNewAlive;

	g_ParticleShare[0].UVMoveEnable = g_ParticleUVMoveEnable;
	g_ParticleShare[0].UVRowN = g_ParticleUVRowN;
	g_ParticleShare[0].UVColN   = g_ParticleUVColN;


	// 매번 초기화 해줄 것이다.
	g_ParticleArray[ThreadID.x].SeperateRotAngleOffset = float3(0.f, 0.f, 0.f);

	// 처음에는 자기 자신의 Rot Angle 을 -1으로 세팅 (일종의 초기화)
	g_ParticleArray[ThreadID.x].CurrentParticleAngle = PARTICLE_INIT_ANGLE;

	// 동작되는 스레드의 수가 생성되는 파티클의 최대 수 보다 크거나 같다면
	// 잘못된 인덱스로 동작하기 때문에 처리를 안해준다.
	if (g_ParticleSpawnCountMax <= ThreadID.x)
		return;

	// 원래 기본 설정 처럼, SpawnTime 에 맞춰서 지속적으로 Particle을 생성하거나
	// 일시적으로 Restart 버튼을 누른 것이라면
	if (g_ParticleDisableNewAlive == 0 ||
		g_ParticleResetSharedInfoSpawnCntSum == 1)
	{
		// SpawnTime 에 맞춰서 지속적으로 생성하게끔 하려면 
		// g_ParticleDisableNewAlive == 1 일때, 즉, 한번에 확 생성하고, 지금까지 누적 생성된 Particle 개수가
		// g_ParticleSpawnCountMax 를 넘어가면 생성되지 않는 원리인데.
		// 이를 무효화 하기 위해서, 
		// 지금까지 누적 생성된 Particle 개수를 계속 0이 되게 할 것이다.
		g_ParticleShare[0].CurrentSpawnCountSum = 0;
	}

	// 파티클이 살아있는 파티클인지 판단한다.
	if (g_ParticleArray[ThreadID.x].Alive == 0)
	{
		// SpawnCount 만큼 한번에 생성해버리고, 
		// 차후 새로운 Particle 은 생성하지 않으려고 한다면
		if (g_ParticleShare[0].CurrentSpawnCountSum >= g_ParticleSpawnCountMax && 
			g_ParticleDisableNewAlive == 1)
		// if (g_ParticleShare[0].CurrentSpawnCountSum > g_ParticleSpawnCountMax && g_ParticleDisableNewAlive == 1) -> 한개가 더 생성됨
			return;

		int	SpawnEnable = g_ParticleShare[0].SpawnEnable;
		int	Exchange = 0;

		if (g_ParticleShare[0].SpawnEnable == 1)
		{
			int	InputValue = SpawnEnable - 1;

			// 함수의 인자는 in, in, out으로 구성되어 있다.
			// in은 일반적으로 변수의 값을 넘겨줄때 사용한다.
			// out은 반쪽짜리 레퍼런스처럼 결과값을 여기의 변수에 받아올때 사용한다.
			// InputValue 를 g_ParticleShare[0].SpawnEnable 에 새롭게 세팅해주고
			// 기존에 g_ParticleShare[0].SpawnEnable 에 들어있던 값을 ExChange 로 가져온다.
			// 그러면, 지금 g_ParticleShare[0].SpawnEnable 에 세팅하려는 값은 0 이다.
			// InputValue 에는 0이 들어가있으니까
			// 만약에 g_ParticleShare[0].SpawnEnable 에 1 이 들어있다면
			// 다른 쓰레드가 아직 접근안했다는 의미이고, Exchange 에 1이 들어올 것이다.
			// 즉, 결과적으로 지금 현재 g_ParticleShare[0].SpawnEnable 의 값을 Exchange 로 가져오는 것이다.
			InterlockedExchange(g_ParticleShare[0].SpawnEnable, InputValue, Exchange);

			if (Exchange == SpawnEnable)
				g_ParticleArray[ThreadID.x].Alive = 1;
		}

		// 아래를 거쳐갔다는 것은, 현재 바로 생성된 Particle 이라는 의미이다.
		if (g_ParticleArray[ThreadID.x].Alive == 0)
			return;

		// g_ParticleDisableNewAlive 에 따라서, 공유 구조화 버퍼에 CurrentSpawnCountSum 세팅하기 
		if (g_ParticleDisableNewAlive == 1)
			g_ParticleShare[0].CurrentSpawnCountSum += 1;
		else
		{
			// SpawnTime 에 맞춰서 지속적으로 생성하게끔 하려면 
			// g_ParticleDisableNewAlive == 1 일때, 즉, 한번에 확 생성하고, 지금까지 누적 생성된 Particle 개수가
			// g_ParticleSpawnCountMax 를 넘어가면 생성되지 않는 원리인데.
			// 이를 무효화 하기 위해서, 
			// 지금까지 누적 생성된 Particle 개수를 계속 0이 되게 할 것이다.
			g_ParticleShare[0].CurrentSpawnCountSum = 0;
		}

		// 살려야 하는 파티클이라면 파티클 생성정보를 만들어낸다.
		float	key = ThreadID.x / g_ParticleSpawnCountMax;
		float3	RandomPos = float3(Rand(key), Rand(2.142f), Rand(key * 3.f));
		float	Rand = (RandomPos.x + RandomPos.y + RandomPos.z) / 3.f;
		float RandomAngle = 360.f * Rand;

		// x,y,z 의 Relative Scale 의 중간 비율만큼 세팅해준다.
		// 아래 코드처럼 하면, 너무 Dramatic 하게 변한다.
		// float ParticleComponentScaleRatio = g_ParticleCommonRelativeScale.x * g_ParticleCommonRelativeScale.y * g_ParticleCommonRelativeScale.z;
		float ParticleComponentScaleRatio = (g_ParticleCommonRelativeScale.x + g_ParticleCommonRelativeScale.y + g_ParticleCommonRelativeScale.z) / 3.f;

		float3	StartRange = g_ParticleStartMax - g_ParticleStartMin;
		g_ParticleArray[ThreadID.x].WorldPos = Rand * (g_ParticleStartMax * ParticleComponentScaleRatio - g_ParticleStartMin * ParticleComponentScaleRatio);
		g_ParticleArray[ThreadID.x].WorldPos += g_ParticleStartMin * ParticleComponentScaleRatio;
		
		g_ParticleArray[ThreadID.x].InitWorldPosY = g_ParticleArray[ThreadID.x].WorldPos.y;

		g_ParticleArray[ThreadID.x].FallTime = 0.f;
		g_ParticleArray[ThreadID.x].FallStartY = g_ParticleArray[ThreadID.x].WorldPos.y;

		g_ParticleArray[ThreadID.x].LifeTime = 0.f;
		g_ParticleArray[ThreadID.x].LifeTimeMax = Rand * (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;

		// Scale 크기도 그만큼 조정한다.
		g_ParticleArray[ThreadID.x].LifeTimeMax *= ParticleComponentScaleRatio;

		float FinalAppliedRadius = g_ParcticleGenerateRadius * ParticleComponentScaleRatio;

		// Special 한 모양으로 Particle 을 만들고자 한다면
		ApplySpecialParticleGenerateShape(RandomAngle, ThreadID.x, FinalAppliedRadius, Rand);

		// Move 하는 Particle 이라면, Speed 와 Dir 를 세팅한다.
		ApplyParticleMove(RandomPos, ThreadID.x, RandomAngle, Rand);

		// 생성되는 순간 각 Particle 의 Rot 정도를 세팅한다.
		// 최종 각각의 Particle 회전 정도 세팅
		// '=' 가 아니라 '+=' 를 해줘야 한다.
		// ApplySpecialParticleGenerateShape 에서 일부 미리 FinalSeperateRotAngle 값에 
		// 회전할 Offset 값을 더해놓은 상태이기 때문이다.
		// g_ParticleArray[ThreadID.x].FinalSeperateRotAngle += ((g_ParticleSeperateRotAngleMax - g_ParticleSeperateRotAngleMin) * Rand + g_ParticleSeperateRotAngleMin);
		g_ParticleArray[ThreadID.x].FinalSeperateRotAngle = (g_ParticleSeperateRotAngleMax - g_ParticleSeperateRotAngleMin) * Rand + g_ParticleSeperateRotAngleMin;

		// 자신의 진행 방향에 따른 회전을 추가한다.
		ApplyRotationAccordingToDir(ThreadID.x);
		
		// 가장 마지막에 중심에서 가장 멀리떨어져 있는 Max 거리값을 공유 구조화 버퍼에 세팅한다.
		float DistFromCenter = length(g_ParticleArray[ThreadID.x].WorldPos);
		g_ParticleShare[0].MaxDistFromCenter = max(g_ParticleShare[0].MaxDistFromCenter, DistFromCenter);

		// Alpha 시작값을 세팅한다. 기본적으로 상수버퍼로 넘어온 g_ParticleAlphaStart 값을 이용한다.
		g_ParticleArray[ThreadID.x].AlphaDistinctStart = g_ParticleAlphaStart;

		// 중앙으로부터 가장 멀리 떨어져 있는 값만큼 세팅
		// 아래에서 한번 더 
		ApplyLinearEffect(ThreadID.x);
	}

	// 현재 생성이 되어 있는 파티클일 경우
	// 혹은 처음에 그냥 다 생성해버리는 코드라면
	// if (g_ParticleDisableNewAlive == 1 || g_ParticleArray[ThreadID.x].Alive == 0)
	else
	{
		g_ParticleArray[ThreadID.x].LifeTime += g_DeltaTime;

		float3	MovePos = (float3)0.f;

		if (g_ParticleMove == 1)
		{
			MovePos = g_ParticleArray[ThreadID.x].Dir *
				g_ParticleArray[ThreadID.x].Speed * g_DeltaTime;
		}

		// 중력 적용
		ApplyGravity(ThreadID.x, MovePos);

		if (g_ParticleArray[ThreadID.x].LifeTime >= g_ParticleArray[ThreadID.x].LifeTimeMax)
		{
			g_ParticleArray[ThreadID.x].Alive = 0;
		}
	}

}

struct VertexParticle
{
	float3	Pos	: POSITION;
	uint InstanceID : SV_InstanceID;	// 정점버퍼에 만들어서 넣어주는것이 아니다.
};

struct VertexParticleOutput
{
	uint	InstanceID : TEXCOORD;
};

StructuredBuffer<ParticleInfo>		g_ParticleArraySRV	: register(t30);
StructuredBuffer<ParticleInfoShared>	g_ParticleShareSRV	: register(t31);


VertexParticleOutput ParticleVS(VertexParticle input)
{
	VertexParticleOutput	output = (VertexParticleOutput)0;

	output.InstanceID = input.InstanceID;

	return output;
}

struct GeometryParticleOutput
{
	float4	Pos : SV_POSITION;
	float4	Color : COLOR;
	float2	UV	: TEXCOORD;
	float4	ProjPos : POSITION;
};

static float3	g_ParticleLocalPos[4] =
{
	{-0.5f, 0.5f, 0.f},
	{0.5f, 0.5f, 0.f},
	{-0.5f, -0.5f, 0.f},
	{0.5f, -0.5f, 0.f}
};

// in : 값을 함수 안으로 넘겨줄때
// out : 함수안에서 결과를 넘겨받을때. 단 함수 안에서 이 값을 사용한 연산은 할 수 없다.
// inout : 그냥 레퍼런스.
[maxvertexcount(6)]
void ParticleGS(point VertexParticleOutput input[1],
	inout TriangleStream<GeometryParticleOutput> output)
{
	// 점을 사각형으로 확장한다.
	uint	InstanceID = input[0].InstanceID;

	if (g_ParticleArraySRV[InstanceID].Alive == 0)
		return;

	GeometryParticleOutput	OutputArray[4] =
	{
		(GeometryParticleOutput)0.f,
		(GeometryParticleOutput)0.f,
		(GeometryParticleOutput)0.f,
		(GeometryParticleOutput)0.f
	};

	OutputArray[0].UV = float2(0.f, 0.f);
	OutputArray[1].UV = float2(1.f, 0.f);
	OutputArray[2].UV = float2(0.f, 1.f);
	OutputArray[3].UV = float2(1.f, 1.f);

	// UV Animation 적용
	if (g_ParticleShareSRV[0].UVMoveEnable == 1)
	{
		int UVRowN = g_ParticleShareSRV[0].UVRowN;
		int UVColN = g_ParticleShareSRV[0].UVColN;

		// 가로 UV 단일 크기
		float WUVSize = 1.f / (float)UVColN;
		// 세로 UV 단일 크기 
		float HUVSize = 1.f / (float)UVRowN;

		// ex) 5 * 2 => 10개
		// 9 / 10 => 0.9 
		// 10 * 0.9 => 9번째 UV => Idx 상으로는 8번째 (총 Idx 가 0 에서 9까지 존재) 
		int TotalUVNum = UVRowN * UVColN;
		float FloatCurUVIdx = ((float)g_ParticleArraySRV[InstanceID].LifeTime / (float)(g_ParticleArraySRV[InstanceID].LifeTimeMax)) * (float)TotalUVNum;
		int CurUVIdx = floor(FloatCurUVIdx);
		// CurUVIdx -= 1;

		// row * col = 2 * 5
		// rowS = 0.5 (height)
		// colS  = 0.2 (width)
		// 0.95 * 10 => 9.5 => floor(9.5) => 9;
		// 10개의 칸 중에서, 가장 마지막 번째 칸이다.
		// 9 / 5 => 1행
		// 9 % 5 => 4열
		int RowUVIdx = CurUVIdx / UVColN;
		int ColUVIdx = (CurUVIdx % UVColN);

		float2 UVStartPos = float2(WUVSize * ColUVIdx, HUVSize * RowUVIdx);
		float2 UVEndPos = UVStartPos + float2(WUVSize, HUVSize);

		OutputArray[0].UV = float2(UVStartPos.x, UVStartPos.y);
		OutputArray[1].UV = float2(UVEndPos.x, UVStartPos.y);
		OutputArray[2].UV = float2(UVStartPos.x, UVEndPos.y);
		OutputArray[3].UV = float2(UVEndPos.x, UVEndPos.y);
	}

	float	Ratio = g_ParticleArraySRV[InstanceID].LifeTime / g_ParticleArraySRV[InstanceID].LifeTimeMax;
	
	float3	Scale = lerp(
		g_ParticleShareSRV[0].ScaleMin * g_ParticleShareSRV[0].CommonRelativeScale, 
		g_ParticleShareSRV[0].ScaleMax * g_ParticleShareSRV[0].CommonRelativeScale,
		float3(Ratio, Ratio, Ratio));

	float4 ColorStart = g_ParticleShareSRV[0].ColorStart;
	ColorStart.a = g_ParticleArraySRV[InstanceID].AlphaDistinctStart;

	float4	Color = lerp(ColorStart, g_ParticleShareSRV[0].ColorEnd,
		float4(Ratio, Ratio, Ratio, Ratio));

	// 회전 (전체 회전 + 각 Particle 회전 정도)
	// 최종 회전 정보를 여기서 반영해주는 것이 아니라, Update Shader 측에서 반영해줄 것이다.
	float3 FinalRotAngle = g_ParticleShareSRV[0].RotationAngle + g_ParticleArraySRV[InstanceID].FinalSeperateRotAngle;
	// float3 FinalRotAngle = g_ParticleShareSRV[0].RotationAngle + g_ParticleArraySRV[InstanceID].FinalSeperateRotAngle;

	float3x3 matRot = ComputeRotationMatrix(FinalRotAngle);

	// 4개의 최종 정점정보를 만들어준다.
	for (int i = 0; i < 4; ++i)
	{
		// Particle Component 의 World Post 도 더한다.
		float3	WorldPos = g_ParticleArraySRV[InstanceID].WorldPos + mul(g_ParticleLocalPos[i] * Scale, matRot);
		// float3	WorldPos = g_ParticleArraySRV[InstanceID].WorldPos + g_ParticleLocalPos[i] * Scale;
		WorldPos += g_ParticleShareSRV[0].ParticleComponentWorldPos;

		OutputArray[i].ProjPos = mul(float4(WorldPos, 1.f), g_matVP);
		// OutputArray[i].ProjPos.xyz = mul(OutputArray[i].ProjPos.xyz, matRot);
		OutputArray[i].Pos = OutputArray[i].ProjPos;
		OutputArray[i].Color = Color;
	}

	output.Append(OutputArray[0]);
	output.Append(OutputArray[1]);
	output.Append(OutputArray[3]);
	output.RestartStrip();

	output.Append(OutputArray[0]);
	output.Append(OutputArray[3]);
	output.Append(OutputArray[2]);
	output.RestartStrip();
}

PSOutput_Single ParticlePS(GeometryParticleOutput input)
{
	PSOutput_Single output = (PSOutput_Single)0;

	float4 Color = g_BaseTexture.Sample(g_BaseSmp, input.UV);

	if (Color.a == 0.f || input.Color.a == 0.f)
		clip(-1);

	float2 ScreenUV = input.ProjPos.xy / input.ProjPos.w;
	ScreenUV = ScreenUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	int2 TargetPos = (int2) 0;

	TargetPos.x = (int)(ScreenUV.x * g_Resolution.x);
	TargetPos.y = (int)(ScreenUV.y * g_Resolution.y);

	// Soft Particle
	float4 Depth = g_GBufferDepth.Load(TargetPos, 0);
	float Alpha = 1.f;

	if (Depth.a > 0.f)
		Alpha = (Depth.g - input.ProjPos.w) / 0.5f;

	Alpha = clamp(Alpha, 0.f, 1.f);

	Color = PaperBurn2D(Color * input.Color, input.UV);

	output.Color = Color;
	output.Color.a = Color.a * g_MtrlOpacity * Alpha;

	return output;
}
