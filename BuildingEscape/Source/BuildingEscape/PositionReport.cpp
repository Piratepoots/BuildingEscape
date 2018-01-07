// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"
#include "Gameframework/Actor.h" // as of 4.17, IWYU (include what you use) was introduced so now we need to include header files that we need.


// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();  // "->" instead of "." because this (GetOwner) is a pointer.
	UE_LOG(LogTemp, Warning, TEXT("PositionReport reporting for %s!"), *ObjectName); // Prints log to the console (Output log).  This Warning Log is telling what item is reporting in.
	
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

