// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter_GameMode.h"
#include "GameFramework/Actor.h"


ATestCharacter_GameMode::ATestCharacter_GameMode()
{
	PrimaryActorTick.bCanEverTick = true; //we want to tick, and call the tick function


}

void ATestCharacter_GameMode::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ATestCharacter_GameMode::SpawnPlayerRecharge,
		FMath::RandRange(2, 5), true);

}

void ATestCharacter_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestCharacter_GameMode::SpawnPlayerRecharge()
{
	float RandX = FMath::RandRange(Spawn_X_Min, Spawn_X_Max);
	float RandY = FMath::RandRange(Spawn_Y_Min, Spawn_Y_Max);

	FVector SpawnPosition = FVector(RandX, RandY, Spawn_Z);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(PlayerRecharge, &SpawnPosition, &SpawnRotation);
}
