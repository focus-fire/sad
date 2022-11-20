using Sad;

public class PlayerController : SadBehaviour 
{
    void Awake()
    {
        Log.Debug($"Player.Awake => {GUID}");

        bool hasTransform = HasComponent<Transform>();
        string yay = hasTransform ? "a transform" : "F";
        Log.Debug($"Player has.... {yay}");
    }

    void Update()
    {
        Log.Debug("Player.Update");

        float speed = 0.001f;
        transform.Translate(new Vector3(speed, speed, speed));
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
