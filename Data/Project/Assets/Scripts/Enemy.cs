﻿using Sad;

public class Enemy : SadBehaviour
{
    public int Health = 100; 
    public int Damage = 10;

    void Awake() 
    { 
        // Testing updates to enemy public variables
        Health = 50;
        Damage = 5;
    }

    void Update() 
    { }

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
        Player.Enemies.Remove(this);
        Destroy(this);
    }
}