#ifdef BASICMAP

/**
 * In some deployments the BasicMap mod might not be present. When it is
 * missing the original <code>BasicMapController</code> class is undefined
 * causing a compilation error.  To avoid this we provide a very small stub
 * implementation that satisfies the compiler.  The methods simply perform no
 * action which means the map marker feature is effectively disabled when the
 * real module is not available.
 */
class BasicMapMarker {}

class BasicMapController extends Managed
{
    bool IsInit;

    void SetMarkers(string group, array<autoptr BasicMapMarker> markers) {}
    void SetMarkersRemote(string group, array<ref BasicMapMarker> markers, PlayerIdentity target = NULL) {}
    void RequestGroupUpdate(string group) {}

    void Init()
    {
        if (IsInit)
            SetMarkers("KOTH", new array<autoptr BasicMapMarker>);
    }
}

static ref BasicMapController g_BasicMap;

BasicMapController BasicMap()
{
    if (!g_BasicMap)
        g_BasicMap = new BasicMapController();
    return g_BasicMap;
}

#endif
