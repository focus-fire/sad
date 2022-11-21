using Sad;

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
}
