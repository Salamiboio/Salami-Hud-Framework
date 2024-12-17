class SAL_HUDCharacterComponentClass: ScriptComponentClass
{
}

class SAL_HUDCharacterComponent: ScriptComponent
{
	
	protected IEntity m_eOwner;
	
	override void EOnInit(IEntity owner)
	{
		m_eOwner = owner;
		
		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(owner.FindComponent(SCR_InventoryStorageManagerComponent));
		if(!inventory)
			return;
		
		inventory.m_OnItemAddedInvoker.Insert(HUDItemAdded);
		inventory.m_OnItemRemovedInvoker.Insert(HUDItemRemoved);
		ChimeraCharacter character = ChimeraCharacter.Cast(owner);
		if (character)
		{
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

		if(SCR_PlayerController.GetLocalControlledEntity() != m_eOwner)
			return;

		SCR_PlayerController.Cast(GetGame().GetPlayerController()).AddHUD(item);
	}
	
	void HUDItemRemoved(IEntity item, BaseInventoryStorageComponent storageOwner)
	{
		if(!item.FindComponent(SAL_HUDComponent))
			return;

		if(SCR_PlayerController.GetLocalControlledEntity() != m_eOwner)
			return;
		
		SCR_PlayerController.Cast(GetGame().GetPlayerController()).RemoveHUD(item);
	}
	
	void OnThirdPerson()
	{
		if(SCR_PlayerController.GetLocalControlledEntity() != m_eOwner)
			return;
		
		SCR_PlayerController.Cast(GetGame().GetPlayerController()).OnThirdPerson();
	}
}