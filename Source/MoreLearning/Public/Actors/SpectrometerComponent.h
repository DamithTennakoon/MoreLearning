// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "../Structures/SpectroPoint.h"
#include "SpectrometerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta=(BlueprintSpawnableComponent))
class MORELEARNING_API USpectrometerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USpectrometerComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Define methods/functions of the class
	void ComputeSpectrometerScan(FVector SensorLocation, FRotator SensorRotation);

	// Define spectrometer scan paramters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectrometerParams")
	float AzimuthRange = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectrometerParams")
	float ElevationRange = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectrometerParams")
	float RayDistance = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectrometerParams")
	float AngularStepSize = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpectrometerParams")
	bool RenderSpectrometerParam = false;

	// Declare a Vector of the type SpectroPoint Struct
	TArray<FSpectroPoint> SpectrometerDataStorage;

private:
	// Define search tags for the different spectral channels
	FName WaterIceTag = "WaterIce";
};
