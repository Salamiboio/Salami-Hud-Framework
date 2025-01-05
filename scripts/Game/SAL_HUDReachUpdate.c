[BaseContainerProps(category: "HUDUpdate")]
class SAL_HUDReachUpdate: SAL_HUDUpdate
{
	SCR_ChimeraCharacter m_ChimeraCharacter;
	IEntity m_eCurrentWeapon;
	int m_iCurrentAmmo = 0;
	int m_iCurrentHealth = 0;
	protected float m_iSearchRadius;
	protected ref array<SCR_ChimeraCharacter> m_aAllFriendliesWithinRange = {};
	protected ref array<SCR_ChimeraCharacter> m_aAllEnemiesWithinRange = {};
	protected ref array<SCR_ChimeraCharacter> m_aAllEntitesWithinRange = {};
	ref array<ImageWidget> m_aFriendlyActivePings = {};
	ref array<ImageWidget> m_aEnemyActivePings = {};
	Widget m_wRoot;
	float m_fYaw;
	
	protected bool IsPlayerInVehicle(SCR_ChimeraCharacter playerCharacter)
	{
		CompartmentAccessComponent compartmentAccess = CompartmentAccessComponent.Cast(playerCharacter.FindComponent(CompartmentAccessComponent));
		if (compartmentAccess) 
		{
			BaseCompartmentSlot compartment = compartmentAccess.GetCompartment();
			if (compartment) 
				return true;
		};
		return false;
	};
	
	void UpdateCompass(Widget HUDWidget)
	{
		if(!m_ChimeraCharacter)
			m_ChimeraCharacter = SCR_ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		AimingComponent playerControllerComponent = m_ChimeraCharacter.GetHeadAimingComponent();
		if (!playerControllerComponent) 
			return;
	
		// Freelook Direction
		float altYaw = playerControllerComponent.GetAimingDirection().ToYaw();
		
	
		if (IsPlayerInVehicle(m_ChimeraCharacter) || altYaw > 0)
		{
			vector transform[4];
			GetGame().GetWorld().GetCurrentCamera(transform);

			m_fYaw = -Math3D.MatrixToAngles(transform)[0];
		} 
		else 
		{
			m_fYaw = playerControllerComponent.GetAimingDirectionWorld().ToYaw();
		}
	
		float yawFloat = -m_fYaw;
			
		if (yawFloat < 0) 
			yawFloat = 360 - Math.AbsFloat(yawFloat);
		
		
		FrameSlot.SetOffsets(HUDWidget.FindAnyWidget("CompassFrame"), -1090 - 1880 * (yawFloat / 360), -63, -2750 + 1880 * (yawFloat / 360), -995);
	}
	
	void UpdateWeapon(Widget HUDWidget)
	{
		IEntity currentWeapon;
		if(CharacterWeaponManagerComponent.Cast(m_ChimeraCharacter.FindComponent(CharacterWeaponManagerComponent)).GetCurrentWeapon())
			currentWeapon = CharacterWeaponManagerComponent.Cast(m_ChimeraCharacter.FindComponent(CharacterWeaponManagerComponent)).GetCurrentWeapon().GetOwner();
				
		ImageWidget weaponIcon = ImageWidget.Cast(HUDWidget.FindAnyWidget("WeaponOne"));
		TextWidget weaponMaxAmmo = TextWidget.Cast(HUDWidget.FindAnyWidget("WeaponOneText"));
		VerticalLayoutWidget weaponAmmo = VerticalLayoutWidget.Cast(HUDWidget.FindAnyWidget("AmmoVerticalLayout"));
		ImageWidget reticle = ImageWidget.Cast(HUDWidget.FindAnyWidget("Reticle"));
		
		if(!currentWeapon)
		{
			weaponMaxAmmo.SetVisible(false);
			weaponAmmo.SetVisible(false);
			weaponIcon.SetVisible(false);
			reticle.SetVisible(false);
			return;
		}
		
		if(m_eCurrentWeapon == currentWeapon || currentWeapon.FindComponent(GrenadeMoveComponent))
			return;
		
		m_eCurrentWeapon = currentWeapon;
		if(SAL_HUDWeaponComponent.Cast(currentWeapon.FindComponent(SAL_HUDWeaponComponent)).m_sCrossHairIcon != "")
		{
			reticle.SetVisible(true);
			reticle.LoadImageTexture(1, SAL_HUDWeaponComponent.Cast(currentWeapon.FindComponent(SAL_HUDWeaponComponent)).m_sCrossHairIcon);
			reticle.SetImage(1);
		}
		else
			reticle.SetVisible(false);
		
		if(SAL_HUDWeaponComponent.Cast(currentWeapon.FindComponent(SAL_HUDWeaponComponent)).m_sWeaponIcon != "")
		{
			weaponIcon.SetVisible(true);
			weaponMaxAmmo.SetVisible(true);
			weaponAmmo.SetVisible(true);
			weaponIcon.LoadImageTexture(1, SAL_HUDWeaponComponent.Cast(currentWeapon.FindComponent(SAL_HUDWeaponComponent)).m_sWeaponIcon);
			weaponIcon.SetImage(1);
		}
		else
		{
			weaponMaxAmmo.SetVisible(false);
			weaponAmmo.SetVisible(false);
			weaponIcon.SetVisible(false);
		}
	}
	
