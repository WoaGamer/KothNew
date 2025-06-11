modded class MissionGameplay extends MissionBase {
    protected ref KOTH_ProgressBar m_KOTHBar;

    override void OnMissionStart() {
        super.OnMissionStart();
#ifdef BASICMAP
        Print("Requesting group update for KOTH.");
        BasicMap().RequestGroupUpdate("KOTH");
#endif
        m_KOTHBar = new KOTH_ProgressBar();
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(KOTH_UpdateProgress, 1000, true, this);
    }

    void KOTH_UpdateProgress() {
        if (!KOTH_Settings.IsEnabled()) {
            if (m_KOTHBar && m_KOTHBar.IsVisible()) m_KOTHBar.Hide();
            return;
        }

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player) {
            if (m_KOTHBar && m_KOTHBar.IsVisible()) m_KOTHBar.Hide();
            return;
        }

        KOTH_Flag nearestFlag;
        KOTH_Zone zone;
        float nearestDist = 0;
        foreach(KOTH_Zone z : KOTH_Settings.GetZones()) {
            vector zp = z.GetPosition();
            if (vector.Distance(player.GetPosition(), zp) <= z.GetRadius()) {
                array<Object> objs = new array<Object>;
                array<CargoBase> prox = new array<CargoBase>;
                GetGame().GetObjectsAtPosition(zp, 5, objs, prox);
                foreach(Object obj : objs) {
                    if (Class.CastTo(nearestFlag, obj)) {
                        zone = z;
                        break;
                    }
                }
            }
            if (nearestFlag) break;
        }

        if (nearestFlag && zone) {
            float progress = (1.0 - nearestFlag.GetAnimationPhase("flag_mast")) * 100.0;
            if (progress < 0) progress = 0;
            if (progress > 100) progress = 100;
            m_KOTHBar.SetProgress(progress);
            if (!m_KOTHBar.IsVisible()) m_KOTHBar.Show();
        } else {
            if (m_KOTHBar.IsVisible()) m_KOTHBar.Hide();
        }
    }
}
