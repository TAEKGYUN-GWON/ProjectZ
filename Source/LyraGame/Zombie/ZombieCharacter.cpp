// ZombieCharacter.cpp
#include "ZombieCharacter.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Character/LyraHealthComponent.h"
#include "Components/CapsuleComponent.h"

AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<ULyraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	HealthSet = CreateDefaultSubobject<ULyraHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<ULyraCombatSet>(TEXT("CombatSet"));
	HealthComponent = CreateDefaultSubobject<ULyraHealthComponent>(TEXT("HealthComponent"));
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	
	if (HealthComponent)
	{
		HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
		HealthComponent->OnDeathStarted.AddDynamic(this, &AZombieCharacter::OnDeathStarted);
		HealthComponent->OnHealthChanged.AddDynamic(this, &AZombieCharacter::OnHealthChanged);
		UE_LOG(LogTemp, Warning, TEXT("Zombie Init - MaxHealth: %f"), HealthComponent->GetMaxHealth());
	}
    
	SetGenericTeamId(FGenericTeamId(ZombieTeamID));
}

void AZombieCharacter::OnHealthChanged(ULyraHealthComponent* Health, float OldValue, float NewValue, AActor* OwningActor)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString::SanitizeFloat(HealthComponent->GetHealth()));


	if (NewValue <= 0.00f && Health)
	{
		OnDeathStarted(OwningActor);
	}
}
void AZombieCharacter::OnDeathStarted(AActor* OwningActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie Death"));
	if (Controller)
	{
		Controller->UnPossess();
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StartRagdoll();
    
}

void AZombieCharacter::StartRagdoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
    
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
        
		float MontageLength = DeathMontage->GetPlayLength();
		SetLifeSpan(MontageLength - 0.25f);
	}
	else
	{
		SetLifeSpan(0.1f);
	}
}

UAbilitySystemComponent* AZombieCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AZombieCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (ZombieTeamID != NewTeamID.GetId())
	{
		OnTeamChangedDelegate.Broadcast(this, ZombieTeamID, NewTeamID);
		ZombieTeamID = NewTeamID.GetId();
	}
}

FGenericTeamId AZombieCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(ZombieTeamID);
}

FOnLyraTeamIndexChangedDelegate* AZombieCharacter::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}