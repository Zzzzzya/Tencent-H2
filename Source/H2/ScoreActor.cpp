// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreActor.h"

#include "H2Projectile.h"
#include "Components/BoxComponent.h"

// Sets default values
AScoreActor::AScoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionComp->BodyInstance.SetCollisionProfileName("PhysicsActor");
	CollisionComp->OnComponentHit.AddDynamic(this, &AScoreActor::OnHit);
	CollisionComp->SetSimulatePhysics(true);
	
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	

	bReplicates = true;
}

// Called when the game starts or when spawned
void AScoreActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AScoreActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr))
	{
		return;
	}

	
	if(OtherActor->IsA(AH2Projectile::StaticClass()))
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Hit!"));
		}
		MeshComp->SetWorldScale3D(FVector(2.0f,2.0f,2.0f));
	}
}

// Called every frame
void AScoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

