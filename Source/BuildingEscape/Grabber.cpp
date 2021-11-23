// Joshua Pearson 2021 UE4

#include "Grabber.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

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

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLocationOfPlayerViewReach());
	}
}

void UGrabber::Grab() 
{
	FHitResult HitBody = GetFirstPhysicsBodyWithinReach();
	UPrimitiveComponent* ComponentToGrab = HitBody.GetComponent();

	if (HitBody.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetLocationOfPlayerViewReach());
	}
}

void UGrabber::Release() 
{
	UPrimitiveComponent* ComponentInHand = PhysicsHandle->GetGrabbedComponent();

	if (ComponentInHand)
	{
		PhysicsHandle->ReleaseComponent();
	}
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
	FVector LineTraceEnd = GetLocationOfPlayerViewReach();
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, 
		GetPlayerLocation(), 
		LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetLocationOfPlayerViewReach() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);
	return PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerLocation() const 
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);
	return PlayerViewLocation;
}