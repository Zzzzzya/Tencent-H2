// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "H2Character.h"
#include "H2Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent():bAttached(false)
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	
	Character->Fire();
}

void UTP_WeaponComponent::AttachWeapon(AH2Character* TargetCharacter)
{
	if(bAttached)
	{
		return;
	}
	
	Character = TargetCharacter;
	
	
	if (Character == nullptr)
	{
		return;
	}

	bAttached = true;
	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Player:%d"), Character->ScoreArrayIndex));
	// }
	
	UE_LOG(LogTemp, Warning, TEXT("Attach Weapon1！"));

	Character->ProjectileClass = ProjectileClass;
	
	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attach Weapon2！"));
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
}

// void UTP_WeaponComponent::Fire_Implementation()
// {
// 	if (Character == nullptr || Character->GetController() == nullptr)
// 	{
// 		return;
// 	}
//
// 	// Try and fire a projectile
// 	if (ProjectileClass != nullptr)
// 	{
// 		UWorld* const World = GetWorld();
// 		if (World != nullptr)
// 		{
// 			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
// 			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
// 			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
//
// 			FActorSpawnParameters ActorSpawnParams;
// 			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
//
// 			auto Projectile = World->SpawnActor<AH2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
// 			if (Projectile)
// 			{
// 				Projectile->OnwerCharacter = Character;
// 			}
// 		}
// 	}
//
// 	// Try and play the sound if specified
// 	if (FireSound != nullptr)
// 	{
// 		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
// 	}
//
// 	// Try and play a firing animation if specified
// 	if (FireAnimation != nullptr)
// 	{
// 		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
// 		if (AnimInstance != nullptr)
// 		{
// 			AnimInstance->Montage_Play(FireAnimation, 1.f);
// 		}
// 	}
// }
//
// bool UTP_WeaponComponent::Fire_Validate()
// {
// 	return true;
// }

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
