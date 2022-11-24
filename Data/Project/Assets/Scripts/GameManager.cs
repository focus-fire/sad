using Sad;
using System;
using System.Collections.Generic;

public class GameManager : SadBehaviour
{
    public static GameManager Instance { get; private set; }

    public List<Enemy> Enemies;
    public Player Player;

    private int m_CurrentWave;
    private int m_MinSpawnRange;
    private int m_MaxSpawnRange;

    void Awake()
    {
        // Instantiate Singleton
        if (Instance != null && Instance != this)
            Destroy(this);
        else
            Instance = this;

        // Instantiate member variables
        Entity e = FindEntityWithName("Player"); // The Player
        Player = e.GetScriptComponent<Player>();
        Player.transform.position = Vector3.zero;

        m_MinSpawnRange = -10;
        m_MaxSpawnRange = 10;

        Log.Debug($"Player info: {Player.GUID}");

        Enemies = new List<Enemy>();
        m_CurrentWave = 0;

        Log.Debug($"GameManager created! Current Wave: {m_CurrentWave}");

        MSpawnEnemy();
    }

    void Update()
    {
        if (Enemies.Count == 0) {
            MSpawnWave();
            Upgrade();
        }
    }

    /// <summary>
    /// Spawn a wave of enemies. Number of spawned enemies depends on the current wave (?)
    /// </summary>
    private void MSpawnWave()
    {
        m_CurrentWave++;
        Log.Debug($"Spawning new Wave of enemies! Current Wave: {m_CurrentWave}");

        // TODO: Logic for # of enemies spawned. Temporarily spawn only 1 enemy.
        for (int i = 0; i <= m_CurrentWave; i++) {
            MSpawnEnemy();
            if (m_CurrentWave % 2 == 0)
            {
                Upgrade();
            }
        }
    }

    /// <summary>
    /// Spawn a single enemy.
    /// </summary>
    private void MSpawnEnemy()
    {
        // TODO: Instantiate enemies with models and provide spawn locations.

        // Instantiate new Entity (without model)
        Entity enemyEntity = InstantiateWithModelWithPosition("Enemy", "Ghost_Mob.fbx", MGenRandomEnemyLocation());

        // Add Enemy script to newly generated Entity
        enemyEntity.AddScriptComponent<Enemy>();
        Enemy enemy = enemyEntity.GetScriptComponent<Enemy>();
        enemy.transform.Scale(new Vector3(0.1f));

        Log.Debug($"Found an enemy with {enemy?.Health} health @ {enemy.transform.position}");

        Enemies.Add(enemy);
    }

    private void Upgrade()
    {
        Player.PrimaryAttackDamage = Player.PrimaryAttackDamage + 5000000;
        Log.Debug($"Wave {m_CurrentWave} reached {Player.PrimaryAttackDamage} health "); 
    }

    /// <summary>
    /// Randomly generate an enemy spawn location on X- and Z-axis within a specified range.
    /// </summary>
    /// <returns></returns>
    private Vector3 MGenRandomEnemyLocation()
    {
        Random rand = new Random();
        int x = rand.Next(m_MinSpawnRange, m_MaxSpawnRange);
        int z = rand.Next(m_MinSpawnRange, m_MaxSpawnRange);

        return new Vector3(x, 0, z);
    }
}
