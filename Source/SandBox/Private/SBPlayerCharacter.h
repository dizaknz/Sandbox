#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SBPlayerCharacter.generated.h"

UCLASS()
class ASBPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASBPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void BeginSprint();
	void EndSprint();
	void BeginCrouch();
	void EndCrouch();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SprintSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float WalkSpeed;

};
