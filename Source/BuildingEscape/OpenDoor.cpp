// Joshua Pearson 2021 UE4

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw - DoorOpenAngle;

	FindAudioComponent();
	FindPressurePlate();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate)
	{
		if (GetTotalMassOfActors() >= MassToOpenDoor)
		{
			OpenDoor(DeltaTime);
			//Door last opened
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
		else 
		{
			if (GetWorld()->GetTimeSeconds() - DoorLastOpened >= DoorCloseDelayInSeconds)
			{
				CloseDoor(DeltaTime);
			}
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, DeltaTime, DoorOpenSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);

	if (!AudioComponent) { return; }
	if (!SoundPlayed)
	{
		AudioComponent->Play();
		SoundPlayed = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);

	if (!AudioComponent) { return; }
	if (SoundPlayed)
	{
		AudioComponent->Play();
		SoundPlayed = false;
	}
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (!PressurePlate) { return TotalMass; }

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass =+ Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	// Input bindings
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it but no PressurePlate set!"), *GetOwner()->GetName());
	}
}