	void UpdateAmmoCount(Widget HUDWidget)
	{
		BaseWeaponComponent currentWeapon = CharacterWeaponManagerComponent.Cast(m_ChimeraCharacter.FindComponent(CharacterWeaponManagerComponent)).GetCurrentWeapon();
		
		if(!currentWeapon)
			return;
		
		if(currentWeapon.GetOwner().FindComponent(GrenadeMoveComponent))
			return;
		
		BaseMagazineWell magazineWell = MuzzleComponent.Cast(currentWeapon.FindComponent(MuzzleComponent)).GetMagazineWell();
		int maxAmmo;
		int currentMagAmmo;
		int currentMaxAmmo;
		ref array<IEntity> items = {};
		
		if(!currentWeapon.GetCurrentMagazine())
			return;
		
		currentMagAmmo = currentWeapon.GetCurrentMagazine().GetAmmoCount();
		currentMaxAmmo = currentWeapon.GetCurrentMagazine().GetMaxAmmoCount();
		
		SCR_InventoryStorageManagerComponent.Cast(SCR_PlayerController.GetLocalControlledEntity().FindComponent(SCR_InventoryStorageManagerComponent)).GetItems(items);
		foreach(IEntity item: items)
		{
			if(item.FindComponent(MagazineComponent))
			{
				MagazineComponent mag = MagazineComponent.Cast(item.FindComponent(MagazineComponent));
				if(magazineWell.Type() != mag.GetMagazineWell().Type())
					continue;
				maxAmmo += mag.GetAmmoCount();
			}		
		}
		
		TextWidget.Cast(HUDWidget.FindAnyWidget("WeaponOneText")).SetText(maxAmmo.ToString());
		
		
		if(m_iCurrentAmmo == currentMagAmmo)
			return;
		
		if(!SAL_HUDWeaponComponent.Cast(currentWeapon.GetOwner().FindComponent(SAL_HUDWeaponComponent)).m_iBulletPerRow || !SAL_HUDWeaponComponent.Cast(currentWeapon.GetOwner().FindComponent(SAL_HUDWeaponComponent)).m_sAmmoCounterLayout)
			return;
		
		m_iCurrentAmmo = currentMagAmmo;
		int bulletsPerRow = SAL_HUDWeaponComponent.Cast(currentWeapon.GetOwner().FindComponent(SAL_HUDWeaponComponent)).m_iBulletPerRow;
		int iterations = currentMagAmmo/bulletsPerRow;
		ResourceName ammoLayout = SAL_HUDWeaponComponent.Cast(currentWeapon.GetOwner().FindComponent(SAL_HUDWeaponComponent)).m_sAmmoCounterLayout;
		VerticalLayoutWidget ammoVertical = VerticalLayoutWidget.Cast(HUDWidget.FindAnyWidget("AmmoVerticalLayout"));
		bool widgetClear = false;
		while(!widgetClear)
		{
			if(ammoVertical.GetChildren())
				delete ammoVertical.GetChildren();
			else
				widgetClear = true;
		}
		for(int i = 0; i < iterations; i++)
		{
			GetGame().GetWorkspace().CreateWidgets(ammoLayout, ammoVertical);
		}
		
		if(currentMagAmmo % bulletsPerRow != 0)
		{
			int bulletsToDelete = bulletsPerRow - (currentMagAmmo - iterations * bulletsPerRow);
			HorizontalLayoutWidget bulletLayout = HorizontalLayoutWidget.Cast(GetGame().GetWorkspace().CreateWidgets(ammoLayout, ammoVertical));
			for(int i = 0; i < bulletsToDelete; i++)
			{
				delete bulletLayout.GetChildren();
			}
		}
	}
	
	
	override void Update(Widget HUDWidget, IEntity HUDItem)
	{
		if(!m_wRoot)
			m_wRoot = HUDWidget;
		UpdateCompass(HUDWidget);
		SquadRadarSearch();
		UpdateWeapon(HUDWidget);
		UpdateAmmoCount(HUDWidget);
		if(SCR_ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity()).IsInVehicle())
			ImageWidget.Cast(HUDWidget.FindAnyWidget("Reticle")).SetVisible(false);
		else if(CharacterWeaponManagerComponent.Cast(m_ChimeraCharacter.FindComponent(CharacterWeaponManagerComponent)).GetCurrentWeapon())
		{
			if(CharacterWeaponManagerComponent.Cast(m_ChimeraCharacter.FindComponent(CharacterWeaponManagerComponent)).GetCurrentWeapon().IsSightADSActive())
				ImageWidget.Cast(HUDWidget.FindAnyWidget("Reticle")).SetVisible(false);
		}
		else
			ImageWidget.Cast(HUDWidget.FindAnyWidget("Reticle")).SetVisible(true);
		
