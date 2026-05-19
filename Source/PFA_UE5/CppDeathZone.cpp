// Fill out your copyright notice in the Description page of Project Settings.


#include "CppDeathZone.h"

#include "Animation/AnimInstanceProxy.h"
#include "Chaos/Capsule.h"
#include "Chaos/DebugDrawCommand.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
	canDie = true;
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
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

void ACppDeathZone::Timer()
{
	{
		canDie = false;
		AActor* LocalTarget = TargetActor; // capture
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this, LocalTarget]()
			{
				Respawn(TargetActor);
			},
			2.0f,
			false
		);
	}
}

void ACppDeathZone::OnDeathZoneOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass())&&canDie)
	{
		canDie = true;
		OtherActor->DisableInput(PlayerController);
		PlayerCameraManager->StartCameraFade(
			0.0f,
			1.0f,
			0.5f,
			FColor::Black,
			true,
			true);
		
		TargetActor = OtherActor;
		Timer();
	}
}
void ACppDeathZone::Respawn(AActor* OtherActor)
{
	PlayerCameraManager->StartCameraFade(
		1.0f,
		0.0f,
		1.5f,
		FColor::Black,
		true,
		true);
	
	OtherActor->SetActorLocation(RespawnCapsule->GetComponentLocation());
	OtherActor->EnableInput(PlayerController);
	canDie = true;
}
