// Joshua Pearson 2021 UE4

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindAudioComponent();
	void FindPressurePlate();
	float GetTotalMassOfActors() const;

private:
	float InitialYaw;
	float CurrentYaw;

	float TargetYaw = 90.f;
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelayInSeconds = 5.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 1.5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 3.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	bool SoundPlayed = false;
};
