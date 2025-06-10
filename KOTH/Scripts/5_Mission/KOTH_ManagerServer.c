class KOTH_ManagerServer {
    const float MAIN_THREAD_INTERVAL = 10.0;
    protected bool m_IsActive;

    protected ref array < ref KOTH_Event > m_ActiveEvents;
    protected ref Timer m_SpawnDelayTimer;
    protected ref Timer m_MainTimer;
    protected ref Timer m_EventTimer;

    #ifdef BASICMAP
    ref array < ref BasicMapMarker > m_BasicMapMarkers;
    #endif

    protected float m_TimeToNextEvent;
    protected float m_TimeSinceLastEvent;

    protected ref array < Man > m_Players;

    protected ref map < string, float > m_CooldownZones;

    void KOTH_ManagerServer() {
        KOTH_Log.LogVerbose("Creating server instance");

        Init();
    }

    protected void CleanupEvents() {
        if (m_ActiveEvents.Count() < 1) {
            m_EventTimer.Stop();
            return;
        }

        for (int i = 0; i < m_ActiveEvents.Count(); i++) {
            if (!m_ActiveEvents[i].IsActive()) {
                #ifdef BASICMAP
                if (m_BasicMapMarkers) {
                    if (m_BasicMapMarkers.Find(m_ActiveEvents[i].GetBasicMapMarker()) > -1) m_BasicMapMarkers.RemoveItem(m_ActiveEvents[i].GetBasicMapMarker());

                    UpdateBasicMapMarkers();
                }
                #endif

                #ifdef LBGROUP_SYSTEM
                if (m_ActiveEvents[i].GetAdvancedGroupsMapMarker()) {
                    #ifdef LBmaster_Rework
                    LBStaticMarkerManager.Get.RemoveServerMarker(m_ActiveEvents[i].GetAdvancedGroupsMapMarker());
                    #else
                    LBStaticMarkerManager.Get().RemoveServerMarker(m_ActiveEvents[i].GetAdvancedGroupsMapMarker());
                    #endif
                }
                #endif

                #ifdef EXPANSIONMODNAVIGATION
                m_ActiveEvents[i].RemoveExpansionServerMarker();
                #endif

                #ifdef VanillaPPMap
                m_ActiveEvents[i].RemoveVPPMapMarker();
                #endif

                m_ActiveEvents.Remove(i);
            }
        }
    }

    protected string GetLocation(vector zonePosition) {
        string currentName, locationName, localizedName, worldName;

        vector currentPosition, locationPosition;

        float currentDistance;
        float lowestDistance = int.MAX;

        GetGame().GetWorldName(worldName);

        string configEntry = "CfgWorlds " + worldName + " Names";

        for (int i = 0; i < GetGame().ConfigGetChildrenCount(configEntry); i++) {
            GetGame().ConfigGetChildName(configEntry, i, currentName);
            string tempName = currentName;
            tempName.ToLower();
            if (!tempName.Contains("settlement") && !tempName.Contains("local") && !tempName.Contains("railroadstation") && !tempName.Contains("ruin") && !tempName.Contains("area") && !tempName.Contains("marine")) continue;

            array < float > positionFloatArray = new array < float > ();
            GetGame().ConfigGetFloatArray(configEntry + " " + currentName + " position", positionFloatArray);
            currentPosition[0] = positionFloatArray[0];
            currentPosition[2] = positionFloatArray[1];
            currentPosition[1] = GetGame().SurfaceY(currentPosition[0], currentPosition[2]);

            currentDistance = vector.Distance(currentPosition, zonePosition);
            if (currentDistance < lowestDistance) {
                //GetGame().ConfigGetText(configEntry + " " + currentName + " name", localizedName);
                //locationName = string.Format("%1 (%2)", localizedName, currentName.SubstringInverted(currentName, 0, currentName.IndexOf("_") + 1));
                locationName = string.Format("%1", currentName.SubstringInverted(currentName, 0, currentName.IndexOf("_") + 1));
                lowestDistance = currentDistance;
                locationPosition = currentPosition;
            }
        }

        return locationName;
    }

    protected void Init() {
        if (KOTH_Settings.IsEnabled()) {
            m_ActiveEvents = new array < ref KOTH_Event > ;
            m_CooldownZones = new map < string, float > ;

            m_EventTimer = new Timer();
            m_MainTimer = new Timer();
            m_SpawnDelayTimer = new Timer();

            m_Players = new array < Man > ;

            if (KOTH_Settings.IsUseMapMarker()) {
                #ifdef BASICMAP
                m_BasicMapMarkers = new array < ref BasicMapMarker > ;
                #endif
            }

            if (KOTH_Settings.GetTimeSpawn() > 0) {
                m_SpawnDelayTimer.Run(KOTH_Settings.GetTimeSpawn(), this, "StartMainThread", NULL, false);
            } else {
                StartMainThread();
            }
        }
    }

    protected void StartMainThread() {
        KOTH_Log.LogVerbose("Starting main thread");
        m_MainTimer.Run(MAIN_THREAD_INTERVAL, this, "Update", NULL, true);
    }

    protected void StartEvent() {
        bool zoneFound = false;
        int zoneAttempts = 0;
        KOTH_Zone kothZone;

        while (!zoneFound) {
            bool zoneConflict;
            kothZone = KOTH_Settings.GetZones().GetRandomElement();
            KOTH_Log.LogVerbose("Testing zone with name: " + kothZone.GetName());

            if (m_CooldownZones.Contains(kothZone.GetName())) zoneConflict = true;

            if (!zoneConflict) {
                foreach(KOTH_Event checkEvent: m_ActiveEvents) {
                    if (checkEvent.GetZone().GetName() == kothZone.GetName()) zoneConflict = true;
                }
            }

            if (!zoneConflict) {
                zoneFound = true;
                continue;
            }

            zoneAttempts++;

            if (zoneAttempts > 10) {
                KOTH_Log.LogVerbose("Maximum attempts to find new zone reached.");
                return;
            }
        }

        KOTH_Event kothEvent = new KOTH_Event(kothZone);
        m_ActiveEvents.Insert(kothEvent);

        string locationName = GetLocation(kothZone.GetPosition());
        kothEvent.SetLocationName(locationName);

        KOTH_Log.LogVerbose(string.Format("Starting new KOTH event [%1]", locationName));

        string notificationMessage = string.Format("New event [%1] starting", kothZone.GetName());

        if (KOTH_Settings.IsEstimateLocation()) notificationMessage = string.Format("%1 at %2", notificationMessage, locationName);
        else notificationMessage = string.Format("%1...", notificationMessage);

        SendKOTHNotification(notificationMessage);

        if (KOTH_Settings.IsUseMapMarker()) {
            #ifdef BASICMAP
            if (m_BasicMapMarkers) {
                m_BasicMapMarkers.Insert(kothEvent.CreateBasicMapMarker());
                UpdateBasicMapMarkers();
            }
            #endif

            #ifdef LBGROUP_SYSTEM_NEW
            kothEvent.CreateAdvancedGroupsMapMarker();
            #endif

            #ifdef EXPANSIONMODNAVIGATION
            kothEvent.CreateExpansionServerMarker();
            #endif

            #ifdef VanillaPPMap
            kothEvent.CreateVPPMapMarker();
            #endif
        }

        if (!m_EventTimer.IsRunning()) m_EventTimer.Run(1, this, "UpdateEvents", NULL, true);

        m_TimeSinceLastEvent = 0;
        m_TimeToNextEvent = 0;
    }

    protected array < Man > SyncPlayers(KOTH_Event targetEvent) {
        array < Man > eventPlayers = new array < Man > ;

        foreach(Man player: m_Players) {
            if (vector.Distance(targetEvent.GetPosition(), player.GetPosition()) <= targetEvent.GetRadius() && player.IsAlive()) eventPlayers.Insert(player);
        }

        return eventPlayers;
    }

    protected void Update() {
        m_TimeSinceLastEvent += MAIN_THREAD_INTERVAL;

        UpdateCooldowns(MAIN_THREAD_INTERVAL);

        if (m_TimeToNextEvent == 0) m_TimeToNextEvent = Math.RandomFloatInclusive(KOTH_Settings.GetMinTimeBetweenEvents(), KOTH_Settings.GetMaxTimeBetweenEvents());

        if (m_TimeToNextEvent < m_TimeSinceLastEvent) {
            if (m_ActiveEvents.Count() < KOTH_Settings.GetMaxEvents()) {
                UpdatePlayers();

                if (m_Players.Count() >= KOTH_Settings.GetMinPlayerCount()) {
                    StartEvent();
                } else {
                    m_TimeSinceLastEvent = 0;
                    m_TimeToNextEvent = 0;
                    KOTH_Log.LogVerbose("Not enough players online, resetting timer and waiting.");
                }
            } else {
                KOTH_Log.LogVerbose("Max events running, waiting...");
            }
        }
    }

    protected void UpdateEvents() {
        CleanupEvents();
        UpdatePlayers();

        for (int i = 0; i < m_ActiveEvents.Count(); i++) {
            if (m_ActiveEvents[i].IsActive()) {
                m_ActiveEvents[i].UpdatePlayers(SyncPlayers(m_ActiveEvents[i]));
                m_ActiveEvents[i].Update();
            }
        }
    }

    protected void UpdatePlayers() {
        GetGame().GetPlayers(m_Players);
    }

    protected void UpdateCooldowns(float deltaTime) {
        foreach(string zoneName, float zoneTime: m_CooldownZones) {
            if (zoneTime - deltaTime <= 0) m_CooldownZones.Remove(zoneName);
            else m_CooldownZones.Set(zoneName, zoneTime - deltaTime);
        }
    }

    void AddCooldown(string zone) {
        if (KOTH_Settings.GetTimeZoneCooldown() <= 0) return;

        if (!m_CooldownZones.Contains(zone)) m_CooldownZones.Insert(zone, KOTH_Settings.GetTimeZoneCooldown());
    }

    void SendKOTHNotification(string message) {
        if (!KOTH_Settings.IsUseNotifications()) return;

        UpdatePlayers();

        for (int i = 0; i < m_Players.Count(); ++i) {
            NotificationSystem.SendNotificationToPlayerExtended(m_Players[i], 5, "King of the Hill", message, "set:dayz_gui image:icon_flag");
        }
    }

    void UpdateBasicMapMarkers(PlayerIdentity target = NULL) {
        #ifdef BASICMAP
        if (m_BasicMapMarkers) BasicMap().SetMarkersRemote("KOTH", m_BasicMapMarkers, target);
        #endif
    }
}