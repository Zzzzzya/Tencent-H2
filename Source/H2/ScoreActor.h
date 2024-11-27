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
	UPROPERTY(VisibleDefaultsOnly,Replicated, Category=Projectile)
	UBoxComponent* CollisionComp;

	/** Mesh Component */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Replicated, Category=Projectile, meta = (AllowPrivateAccess = "true"))
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

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void SetSpecial(bool Special){bIsSpecial = Special;}

	UFUNCTION(BlueprintCallable,Category="Score")
	bool IsSpecial(){return bIsSpecial;}

	UFUNCTION(BlueprintCallable,Category="Score")
	void DoubleScore()
	{
		if(!HasAuthority())
		{
			CallDoubleScore();
		}
	}

	UFUNCTION(NetMulticast,Reliable,BlueprintCallable,Category="Score")
	void UpdateScoreScale(int sScore,FVector Scale,UMaterialInterface* InMaterial);
	
	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Score")
	void CallDoubleScore();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated, Category = "Score")
	bool bIsSpecial = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,Category = "Score")
	int Score;

	UPROPERTY(EditDefaultsOnly,Replicated, Category = "Score")
	int maxHitNum;

	UPROPERTY(EditDefaultsOnly,Replicated, Category = "Score")
	float ScaleUpRate;

	
private:
	UPROPERTY(Replicated)
	int nHitted;

};
