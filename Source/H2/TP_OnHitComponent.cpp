// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_OnHitComponent.h"

// Sets default values for this component's properties
UTP_OnHitComponent::UTP_OnHitComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTP_OnHitComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTP_OnHitComponent::ScaleUp()
{
	AActor* Owner = GetOwner();
	if(Owner)
	{
		FVector NewScale = Owner->GetActorScale3D() * 1.2f;
		Owner->SetActorScale3D(NewScale);
	}
}




