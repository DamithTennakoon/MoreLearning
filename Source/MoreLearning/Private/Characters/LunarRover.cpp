// Objective: Attach visuals for the Lunar Rover actor object and provide controls.


#include "Characters/LunarRover.h"
// Import libraries for cameras, spring arms, and meshes
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
// Import libraries for user input


// Sets default values
ALunarRover::ALunarRover()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Contruct objects for camera, spring arm, and mesh onto the character 
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent); // Set the object to be the parent of the character

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp); // Attach as a child of the the spring arm

	StaticMeshBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh1"));
	StaticMeshBody->SetupAttachment(RootComponent); // Attach to the parent of the character

	StaticMeshLidar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh2"));
	StaticMeshLidar->SetupAttachment(StaticMeshBody);

	PCDSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp1"));
	PCDSceneComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALunarRover::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ALunarRover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (StaticMeshLidar)
	{
		// Get and store location and rotation of the object
		LidarSensorLocation = StaticMeshLidar->GetComponentLocation();
		LidarSensorRotation = StaticMeshLidar->GetComponentRotation();

		ComputeLidar(LidarSensorLocation, LidarSensorRotation, 45.0f, 45.0f);
		GeneratePointCloud();
		LidarData.Empty(); // Empty the Lidar data

	}

}

void ALunarRover::MoveForward(float InputValue)
{
	// Check if a Controller object is defined for this character 
	if (Controller)
	{
		// Check if there is a non-zero (valid) input from the user
		if (InputValue != 0.0f)
		{
			const FRotator Rotation = Controller->GetControlRotation(); // Get the rotation of the character (roll, yaw, pitch) and store in Rotatation variable
			const FRotator YawRotation(0, Rotation.Yaw, 0); // Create a new FRotation variable that only takes the yaw rotation angle

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Perform matrix manipulation to compute direction we want to move in
			AddMovementInput(Direction, InputValue);
		}
	}
}

void ALunarRover::MoveSideways(float InputValue)
{
	if (Controller)
	{
		if (InputValue != 0.0f)
		{
			//const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, InputValue);
		}
	}
}

void ALunarRover::TurnAtRate(float InputValue)
{
}

void ALunarRover::LookUpAtRate(float InputValue)
{
}

void ALunarRover::OnLeftClick()
{
	// Print our that the user clicked, to the screen
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("[INFO] LEFT CLICK DETECTED"));
}

void ALunarRover::ComputeLidar(FVector Location, FRotator Rotation, float AzimuthRange, float ElevationRange)
{
	// Define fixed paramter for degree step-size
	float StepSize = 1.5f;

	// Outer loop for sweeping the Elevation Angles
	for (float Elevation = -ElevationRange; Elevation <= ElevationRange; Elevation += StepSize)
	{
		// Inner loop for sweeping the Azumith Angles
		for (float Azimuth = -AzimuthRange; Azimuth <= AzimuthRange; Azimuth += StepSize)
		{
			// Create an instance of the rotation angles and slightly modify it based on the current Az, El angle
			FRotator RayRotation = Rotation;
			RayRotation.Yaw += Azimuth; // Add the current Az angle to the initial yaw angle of the lidar sensor
			RayRotation.Pitch += Elevation;

			// Define start and end locations for the ray casting
			FVector Start = Location;
			FVector End = Start + (RayRotation.Vector() * 700.0f);

			// Cast the ray
			FHitResult Hit;
			FCollisionQueryParams TraceParams;
			bool hitState = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

			// Change ray colour drawn based on hit result
			if (hitState)
			{
				// Create an instance of the FLidarPoint struct and populate the paramters
				FLidarPoint Point; 
				Point.Location = Hit.ImpactPoint - Start; // Compute relative to the lidar sensor
				Point.Distance = FVector::Distance(Start, Hit.ImpactPoint);
				// Append point to Lidar Points array
				LidarData.Add(Point);

				DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Red, false, 0.1f);
			}
			else
			{
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.05f);
			}
		}
	}

}

void ALunarRover::GeneratePointCloud()
{
	// For each loop to iterate over all lidar data
	for (const FLidarPoint& CurLidarPoint : LidarData)
	{
		// Normalize the distance of the hit point
		float NormalizeDistance = CurLidarPoint.Distance / 700.0f;
		float ScaledNormalizedDistance = FMath::Clamp(NormalizeDistance * 40.0f, 0.0f, 1.0f);

		FLinearColor HeatColour = GenerateHeatMap(NormalizeDistance);

		// Draw PCD to screen
		FVector RootLocation = PCDSceneComponent->GetComponentLocation(); // Define the root location of the drawing location
		FVector DrawLocation = RootLocation + CurLidarPoint.Location;
		DrawDebugPoint(GetWorld(), DrawLocation, 10.0f, HeatColour.ToFColor(true), false, 0.1f);
	}

}

FLinearColor ALunarRover::GenerateHeatMap(float NormInputPoint)
{
	// Interpolate betweeen blue (closer) and red (further)
	return FLinearColor::LerpUsingHSV(FLinearColor::Yellow, FLinearColor::Red, NormInputPoint);
}

// Called to bind functionality to input
void ALunarRover::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Execute functions to control the Lunar Rover character when input is provided
	PlayerInputComponent->BindAxis("MoveForward", this, &ALunarRover::MoveForward); // We are executing functions in the ALunarRover class
	PlayerInputComponent->BindAxis("MoveRight", this, &ALunarRover::MoveSideways);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); // We are executing functions in the APawn class
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALunarRover::OnLeftClick); // We are using the InputEvent_Pressed method to run our custom function
	
}

