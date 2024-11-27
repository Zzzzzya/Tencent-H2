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

	UFUNCTION(BlueprintCallable, Category = "Score")
	int GetScoreSum() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int GetScore(int Index) const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int GetNumOfPlayers() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int UpdateSomeoneScore(int Index, int Score);
	
	bool AddScore(int Score);

	UFUNCTION(BlueprintCallable, Category = "Score")
	int Register();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void GetOneReady();

	UFUNCTION(BlueprintCallable,Category="Score")
	int GetReadyNum() const {return ReadyNum;}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(Replicated)
	int ScoreSum;

	UPROPERTY(Replicated)
	TArray<int> Scores;

	UPROPERTY(Replicated)
	int NumOfPlayers;

	UPROPERTY(Replicated)
	int ReadyNum;
};
