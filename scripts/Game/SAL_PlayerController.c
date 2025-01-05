modded class SCR_PlayerController : PlayerController
{
	ref array<Widget> m_aCurrentHuds = {};
	ref array<IEntity> m_aHudEntities = {};
	
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
		InitializeHUD(to);
	}
	
	override void OnDestroyed(notnull Instigator killer)
	{
		SCR_PlayerController.Cast(GetGame().GetPlayerController()).ClearHUD();
	}
	
	void OnThirdPerson()
	{
		ClearHUD();
		InitializeHUD(GetLocalControlledEntity());
	}
	
	void InitializeHUD(IEntity entity)
	{
		if(GetLocalControlledEntity() != entity)
			return;
		
		if(SCR_EditorManagerEntity.GetInstance().IsOpened())
			return;

		if(SCR_CharacterControllerComponent.Cast(entity.FindComponent(SCR_CharacterControllerComponent)).GetLifeState() == ECharacterLifeState.DEAD)
			return;
		
		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(entity.FindComponent(SCR_InventoryStorageManagerComponent));
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
			if(SCR_CharacterCameraHandlerComponent.Cast(entity.FindComponent(SCR_CharacterCameraHandlerComponent)).IsInThirdPerson())
			{
				if(SAL_HUDComponent.Cast(HUDItem.FindComponent(SAL_HUDComponent)).GetThirdPerson())
					GetGame().GetCallqueue().CallLater(AddHUD, 100, false, HUDItem);
			}
			else
				GetGame().GetCallqueue().CallLater(AddHUD, 100, false, HUDItem);
		}
	}
		
	void AddHUD(IEntity item)
	{
		Widget newWidget = GetGame().GetWorkspace().CreateWidgets(SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetHudLayout());
		m_aCurrentHuds.Insert(newWidget);
		m_aHudEntities.Insert(item);
		if(SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetHudUpdate())
			GetGame().GetCallqueue().CallLater(SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetHudUpdate().Update, 0, true, newWidget, item);
	}
		
	void RemoveHUD(IEntity item)
	{
		if(SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetHudUpdate())
		{
			SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetHudUpdate().OnDestroyed();
			GetGame().GetCallqueue().Remove(SAL_HUDComponent.Cast(item.FindComponent(SAL_HUDComponent)).GetHudUpdate().Update);
		}
		
		if(m_aHudEntities.Find(item) == -1)
			return;
		
		m_aCurrentHuds.Get(m_aHudEntities.Find(item)).RemoveFromHierarchy();
		m_aCurrentHuds.RemoveOrdered(m_aHudEntities.Find(item));
		m_aHudEntities.RemoveOrdered(m_aHudEntities.Find(item));
	}
	
	void ClearHUD()
	{
		foreach(Widget widget: m_aCurrentHuds)
		{
			widget.RemoveFromHierarchy();
		}
		foreach(IEntity hudItem: m_aHudEntities)
		{
			if(SAL_HUDComponent.Cast(hudItem.FindComponent(SAL_HUDComponent)).GetHudUpdate())
			{
				SAL_HUDComponent.Cast(hudItem.FindComponent(SAL_HUDComponent)).GetHudUpdate().OnDestroyed();
				GetGame().GetCallqueue().Remove(SAL_HUDComponent.Cast(hudItem.FindComponent(SAL_HUDComponent)).GetHudUpdate().Update);
			}
		}
		m_aCurrentHuds.Clear();
		m_aHudEntities.Clear();
	}
}