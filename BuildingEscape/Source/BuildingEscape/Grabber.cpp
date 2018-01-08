// Copyright Piratepoots Media 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UGrabber::FindPhysicsHandleComponent();
	UGrabber::SetupInputComponent();
}

/// Look For attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics handle is found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s : Missing PhysicsHandle Component"), *GetOwner()->GetName());
	}
}

/// Look For attached Input Component (only appears at runtime)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found."));
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s : Missing Input Component"), *GetOwner()->GetName());
	}
}



void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));

	/// LINE TRACE and try and reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();

	/// If we hit something then attach a physics handle
	// TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));

	// TODO release physics handle
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
		// move the object that we're holding

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);  // GetPlayerViewPoint outputs to the FVector and FRotator so we don't need to set it, 
		// hence the OUT macro that we defined and put in there to tell us what it's doing.

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	/// Setup query parameters 
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// line-trace (aka ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("I am hitting: %s"), *(ActorHit->GetName()));
	}

	return FHitResult();
}