[BaseContainerProps(category: "HUDUpdate")]
class SAL_TestHudUpdate: SAL_HUDUpdate
{
	override void Update(Widget HUDWidget, IEntity HUDItem)
	{
		Print("test");
	}
}