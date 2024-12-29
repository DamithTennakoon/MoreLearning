// Fill out your copyright notice in the Description page of Project Settings.
// (X=-411.745387,Y=1552.292433,Z=467.305060)

#include "Actors/PointCloudComponent.h"

// Sets default values
UPointCloudComponent::UPointCloudComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void UPointCloudComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPointCloudComponent::RenderPointCloud(TArray<FLidarPoint> InputLidarData, float InputRayDistance, FVector SensorLocation)
{
	// For each loop that iterates over each LidarPoint struct in the array of LidarPoints
	for (const FLidarPoint& CurLidarPoint : InputLidarData)
	{
		// Normalize the distance of the point
		float NormalizedDistance = CurLidarPoint.Distance / InputRayDistance;

		// Compute and retrieve the heat map colour value of the point
		FLinearColor HeatColour = GenerateHeatMap(NormalizedDistance);

		// Draw the point to the screen
		FVector DrawLocation = SensorLocation + CurLidarPoint.Location;

		if (RenderPointCloudParam)
		{
			DrawDebugPoint(GetWorld(), DrawLocation, 10.0f, HeatColour.ToFColor(true), false, 0.05f);
		}

	}
}

void UPointCloudComponent::RenderPointCloudWithMaterials(TArray<FSpectroPoint> InputSpectrometerData, float InputRayDistance, FVector SensorLocation)
{ 
	// For each loop that iterates over each SpectroPoint struct in the SpectroPoint struct array
	for (const FSpectroPoint& CurSpectroPoint : InputSpectrometerData)
	{
		// Compute the draw location vector for the point
		FVector DrawLocation = SensorLocation + CurSpectroPoint.Location;

		// Draw the point to the screen based on the material type
		if (CurSpectroPoint.MaterialType == "WaterIce")
		{
			if (RenderSpectralWaterChannelParam)
			{
				DrawDebugPoint(GetWorld(), DrawLocation, 30.0f, FColor::Cyan, false, 0.05f);
			}
		}
	}

}

FLinearColor UPointCloudComponent::GenerateHeatMap(float NormalizedInputPoint)
{
	return FLinearColor::LerpUsingHSV(FLinearColor::Green, FLinearColor::Red, NormalizedInputPoint);
}


