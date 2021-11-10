// Fill out your copyright notice in the Description page of Project Settings.


#include "HMDSceneComponent.h"
#include "IXRTrackingSystem.h"
#include "IXRCamera.h"


// Sets default values for this component's properties
UHMDSceneComponent::UHMDSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHMDSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHMDSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (GEngine && GEngine->XRSystem.IsValid() && GetWorld() && GetWorld()->WorldType != EWorldType::Editor)
	{
		{
			IXRTrackingSystem* XRSystem = GEngine->XRSystem.Get();
			auto XRCamera = XRSystem->GetXRCamera();
			if (XRCamera.IsValid())
			{
				if (XRSystem->IsHeadTrackingAllowedForWorld(*GetWorld()))
				{
					const FTransform ParentWorld = CalcNewComponentToWorld(FTransform());

					XRCamera->SetupLateUpdate(ParentWorld, this, bLockToHmd == 0);
					
					if (bLockToHmd)
					{
						FQuat Orientation;
						FVector Position;
						if (XRCamera->UpdatePlayerCamera(Orientation, Position))
						{
							SetRelativeTransform(FTransform(Orientation, Position));
						}
						else
						{
							ResetRelativeTransform();
						}
					}
				}
			}
		}
	}
}

