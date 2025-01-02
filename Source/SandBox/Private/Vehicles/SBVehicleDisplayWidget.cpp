#include "Vehicles/SBVehicleDisplayWidget.h"

#include "Components/TextBlock.h"

void USBVehicleDisplayWidget::Update(const FDisplayState& State)
{
    FNumberFormattingOptions Opts;
    Opts.SetMaximumFractionalDigits(0);
    CurrentSpeed->SetText(FText::AsNumber(State.Speed));
    CurrentGear->SetText(FText::AsNumber(State.Gear));
    if (State.RPM != -1)
    {
        CurrentRPM->SetText(FText::AsNumber(State.RPM));
    } 

}
