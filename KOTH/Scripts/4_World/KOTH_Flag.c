class KOTH_Flag extends BaseBuildingBase {
    bool m_IsAnimated;
    bool m_NeedsAnimate;
    float m_TimeSinceSync;
    float m_LastSyncedHeight;
    float m_TargetFlagHeight;
    ref Timer m_FlagTimer;

    string m_FlagType;

    void KOTH_Flag() {
        RegisterNetSyncVariableBool("m_NeedsAnimate");
        RegisterNetSyncVariableFloat("m_LastSyncedHeight");
        RegisterNetSyncVariableFloat("m_TargetFlagHeight");
    }

    override void EEInit() {
        super.EEInit();
    }

    void AnimateFlag() {
        if (m_LastSyncedHeight != m_TargetFlagHeight) {
            float newHeight = Math.Lerp(m_TargetFlagHeight, m_LastSyncedHeight, m_TimeSinceSync / 100);
            KOTH_Log.LogVerbose(string.Format("LastSyncedHeight: %1, TargetFlagHeight: %2, Height: %3", m_LastSyncedHeight, m_TargetFlagHeight, newHeight));
            SetAnimationPhase("flag_mast", newHeight);
            m_TimeSinceSync++;
        }
    }

    void AttachFlag(string flagType) {
        if (!this.GetInventory().FindAttachmentByName("Material_FPole_Flag")) this.GetInventory().CreateAttachment(flagType);
    }

    void BuildFlag() {
        if (GetGame().IsDedicatedServer()) {
            KOTH_BuildPartServer("base", AT_BUILD_PART);
            KOTH_BuildPartServer("support", AT_BUILD_PART);
            KOTH_BuildPartServer("pole", AT_BUILD_PART);

            m_LastSyncedHeight = 1.0;
            m_TargetFlagHeight = 1.0;
        }
    }

    protected void KOTH_BuildPartServer(string part_name, int action_id) {
        ConstructionPart constrution_part = GetConstruction().GetConstructionPart(part_name);

        if (constrution_part.IsBase()) SetBaseState(true);

        RegisterPartForSync(constrution_part.GetId());
        RegisterActionForSync(constrution_part.GetId(), action_id);
        SynchronizeBaseState();
        SetPartFromSyncData(constrution_part);
        UpdateVisuals();

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetActionSyncData, 100, false, this);
    }

    override void OnVariablesSynchronized() {
        super.OnVariablesSynchronized();

        if (GetGame().IsClient() && !m_IsAnimated && m_NeedsAnimate) StartAnimateFlag();
    }

    void SetFlagType(string flagType) {
        m_FlagType = flagType;
    }

    protected void StartAnimateFlag() {
        if (GetGame() && GetGame().IsClient()) {
            SetAnimationPhase("flag_mast", 1.0);
            m_FlagTimer = new Timer();
            m_FlagTimer.Run(0.01, this, "AnimateFlag", NULL, true);
            m_IsAnimated = true;
        }
    }

    // protected void RaiseFlag()
    // {
    // if (m_TargetFlagHeight == 1.0 && !this.GetInventory().FindAttachmentByName("Material_FPole_Flag"))
    // this.GetInventory().CreateAttachment(m_FlagType);

    // m_TargetFlagHeight -= 0.001;
    // SetSynchDirty();
    // if (m_TargetFlagHeight <= 0)
    // {
    // EntityAI fireworksLauncher = EntityAI.Cast( GetGame().CreateObjectEx( "FireworksLauncher", GetPosition(), ECE_PLACE_ON_SURFACE, RF_DEFAULT ) );
    // fireworksLauncher.OnIgnitedThis(null);
    // fireworksLauncher.SetSynchDirty();
    // m_FlagTimer.Stop();
    // }
    // }

    void SetNeedsAnimate(bool state) {
        m_NeedsAnimate = state;
        SetSynchDirty();
    }

    void SetTargetFlagHeight(float height) {
        m_LastSyncedHeight = m_TargetFlagHeight;
        m_TargetFlagHeight = height;
        m_TimeSinceSync = 0;
        KOTH_Log.LogVerbose(string.Format("LastSyncedHeight: %1, TargetFlagHeight: %2", m_LastSyncedHeight, m_TargetFlagHeight));
        SetSynchDirty();
    }

    override bool CanReleaseAttachment(EntityAI attachment) {
        return false;
    }

    override void SetActions() {
        super.SetActions();

        AddAction(ActionRaiseFlag);
        AddAction(ActionLowerFlag);
    }
}