﻿using Sad;

public class Player : SadBehaviour 
{
    void Awake()
    {
        Log.Debug($"Player.Awake => {GUID}");

        // 1. AddComponent
        // 2. std::string m_ClassName -> std::list<std::string> m_ClassNames
        // 3. AddScriptComponent
        // 4. Instantiate / InstantiateWithResource
        // 5. Hot Reloading

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

namespace SampleScripts
{
    public class PlayerController : SadBehaviour
    {
        void Awake()
        {
            Log.Debug($"PlayerController.Awake => {GUID}");
        }

        void Update()
        {
            Log.Debug("PlayerController.Update");
        }
    }
}
