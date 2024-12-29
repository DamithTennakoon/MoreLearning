// Objective: Define the properties of the Lunar Rover as a cubic static mesh.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Structures/LidarPoint.h"
#include "LunarRover.generated.h"

// Define classes require for spring arm, camera, and mesh
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class MORELEARNING_API ALunarRover : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALunarRover();

	// Define the Spring Arm component 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp; // Created a null pointer to Spring Arm we will eventually attach

	// Define the Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp; // Created a null pointer for the Camera object

	// Define a Static Mesh component (Base of the Rover)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UStaticMeshComponent* StaticMeshBody; // Created a null pointer for the Static Mesh object

	// Define a Satic Mesh component (Lidar Sensor)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UStaticMeshComponent* StaticMeshLidar;

	// Declare UScene component to render the point cloud
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneComp1")
	USceneComponent* PCDSceneComponent;

	//


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Define methods to that will be executed when input is provided by the user
	void MoveForward(float InputValue);
	void MoveSideways(float InputValue);
	void TurnAtRate(float InputValue);
	void LookUpAtRate(float InputValue);
	void OnLeftClick();
	void ComputeLidar(FVector Location, FRotator Rotation, float AzimuthRange, float ElevationRange);
	void GeneratePointCloud();
	FLinearColor GenerateHeatMap(float NormInputPoint);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Get the location and rotation values of the lidar component
	FVector LidarSensorLocation;
	FRotator LidarSensorRotation;
	float InputAzumithRange;
	float InputElevationRange;

	// Declare Vector of type LidarPoint from the struct
	TArray<FLidarPoint> LidarData;

};
