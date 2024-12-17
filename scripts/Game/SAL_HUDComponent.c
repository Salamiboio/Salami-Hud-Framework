class SAL_HUDComponentClass : ScriptComponentClass
{
}

class SAL_HUDComponent : ScriptComponent
{
	[Attribute(desc: "Layouts Resource Name", params: "layout")]
	protected ResourceName m_sHudName;
	
	[Attribute(desc: "Update Script for the HUD called on each frame", params: "")]
	protected ref SAL_HUDUpdate m_HudUpdate;
	
	[Attribute("1", desc: "Can this HUD be seen in third person")]
	protected bool m_bThirdPerson;
	
	
	
	ResourceName GetHudLayout()
	{
		return m_sHudName;
	}
	
	SAL_HUDUpdate GetHudUpdate()
	{
		return m_HudUpdate;
	}
	
	bool GetThirdPerson()
	{
		return m_bThirdPerson;
	}
}