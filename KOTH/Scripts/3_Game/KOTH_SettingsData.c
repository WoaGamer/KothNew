class KOTH_SettingsData {
    int configVersion; // Internal

    bool enabled; // KOTH events enabled
    int loggingLevel; // 0 = off, 1 = normal, 2 = basic, 3 = verbose

    bool useLocationText; // Use LocationText notification
    bool useMapMarker; // Add marker to Advanced Groups / BasicMap / Expansion / VanillaPlusPlusMap
    bool useNotifications; // Use vanilla notifications system
    bool reduceProgressOnAbandoned; // Reduce progress of zone when no players are within radius
    bool reduceProgressOnDeathFromOutside; // Reduce progress of zone on every player death when killed from outside zone
    bool requireFlagConstruction; // Require players to build the flag before the timer begins.
    bool estimateLocation; // Find nearest location to hill and display in notification
    bool celebrateWin; // Launch fireworks to celebrate a victory
    bool punishLoss; // Spawn toxic zone on failure of event

    float baseCaptureTime; // Base time to capture zone with a single player within radius.
    float maxTimeBetweenEvents; // Maximum time between events
    float minTimeBetweenEvents; // Minimum time between events
    float playerTimeMultiplier; // Each additional player in zone will affect basetime by this multiplier
    float timeDespawn; // Time taken for an unpopulated event to despawn
    float timeLimit; // Maximum time limit a zone can take to be captured
    float timeStart; // Minimum time to start the event

    float timeSpawn; // Time to wait before spawning events
    float timeZoneCooldown; // Time a zone will go on cooldown
    float timeGasDuration; // Time gas will remain before dissipating 

    int minPlayerCount; // Minimum number of players that must be online to spawn events.

    int maxEnemyCount; // Maximum number of enemies to spawn
    int minEnemyCount; // Minimum number of enemies to spawn
    int maxEvents; // Maximum events that can run simultaneously
    int minimumDeaths; // Minimum deaths in zone required to pay reward
    int minimumPlayers; // Minimum players required to be in zone to start event
    int maximumPlayers; // Maximum players that can be alive and in zone for completion of event

    int rewardCount; // Number of loot sets to spawn

    string flagClassname; // Use this class for the flag

    ref array < string > enemies = {
        "ZmbM_PatrolNormal_Autumn",
            "ZmbM_PatrolNormal_Flat",
            "ZmbM_PatrolNormal_PautRev",
            "ZmbM_PatrolNormal_Summer",
            "ZmbM_SoldierNormal",
            "ZmbM_usSoldier_normal_Desert",
            "ZmbM_usSoldier_normal_Woodland"
    };

    string lootCrate; // Class of loot crate
    int crateLifeTime; // Custom CE lifetime for crate

    ref array < string > lootSets = {};

    ref array < ref KOTH_Zone > zones = new array < ref KOTH_Zone > ;

    void InitDefaults(string worldName) {
        configVersion = 1;

        enabled = true;
        loggingLevel = 2;

        useLocationText = false;
        useMapMarker = true;
        useNotifications = true;
        reduceProgressOnAbandoned = true;
        reduceProgressOnDeathFromOutside = true;
        requireFlagConstruction = false;
        celebrateWin = true;
        estimateLocation = false;

        baseCaptureTime = 900;
        maxTimeBetweenEvents = 1200;
        minTimeBetweenEvents = 600;
        playerTimeMultiplier = 1.5;
        timeDespawn = 120;
        timeLimit = 0;
        timeStart = 30;
        timeSpawn = 120;
        timeZoneCooldown = 120;
        timeGasDuration = 300;

        minPlayerCount = 1;

        maxEnemyCount = 20;
        minEnemyCount = 5;
        maxEvents = 2;
        minimumDeaths = 0;
        minimumPlayers = 1;
        maximumPlayers = 4;

        rewardCount = 2;

        flagClassname = "Flag_DayZ";

        lootCrate = "KOTH_Chest";

        worldName.ToLower();

        switch (worldName) {
            case "chernarusplus":
            case "chernarusplusgloom":
                RegisterZone("Tisy Military", Vector(1677.6, 451.0, 14348.9), 50);
                RegisterZone("Grozovy Military", Vector(3669.2, 363.0, 14802.8), 20);
                RegisterZone("Kamensk Military", Vector(7105.2, 355.0, 14582.8), 20);
                RegisterZone("Novy Lug Military", Vector(9559.2, 242.0, 11802.8), 20);
                RegisterZone("Krasno Airfield", Vector(11871.2, 140.0, 12552.8), 20);
                RegisterZone("Staroye Military", Vector(10432.2, 283.0, 5954.8), 20);
                RegisterZone("Pavlovo Military", Vector(2194.2, 91.0, 3363.8), 20);
                RegisterZone("Myshkino Military", Vector(1161.2, 185.0, 7248.8), 20);
                RegisterZone("VMC Military", Vector(4564.2, 317.0, 8280.8), 20);
                RegisterZone("Zelengorsk Military", Vector(2515.2, 193.0, 5078.8), 20);
                RegisterZone("Vybor Airfield", Vector(4643.2, 339.0, 10464.8), 20);
                RegisterZone("Kamensk Military Base", Vector(8006.3, 339.5, 14627.9), 20);
                break;

            case "namalsk":
                RegisterZone("Airfield", Vector(6247.6, 21.1, 9314.9), 50);
                RegisterZone("Factory", Vector(6473.2, 16.0, 9305.8), 20);
                RegisterZone("BK-T07", Vector(4408.0, 15.0, 10736.4), 20);
                RegisterZone("BK-M06", Vector(4847.4, 13.2, 10869.5), 50);
                RegisterZone("Citadel", Vector(4685.6, 23.5, 8910.3), 50);
                RegisterZone("Eagle's Nest", Vector(4084.0, 70.3, 9212.82), 25);
                RegisterZone("RefugeeCamp", Vector(7310.3, 45.5, 10525.86), 20);
                RegisterZone("Seraja", Vector(4202.8, 390.65, 6607.4), 50);
                RegisterZone("Tara", Vector(7254.5, 238.9, 7077.0), 50);
                RegisterZone("Uranium Mine", Vector(5074.4, 39.5, 8172.8), 50);
                RegisterZone("Vorkuta", Vector(6685.6, 15.0, 11158.7), 50);
                break;

            case "deerisle":
                RegisterZone("Alcatraz", Vector(15757.2, 22.3, 5102.7), 75);
                RegisterZone("Area 42 Prison", Vector(13282.0, 12.9, 9833.4), 20);
                RegisterZone("Broken Houses", Vector(14285.1, 2.5, 6458.3), 20);
                RegisterZone("Crater", Vector(14145.9, 134.1, 2642.0), 25);
                RegisterZone("Fort Knox", Vector(3779.5, 16.8, 9050.9), 20);
                RegisterZone("Paris Island", Vector(2481.5, 35.1, 3352.0), 100);
                RegisterZone("Prison", Vector(5522.8, 34.85, 681.0), 25);
                RegisterZone("Rockenheim", Vector(9790.0, 40.0, 11093.1), 20);
                break;

            default:
                RegisterZone("Dummy", Vector(0.0, 0.0, 0.0), 50);
                break;
        }
    }

    protected void RegisterZone(string name, vector position, int radius) {
        KOTH_Zone zone = new KOTH_Zone(name, position, radius);
        zone.InitDefaults();
        zones.Insert(zone);
    }
}