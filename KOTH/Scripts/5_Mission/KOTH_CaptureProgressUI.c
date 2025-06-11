class KOTH_CaptureProgressUI
{
    protected Widget m_Root;
    protected ImageWidget m_Background;
    protected ImageWidget m_Bar;
    protected TextWidget m_Text;
    protected bool m_IsVisible;
    protected bool m_Initialized;

    protected vector m_ZonePos;
    protected float m_ZoneRadius;
    protected float m_Progress;

    static const string LAYOUT_PATH = "KOTH/GUI/layouts/koth_capture_bar.layout";

    void KOTH_CaptureProgressUI()
    {
        // Schedule initialization on the GUI call queue. Providing the object
        // reference ensures the method executes on this instance even if the
        // call is processed after construction. Delay slightly to make sure
        // the game UI has been fully created before we attempt to spawn
        // our widgets.
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Init, 1000, false);
        m_IsVisible = false;
        m_Initialized = false;
    }

    void Init()
    {
        if (m_Initialized)
            return;

        WorkspaceWidget workspace = game.GetWorkspace();
        if (!workspace)
        {
            // Workspace might not be available yet during login. Try again
            // shortly to avoid a crash when creating widgets too early.
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Init, 100, false);
            return;
        }

        if (!FileExist(LAYOUT_PATH))
        {
            Print("[KOTH] " + LAYOUT_PATH + " missing. Retrying initialization...");
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Init, 100, false);
            return;
        }
        m_Root = workspace.CreateWidgets(LAYOUT_PATH);

        if (!m_Root)
        {
            // If the widget failed to create, the UI may still be initializing.
            // Retry shortly instead of letting the game crash on a null pointer.
            Print("[KOTH] Failed to create capture progress UI, layout missing or workspace not ready. Retrying...");
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Init, 100, false);
            return;
        }

        m_Background = ImageWidget.Cast(m_Root.FindAnyWidget("Background"));
        m_Bar = ImageWidget.Cast(m_Root.FindAnyWidget("Bar"));
        m_Text = TextWidget.Cast(m_Root.FindAnyWidget("ProgressText"));

        if (!m_Background || !m_Bar || !m_Text)
        {
            Print("[KOTH] Capture progress UI widgets missing. Retrying initialization...");
            if (m_Root)
            {
                m_Root.Show(false);
                m_Root.Unlink();
            }
            m_Root = null;
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Init, 100, false);
            return;
        }

        m_Root.Show(false);
        m_IsVisible = false;
        m_Initialized = true;
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
        if (!player) return;

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
        if (!m_Initialized) return;
        if (!m_IsVisible) return;
        if (!m_Background || !m_Bar || !m_Text) return;

        m_Text.SetText(string.Format("%1%%", Math.Round(m_Progress)));

        float width, height;
        m_Background.GetSize(width, height);
        float newWidth = width * (m_Progress / 100.0);
        m_Bar.SetSize(newWidth, height);
    }

    protected void Show()
    {
        if (!m_Initialized)
            return;

        if (!m_IsVisible)
        {
            if (m_Root)
            {
                m_Root.Show(true);
                m_IsVisible = true;
            }
        }
    }

    protected void Hide()
    {
        if (!m_Initialized)
            return;

        if (m_IsVisible)
        {
            if (m_Root)
                m_Root.Show(false);
            m_IsVisible = false;
        }
    }
}
