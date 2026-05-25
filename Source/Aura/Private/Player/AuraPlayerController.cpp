// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(subsystem);
	
	subsystem->AddMappingContext(AuraContext, 0);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI inputModeData;
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(inputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	const FVector2D inputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yawRotation = FRotator(0.0f, rotation.Yaw, 0.0f);
	
	const FVector forwardDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector rightDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* controlledPawn = GetPawn<APawn>())
	{
		controlledPawn->AddMovementInput(forwardDir, inputAxisVector.Y);
		controlledPawn->AddMovementInput(rightDir, inputAxisVector.X);
	}
}
