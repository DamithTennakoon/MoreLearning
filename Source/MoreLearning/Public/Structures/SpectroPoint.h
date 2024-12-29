// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpectroPoint.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MORELEARNING_API FSpectroPoint
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	float Distance;

	UPROPERTY(BlueprintReadWrite)
	FName MaterialType;

	FSpectroPoint();
	~FSpectroPoint();
};
