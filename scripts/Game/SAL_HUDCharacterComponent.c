class SAL_HUDCharacterComponentClass: ScriptComponentClass
{
}

class SAL_HUDCharacterComponent: ScriptComponent
{
	ref array<Widget> m_aCurrentHuds = {};
	ref array<IEntity> m_aCurrentHudObjects = {};
	ref array<SAL_HUDUpdate> m_aHudUpdates = {};
	IEntity characterEntity;
	
	override void EOnInit(IEntity owner)
	{
		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(owner.FindComponent(SCR_InventoryStorageManagerComponent));
		if(!inventory)
			return;
		
		inventory.m_OnItemAddedInvoker.Insert(HUDItemAdded);
		inventory.m_OnItemRemovedInvoker.Insert(HUDItemRemoved);
		characterEntity = owner;
		ChimeraCharacter character = ChimeraCharacter.Cast(owner);
		if (character)
		{
			SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.FindComponent(SCR_CharacterControllerComponent));
			if (characterController)
				characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
			SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(character.FindComponent(SCR_CharacterCameraHandlerComponent));
			if(cameraController)
				cameraController.GetThirdPersonSwitchInvoker().Insert(OnThirdPerson);
		}
	}
	
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	void HUDItemAdded(IEntity item, BaseInventoryStorageComponent storageOwner)
	{
		if(!item.FindComponent(SAL_HUDComponent))
			return;
		
		if(SCR_PlayerController.GetLocalControlledEntity() != item.GetRootParent())
			return;
		
		AddHUD(item);
	}
	
	void AddHUD(IEntity item)
	{
		if(m_aCurrentHudObjects.Find(item) != -1)
			return;
		
		int index = m_aCurrentHuds.Insert(GetGame().GetWorkspace().CreateWidgets(SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetHudLayout()));
		
		m_aCurrentHuds.Get(index).SetZOrder(0);
		
		int updateIndex = m_aHudUpdates.Insert(SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetUpdate());
		
		m_aCurrentHudObjects.Insert(item);
		
		if(m_aHudUpdates.Get(updateIndex))
			GetGame().GetCallqueue().CallLater(m_aHudUpdates.Get(updateIndex).Update, 1, true, m_aCurrentHuds.Get(index), item);
	}
	
	void HUDItemRemoved(IEntity item, BaseInventoryStorageComponent storageOwner)
	{
		if(!item.FindComponent(SAL_HUDComponent))
			return;
		
		if(SCR_PlayerController.GetLocalControlledEntity() != storageOwner.GetOwner().GetRootParent())
			return;
		
		RemoveHUD(item);
	}
	
	void RemoveHUD(IEntity item)
	{
		int index = m_aCurrentHudObjects.Find(item);
		
		if(index == -1)
			return;
		
		m_aCurrentHuds.Get(index).RemoveFromHierarchy();
		
		m_aCurrentHuds.RemoveOrdered(index);
		m_aCurrentHudObjects.RemoveOrdered(index);
		
		if(m_aHudUpdates.Get(index))
			GetGame().GetCallqueue().Remove(m_aHudUpdates.Get(index).Update);
		
		m_aHudUpdates.RemoveOrdered(index);
	}
	
	void ClearHUDs()
	{
		for(int i = 0; i < m_aCurrentHuds.Count(); i++)
		{
			if (m_aCurrentHuds.Get(i))
				m_aCurrentHuds.Get(i).RemoveFromHierarchy();
		}
		for(int i = 0; i < m_aCurrentHuds.Count(); i++)
		{
			m_aCurrentHuds.Remove(i);
			m_aHudUpdates.Remove(i);
			m_aCurrentHudObjects.Remove(i);
		}
	}
	
	void OnThirdPerson()
	{
		if(SCR_PlayerController.GetLocalControlledEntity() != characterEntity)
			return;
		
		
		SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(characterEntity.FindComponent(SCR_CharacterCameraHandlerComponent));
		if(cameraController.IsInThirdPerson())
		{
			SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(characterEntity.FindComponent(SCR_InventoryStorageManagerComponent));
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
				if(!SAL_HUDComponent.Cast(HUDItem.FindComponent(SAL_HUDComponent)).GetThirdPerson())
					RemoveHUD(HUDItem);
			}
		}
		else
		{
			SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(characterEntity.FindComponent(SCR_InventoryStorageManagerComponent));
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
				AddHUD(HUDItem);
			}
		}
	}
	
	protected void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		if (newLifeState != ECharacterLifeState.DEAD)
			return;
		
		ClearHUDs();
	}
}