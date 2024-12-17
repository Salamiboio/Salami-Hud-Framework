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
			
			SAL_HUDCharacterComponent hudCharacterComponent = SAL_HUDCharacterComponent.Cast(SCR_PlayerController.GetLocalControlledEntity().FindComponent(SAL_HUDCharacterComponent));
			
			hudCharacterComponent.ClearHUDs();
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
		
		SAL_HUDCharacterComponent hudCharacterComponent = SAL_HUDCharacterComponent.Cast(SCR_PlayerController.GetLocalControlledEntity().FindComponent(SAL_HUDCharacterComponent));
		
		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(SCR_PlayerController.GetLocalControlledEntity().FindComponent(SCR_InventoryStorageManagerComponent));
		ref array<IEntity> items = {};
		ref array<IEntity> HUDItems = {};
		inventory.GetItems(items);
		
		foreach(IEntity item: items)
		{
			if(item.FindComponent(SAL_HUDComponent))
				HUDItems.Insert(item);
		}
		
		if(HUDItems.Count() == 0)
			return;
		
		foreach(IEntity HUDItem: HUDItems)
		{ 
			hudCharacterComponent.AddHUD(HUDItem);
		}
	}
}