#include "Vehicles/SBVehicleDisplayWidget.h"

#include "Components/TextBlock.h"

void USBVehicleDisplayWidget::Update(const FDisplayState& State)
{
    FNumberFormattingOptions Opts;
    Opts.SetMaximumFractionalDigits(0);
    if (State.bUseKph)
    {
        CurrentSpeed->SetText(FText::AsNumber(State.Speed * 1.60934f));
        SpeedUnit->SetText(FText::FromString("KPH"));
    }
    else
    {
        CurrentSpeed->SetText(FText::AsNumber(State.Speed));
        SpeedUnit->SetText(FText::FromString("MPH"));
    }
    CurrentGear->SetText(FText::AsNumber(State.Gear));
    CurrentRPM->SetText(FText::AsNumber(State.RPM));

}
