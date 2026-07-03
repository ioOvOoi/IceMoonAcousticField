// AcousticIdentifier/Source/IceMoonAcousticField/Private/IMAcousticFieldMath.h

#pragma once

#include "CoreMinimal.h"

/**
 * 本地声学数学工具 - 替代 IMMathUtils 依赖
 * 提供 remap、smoothstep、Fibonacci球面采样等常用数学函数
 */
namespace IMAcousticFieldMath
{
	/**
	 * 值重映射（带饱和）
	 * 将 Value 从 [InputMin, InputMax] 映射到 [OutputMin, OutputMax]，超出范围时钳制
	 */
	template<typename T>
	FORCEINLINE T Remap_Sat(T InputMin, T InputMax, T OutputMin, T OutputMax, T Value)
	{
		if (FMath::IsNearlyEqual(InputMin, InputMax))
		{
			return (Value >= InputMax) ? OutputMax : OutputMin;
		}
		const T TValue = (Value - InputMin) / (InputMax - InputMin);
		const T Clamped = FMath::Clamp(TValue, static_cast<T>(0), static_cast<T>(1));
		return OutputMin + Clamped * (OutputMax - OutputMin);
	}

	/**
	 * Hermite平滑插值（带饱和）
	 * 将 Value 从 [Edge0, Edge1] 区间做 smooth Hermite 映射到 [0, 1]，超出钳制
	 */
	template<typename T>
	FORCEINLINE T Smoothstep_Sat(T Edge0, T Edge1, T Value)
	{
		if (FMath::IsNearlyEqual(Edge0, Edge1))
		{
			return (Value >= Edge1) ? static_cast<T>(1) : static_cast<T>(0);
		}
		const T TValue = (Value - Edge0) / (Edge1 - Edge0);
		const T Clamped = FMath::Clamp(TValue, static_cast<T>(0), static_cast<T>(1));
		return Clamped * Clamped * (static_cast<T>(3) - static_cast<T>(2) * Clamped);
	}

	/**
	 * Fibonacci球面采样 - 在方向锥体内生成均匀分布的采样方向
	 * @param OutSamples 输出的采样方向数组
	 * @param NumSamples 采样数量
	 * @param Direction 锥体中心方向（需要归一化）
	 * @param ConeDegree 锥体半角（度），180度覆盖全球
	 * @param bUniform 是否均匀分布（true=Fibonacci球面均匀，false=锥体内随机）
	 * @param RandomSeed 随机种子（-1使用默认种子）
	 */
	void GetFibonacciSphereSamples(
		TArray<FVector>& OutSamples,
		int32 NumSamples,
		const FVector& Direction,
		float ConeDegree,
		bool bUniform = true,
		int32 RandomSeed = -1);
}
