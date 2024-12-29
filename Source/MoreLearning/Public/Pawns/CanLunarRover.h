// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Actors/RoverMovementComponent.h"
#include "../Actors/LidarSensorComponent.h"
#include "../Actors/SpectrometerComponent.h"
#include "../Actors/PointCloudComponent.h"
#include "CanLunarRover.generated.h"

// Import classes for visual and physics components
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class MORELEARNING_API ACanLunarRover : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACanLunarRover();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneComp")
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMeshComp")
	UStaticMeshComponent* RoverBodyStaticMeshComp;

	// Define the Spring Arm component 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp; // Created a null pointer to Spring Arm we will eventually attach

	// Define the Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp; // Created a null pointer for the Camera object

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FrontLeftWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FrontRightWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RearLeftWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RearRightWheel;

	// Declare a Static Mesh Component that will store the visual for the Lidar sensor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LidarSensorStaticMeshComp;

	// Declare a Static Mesh Component that will store the visual for the Spectrometer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SpectrometerStaticMeshComp;

	// Declare an empty object (USceneComponent) to render the lidar point cloud
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RenderPointComp;

	// Declare an empty object (USceneComponent) to render the spectrometer point cloud
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RenderSpectrometerPointCloudComp;

	// Declare a reference to the LidarSensorComponent class
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULidarSensorComponent* LidarSensorComp;

	// Declare a reference to the  SpectrometerComponent class
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpectrometerComponent* SpectrometerComp;

	// Declare a reference to the PointCloudComponent class for rendering the lidar point cloud
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPointCloudComponent* PointCloudComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Declare an reference to the RoverMovementComponent class
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URoverMovementComponent* RoverMovementComp;

};
