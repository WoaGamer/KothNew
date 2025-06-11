// Capture progress HUD for KOTH mode.
class KOTH_CaptureProgressUI
{
    static const string LAYOUT_PATH = "KOTH/GUI/layouts/koth_capture_bar.layout";
    static const int RETRY_DELAY_MS = 100;

    protected Widget m_Root;
    protected ImageWidget m_Background;
    protected ImageWidget m_Bar;
    protected TextWidget m_Text;

    protected vector m_ZonePos;
    protected float m_ZoneRadius;
    protected float m_Progress;

    protected bool m_Initialized;
    protected bool m_Visible;

    void KOTH_CaptureProgressUI()
    {
        m_Initialized = false;
        m_Visible = false;
        QueueInit();
    }

    protected void QueueInit()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Init, RETRY_DELAY_MS, false);
    }

    protected void Init()
    {
        if (m_Initialized)
            return;

        if (GetGame().IsDedicatedServer())
            return; // no UI on dedicated servers

        DayZGame game = DayZGame.Cast(GetGame());
        if (!game)
        {
            QueueInit();
            return;
        }

        WorkspaceWidget workspace = game.GetWorkspace();
        if (!workspace)
        {
            QueueInit();
            return;
        }

        if (!FileExist(LAYOUT_PATH))
        {
            Print("[KOTH] Layout " + LAYOUT_PATH + " not found, retrying...");
            QueueInit();
            return;
        }

        m_Root = workspace.CreateWidgets(LAYOUT_PATH);
        if (!m_Root)
        {
            Print("[KOTH] Failed to create capture progress UI, retrying...");
            QueueInit();
            return;
        }

        m_Background = ImageWidget.Cast(m_Root.FindAnyWidget("Background"));
        m_Bar = ImageWidget.Cast(m_Root.FindAnyWidget("Bar"));
        m_Text = TextWidget.Cast(m_Root.FindAnyWidget("ProgressText"));

        if (!m_Background || !m_Bar || !m_Text)
        {
            Print("[KOTH] Capture progress UI widgets missing, retrying...");
            m_Root.Unlink();
            m_Root = null;
            QueueInit();
            return;
        }

        m_Root.Show(false);
        m_Initialized = true;
    }

    void ~KOTH_CaptureProgressUI()
    {
        if (m_Root)
            m_Root.Unlink();
    }

    void Start(vector pos, float radius)
    {
        if (!m_Initialized)
            return;

        m_ZonePos = pos;
        m_ZoneRadius = radius;
        m_Progress = 0;
        Show();
    }

    void Stop()
    {
        if (!m_Initialized)
            return;
        Hide();
    }

    void SetProgress(float progress)
    {
        if (!m_Initialized)
            return;

        m_Progress = Math.Clamp(progress, 0, 100);
        UpdateUI();
    }

    void Update()
    {
        if (!m_Initialized)
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        if (m_ZoneRadius > 0 && vector.Distance(player.GetPosition(), m_ZonePos) <= m_ZoneRadius)
        {
            Show();
            UpdateUI();
        }
        else
        {
            Hide();
        }
    }

    protected void UpdateUI()
    {
        if (!m_Initialized || !m_Visible)
            return;

        float width, height;
        m_Background.GetSize(width, height);

        float newWidth = width * (m_Progress / 100.0);
        m_Bar.SetSize(newWidth, height);

        m_Text.SetText(string.Format("%1%%", Math.Round(m_Progress)));
    }

    protected void Show()
    {
        if (!m_Initialized || m_Visible || !m_Root)
            return;

        m_Root.Show(true);
        m_Visible = true;
    }

    protected void Hide()
    {
        if (!m_Initialized || !m_Visible || !m_Root)
            return;

        m_Root.Show(false);
        m_Visible = false;
    }
}
