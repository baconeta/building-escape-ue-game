// Joshua Pearson 2021 UE4

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

	// Get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);
	UE_LOG(LogTemp, Warning, TEXT("Player Viewpoint Location: %s, Player Viewpoint Rotation: %s"),
		*PlayerViewLocation.ToString(),
		*PlayerViewRotation.ToString());

	// Ray-cast out to a certain distance (reach)

	// See what we are hitting with it
}

