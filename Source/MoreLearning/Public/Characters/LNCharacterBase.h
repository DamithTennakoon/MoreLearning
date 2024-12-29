// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LNCharacterBase.generated.h"

// Define REFERENCES to the CLASSES we want to use
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

UCLASS()
class MORELEARNING_API ALNCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALNCharacterBase();

	// Add spring arm component to add camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// Add a Camera Component to add our camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UStaticMeshComponent* MeshComp;

	virtual void BeginPlay() override;

// Only reelevant to things class and classes that inherits from this class (child classes)
protected:

	// Function definitions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void InteractPressed();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpAtRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float TraceDistance;
	UFUNCTION(BlueprintNativeEvent) // specifier makes it so the bulk of the logic is expected to be done in Blueprint
	void TraceForward();
	void TraceForward_Implementation();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

public:	

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AActor* FocusedActor; // Reference to the actor that we are currently looking at

};
