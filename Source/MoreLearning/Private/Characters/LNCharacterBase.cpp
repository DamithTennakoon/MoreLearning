// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LNCharacterBase.h"
// Include Spring Arm Component in CPP
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Interact/InteractInterface.h"

// Sets default values
ALNCharacterBase::ALNCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Getting the mesh to appear
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent); // Attach it to the parent component
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp); // Cameras are attached to spring arms

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(RootComponent);

	// Setup default values for turn rate and look rates
	BaseTurnRate = 45.0f;
	BaseLookUpAtRate = 45.0f;
	TraceDistance = 2000.0f;
}

void ALNCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ALNCharacterBase::OnOverlapBegin);
}

void ALNCharacterBase::MoveForward(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Direction we want to move
		AddMovementInput(Direction, Value);
	}
}

void ALNCharacterBase::MoveRight(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Direction we want to move
		AddMovementInput(Direction, Value);
	}
}

void ALNCharacterBase::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALNCharacterBase::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}

void ALNCharacterBase::InteractPressed()
{
	TraceForward();
	if (FocusedActor)
	{
		IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
		if (Interface)
		{
			Interface->Execute_OnInteract(FocusedActor, this);
		}
	}
}

void ALNCharacterBase::TraceForward_Implementation()
{
	// Find the characters viewpoint and trace it into the world.
	FVector Loc;
	FRotator Rot;
	FHitResult Hit; //why f?

	// Get the players start and end points
	GetController()->GetPlayerViewPoint(Loc, Rot); // Get the variables from the controller and fill in our variables with those values

	// Store the actual start point
	FVector Start = Loc; // Set it to the location we just recieved from the controller
	FVector End = Start + (Rot.Vector() * TraceDistance); // Converted Rotation into a vector -> but why?

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams); // This casts the line but nothing will show (not rendered). Use DrawDebug tool.

	// Draw the line using the "DrawDebugHelpers" library.
	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);

	// Check if we have hit something
	if (bHit) // If we are in this statement, we have hit something with the ray cast
	{
		//DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);

		// If we are looking at something, we want to find and store information about that class
		AActor* Interactable = Hit.GetActor(); // Getting a reference to actor that we hit

		if (Interactable) // Check that we have an interactable actor (kinda rudundant)
		{
			if (Interactable != FocusedActor)
			{
				if (FocusedActor)
				{
					IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
					if (Interface)
					{
						Interface->Execute_EndFocus(FocusedActor);
					}
				}
				IInteractInterface* Interface = Cast<IInteractInterface>(Interactable);
				if (Interface)
				{
					Interface->Execute_StartFocus(Interactable);
				}
				FocusedActor = Interactable;
			}
		}
		else
		{
			if (FocusedActor)
			{
				IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
				if (Interface)
				{
					Interface->Execute_EndFocus(FocusedActor);
				}
			}

			FocusedActor = nullptr;
		}
	}

}

void ALNCharacterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Interface = Cast<IInteractInterface>(OtherActor);
	if (Interface)
	{
		Interface->Execute_OnInteract(OtherActor, this);
	}
}

void ALNCharacterBase::Tick(float DeltaTime)
{
	TraceForward();
}

// Called to bind functionality to input
void ALNCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Provud input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALNCharacterBase::InteractPressed);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALNCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALNCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALNCharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALNCharacterBase::LookUpAtRate);
}



