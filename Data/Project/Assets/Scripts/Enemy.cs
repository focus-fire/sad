using Sad;
using System;
using System.Diagnostics;

public class Enemy : SadBehaviour
{
    public int Health = 100; 
    public int Damage = 10;

    private float m_RotSpeed;
    private float m_MoveSpeed;

	// enemy bob stats
	private float m_StartingHeight;
	private float m_SpeedUpDown;
	private float m_DistanceUpDown;

	void Awake() 
    { 
        // Testing updates to enemy public variables
        Health = 50;
        Damage = 5;

        m_RotSpeed = 3.0f;
	    m_MoveSpeed = 0.1f;

		m_SpeedUpDown = 3.0f;
		m_DistanceUpDown = 1.0f;
		m_StartingHeight = transform.position.y;

		Quaternion quatStart = Quaternion.LookAt(Vector3.zero, Vector3.zero, Vector3.zero);
    }

    void Update() 
    {
        Move();
    }

    /// <summary>
    /// Take damage from another entity's attack. Die when Health reaches 0.
    /// </summary>
    /// <param name="incomingDamage"></param>
    public void TakeDamage(int incomingDamage)
    {
        Health -= incomingDamage;

        Log.Debug($"Enemy#{this?.GUID} took {incomingDamage} damage! New Health is {Health}");

        if (Health <= 0)
            Die();
	}

    void Die()
    {
        // Enemy kills itself after removing itself from list of existing Enemies
        Log.Debug($"Enemy#{this?.GUID} got dead.");
        this.RemoveScriptComponent<Enemy>();
        GameManager.Instance.Enemies.Remove(this);
        Destroy(this);
    }

	void EnemyBobbing() 
    {
		transform.position = new Vector3(transform.position.x, m_StartingHeight, transform.position.z) +
			new Vector3(0.0f, (float)Math.Sin(Time.dt * m_SpeedUpDown) * m_DistanceUpDown, 0);
	}

    void Move()
    {
		/* look at player*/
		transform.rotation = Quaternion.Slerp(transform.rotation
			, Quaternion.LookAt(GameManager.Instance.Player.transform.position - transform.position, transform.position, Vector3.up)
			, m_RotSpeed * Time.dt);

        /* move at player*/
        //transform.position += Vector3.forward * m_MoveSpeed * Time.dt;
        transform.Translate(Vector3.Normalize(GameManager.Instance.Player.transform.position - transform.position) * m_MoveSpeed);

		EnemyBobbing();

        Log.Debug($"position of enemy: {transform.position}");
        //Log.Debug($"rotation of enemy: {transform.rotation}");
	}
}
