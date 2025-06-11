modded class MissionServer extends MissionBase {
    protected ref KOTH_ManagerServer m_KOTHManagerServer;

    override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx) {
        PlayerBase player = super.OnClientNewEvent(identity, pos, ctx);

        if (player)
            KOTH_Settings.SyncDataSend(player);
        return player;
    }

    override void OnClientReadyEvent(PlayerIdentity identity, PlayerBase player) {
        super.OnClientReadyEvent(identity, player);

        if (identity && player)
            KOTH_Settings.SyncDataSend(player);

        #ifdef BASICMAP
        if (KOTH_Settings.IsUseMapMarker() && GetKOTHManager()) GetKOTHManager().UpdateBasicMapMarkers(identity);
        #endif
    }

    override void OnInit() {
        super.OnInit();

        KOTH_Log.LogRaw("v1.2 is installed on this server.");
        KOTH_Loot.LoadData();
        KOTH_Settings.LoadData();

        if (KOTH_Settings.IsEnabled()) m_KOTHManagerServer = new KOTH_ManagerServer();
    }

    KOTH_ManagerServer GetKOTHManager() {
        return m_KOTHManagerServer;
    }
}