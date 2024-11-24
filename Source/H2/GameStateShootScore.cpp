// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateShootScore.h"

#include "Net/UnrealNetwork.h"

AGameStateShootScore::AGameStateShootScore()
	:ScoreSum(0)
{
}

int AGameStateShootScore::GetScoreSum() const
{
	return ScoreSum;
}



bool AGameStateShootScore::AddScore(int Score)
{
	ScoreSum+=Score;
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Score Add: %d Sum: %d"), Score,ScoreSum));
	}

	
	return true;
}

int AGameStateShootScore::Register()
{
	return 0;
}

void AGameStateShootScore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateShootScore,ScoreSum);
	
}
