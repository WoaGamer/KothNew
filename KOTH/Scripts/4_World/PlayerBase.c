modded class PlayerBase extends ManBase {
    protected PlayerBase m_KilledByKOTH;

    /*
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		switch (rpc_type)
		{
			case eKOTH.RPC_KOTH_CONFIG_SYNC:
			{
				if ( GetGame().IsClient() )
				{
					KOTH_Settings.OnRPC(this, ctx);
				}
				break;
			}
		}
	}

	override void OnGameplayDataHandlerSync()
	{
		super.OnGameplayDataHandlerSync();
	}
	*/

    override void EEKilled(Object killer) {
        super.EEKilled(killer);

        if (!GetGame().IsDedicatedServer()) return;

        if (killer.IsEntityAI()) Class.CastTo(m_KilledByKOTH, EntityAI.Cast(killer).GetHierarchyRootPlayer());
    }

    PlayerBase GetKillerKOTH() {
        if (m_KilledByKOTH != this) return m_KilledByKOTH;

        return NULL;
    }
}