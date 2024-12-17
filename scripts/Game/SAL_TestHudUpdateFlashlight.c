[BaseContainerProps(category: "HUDUpdate")]
class SAL_TestHudUpdateFlashlight: SAL_HUDUpdate
{
	override void Update(Widget HUDWidget, IEntity HUDItem)
	{
		Print("testflashlight");
	}
}