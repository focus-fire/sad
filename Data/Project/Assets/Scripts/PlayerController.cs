using Sad;

public class Test : SadBehaviour
{
    public string test = "Test";
}

public class PlayerController : SadBehaviour
{
    private Entity AnotherCube;
    private int Counter = 0;

    void Awake()
    {
        Log.Debug($"PlayerController.Awake => {GUID}");

        // Testing Instantiation
        // AnotherCube = Instantiate("AnotherCube");
        // Log.Debug($"Instantiated an entity with GUID => {AnotherCube?.GUID}");

        // Testing Finds
        AnotherCube = FindEntityWithName("AnotherCube");
        Log.Debug($"Found a component with GUID => {AnotherCube?.GUID}");

        // Testing Script Retrieval
        //Test t = AnotherCube.GetScriptComponent<Test>();
        //Log.Debug($"Found a test component with a string: {t?.test}");
    }

    void Update()
    {
        float moveSpeed = 0.01f;

        // INPUT TESTS
        if (Input.GetButton(ControllerButton.South))
        {
            Log.Warn("South Button On!!!");
        }

        // Isn't working properly, possibly related to delay/syncing across scripting-engine communication
        if (Input.GetKeyPressed(KeyCode.Space))
        {
            Log.Warn("Space Key Pressed!!!");
        }

        // Isn't working properly, possibly related to delay/syncing across scripting-engine communication
        if (Input.GetKeyReleased(KeyCode.Space))
        {
            Log.Warn("Space Key Released!!!");
        }

        if (Input.GetMouseButton(MouseButton.Right))
        {
            Log.Warn("Right Mouse Pressed!!!");
            transform.Translate(new Vector3(-moveSpeed, -moveSpeed, -moveSpeed));
        }
    }

    void DrawGizmos()
    {
        Vector3 lineStart = transform.position;
        Vector3 lineEnd = Vector3.up * 2.0f;
        Color lineColor = Color.magenta;
        Gizmos.DrawLine(lineStart, lineEnd, lineColor);

        Vector3 boxMin = bound.min;
        Vector3 boxMax = bound.max;
        Color boxColor = Color.red;
        Gizmos.DrawBox(boxMin, boxMax, boxColor);
    }
}