		int health = DamageManagerComponent.Cast(SCR_PlayerController.GetLocalControlledEntity().FindComponent(DamageManagerComponent)).GetHealthScaled() * 100;
		
		if(m_iCurrentHealth == health)
			return;
		
		m_iCurrentHealth = health;
		
		int barsToAdd = Math.Floor(health/10 - 1);
		bool health1 = false;
		HorizontalLayoutWidget leftHealth = HorizontalLayoutWidget.Cast(HUDWidget.FindAnyWidget("HealthBar2"));
		while(!health1)
		{
			if(leftHealth.GetChildren())
				delete leftHealth.GetChildren();
			else
				health1 = true;
		}
		
		bool health2 = false;
		HorizontalLayoutWidget rightHealth = HorizontalLayoutWidget.Cast(HUDWidget.FindAnyWidget("HealthBar1"));
		while(!health2)
		{
			if(rightHealth.GetChildren())
				delete rightHealth.GetChildren();
			else
				health2 = true;
		}
		
		for(int i = 0; i < barsToAdd; i++)
		{
			GetGame().GetWorkspace().CreateWidgets("{C035B66F99930D68}UI/ReachHUD/HealthIconRight.layout", leftHealth);
			GetGame().GetWorkspace().CreateWidgets("{14A5B7281BC88495}UI/ReachHUD/HealthIconLeft.layout", rightHealth);
		}
	}	
	
	protected void SquadRadarSearch() 
	{
		m_aAllFriendliesWithinRange = {};
		m_aAllEnemiesWithinRange = {};
		m_aAllEntitesWithinRange = {};

		string squadRadarSelfIconVisible = "";

		m_iSearchRadius = 25;
		if (IsPlayerInVehicle(m_ChimeraCharacter)) 
			m_iSearchRadius = 8;

		vector origin = SCR_PlayerController.GetLocalControlledEntity().GetOrigin();
		GetGame().GetWorld().QueryEntitiesBySphere(origin, m_iSearchRadius, ProcessEntity, null, EQueryEntitiesFlags.DYNAMIC | EQueryEntitiesFlags.WITH_OBJECT);

		UpdateSquadRadarPositions(origin);
	}
	
	protected void UpdateSquadRadarPositions(vector origin)
	{
		ClearSquadRadar();
		foreach(int i, SCR_ChimeraCharacter playerCharacter : m_aAllFriendliesWithinRange)
		{
			ImageWidget radarPlayer;
			radarPlayer = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("{6A7FD1FACA261362}UI/ReachHUD/RadarPip.layout", m_wRoot));
			m_aFriendlyActivePings	.Insert(radarPlayer);
			vector playerCharacterOrigin = playerCharacter.GetOrigin();

			// Get Distance
			float dis = vector.Distance(origin, playerCharacterOrigin);
			float disT = dis * 3.2;

			if (IsPlayerInVehicle(m_ChimeraCharacter)) 
				disT = dis * 6.215;

			// Get Direction
			vector dirV = vector.Direction(playerCharacterOrigin, origin);
			float dir = dirV.ToYaw();

			// Get Relative Direction
			float relDir = Math.Mod(((dir - m_fYaw) + 360), 360);
			relDir = Math.Mod(relDir - (dir * 2), 360);
			relDir = relDir * Math.DEG2RAD;

			float x = (Math.Sin(relDir) * disT);
			float y = (Math.Cos(relDir) * disT);

			SetSquadRadarImage(radarPlayer, x, y, playerCharacter);
		};
		foreach(int i, SCR_ChimeraCharacter playerCharacter : m_aAllEnemiesWithinRange)
		{
			ImageWidget radarPlayer;
			radarPlayer = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("{6A7FD1FACA261362}UI/ReachHUD/RadarPip.layout", m_wRoot));
			m_aEnemyActivePings.Insert(radarPlayer);
			vector playerCharacterOrigin = playerCharacter.GetOrigin();

			// Get Distance
			float dis = vector.Distance(origin, playerCharacterOrigin);
			float disT = dis * 3.2;

			if (IsPlayerInVehicle(m_ChimeraCharacter)) 
				disT = dis * 6.215;

			// Get Direction
			vector dirV = vector.Direction(playerCharacterOrigin, origin);
			float dir = dirV.ToYaw();

			// Get Relative Direction
			float relDir = Math.Mod(((dir - m_fYaw) + 360), 360);
			relDir = Math.Mod(relDir - (dir * 2), 360);
			relDir = relDir * Math.DEG2RAD;

			float x = (Math.Sin(relDir) * disT);
			float y = (Math.Cos(relDir) * disT);

			SetSquadRadarImage(radarPlayer, x, y, playerCharacter);
		};
	}
	
	protected void ClearSquadRadar()
	{
		foreach(ImageWidget widget: m_aFriendlyActivePings)
		{
			delete widget;
		}
		
		foreach(ImageWidget widget: m_aEnemyActivePings)
		{
			delete widget;
		}
		m_aFriendlyActivePings.Clear();
		m_aEnemyActivePings.Clear();
	}
	
	protected void SetSquadRadarImage(ImageWidget radarPlayer, float x, float y, SCR_ChimeraCharacter playerCharacter)
	{
		y = y - 173;
		x = x - 716;
		
		float widthAndHeight = 16 * (100 * 0.01);
		
		FrameSlot.SetPos(radarPlayer, (x - widthAndHeight/2), y);
		FrameSlot.SetSize(radarPlayer, widthAndHeight, widthAndHeight);
		
		if(SCR_FactionManager.SGetPlayerFaction(SCR_PlayerController.GetLocalPlayerId()) == playerCharacter.GetFaction())
			radarPlayer.SetColor(Color.Yellow);
		else
			radarPlayer.SetColor(Color.Red);
	}
	
	protected bool IsAlive(IEntity entity)
	{
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.GetDamageManager(entity);
		if (damageManager)
			return damageManager.GetState() != EDamageState.DESTROYED;
		else
			return true;
	}
	
	protected bool ProcessEntity(IEntity processEntity)
	{
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(processEntity);
		if (!playerCharacter) 
			return true;
		
		if(!IsAlive(processEntity))
			return true;
		
		if(!playerCharacter.GetCharacterController().GetInputContext().IsMoving())
			return true;

		if(FactionAffiliationComponent.Cast(processEntity.FindComponent(FactionAffiliationComponent)).GetAffiliatedFaction() == SCR_FactionManager.SGetPlayerFaction(SCR_PlayerController.GetLocalPlayerId()))
		{	
			m_aAllFriendliesWithinRange.Insert(playerCharacter);
			m_aFriendlyActivePings.Insert(null);
			m_aAllEntitesWithinRange.Insert(playerCharacter);
		}
		else
		{
			m_aAllEnemiesWithinRange.Insert(playerCharacter);
			m_aEnemyActivePings.Insert(null);
			m_aAllEntitesWithinRange.Insert(playerCharacter);
		}

		return true;
	}
	
	override void OnDestroyed()
	{
		IEntity noWeapon;
		m_eCurrentWeapon = noWeapon;
		m_iCurrentAmmo = 0;
		m_iCurrentHealth = 0;
	}
}