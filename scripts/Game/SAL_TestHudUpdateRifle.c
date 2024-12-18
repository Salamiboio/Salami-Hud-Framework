[BaseContainerProps(category: "HUDUpdate")]
class SAL_TestHudUpdateRifle: SAL_HUDUpdate
{
	override void Update(Widget HUDWidget, IEntity HUDItem)
	{
		Print("testrifle");
	}
}