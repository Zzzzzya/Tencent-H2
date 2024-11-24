// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateShootScore.generated.h"

/**
 * 
 */
UCLASS()
class H2_API AGameStateShootScore : public AGameStateBase
{
	GENERATED_BODY()
public:
	AGameStateShootScore();

	int GetScoreSum() const;

	bool AddScore(int Score);

	int Register();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	int ScoreSum;
};
