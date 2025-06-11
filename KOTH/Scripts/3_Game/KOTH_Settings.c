class KOTH_Settings {
    private static string m_Directory = "$profile:KOTH";
    private static string m_Path = m_Directory + "/%1.json";
    private static string m_WorldName;

    static ref KOTH_SettingsData m_Data = new KOTH_SettingsData;

    static bool LoadData() {
        GetGame().GetWorldName(m_WorldName);

        if (!FileExist(m_Directory))
            MakeDirectory(m_Directory);

        string cfgPath = string.Format(m_Path, m_WorldName);

        if (!FileExist(cfgPath)) {
            KOTH_Log.LogVerbose("Writing default config.");
            m_Data.InitDefaults(m_WorldName);
            SaveData();
        } else {
            JsonFileLoader<KOTH_SettingsData>.JsonLoadFile(cfgPath, m_Data);
            UpgradeData();
            SaveData(); // Update settings
        }

        OnLoaded();

        return true;
    }

    static bool SaveData() {
        GetGame().GetWorldName(m_WorldName);
        JsonFileLoader < KOTH_SettingsData > .JsonSaveFile(string.Format(m_Path, m_WorldName), m_Data);
        return true;
    }

    static void UpgradeData() {
        if (m_Data.configVersion < 1) {
            m_Data.timeGasDuration = 300;

            foreach(KOTH_Zone zone: m_Data.zones) {
                zone.timeGasDuration = -1;
            }

            m_Data.configVersion = 1;
        }
    }

    static void OnLoaded() {
        KOTH_Log.LogVerbose("Config loaded.");
    }

    static void SyncDataSend(notnull Man player) {
        if (!player)
            return;

        PlayerIdentity identity = player.GetIdentity();
        if (!identity) {
            // Retry later if identity not ready yet
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SyncDataSend, 1000, false, player);
            return;
        }

        GetGame().RPCSingleParam(player, eKOTH.RPC_KOTH_CONFIG_SYNC, new Param1<KOTH_SettingsData>(m_Data), true, identity);
    }

    static void OnRPC(Man player, ParamsReadContext ctx) {
        Param1 < KOTH_SettingsData > data = new Param1 < KOTH_SettingsData > (null);

        if (ctx.Read(data)) m_Data = data.param1;
        else KOTH_Log.LogCritical("Failed to read configuration data.");

        player.OnGameplayDataHandlerSync();
    }

    static bool IsEnabled() {
        return m_Data.enabled;
    }
    static bool IsUseLocationText() {
        return m_Data.useLocationText;
    }
    static bool IsUseMapMarker() {
        return m_Data.useMapMarker;
    }
    static bool IsUseNotifications() {
        return m_Data.useNotifications;
    }

    static bool IsReduceProgressOnAbandoned() {
        return m_Data.reduceProgressOnAbandoned;
    }
    static bool IsReduceProgressOnDeathFromOutside() {
        return m_Data.reduceProgressOnDeathFromOutside;
    }
    static bool IsRequireFlagConstruction() {
        return m_Data.requireFlagConstruction;
    }
    static bool IsCelebrateWin() {
        return m_Data.celebrateWin;
    }
    static bool IsPunishLoss() {
        return m_Data.punishLoss;
    }
    static bool IsEstimateLocation() {
        return m_Data.estimateLocation;
    }


    static float GetBaseCaptureTime() {
        return m_Data.baseCaptureTime;
    }
    static float GetMaxTimeBetweenEvents() {
        return m_Data.maxTimeBetweenEvents;
    }
    static float GetMinTimeBetweenEvents() {
        return m_Data.minTimeBetweenEvents;
    }
    static float GetPlayerTimeMultiplier() {
        return m_Data.playerTimeMultiplier;
    }
    static float GetTimeDespawn() {
        return m_Data.timeDespawn;
    }
    static float GetTimeLimit() {
        return m_Data.timeLimit;
    }
    static float GetTimeSpawn() {
        return m_Data.timeSpawn;
    }
    static float GetTimeStart() {
        return m_Data.timeStart;
    }
    static float GetTimeZoneCooldown() {
        return m_Data.timeZoneCooldown;
    }
    static float GetTimeGasDuration() {
        return m_Data.timeGasDuration;
    }

    static int GetLoggingLevel() {
        return m_Data.loggingLevel;
    }
    static int GetMinPlayerCount() {
        return m_Data.minPlayerCount;
    }

    static int GetMaxEnemyCount() {
        return m_Data.maxEnemyCount;
    }
    static int GetMinEnemyCount() {
        return m_Data.minEnemyCount;
    }

    static int GetMaxEvents() {
        return m_Data.maxEvents;
    }
    static int GetMinimumDeaths() {
        return m_Data.minimumDeaths;
    }
    static int GetMinimumPlayers() {
        return m_Data.minimumPlayers;
    }
    static int GetMaximumPlayers() {
        return m_Data.maximumPlayers;
    }

    static int GetRewardCount() {
        return m_Data.rewardCount;
    }

    static string GetFlagClassname() {
        return m_Data.flagClassname;
    }

    static array < string > GetEnemies() {
        return m_Data.enemies;
    }

    static string GetLootCrate() {
        return m_Data.lootCrate;
    }

    static int GetCrateLifeTime() {
        return m_Data.crateLifeTime;
    }

    static array < string > GetLootSets() {
        return m_Data.lootSets;
    }
    static array < ref KOTH_Zone > GetZones() {
        return m_Data.zones;
    }
}