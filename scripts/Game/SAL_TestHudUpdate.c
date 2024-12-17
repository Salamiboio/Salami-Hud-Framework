[BaseContainerProps(category: "HUDUpdate")]
class SAL_TestHudUpdate: SAL_HUDUpdate
{
	ref array<ref CanvasWidgetCommand> canvasCommands = {};
	override void Update(Widget HUDWidget, IEntity HUDItem)
	{
		ImageWidget compass = ImageWidget.Cast(HUDWidget.FindAnyWidget("Compass"));
		AimingComponent playerControllerComponent = SCR_ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity()).GetHeadAimingComponent();
		float yaw = playerControllerComponent.GetAimingDirectionWorld().ToYaw();
		float direction = ((yaw * 0.88) * -1) - 8;
		FrameSlot.SetPos(compass, direction, 1.5);
		
	}
}