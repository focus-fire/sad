using Sad;
using System.Collections.Generic;
using System.Diagnostics;

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
        Vector3 firingDirection5 = Vector3.Normalize(transform.forward) * 10000.0f;

        // Check if any enemies are hit by the player's attack
        for (int i = 0; i < GameManager.Instance.Enemies.Count; ++i)
        {
            bool hit5 = Raycast.Intersects(initialFiringPos, firingDirection5, GameManager.Instance.Enemies[i]);

            if (hit5)
            { 
                if (hit5) Log.Debug("HIT");
                GameManager.Instance.Enemies[i].TakeDamage(m_PrimaryAttackDamage);
            }
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
            transform.position += transform.forward * Time.dt * m_MoveSpeed;
            Audio.PlaySFX("step.wav");
        }

        // Move Left
        if (Input.GetKey(KeyCode.A))
        {
            transform.position += transform.right * Time.dt * m_MoveSpeed;
            Audio.PlaySFX("step.wav");
        }

        // Move Backward 
        if (Input.GetKey(KeyCode.S))
        {
            transform.position += transform.backward * Time.dt * m_MoveSpeed;
            Audio.PlaySFX("step.wav");
        }

        // Move Right 
        if (Input.GetKey(KeyCode.D))
        {
            transform.position += transform.left * Time.dt * m_MoveSpeed;
            Audio.PlaySFX("step.wav");
        }
    }

    void DrawGizmos()
    {
        Vector3 lineStart = transform.position;
        Vector3 lineEnd = Vector3.Normalize(transform.forward) * 10000.0f;
        Color lineColor = Color.magenta;
        Gizmos.DrawLine(lineStart, lineEnd, lineColor);

        Vector3 boxMin = bound.min;
        Vector3 boxMax = bound.max;
        Color boxColor = Color.red;
        Gizmos.DrawBox(boxMin, boxMax, boxColor);
    }
}
