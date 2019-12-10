// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Blueprint/UserWidget.h"

#include "TestCharacter.generated.h"

UCLASS()
class THIRDPERSONGAME_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestCharacter(); //constructor help us create object out of the class.

	UPROPERTY(VisibleAnyWhere, BluePrintReadOnly, Category = Camera)  /*VisibileAnyWhere - , BluePrintReadOnly - allow you to only read these variable in the blue print and not edit them.*/
		USpringArmComponent* CameraBoom; //A pointer - use (USpringArmComponent) to attach a camera to it and to hold the camera.
	
	UPROPERTY(VisibleAnyWhere, BluePrintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera; //also attach a camera to it.

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	bool bDead; //to determine if our charactor is dead or not dead, other words to determine if we are able to move or not.

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly) //if you dont include blueprintonly, then you will unable to get the power
		float Power;
	UPROPERTY(EditAnyWhere) //difference between VisibleAnyWhere and EditAnywhere, is that VisibleAnyWhere which you can see but can't edit, but EditAnywhere you can both see and edit that variable.
		float Power_Treshold;
	
	UFUNCTION() //declare a u function, this is going to tell the delegate system that the next function is going to be tied to a delegate.
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweetResult);//ignore all these parameters, we are ony going to use (OtherComp), we need to declare the parameter like this because this is the declaration of it.

	UPROPERTY(EditAnyWhere, Category = "UI HUD") //this function is to show health bar in game.
		TSubclassOf<UUserWidget> Player_Power_Widget_Class;
	UUserWidget* Player_Power_Widget;

	void RestartGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
