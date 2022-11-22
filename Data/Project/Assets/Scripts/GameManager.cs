using Sad;
using System.Collections.Generic;

public class GameManager : SadBehaviour
{
    public static GameManager Instance { get; private set; }

    public List<Enemy> Enemies;

    private int m_CurrentWave;

    void Awake()
    {
        // Instantiate Singleton
        if (Instance != null && Instance != this)
            Destroy(this);
        else
            Instance = this;

        // Instantiate member variables
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
}
