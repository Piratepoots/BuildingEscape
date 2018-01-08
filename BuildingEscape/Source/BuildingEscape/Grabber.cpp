// Copyright Piratepoots Media 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
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
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Missing PhysicsHandle Component"), *GetOwner()->GetName());
	}
}

/// Look For attached Input Component (only appears at runtime)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s : Missing Input Component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
		

}

void UGrabber::Grab()
{
	/// LINE TRACE and try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// line-trace (aka ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("I am hitting: %s"), *(ActorHit->GetName()));
	}

	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);  // GetPlayerViewPoint outputs to the FVector and FRotator so we don't need to set it, 
		// hence the OUT macro that we defined and put in there to tell us what it's doing.

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}
