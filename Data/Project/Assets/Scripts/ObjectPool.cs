using Sad;
using System;
using System.Collections;
using System.Collections.Generic;

public class ObjectPool : SadBehaviour
{
    public int activeEnemyCount = 0;


    private Queue enemies = new Queue(5);
    private int queueSize = 5;

    public ObjectPool(int _queueSize)
    {
        queueSize = _queueSize;
        for (int i = 0; i < queueSize; i++)
        {
            // Instantiate new Entity (without model)
            Entity enemyEntity = InstantiateWithModelWithPosition("Enemy", "Ghost_Mob.fbx", new Vector3(0, -100, 0));

            // Add Enemy script to newly generated Entity
            enemyEntity.AddScriptComponent<Enemy>();
            Enemy enemy = enemyEntity.GetScriptComponent<Enemy>();
            enemy.transform.Scale(new Vector3(0.1f));
            enemies.Enqueue(enemy);
        }
    }

    public Enemy GetEnemyFromPool()
    {
        if (enemies.Count == 0)
        {
            ExpandPool();
        }
        Enemy enemy = (Enemy)enemies.Dequeue();
        enemy.transform.position = GameManager.Instance.MGenRandomEnemyLocation();
        enemy.SetActive(true);
        activeEnemyCount++;
        return enemy;
    }

    public void ReturnEnemyToPool(Enemy enemy)
    {
        enemy.SetActive(false);
        enemy.transform.position = new Vector3(0, -100, 0);
        activeEnemyCount--;
    }

    private void ExpandPool()
    {
        for (int i = 0; i < queueSize; i++)
        {
            // Instantiate new Entity (without model)
            Entity enemyEntity = InstantiateWithModelWithPosition("Enemy", "Ghost_Mob.fbx", new Vector3(0, -100, 0));

            // Add Enemy script to newly generated Entity
            enemyEntity.AddScriptComponent<Enemy>();
            Enemy enemy = enemyEntity.GetScriptComponent<Enemy>();
            enemy.transform.Scale(new Vector3(0.1f));
            enemies.Enqueue(enemy);
        }
        queueSize *= 2;
    }
}
