// DEPRACATED: WRONG CLASS NAMING
// Objective: Define methods and functions that can be used when a instance of this class is created in another class (driving controls)
// Written by: Damith Tennakoon

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

// Import Static Mesh Component class
class UStaticMeshComponent;

// Add parameters to Class type to make it openable in other classes
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MORELEARNING_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
