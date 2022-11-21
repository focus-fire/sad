using Sad;
using System.Collections.Generic;

public class Player : SadBehaviour
{
    public static List<Enemy> Enemies;

    private int m_PrimaryAttackDamage = 100;

    void Awake()
    {
        Enemies = new List<Enemy>();

        Log.Debug($"PlayerController.Awake => {GUID}");

        // Testing Instantiation
        Entity EnemyCube = Instantiate("EnemyCube");
        Log.Debug($"Instantiated an entity with GUID => {EnemyCube?.GUID}");

        // Testing Finds
        Entity foundEnemyCube = FindEntityWithName("EnemyCube");
        Log.Debug($"Found a component with GUID => {foundEnemyCube?.GUID}");

        // Testing Script Adding/Getting
        foundEnemyCube.AddScriptComponent<Enemy>();
        Enemy e = foundEnemyCube.GetScriptComponent<Enemy>();
        Log.Debug($"Found an enemy with {e?.Health} health");

        Enemies.Add(e);
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

        if (Input.GetMouseButton(MouseButton.Left))
        {
            // Shoot raycast forward
            Log.Warn("LMB Clicked!!!");
            PrimaryAttack();
        }

        if (Input.GetMouseButton(MouseButton.Right))
        {
            Log.Warn("Right Mouse Pressed!!!");
            transform.Translate(new Vector3(-moveSpeed, -moveSpeed, -moveSpeed));
        }
    }

    /// <summary>
    /// Primary attack that is performed on Left Mouse Button click.
    /// </summary>
    void PrimaryAttack()
    {
        // Fire Raycast from current position to infinity (and beyond!), where the player is aiming
        Vector3 initialFiringPos = this.transform.position;
        Vector3 firingDirection = Vector3.forward * SadMath.Infinity;
        bool hit;

        // Check if any enemies are hit by the player's attack
        for (int i = 0; i < Enemies.Count; ++i)
        {
            hit = Raycast.Intersects(initialFiringPos, firingDirection, Enemies[i]);

            if (hit)
                Enemies[i].TakeDamage(m_PrimaryAttackDamage);
        }
    }

    /// <summary>
    /// Offhand attack that is performed on (?) input.
    /// </summary>
    void OffhandAttack()
    {
    }

    void DrawGizmos()
    {
        Vector3 lineStart = transform.position;
        Vector3 lineEnd = Vector3.up * 2.0f;
        Color lineColor = Color.magenta;
        Gizmos.DrawLine(lineStart, lineEnd, lineColor);

        Vector3 rayStart = transform.position;
        Vector3 rayEnd = Vector3.forward * SadMath.Infinity;
        Color rayColour = Color.green;
        Gizmos.DrawLine(rayStart, rayEnd, rayColour);

        Vector3 boxMin = bound.min;
        Vector3 boxMax = bound.max;
        Color boxColor = Color.red;
        Gizmos.DrawBox(boxMin, boxMax, boxColor);
    }
}