// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpectrometerComponent.h"

// Sets default values
USpectrometerComponent::USpectrometerComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void USpectrometerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USpectrometerComponent::ComputeSpectrometerScan(FVector SensorLocation, FRotator SensorRotation)
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

			// Store the water channel results if spectral signature is received (returns a hit for a tag)
			if (HitState)
			{
				// Check if the hit actor has a 'WaterIce' tag
				AActor* HitActor = HitRes.GetActor();
				if (HitActor->ActorHasTag(FName(WaterIceTag)))
				{
					// Create an instance of the of SpectroPoint struct and populate the struct properites -> prepare for adding to struct array
					FSpectroPoint Point;
					Point.Location = HitRes.ImpactPoint - StartLoc; // Compute the relative position vector
					Point.Distance = FVector::Distance(StartLoc, HitRes.ImpactPoint);
					Point.MaterialType = WaterIceTag;

					// Add the SpectroPoint stuct point to the SpectrometerDataStorage struct array
					SpectrometerDataStorage.Add(Point);

					// Render spectrometer rays
					if (RenderSpectrometerParam)
					{
						DrawDebugLine(GetWorld(), StartLoc, HitRes.ImpactPoint, FColor::Purple, false, 0.1f);
					}
				}
				else
				{
					
				}
			}
			else
			{
				// Render spectrometer rays
				if (RenderSpectrometerParam)
				{
					DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 0.1f);
				}
			}


		}
	}
}
