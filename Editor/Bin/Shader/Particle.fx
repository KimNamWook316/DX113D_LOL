
#include "ShaderInfo.fx"

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
	
	int g_ParticleGenerateRing;
	float g_ParcticleGenerateRadius;
	int g_LoopGenerateRing;
	int g_ParticleRandomMoveDir;

	float g_ParticleAlphaMax;
	float g_ParticleAlphaMin;
	int g_ParticleGenerateCircle;
	float g_ParticleEmpty1;

	int g_ParticleGenerateTorch;
	float g_ParticleEmpty2;
	int  g_ParticleLifeTimeLinear;
	float ParticleEmpty4;
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

	float  InitWorldPosY;
};

struct ParticleInfoShared
{
	uint	SpawnEnable;

	float3	ScaleMin;
	float3	ScaleMax;

	float4	ColorMin;
	float4	ColorMax;

	int		GravityEnable;

	float3 RotationAngle;
	float PrevRingAngle;
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

[numthreads(64, 1, 1)]	// 스레드 그룹 스레드 수를 지정한다.
void ParticleUpdate(uint3 ThreadID : SV_DispatchThreadID)
{
	g_ParticleShare[0].SpawnEnable = g_ParticleSpawnEnable;
	g_ParticleShare[0].ScaleMin = g_ParticleScaleMin;
	g_ParticleShare[0].ScaleMax = g_ParticleScaleMax;

	g_ParticleShare[0].ColorMin = g_ParticleColorMin;
	g_ParticleShare[0].ColorMin.a = g_ParticleAlphaMin;

	g_ParticleShare[0].ColorMax = g_ParticleColorMax;
	g_ParticleShare[0].ColorMax.a = g_ParticleAlphaMax;

	g_ParticleShare[0].GravityEnable = g_ParticleGravity;
	g_ParticleShare[0].RotationAngle = g_ParticleRotationAngle;


	// 동작되는 스레드의 수가 생성되는 파티클의 최대 수 보다 크거나 같다면
	// 잘못된 인덱스로 동작하기 때문에 처리를 안해준다.
	if (g_ParticleSpawnCountMax <= ThreadID.x)
		return;

	// 파티클이 살아있는 파티클인지 판단한다.
	if (g_ParticleArray[ThreadID.x].Alive == 0)
	{
		int	SpawnEnable = g_ParticleShare[0].SpawnEnable;
		int	Exchange = 0;

		if (g_ParticleShare[0].SpawnEnable == 1)
		{
			int	InputValue = SpawnEnable - 1;

			// 함수의 인자는 in, in, out으로 구성되어 있다.
			// in은 일반적으로 변수의 값을 넘겨줄때 사용한다.
			// out은 반쪽짜리 레퍼런스처럼 결과값을 여기의 변수에 받아올때 사용한다.
			InterlockedExchange(g_ParticleShare[0].SpawnEnable, InputValue, Exchange);

			if (Exchange == SpawnEnable)
				g_ParticleArray[ThreadID.x].Alive = 1;
		}

		if (g_ParticleArray[ThreadID.x].Alive == 0)
			return;

		// 살려야 하는 파티클이라면 파티클 생성정보를 만들어낸다.
		float	key = ThreadID.x / g_ParticleSpawnCountMax;
		float3	RandomPos = float3(Rand(key), Rand(2.142f), Rand(key * 3.f));
		float	Rand = (RandomPos.x + RandomPos.y + RandomPos.z) / 3.f;
		float RandomAngle = 360.f * Rand;

		float3	StartRange = g_ParticleStartMax - g_ParticleStartMin;
		g_ParticleArray[ThreadID.x].WorldPos = Rand * (g_ParticleStartMax - g_ParticleStartMin) + g_ParticleStartMin;
		g_ParticleArray[ThreadID.x].InitWorldPosY = g_ParticleArray[ThreadID.x].WorldPos.y;

		g_ParticleArray[ThreadID.x].FallTime = 0.f;
		g_ParticleArray[ThreadID.x].FallStartY = g_ParticleArray[ThreadID.x].WorldPos.y;

		g_ParticleArray[ThreadID.x].LifeTime = 0.f;
		g_ParticleArray[ThreadID.x].LifeTimeMax = Rand * (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;

		if (g_ParticleMove == 1)
		{
			// RandomPos.xyz 는 각각 0 에서 1 사이의 값 --> -1 에서 1 사이의 값으로 바꾼다.
			// ex) 360도 => -180 ~ 180 도 사이의 랜덤한 각도로 회전을 한다고 생각하면 된다.
			float3	ConvertAngle = (RandomPos.xyz * 2.f - 1.f) * g_ParticleMoveAngle;

			// float3x3 matRot = ComputeRotationMatrix(ConvertAngle);
			float3x3 matRot = ComputeRotationMatrix(ConvertAngle);

			// 현재 Rot 상으로는, Particle Rotation 기능만 수행중이다.
			float3 OriginDir = mul(g_ParticleMoveDir, matRot);
			float3	Dir = normalize(mul(g_ParticleMoveDir, matRot));

			g_ParticleArray[ThreadID.x].Speed = Rand * (g_ParticleSpeedMax - g_ParticleSpeedMin) + g_ParticleSpeedMin;
			g_ParticleArray[ThreadID.x].Dir = Dir;

			if (g_ParticleRandomMoveDir == 1)
			{
				// 완전 랜덤한 방향으로 이동하기 
				float3 RandDir = float3(0.f, 0.f, 0.f) + float3(
					cos(RandomAngle) * Rand,
					OriginDir.y,
					sin(RandomAngle) * Rand);
				normalize(RandDir);

				g_ParticleArray[ThreadID.x].Dir = RandDir;
			}
		}

		// Ring 모양 생성 => Generate Circle 을 하게 되면, Start Min, Max 가 무의미해지게 되는 것이다.
		 // 0.f, 0.f, 0.f 중심으로 원에 생성하기
		 if (g_ParticleGenerateRing == 1)
		 {
		 	float3 RingPos = float3(0.f, 0.f, 0.f) + float3(
				cos(RandomAngle) * g_ParcticleGenerateRadius,
		 		0.f, 
		 		sin(RandomAngle) * g_ParcticleGenerateRadius);
		 
		 	g_ParticleArray[ThreadID.x].WorldPos = RingPos;
		 
		 	// Loop 을 설정하게 되면, 차례대로 만들어지게 한다.
		 	if (g_LoopGenerateRing == 1)
		 	{
		 		float NextRingAngle = g_ParticleShare[0].PrevRingAngle + (360.f / g_ParticleSpawnCountMax);
		 
				RingPos = float3(0.f, 0.f, 0.f) + float3(cos(NextRingAngle) * g_ParcticleGenerateRadius,  0.f, sin(NextRingAngle) * g_ParcticleGenerateRadius);
		 		g_ParticleArray[ThreadID.x].WorldPos = RingPos;
		 
		 		g_ParticleShare[0].PrevRingAngle = NextRingAngle;
		 	}
		 }

		 // 일정 범위 원 안에서 생성되게 하기 (Ring 과 같이 일정 간격에만 생성 X. 원 안에 랜덤 위치에 생성)
		 if (g_ParticleGenerateCircle == 1)
		 {
			 // 생성 각도도 Random
			 // 생성 반지름 크기도 Random
			 float RandomRadius = g_ParcticleGenerateRadius * Rand;

			 float3 CirclePos = float3(0.f, 0.f, 0.f) + float3(
				 cos(RandomAngle) * RandomRadius,
				 0.f,
				 sin(RandomAngle) * RandomRadius);

			 g_ParticleArray[ThreadID.x].WorldPos = CirclePos;
		 }

		 // 횃불 모양의 Particle 만들어내기
		 // Spawn 위치로, 가운데 쪽에 비율적으로 더 많이 생성되게 해야 한다.
		 if (g_ParticleGenerateTorch == 1)
		 {
			 // 생성 위치 => 정규 분포를 사용하여, 확률적으로 가운데 많이 생성되게 하기
			 // CPU 측에서 평균 0 , 표준편차 0.165 라는 표준 정규 분포 값을 이용하여 값 세팅
			 // 평균 0 의 값이 가장 많이 나올 것이다. (0 라는 값이 거의 집중적으로 나올 것)
			 // 그리고 범위는 -0.5 ~ 0.5
			 // 여기서 할일은, 0에 가까운 값이 거의 대부분 나오게 조절해야 하고, 또한 범위는 0에서 1 사이의 값에 놓이게 해야 한다.
			 // 1) 값에서 *2 를 해서 -1 ~ 1 사이의 값으로 조정한다.
			 // 2) 음수인 값은 양수로 바꿔준다.
			 // 그러면 반쪽 짜리 정규 분포가 만들어질 것이다.
			 float NormalDistVal = g_ParticleNormalDistValArray[ThreadID.x] * 2.f;

			 // 1.f 범위를 넘는 값들 조정하기 
			 if (NormalDistVal > 1.f)
				 NormalDistVal = Rand;
			 if (NormalDistVal < -1.f)
				 NormalDistVal = Rand * -1.f;

			 // 설정한 반지름 범위로 맞춰준다.
			 float TorchGenerateRadius = NormalDistVal * g_ParcticleGenerateRadius;

			 float3 StartMinBase = float3(-1.f, 0, -1.f);
			 StartMinBase *= TorchGenerateRadius;
			 float3 StartMaxBase = float3(1.f, 0, 1.f);
			 StartMaxBase *= TorchGenerateRadius;
			 float3 TorchPos = float3(0.f, 0.f, 0.f);
			 // TorchPos = Rand * (StartMaxBase - StartMinBase) + StartMinBase;
			 // TorchPos.y = 0.f;
			 TorchPos = float3(cos(RandomAngle) * TorchGenerateRadius, 0.f, sin(RandomAngle) * TorchGenerateRadius);

			 g_ParticleArray[ThreadID.x].WorldPos = TorchPos;
		 }

		 // 원뿔 형태로 위로 이동 (즉, 가운데에서 멀어질 수록, LifeTime이 작아지는 효과)
		// LifeTime 만 조정해줄 뿐,  //
		 if (g_ParticleLifeTimeLinear == 1)
		 {
			 // Spawn Pos 를 기준으로 하여 LifeTime 조정하기 
			 float SpawnStartPosDist = distance(g_ParticleArray[ThreadID.x].WorldPos, float3(0.f, 0.f, 0.f));
			 float DistMax = (float)(abs(max(g_ParticleStartMax, g_ParticleStartMin)));

			 // Circle Generate, Torch Generate 가 존재한다면 
			 if (g_ParticleGenerateCircle == 1 || g_ParticleGenerateTorch == 1)
				 DistMax = g_ParcticleGenerateRadius;

			 float SpawnPosRatio = (float)(SpawnStartPosDist / DistMax);

			 if (SpawnPosRatio > 1.f)
				 SpawnPosRatio = 0.01f;

			 g_ParticleArray[ThreadID.x].LifeTimeMax = (1 - SpawnPosRatio) * (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;
		 }
	}
	// 현재 생성이 되어 있는 파티클일 경우
	else
	{
		g_ParticleArray[ThreadID.x].LifeTime += g_DeltaTime;

		float3	MovePos = (float3)0.f;

		if (g_ParticleMove == 1)
		{
			MovePos = g_ParticleArray[ThreadID.x].Dir *
				g_ParticleArray[ThreadID.x].Speed * g_DeltaTime;

			/*g_ParticleArray[ThreadID.x].WorldPos += g_ParticleArray[ThreadID.x].Dir *
				g_ParticleArray[ThreadID.x].Speed * g_DeltaTime;*/
		}

		// 중력 적용
		if (g_ParticleShare[0].GravityEnable)
		{
			g_ParticleArray[ThreadID.x].FallTime += g_DeltaTime;

			float	Velocity = 0.f;

			if (g_ParticleArray[ThreadID.x].Dir.y > 0.f)
				Velocity = g_ParticleArray[ThreadID.x].Speed * g_ParticleArray[ThreadID.x].FallTime;

			g_ParticleArray[ThreadID.x].WorldPos.y = g_ParticleArray[ThreadID.x].FallStartY +
				(Velocity - 0.5f * GRAVITY * g_ParticleArray[ThreadID.x].FallTime * g_ParticleArray[ThreadID.x].FallTime * 10.f);

			// Bounce 효과를 낸다면 
			if (g_ParticleBounce == 1)
			{
				if (g_ParticleArray[ThreadID.x].InitWorldPosY >= g_ParticleArray[ThreadID.x].WorldPos.y)
				{
					g_ParticleArray[ThreadID.x].FallTime = 0.f;
					// g_ParticleArray[ThreadID.x].Speed *= 0.98f;
					g_ParticleArray[ThreadID.x].Speed *= g_ParticleBounceResist;
				}
			}

			g_ParticleArray[ThreadID.x].WorldPos.x += MovePos.x;
			g_ParticleArray[ThreadID.x].WorldPos.z += MovePos.z;
		}
		else
			g_ParticleArray[ThreadID.x].WorldPos += MovePos;

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

	float	Ratio = g_ParticleArraySRV[InstanceID].LifeTime / g_ParticleArraySRV[InstanceID].LifeTimeMax;

	float3	Scale = lerp(g_ParticleShareSRV[0].ScaleMin, g_ParticleShareSRV[0].ScaleMax,
		float3(Ratio, Ratio, Ratio));

	float4	Color = lerp(g_ParticleShareSRV[0].ColorMin, g_ParticleShareSRV[0].ColorMax,
		float4(Ratio, Ratio, Ratio, Ratio));

	float3x3 matRot = ComputeRotationMatrix(g_ParticleShareSRV[0].RotationAngle);

	// 4개의 최종 정점정보를 만들어준다.
	for (int i = 0; i < 4; ++i)
	{
		float3	WorldPos = g_ParticleArraySRV[InstanceID].WorldPos + mul(g_ParticleLocalPos[i] * Scale, matRot);

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
