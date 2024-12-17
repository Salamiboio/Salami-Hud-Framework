modded class SCR_PlayerController : PlayerController
{
	override void SetInitialMainEntity(notnull IEntity entity)
	{
		IEntity controlledEntity = GetControlledEntity();
		super.SetInitialMainEntity(entity);
		
		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(entity.FindComponent(SCR_InventoryStorageManagerComponent));
		ref array<IEntity> items = {};
		ref array<IEntity> HUDItems = {};
		inventory.GetItems(items);
		
		foreach(IEntity item: items)
		{
			if(item.FindComponent(SAL_HUDComponent))
				HUDItems.Insert(item);
		}
		
		SAL_HUDCharacterComponent HUDCharacterComponent = SAL_HUDCharacterComponent.Cast(entity.FindComponent(SAL_HUDCharacterComponent));
		
		if(HUDItems.Count() == 0)
		{
			if(HUDItems.Count() == 0 && controlledEntity)
			{
				if(HUDItems.Count() == 0 && SAL_HUDCharacterComponent.Cast(controlledEntity.FindComponent(SAL_HUDCharacterComponent)).m_aCurrentHuds.Count() > 0)
					SAL_HUDCharacterComponent.Cast(controlledEntity.FindComponent(SAL_HUDCharacterComponent)).ClearHUDs();
			}
			return;
		}

		foreach(IEntity HUDItem: HUDItems)
		{
			GetGame().GetCallqueue().CallLater(AddHud, 100, false, entity, HUDItem);
		}
	}
	
	void AddHud(IEntity entity, IEntity HUDItem)
	{
		Rpc(RpcDo_AddHUD, RplComponent.Cast(entity.FindComponent(RplComponent)).Id(), RplComponent.Cast(HUDItem.FindComponent(RplComponent)).Id());
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_AddHUD(RplId entityID, RplId HUDItemID)
	{	
		if(!SCR_PlayerController.GetLocalMainEntity())
			return;
		
		if(RplComponent.Cast(Replication.FindItem(entityID)).GetEntity() != SCR_PlayerController.GetLocalMainEntity())
			return;
		
		SAL_HUDCharacterComponent.Cast(SCR_PlayerController.GetLocalMainEntity().FindComponent(SAL_HUDCharacterComponent)).AddHUD(RplComponent.Cast(Replication.FindItem(HUDItemID)).GetEntity());
	}
}