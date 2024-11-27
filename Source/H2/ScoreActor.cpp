// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreActor.h"

#include "H2Projectile.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AScoreActor::AScoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AScoreActor::OnHit);

	
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetWorldScale3D(FVector(0.96f, 0.96f, 0.96f));
	
	Score = 1;
	nHitted = 0;
	maxHitNum = 1;
	ScaleUpRate = 1.2f;
	
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
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Hit!"));
		}

		nHitted++;
		if(GEngine)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("nHitted: %d MAXhIT: %d"), nHitted,maxHitNum));
		}
		if(maxHitNum >= 0 && nHitted>maxHitNum)
		{
			this->Destroy();
			return;
		}

		auto CurCollisionScale = CollisionComp->GetUnscaledBoxExtent();
		CollisionComp->SetBoxExtent(CurCollisionScale * ScaleUpRate);
		
		auto CurMeshScale = MeshComp->GetComponentScale();
		MeshComp->SetWorldScale3D(CurMeshScale * ScaleUpRate);
	}
}

// Called every frame
void AScoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AScoreActor, Score);
	DOREPLIFETIME(AScoreActor, maxHitNum);
	DOREPLIFETIME(AScoreActor, nHitted);
	DOREPLIFETIME(AScoreActor, CollisionComp);
	DOREPLIFETIME(AScoreActor, MeshComp);
	DOREPLIFETIME(AScoreActor, bIsSpecial);
	
}

void AScoreActor::UpdateScoreScale_Implementation(int sScore, FVector Scale,UMaterialInterface* InMaterial)
{
	this->Score = sScore;
	this->GetRootComponent()->SetWorldScale3D(Scale);
	MeshComp->SetMaterial(0,InMaterial);
}


void AScoreActor::CallDoubleScore_Implementation()
{
	Score*=2;
}

