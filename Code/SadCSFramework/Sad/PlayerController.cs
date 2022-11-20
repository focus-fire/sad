using Sad;

public class Player : SadBehaviour 
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
        Vector3 lineColor = new Vector3(1.0f, 0.0f, 0.0f);
        Gizmos.DrawLine(lineStart, lineEnd, lineColor);

        Vector3 boxMin = bound.min;
        Vector3 boxMax = bound.max;
        Vector3 boxColor = new Vector3(1.0f, 0.0f, 1.0f);
        Gizmos.DrawBox(boxMin, boxMax, boxColor);
    }
}

namespace SampleScripts
{
    public class PlayerController : SadBehaviour
    {
        void Awake()
        {
            Log.Debug("PlayerController.Awake");
        }

        void Update()
        {
            Log.Debug("PlayerController.Update");
        }
    }
}
