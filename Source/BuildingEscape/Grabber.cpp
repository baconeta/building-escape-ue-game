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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

					/* Visual (debug) representation of raycasting */
	// DrawDebugLine(
	// 	GetWorld(), PlayerViewLocation, LineTraceEnd , FColor(0, 255, 0), false, 0.f, 0, 5.f
	// );

	// See what we are hitting with it
	
}

void UGrabber::Grab() 
{
	GetFirstPhysicsBodyWithinReach();

	// Try and reach any actors with physics body collision channel set

	// If we hit something then attach the physics handle.
}

void UGrabber::Release() 
{
	// Remove/release the physics handle
}

void UGrabber::FindPhysicsHandle() 
{
	// Check for PhysicsHandle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("There is no PhysicsHandleComponent attached to %s."), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	// Input bindings
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyWithinReach() const 
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, 
		PlayerViewLocation, 
		LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s."), *(ActorHit->GetName()));
	}

	return Hit;
}