// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/CanLunarRover.h"

// Import libraries for cameras, spring arms, and meshes
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
//#include "../../Public/Actors/RoverMovementComponent.h"


// Sets default values
ACanLunarRover::ACanLunarRover()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the rover body as the root component
	RoverBodyStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBody"));
	RootComponent = RoverBodyStaticMeshComp;

	// Create and attach the spring arm to the rover body
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RoverBodyStaticMeshComp); // Attach to the rover body

	// Create and attach the camera to the spring arm
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp); // Attach to the spring arm

	// Initialize wheel components and attach them to the rover body
	FrontLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontLeftWheel"));
	FrontLeftWheel->SetupAttachment(RoverBodyStaticMeshComp);

	FrontRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontRightWheel"));
	FrontRightWheel->SetupAttachment(RoverBodyStaticMeshComp);

	RearLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearLeftWheel"));
	RearLeftWheel->SetupAttachment(RoverBodyStaticMeshComp);

	RearRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearRightWheel"));
	RearRightWheel->SetupAttachment(RoverBodyStaticMeshComp);

	// Define RoverMovementComponent component
	RoverMovementComp = CreateDefaultSubobject<URoverMovementComponent>(TEXT("RoverMovementCompp"));

	// Pass references to the UStaticMeshComp pointers of the wheels of the RoverMovementComp
	RoverMovementComp->FrontRightWheel = FrontRightWheel;
	RoverMovementComp->FrontLeftWheel = FrontLeftWheel;
	RoverMovementComp->BackRightWheel = RearRightWheel;
	RoverMovementComp->BackLeftWheel = RearLeftWheel;

	// Initialize the LidarSensorStaticMeshComp static mesh and attach it as a child of the Rover Body static mesh
	LidarSensorStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidarSensorMeshComp"));
	LidarSensorStaticMeshComp->SetupAttachment(RoverBodyStaticMeshComp);

	// Construct the Spectrometer static mesh component and attach it as a child of the Rover Body static mesh
	SpectrometerStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpectrometerMeshComp"));
	SpectrometerStaticMeshComp->SetupAttachment(RoverBodyStaticMeshComp);

	// Construct the LidarSensorComp component
	LidarSensorComp = CreateDefaultSubobject<ULidarSensorComponent>(TEXT("LidarComputeComp"));

	// Construct the SpectrometerComp component
	SpectrometerComp = CreateDefaultSubobject<USpectrometerComponent>(TEXT("SpectrometerComputeComp"));

	// Construct a subobject of the PointCloudComponent component for the lidar point cloud rendering
	PointCloudComp = CreateDefaultSubobject<UPointCloudComponent>(TEXT("PointCloud"));

	// Construct a subobject for the RenderLidarPointCloudComp component
	RenderPointComp = CreateDefaultSubobject<USceneComponent>(TEXT("RenderPointCloud"));
	RenderPointComp->SetupAttachment(RoverBodyStaticMeshComp);

	// Construct a subobject for the RenderSpectrometerPointCloudComp component
	RenderSpectrometerPointCloudComp = CreateDefaultSubobject<USceneComponent>(TEXT("RenderSpectrometerPointCloud"));
	RenderSpectrometerPointCloudComp->SetupAttachment(RoverBodyStaticMeshComp);

}

// Called when the game starts or when spawned
void ACanLunarRover::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACanLunarRover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Perform lidar scan
	LidarSensorComp->ComputeLidarScan(LidarSensorStaticMeshComp->GetComponentLocation(), LidarSensorStaticMeshComp->GetComponentRotation());

	// Perform spectrometer scan
	SpectrometerComp->ComputeSpectrometerScan(SpectrometerStaticMeshComp->GetComponentLocation(), SpectrometerStaticMeshComp->GetComponentRotation());

	// Render the spectrometer point cloud
	PointCloudComp->RenderPointCloudWithMaterials(SpectrometerComp->SpectrometerDataStorage, SpectrometerComp->RayDistance, SpectrometerStaticMeshComp->GetComponentLocation());

	// Clear the spectrometer data vector before computer the next spectral scan
	SpectrometerComp->SpectrometerDataStorage.Empty();

	// Render the lidar point cloud
	PointCloudComp->RenderPointCloud(LidarSensorComp->LidarDataStorage, LidarSensorComp->RayDistance, RenderPointComp->GetComponentLocation());

	// Clear the lidar data vector before computing the next scan (efficiency)
	LidarSensorComp->LidarDataStorage.Empty(); 

}

// Called to bind functionality to input
void ACanLunarRover::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", RoverMovementComp, &URoverMovementComponent::AccelerateRover);
	PlayerInputComponent->BindAxis("MoveRight", RoverMovementComp, &URoverMovementComponent::TurnRover);
}

