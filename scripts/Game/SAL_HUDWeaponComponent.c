class SAL_HUDWeaponComponentClass: ScriptComponentClass
{
}

class SAL_HUDWeaponComponent: ScriptComponent
{
	[Attribute("")]
	ResourceName m_sWeaponIcon;
	
	[Attribute("")]
	ResourceName m_sCrossHairIcon;
	
	[Attribute("", params: "layout")]
	ResourceName m_sAmmoCounterLayout;
	
	[Attribute("16")]
	int m_iBulletPerRow;
}