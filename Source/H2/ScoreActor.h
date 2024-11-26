// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreActor.generated.h"

class UBoxComponent;

UCLASS()
class H2_API AScoreActor : public AActor
{
	GENERATED_BODY()
	/** Box collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UBoxComponent* CollisionComp;

	/** Mesh Component */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category=Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;
	
public:	
	// Sets default values for this actor's properties
	AScoreActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
