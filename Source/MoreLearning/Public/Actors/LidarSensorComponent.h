// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h" //Required to define a custom UActorComponent class (current AActor class)
#include "../Structures/LidarPoint.h"
#include "LidarSensorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MORELEARNING_API ULidarSensorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ULidarSensorComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Define functions for this class here
	void ComputeLidarScan(FVector SensorLocation, FRotator SensorRotation);

	// Define paramters for the Lidar scan
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LidarParams")
	float AzimuthRange = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LidarParams")
	float ElevationRange = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LidarParams")
	float RayDistance = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LidarParams")
	float AngularStepSize = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LidarParams")
	bool DrawLidarParam = true;

	// Declare a Vector of the type LidarPoint Struct
	TArray<FLidarPoint> LidarDataStorage;
};
