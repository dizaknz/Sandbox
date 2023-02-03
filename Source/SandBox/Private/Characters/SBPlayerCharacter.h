#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SBPlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS(config = Game)
class ASBPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASBPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void OnCrouch(const FInputActionValue& Value);
	void OnSprint(const FInputActionValue& Value);

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputActionData* InputActions;


};
