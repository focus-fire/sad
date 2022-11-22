using Sad;
using System.Collections.Generic;

public class Player : SadBehaviour
{
    private int m_PrimaryAttackDamage = 100;
    private float m_MoveSpeed = 1f;

    void Awake()
    {
    }

    void Update()
    {
        Move();

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

        if (Input.GetMouseButtonPressed(MouseButton.Left))
        {
            // Shoot raycast forward
            Log.Warn("LMB Clicked!!!");
            PrimaryAttack();
        }
    }

    /// <summary>
    /// Primary attack that is performed on Left Mouse Button click.
    /// </summary>
    void PrimaryAttack()
    {
        // Fire Raycast from current position to infinity (and beyond!), where the player is aiming
        Vector3 initialFiringPos = transform.position;
        // Currently fires directly in front of the player, not where they're aiming
        Vector3 firingDirection1 = new Vector3(transform.position.x, transform.position.y, transform.position.z * SadMath.Infinity);
        Vector3 firingDirection2 = new Vector3(-transform.position.x, transform.position.y, -transform.position.z * SadMath.Infinity);
        Vector3 firingDirection3 = new Vector3(-transform.position.x, transform.position.y, transform.position.z * SadMath.Infinity);
        Vector3 firingDirection4 = new Vector3(transform.position.x, transform.position.y, -transform.position.z * SadMath.Infinity);

        // Check if any enemies are hit by the player's attack
        for (int i = 0; i < GameManager.Instance.Enemies.Count; ++i)
        {
            bool hit1 = Raycast.Intersects(initialFiringPos, firingDirection1, GameManager.Instance.Enemies[i]);
            bool hit2 = Raycast.Intersects(initialFiringPos, firingDirection2, GameManager.Instance.Enemies[i]);
            bool hit3 = Raycast.Intersects(initialFiringPos, firingDirection3, GameManager.Instance.Enemies[i]);
            bool hit4 = Raycast.Intersects(initialFiringPos, firingDirection4, GameManager.Instance.Enemies[i]);

            if (hit1 || hit2 || hit3 || hit4)
                GameManager.Instance.Enemies[i].TakeDamage(m_PrimaryAttackDamage);
        }
    }

    /// <summary>
    /// Offhand attack that is performed on (?) input.
    /// </summary>
    void OffhandAttack()
    {
    }

    void Move()
    {
        // TODO: Integrate camera rotations

        // Move Forward
        if (Input.GetKey(KeyCode.W))
        {
            transform.position += Vector3.forward * Time.dt * m_MoveSpeed;
        }

        // Move Left
        if (Input.GetKey(KeyCode.A))
        {
            transform.position += Vector3.right * Time.dt * m_MoveSpeed;
        }

        // Move Right
        if (Input.GetKey(KeyCode.S))
        {
            transform.position += Vector3.backward * Time.dt * m_MoveSpeed;
        }

        // Move Backward
        if (Input.GetKey(KeyCode.D))
        {
            transform.position += Vector3.left * Time.dt * m_MoveSpeed;
        }
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
