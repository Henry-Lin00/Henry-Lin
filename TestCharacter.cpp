// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h" //framerworks
#include "Kismet/GameplayStatics.h" //gmaeplaystatics for restarting the game

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //a pointer, also can change it in Unreal engine under CapsuleComponent

	bUseControllerRotationPitch = false;	//all these three will not allow the character to rotate instead it will allow the camera to rotate.
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //a pointer, allow the character to rotate in the direction it is moving.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //(x, y, z) rotation rate, rate by wich we are going to rotate
	GetCharacterMovement()->JumpZVelocity = 600.0f; //when jump the velocity will be 600
	GetCharacterMovement()->AirControl = 0.2f; //how much we are going to control our charactor in air

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); //CreateDefaultSubobject is a type of USpringArmComponent, the name cameraboom is the name that will appear on the testcharacter_bp manu for USpringArmComponent 
	CameraBoom->SetupAttachment(RootComponent); //attach to rootcomponent, the rootcomponent in this case is capsulecomponent therefore cameraboom will become a child of the capsulecomponent.

	CameraBoom->TargetArmLength = 300.0f; //how far away the camera(springArm) will be away from the robot(user).
	CameraBoom->bUsePawnControlRotation = true; //rotate the arm base on the controller.
	//inorder for us to test the rotation of the arm base on the controller we have to create a (camera).
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); 
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);//attach this camera to cameraboom, this is going to attach the camera at the end of the boom and let the boom adjust the match controller rotation of the camera.
	FollowCamera->bUsePawnControlRotation = false; //the camera does not rotate relaltive to the arm.

	bDead = false; 
	Power = 100.0f;


}






// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//bind the OnBeginOverlap with the component, if we dont bind that with the capsule component then which means we can not trigger it when we collided with a actor(game object).
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATestCharacter::OnBeginOverlap);

	if (Player_Power_Widget_Class != nullptr) //we are testing this because we want to make sure its not equal to the null pointer, the reason for this is because we might have not attach the (Player_Power_UI)health bar under (UI HUD)that we have created. 
	{
		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();
	}
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); //Tick is called every frame

	Power -= DeltaTime * Power_Treshold; //subtract the power


	if (Power <= 0)
	{
		if (!bDead)
		{
			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
				UnusedHandle, this, &ATestCharacter::RestartGame, 3.0f, false);
			/*this get worldtimermanager is goint to setTimer(set the timer), by using UnusedHandle(in the above, it is a parameter we need to pass inside of this(this class)), 
			<&ATestCharacter::Restart> is the function we need to excute after three seconds(3.0f), and false because we dont want to repeatedly call this funciton after three second. */
		
		}
	}
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//place move function after input to bind them 
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); /*binding (turn) in (setting) under (input) in (Axis Mappings), (this) refering to this class(the function we are in right now), we are blinding it to (APawn-controllerYawInput) which allow us to contoll the yaw of the player(the yaw is the rotation of the player). (the yaw is under mesh in testcharacter_BP, in transform-rotation-Z(z is the yaw, which you can left click it and hold it then move left and right to see the difference)*/
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput); //by blinding pitch and yaw, now you can move the camera of the character to watch where he is.

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump); //blind a action and the name of the action is going to be jump, and we are going to bind the press and release of it., (ACharacter::Jump) is a function inside of the (character class) that we can use to jump.
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	/*all these are build in functions, (AddControllerYawInput) and (AddControllerPitchInput) are build in functions inside of APawn.
	and (Jump) and (StopJumping) are also build in function but inside of ACharacter. because we inherited the character inside of the TestCharacter.h which mean we have also inherited all its funciton*/

	/*now bind our move forward and move right
	IMPORTANT- the name we create have to match to the name in the Unreal Engine project settings. EX: MoveForward=MoveForward*/
	PlayerInputComponent->BindAxis("MoveForward", this, &ATestCharacter::MoveForward); //this is going to bind the moveforward action which is under (setting-input-axis mappings). so when we do that we will call the moveforward function.
	PlayerInputComponent->BindAxis("MoveRight", this, &ATestCharacter::MoveRight);

}

void ATestCharacter::MoveForward(float Axis)
{
	if (!bDead) //if my character is not dead then I am able to move the character.
	{
		const FRotator Rotation = Controller->GetControlRotation(); //create a constant 
		const FRotator YawRotation(0, Rotation.Yaw, 0); // getting the rotation to find out the forward direction.(x,y,z)-formate for the parameter

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //calculate the forward. direction is the forward direction, we are passing the (YawRotation). this is going to calculate the forward direction of this vector.
		AddMovementInput(Direction, Axis); //this Axis is this Axis(ATestCharacter::MoveForward(float Axis)).
	
	}
}
void ATestCharacter::MoveRight(float Axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation(); //create a constant 
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //remember this is Y axis not X
		AddMovementInput(Direction, Axis);
	}

}

void ATestCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false); 
	//open the level inside of this(refering to this class) and get the world and name(getting the name of the world), we use Astrix(->) because we dealing with a pointer, and false. 
}

void ATestCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweetResult)
{
	if (OtherActor->ActorHasTag("Recharge")) //OtherActor is the OtherActor that we have collided with,recharge is the name of the tag
	{
		//UE_LOG(LogTemp, Warning, TEXT("COLLided with")); //use UE_LOG to print to the console. we are testing if the other actor has a tag recharged
		Power += 10.0f; //to add to the power 10
		if (Power > 100.0f)//test if the power is greater then 100.0f
			Power = 100.0f; //not allowing the power to go above 100

		OtherActor->Destroy(); //remove OtherActor
	} 
}
