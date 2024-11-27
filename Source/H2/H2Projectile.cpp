// Copyright Epic Games, Inc. All Rights Reserved.

#include "H2Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameStateShootScore.h"
#include "GameFramework/Character.h"
#include "H2Character.h"
#include "ScoreActor.h"
#include "TP_OnHitComponent.h"

AH2Projectile::AH2Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AH2Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	bReplicates = true;
	SetReplicatingMovement(true);
}

void AH2Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		if(!HasAuthority())
		{
			return;
		}
			
		if(OnwerCharacter)
		{
			if(UWorld* World = GetWorld())
			{

				int ScoreIncrease = 1;
				AScoreActor* ScoreActor = Cast<AScoreActor>(OtherActor);
				if(!ScoreActor)
				{
					return;
				}
				ScoreIncrease = ScoreActor->Score * (ScoreActor->IsSpecial()?2:1);
				AGameStateShootScore* GameState = World->GetGameState<AGameStateShootScore>();
				if (GameState)
				{
					OnwerCharacter->Score += ScoreIncrease;
					GameState->UpdateSomeoneScore(OnwerCharacter->ScoreArrayIndex, OnwerCharacter->Score);
					GameState->AddScore(ScoreIncrease);


				}
			}
		}
		
		Destroy();
	}
}