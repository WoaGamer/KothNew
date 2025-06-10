modded class MissionGameplay extends MissionBase
{
    protected ref KOTH_CaptureProgressUI m_KOTHCaptureUI;

    override void OnInit()
    {
        super.OnInit();
        m_KOTHCaptureUI = new KOTH_CaptureProgressUI();
    }

    #ifdef BASICMAP
    override void OnMissionStart()
    {
        super.OnMissionStart();
        Print("Requesting group update for KOTH.");
        BasicMap().RequestGroupUpdate("KOTH");
    }
    #endif

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        if (m_KOTHCaptureUI)
            m_KOTHCaptureUI.Update();
    }

    void KOTH_StartCapture(vector pos, float radius)
    {
        if (m_KOTHCaptureUI)
            m_KOTHCaptureUI.Start(pos, radius);
    }

    void KOTH_StopCapture()
    {
        if (m_KOTHCaptureUI)
            m_KOTHCaptureUI.Stop();
    }

    void KOTH_SetCaptureProgress(float progress)
    {
        if (m_KOTHCaptureUI)
            m_KOTHCaptureUI.SetProgress(progress);
    }
}
