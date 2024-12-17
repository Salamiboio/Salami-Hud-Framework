modded class SCR_EditorManagerEntity : SCR_EditorBaseEntity
{
	override void StartEvents(EEditorEventOperation type = EEditorEventOperation.NONE)
	{
		super.StartEvents(type);
		
		if(type == EEditorEventOperation.OPEN)
		{	
			if(!SCR_PlayerController.GetLocalControlledEntity())
				return;
			
			if(!SCR_PlayerController.GetLocalControlledEntity().FindComponent(SAL_HUDCharacterComponent))
				return;
			
			SCR_PlayerController.Cast(GetGame().GetPlayerController()).ClearHUD();
		}
		else if(type == EEditorEventOperation.CLOSE)
		{
			GetGame().GetCallqueue().CallLater(CloseHUD, 100, false);
		}
	}
	
	void CloseHUD()
	{
		if(!SCR_PlayerController.GetLocalControlledEntity())
			return;
		
		if(!SCR_PlayerController.GetLocalControlledEntity().FindComponent(SAL_HUDCharacterComponent))
			return;
		
		if(SCR_PlayerController.Cast(GetGame().GetPlayerController()).m_aCurrentHuds.Count() > 0)
			return;
		
		if(SCR_CharacterControllerComponent.Cast(SCR_PlayerController.GetLocalControlledEntity().FindComponent(SCR_CharacterControllerComponent)).GetLifeState() == ECharacterLifeState.DEAD)
			return;

		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(SCR_PlayerController.GetLocalControlledEntity().FindComponent(SCR_InventoryStorageManagerComponent));
		ref array<IEntity> items = {};
		ref array<IEntity> HUDItems = {};
		inventory.GetItems(items);
		
		foreach(IEntity item: items)
		{
			if(item.FindComponent(SAL_HUDComponent))
				HUDItems.Insert(item);
		}
		
		foreach(IEntity HUDItem: HUDItems)
		{
			GetGame().GetCallqueue().CallLater(AddHUD, 100, false,	HUDItem);
		}
	}
	
	void AddHUD(IEntity item)
	{
		SCR_PlayerController.Cast(GetGame().GetPlayerController()).AddHUD(item);
	}
}