#include "Vehicles/SBOffroadVehicleFrontWheel.h"

USBOffroadVehicleFrontWheel::USBOffroadVehicleFrontWheel()
{
    AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	bAffectedByEngine = true;
	MaxSteerAngle = 40.f;

	WheelRadius = 50.0f;
	CorneringStiffness = 750.0f;
	FrictionForceMultiplier = 4.0f;

	SuspensionMaxRaise = 20.0f;
	SuspensionMaxDrop = 20.0f;
	WheelLoadRatio = 1.0f;
	SpringRate = 100.0f;
	SpringPreload = 100.0f;
	SweepShape = ESweepShape::Shapecast;

	MaxBrakeTorque = 3000.0f;
	MaxHandBrakeTorque = 6000.0f;
}

