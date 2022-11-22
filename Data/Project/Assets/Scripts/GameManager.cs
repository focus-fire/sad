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
        Entity e = FindEntityWithName("TestCube"); // The Player
        Player = e.GetScriptComponent<Player>();

        m_MinSpawnRange = -50;
        m_MaxSpawnRange = 50;

        Log.Debug($"Player info: {Player.GUID}");

        Enemies = new List<Enemy>();
        m_CurrentWave = 0;

        Log.Debug($"GameManager created! Current Wave: {m_CurrentWave}");

        MSpawnEnemy();
    }

    void Update()
    {
        if (Enemies.Count == 0)
            MSpawnWave();
    }

    /// <summary>
    /// Spawn a wave of enemies. Number of spawned enemies depends on the current wave (?)
    /// </summary>
    private void MSpawnWave()
    {
        m_CurrentWave++;
        Log.Debug($"Spawning new Wave of enemies! Current Wave: {m_CurrentWave}");

        // TODO: Logic for # of enemies spawned. Temporarily spawn only 1 enemy.
        MSpawnEnemy();
    }

    /// <summary>
    /// Spawn a single enemy.
    /// </summary>
    private void MSpawnEnemy()
    {
        // TODO: Instantiate enemies with models and provide spawn locations.

        // Instantiate new Entity (without model)
        Entity enemyCube = Instantiate("EnemyCube");

        // Add Enemy script to newly generated Entity
        enemyCube.AddScriptComponent<Enemy>();
        Enemy e = enemyCube.GetScriptComponent<Enemy>();
        e.transform.position = MGenRandomEnemyLocation();

        Log.Debug($"Found an enemy with {e?.Health} health @ {e.transform.position}");

        Enemies.Add(e);
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
