// Joshua Pearson 2021 UE4

#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	// Get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);
	

	// Ray-cast out to a certain distance (reach)
	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	// Raycast
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, 
		PlayerViewLocation, 
		LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// Visual (debug) representation of raycasting
	DrawDebugLine(
		GetWorld(), PlayerViewLocation, LineTraceEnd , FColor(0, 255, 0), false, 0.f, 0, 5.f
	);

	// See what we are hitting with it
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line trace hit %s."), *(ActorHit->GetName()));
	}
	
	
	
	// UE_LOG(LogTemp, Warning, TEXT("Player Viewpoint Location: %s, Player Viewpoint Rotation: %s"),
		// *PlayerViewLocation.ToString(),
		// *PlayerViewRotation.ToString());
}

