// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "CppDeathZone.generated.h"

UCLASS()
class PFA_UE5_API ACppDeathZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACppDeathZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category="Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, Category="Components")
	UBoxComponent* DeathZoneBox;
	UPROPERTY(EditAnywhere, Category="Components")
	UCapsuleComponent* RespawnCapsule;
	UPROPERTY(EditAnywhere, Category="Components")
	AActor* TargetActor;

	UFUNCTION(BlueprintCallable, Category="Action")
	void Timer();

	UFUNCTION(BlueprintCallable, Category="Action")
	void Respawn(AActor* OtherActor);
	
	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup Values")
	FVector RespawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup Values")
	FVector DeathZoneSize;

	UFUNCTION(BlueprintCallable, Category="Actions")
	void OnDeathZoneOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
