// Objective: Define methods and functions that can be used when a instance of this class is created in another class (driving controls)
// Written by: Damith Tennakoon

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "RoverMovementComponent.generated.h"

// Import Static Mesh Component class
class UStaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MORELEARNING_API URoverMovementComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	URoverMovementComponent();

	// Pass references to the rovers wheel Static Mesh Components
	UStaticMeshComponent* FrontRightWheel;
	UStaticMeshComponent* FrontLeftWheel;
	UStaticMeshComponent* BackRightWheel;
	UStaticMeshComponent* BackLeftWheel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Define methods for moving rover
	void AccelerateRover(float InputValue);
	void TurnRover(float InputValue);

	// Define variables for rover movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoverMovement")
	float MaxSpeed = 1800.0f; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoverMovement")
	float TurnSpeed = 10000.0f;

};