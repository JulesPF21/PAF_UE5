// Fill out your copyright notice in the Description page of Project Settings.


#include "CppDeathZone.h"

#include "GameFramework/Character.h"

// Sets default values
ACppDeathZone::ACppDeathZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SceneRoot->SetMobility(EComponentMobility::Movable);
	
	RespawnCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RespawnCapsule"));
	DeathZoneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathZoneBox"));

	DeathZoneBox->SetupAttachment(SceneRoot);
	RespawnCapsule->SetupAttachment(SceneRoot);

	DeathZoneSize = FVector(1, 1, 1);
	RespawnLocation = FVector(0, 0, 0);

	DeathZoneBox->OnComponentBeginOverlap.AddDynamic(this, &ACppDeathZone::OnDeathZoneOverlap);
}

// Called when the game starts or when spawned
void ACppDeathZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACppDeathZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACppDeathZone::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (RespawnCapsule)
	{
		RespawnCapsule->SetRelativeLocation(RespawnLocation);
	}

	if (DeathZoneBox)
	{
		DeathZoneBox->SetBoxExtent(DeathZoneSize * 32.0f);
	}
}


void ACppDeathZone::OnDeathZoneOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		_sleep(2);
		OtherActor->SetActorLocation(RespawnCapsule->GetComponentLocation());
	}
}
