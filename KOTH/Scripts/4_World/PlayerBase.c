modded class PlayerBase extends ManBase {
    protected PlayerBase m_KilledByKOTH;


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
