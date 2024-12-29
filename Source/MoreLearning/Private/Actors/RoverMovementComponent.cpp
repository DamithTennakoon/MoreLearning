// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RoverMovementComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
URoverMovementComponent::URoverMovementComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void URoverMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void URoverMovementComponent::AccelerateRover(float InputValue)
{
	// Check whether Static Mesh Componenent references that been pointed to the wheel components (not nullptr)
	if (!FrontRightWheel || !FrontLeftWheel || !BackRightWheel || !BackLeftWheel) return; // Exit the function if any are nullptr

	// Extract the Forward vector of the inherired class' root component and multiply with input values to compute desired force
	FVector MoveForce = GetOwner()->GetActorForwardVector() * MaxSpeed * InputValue;

	// Set the force on each wheel 
	FrontRightWheel->AddForce(MoveForce);
	FrontLeftWheel->AddForce(MoveForce);
	BackRightWheel->AddForce(MoveForce);
	BackLeftWheel->AddForce(MoveForce);
}

void URoverMovementComponent::TurnRover(float InputValue)
{
	// Check whether Static Mesh Componenent references that been pointed to the wheel components (not nullptr)
	if (!FrontRightWheel || !FrontLeftWheel || !BackRightWheel || !BackLeftWheel) return; // Exit the function if any are nullptr

	// Extract the Right vector of the inherired class' root component and multiply with input values to compute desired force
	FVector TurnForce = GetOwner()->GetActorRightVector() * MaxSpeed * InputValue;

	// Set the force on each wheel 
	FrontRightWheel->AddForce(TurnForce);
	FrontLeftWheel->AddForce(TurnForce);
	BackRightWheel->AddForce(-TurnForce);
	BackLeftWheel->AddForce(-TurnForce);
}
