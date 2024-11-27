// Copyright Epic Games, Inc. All Rights Reserved.

#include "H2Character.h"
#include "H2Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameStateShootScore.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AH2Character

AH2Character::AH2Character()
{
	// Character doesn't have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	bReplicates = true;

}

void AH2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if (!HasAuthority())
	{
		RegisterScoreIndex();
	 }

	UE_LOG(LogTemp, Warning, TEXT("Begin Player :%d"), this->ScoreArrayIndex);
	

	
	

}

//////////////////////////////////////////////////////////////////////////// Input

void AH2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AH2Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AH2Character::Look);
	}
}

void AH2Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AH2Character, Score);
	DOREPLIFETIME(AH2Character, ScoreArrayIndex);
	
}

void AH2Character::GetReadyToServer_Implementation()
{
	UWorld* World = GetWorld();
	if(World)
	{
		AGameStateShootScore* GameState = World->GetGameState<AGameStateShootScore>();
		if (GameState)
		{
			GameState->GetOneReady();
		}
	}
}

void AH2Character::RegisterScoreIndex_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Player"));
	UWorld* World = GetWorld();
	int res = 0;
	if(World)
	{
		AGameStateShootScore* GameState = World->GetGameState<AGameStateShootScore>();
		if (GameState)
		{
			res = GameState->Register();
			this->ScoreArrayIndex = res;
		}
	}
}

void AH2Character::Fire_Implementation()
{
	if (GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			
			const FVector SpawnLocation = PlayerController->PlayerCameraManager->GetCameraLocation() + SpawnRotation.RotateVector( FVector(100.0f, 0.0f, 10.0f));

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			auto Projectile = World->SpawnActor<AH2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Projectile)
			{
				Projectile->OnwerCharacter = this;
			}
		}
	}
	
}


void AH2Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AH2Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AH2Character::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AH2Character::GetHasRifle()
{
	return bHasRifle;
}

void AH2Character::GetReady()
{
	if (!HasAuthority())
	{
		GetReadyToServer();
	}
}
