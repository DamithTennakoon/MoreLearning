// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "../Structures/LidarPoint.h"
#include "../Structures/SpectroPoint.h"
#include "PointCloudComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BluepprintSpawnableComponent))
class MORELEARNING_API UPointCloudComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPointCloudComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Declare methods/functions for this class
	void RenderPointCloud(TArray<FLidarPoint> InputLidarData, float InputRayDistance, FVector SensorLocation);

	// Declare method to render point cloud with materials
	void RenderPointCloudWithMaterials(TArray<FSpectroPoint> InputSpectrometerData, float InputRayDistance, FVector SensorLocation);

	// Declare method for generating a heat map for general lidar
	FLinearColor GenerateHeatMap(float NormalizedInputPoint);

	// Paramter to toggle on/off lidar point cloud 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointCloudParams")
	bool RenderPointCloudParam = false;

	// Paramter to toggle on/off lidar point cloud 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointCloudParams")
	bool RenderSpectralWaterChannelParam = false;
};
