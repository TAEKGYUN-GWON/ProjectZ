// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h" // GAS 인터페이스
#include "Teams/LyraTeamAgentInterface.h" // Lyra 팀 인터페이스
#include "AbilitySystem/Attributes/LyraHealthSet.h" 
#include "AbilitySystem/Attributes/LyraCombatSet.h" 
#include "ZombieCharacter.generated.h"

class ULyraAbilitySystemComponent;
class ULyraHealthComponent;

UCLASS()
class LYRAGAME_API AZombieCharacter : public ACharacter, public IAbilitySystemInterface, public ILyraTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieCharacter();

	// --- [1] Lyra 핵심 컴포넌트 선언 ---
	// GAS (능력 시스템) 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Abilities")
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;

	// 체력 관리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Health")
	TObjectPtr<ULyraHealthComponent> HealthComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnDeathStarted(AActor* OwningActor);
	UFUNCTION()
	void OnHealthChanged(ULyraHealthComponent* Health, float OldValue, float NewValue, AActor* OwningActor);

	void StartRagdoll();
	
	UPROPERTY()
	TObjectPtr<const ULyraHealthSet> HealthSet;
	UPROPERTY()
	TObjectPtr<const ULyraCombatSet> CombatSet;
	UPROPERTY(EditDefaultsOnly, Category = "Zombie|Anim")
	TObjectPtr<UAnimMontage> DeathMontage;
private:
	// 좀비의 팀 ID를 저장할 변수
	UPROPERTY(EditAnywhere, Category = "Lyra|Team")
	uint8 ZombieTeamID = 2; // 보통 0, 1은 플레이어 팀이 쓰니 2번으로 설정

	FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;
};
