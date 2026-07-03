// AcousticIdentifier/Source/IceMoonAcousticField/Private/IMAcousticFieldMath.cpp

#include "IMAcousticFieldMath.h"

void IMAcousticFieldMath::GetFibonacciSphereSamples(
	TArray<FVector>& OutSamples,
	int32 NumSamples,
	const FVector& Direction,
	float ConeDegree,
	bool bUniform,
	int32 RandomSeed)
{
	OutSamples.Empty(NumSamples);
	if (NumSamples <= 0) return;

	const FVector Dir = Direction.IsNearlyZero() ? FVector::UpVector : Direction.GetSafeNormal();
	const float HalfConeRad = FMath::DegreesToRadians(FMath::Clamp(ConeDegree * 0.5f, 0.0f, 180.0f));
	const float CosHalfCone = FMath::Cos(HalfConeRad);

	// 构建锥体局部坐标系
	FVector Right, Forward;
	Dir.FindBestAxisVectors(Right, Forward);

	FRandomStream Random(RandomSeed >= 0 ? RandomSeed : FMath::Rand());

	if (bUniform)
	{
		// Fibonacci球面均匀采样算法
		const float GoldenRatio = (1.0f + FMath::Sqrt(5.0f)) * 0.5f;

		for (int32 i = 0; i < NumSamples; ++i)
		{
			// 使用 Fibonacci sphere 在锥体半角内采样
			// 将标准 Fibonacci sphere (覆盖全球) 映射到锥体内部
			const float Phi = 2.0f * PI * static_cast<float>(i) / GoldenRatio;
			const float CosTheta = 1.0f - (1.0f - CosHalfCone) * (static_cast<float>(i) + 0.5f) / static_cast<float>(NumSamples);
			const float SinTheta = FMath::Sqrt(1.0f - CosTheta * CosTheta);

			const FVector LocalDir(
				SinTheta * FMath::Cos(Phi),
				SinTheta * FMath::Sin(Phi),
				CosTheta
			);

			// 转换到世界空间
			OutSamples.Add(Dir * LocalDir.Z + Right * LocalDir.X + Forward * LocalDir.Y);
		}
	}
	else
	{
		// 锥体内随机采样
		for (int32 i = 0; i < NumSamples; ++i)
		{
			const float Z = FMath::RandRange(CosHalfCone, 1.0f);
			const float Theta = FMath::Acos(Z);
			const float Phi = Random.FRand() * 2.0f * PI;

			const FVector LocalDir(
				FMath::Sin(Theta) * FMath::Cos(Phi),
				FMath::Sin(Theta) * FMath::Sin(Phi),
				Z
			);

			OutSamples.Add(Dir * LocalDir.Z + Right * LocalDir.X + Forward * LocalDir.Y);
		}
	}
}
