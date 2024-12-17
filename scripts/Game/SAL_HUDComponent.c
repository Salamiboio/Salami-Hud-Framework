class SAL_HUDComponentClass : ScriptComponentClass
{
}

class SAL_HUDComponent : ScriptComponent
{
	[Attribute(desc: "Layouts Resource Name", params: "layout")]
	protected ResourceName m_sHudName;
	
	[Attribute(desc: "Script used to update HUD")]
	protected ref SAL_HUDUpdate m_cHudUpdate;
	
	[Attribute(desc: "Can this HUD be seen in third person")]
	protected bool m_bThirdPerson;
	
	
	ResourceName GetHudLayout()
	{
		return m_sHudName;
	}
	
	SAL_HUDUpdate GetUpdate()
	{
		return m_cHudUpdate;
	}
	
	bool GetThirdPerson()
	{
		return m_bThirdPerson;
	}
}