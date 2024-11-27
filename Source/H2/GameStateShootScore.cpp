// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateShootScore.h"

#include "Net/UnrealNetwork.h"

AGameStateShootScore::AGameStateShootScore()
	:ScoreSum(0),NumOfPlayers(0),ReadyNum(0)
{
}

int AGameStateShootScore::GetScoreSum() const
{
	return ScoreSum;
}

int AGameStateShootScore::GetScore(int Index) const
{
	if(Index<0 || Index>=Scores.Num())
	{
		return 0;
	}
	return Scores[Index];
}

int AGameStateShootScore::GetNumOfPlayers() const
{
	return NumOfPlayers;
}

int AGameStateShootScore::UpdateSomeoneScore(int Index, int Score)
{
	if(Index<0 || Index>=Scores.Num())
	{
		return -1;
	}

	Scores[Index]=Score;
	return 0;
}


bool AGameStateShootScore::AddScore(int Score)
{
	ScoreSum+=Score;
	// if(GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Score Add: %d Sum: %d"), Score,ScoreSum));
	// }

	
	return true;
}

int AGameStateShootScore::Register()
{
	Scores.Push(0);
	NumOfPlayers++;
	return NumOfPlayers-1;
}

void AGameStateShootScore::GetOneReady()
{
	ReadyNum++;
}

void AGameStateShootScore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateShootScore,ScoreSum);
	DOREPLIFETIME(AGameStateShootScore,Scores);
	DOREPLIFETIME(AGameStateShootScore,NumOfPlayers);
	DOREPLIFETIME(AGameStateShootScore,ReadyNum);
	
}
