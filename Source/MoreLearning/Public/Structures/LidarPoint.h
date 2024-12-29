// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LidarPoint.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MORELEARNING_API FLidarPoint
{
public:
	GENERATED_BODY()

	// Define FVector to store the location of the hit point
	UPROPERTY(BlueprintReadWrite) // Allows struct to be edited and read in blueprints
	FVector Location; 

	// Define Float to store the distance of each hit point to the Lidar sensor origin
	UPROPERTY(BlueprintReadWrite) 
	float Distance;

	FLidarPoint();
	~FLidarPoint();
};
