class KOTH_ProgressBar {
    protected Widget m_Root;
    protected ProgressBarWidget m_Bar;
    protected TextWidget m_Text;
    protected bool m_Visible;

    void KOTH_ProgressBar() {
        if (!GetGame())
            return;

        m_Root = GetGame().GetWorkspace().CreateWidgets("KOTH/gui/layouts/koth_progress.layout");

        if (m_Root) {
            m_Bar = ProgressBarWidget.Cast(m_Root.FindAnyWidget("ProgressBar"));
            m_Text = TextWidget.Cast(m_Root.FindAnyWidget("ProgressText"));
            Hide();
        } else {
            KOTH_Log.LogCritical("Failed to create KOTH progress bar layout.");
        }
    }

    void Show() {
        if (m_Root) m_Root.Show(true);
        m_Visible = true;
    }

    void Hide() {
        if (m_Root) m_Root.Show(false);
        m_Visible = false;
    }

    bool IsVisible() {
        return m_Visible;
    }

    void SetProgress(float percent) {
        if (!m_Bar || !m_Text) return;
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;
        m_Bar.SetCurrent(percent);
        m_Text.SetText(string.Format("%1%%", Math.Round(percent).ToString()));
    }
}
