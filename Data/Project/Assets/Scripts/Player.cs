using Sad;
using System.Collections.Generic;
using System.Diagnostics;

public class Player : SadBehaviour
{
    public int PlayerHealth = 100;
    public int PrimaryAttackDamage = 50;
    private float m_MoveSpeed = 1f;

    void Awake()
    {
        PlayerHealth = 100;
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
        Vector3 firingDirection = Vector3.Normalize(transform.forward) * 10000.0f;

        // Check if any enemies are hit by the player's attack
        for (int i = 0; i < GameManager.Instance.Enemies.Count; ++i)
        {
            bool hit = Raycast.Intersects(initialFiringPos, firingDirection, GameManager.Instance.Enemies[i]);

            if (hit)
            { 
                if (hit) Log.Debug("HIT");
                GameManager.Instance.Enemies[i].TakeDamage(PrimaryAttackDamage);
            }
        }
    }

    public void TakeDamage(int incomingDamage)
    {
        PlayerHealth -= incomingDamage;

        Log.Debug($"PLAYER HEALTH: {PlayerHealth}");

        if (PlayerHealth <= 0)
            Die();
    }

    void Die()
    {
        Log.Debug("YOU DEAD! BYE BYE!");
    }

    void Move()
    {
        // TODO: Integrate camera rotations

        // Move Forward
        if (Input.GetKey(KeyCode.W))
        {
            transform.position += transform.rotation * Vector3.forward * Time.dt * m_MoveSpeed;
            Audio.PlaySFX("step.wav");
        }

        // Move Left
        if (Input.GetKey(KeyCode.A))
        {
            transform.position += transform.rotation * Vector3.right * Time.dt * m_MoveSpeed;
            Audio.PlaySFX("step.wav");
        }

        // Move Backward 
        if (Input.GetKey(KeyCode.S))
        {
            transform.position += transform.rotation * Vector3.backward * Time.dt * m_MoveSpeed;
            Audio.PlaySFX("step.wav");
        }

        // Move Right 
        if (Input.GetKey(KeyCode.D))
        {
            transform.position += transform.rotation * Vector3.left * Time.dt * m_MoveSpeed;
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
