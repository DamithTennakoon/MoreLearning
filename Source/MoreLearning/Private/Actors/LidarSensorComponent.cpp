// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LidarSensorComponent.h"

// Sets default values
ULidarSensorComponent::ULidarSensorComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ULidarSensorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void ULidarSensorComponent::ComputeLidarScan(FVector SensorLocation, FRotator SensorRotation)
{
	// Outer loop for the Elevation sweep
	for (float CurElevationAngle = -ElevationRange; CurElevationAngle <= ElevationRange; CurElevationAngle += AngularStepSize)
	{
		// Inner loop for the Azimuth sweep
		for (float CurAzimuthAngle = -AzimuthRange; CurAzimuthAngle <= AzimuthRange; CurAzimuthAngle += AngularStepSize)
		{
			// Create an instance of the Sensor Static Meshs' Rotator so that it can be tweeked to be used as a ray direction
			FRotator RayRotation = SensorRotation;
			RayRotation.Yaw += CurAzimuthAngle; // Add the current azumith angle to the yaw axis of the rotation object
			RayRotation.Pitch += CurElevationAngle; // Add the current elevation angle to the pitch axis of the rotation object -> Now the ray is point at the target direection

			// Define the start and end locations of the ray cast (just the Sensor mesh component location)
			FVector StartLoc = SensorLocation;
			FVector EndLoc = StartLoc + (RayRotation.Vector() * RayDistance); //Take the unit vector of the set direction and multiply is by the desired ray depth

			// Define ray cast parameters
			FHitResult HitRes; // Define an object to store the hit results
			FCollisionQueryParams TraceParams; // Define an object to out-store the collision data

			// Cast the ray
			bool HitState = GetWorld()->LineTraceSingleByChannel(HitRes, StartLoc, EndLoc, ECC_Visibility, TraceParams);

			// Draw the colours of the ray based on hit results
			if (HitState)
			{
				// Create a local instance of the LidarPoint Struct -> Attach to the LidarData vector once the struct has been filled
				FLidarPoint Point;
				Point.Location = HitRes.ImpactPoint - StartLoc; // Compute the relative position
				Point.Distance = FVector::Distance(StartLoc, HitRes.ImpactPoint); // Compute depth to the hit point

				// Add the LidarPoint object to the array of LidarPoints
				LidarDataStorage.Add(Point);

				if (DrawLidarParam)
				{
					DrawDebugLine(GetWorld(), StartLoc, HitRes.ImpactPoint, FColor::Red, false, 0.1f);
				}
				
			}
			else
			{
				if (DrawLidarParam)
				{
					DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Blue, false, 0.1f);
				}
			}

		}
	}

}

