using Sad;

public class Player : SadBehaviour 
{ 
    void Awake()
    {
        Log.Debug($"Player.Awake => {GUID}");
    }

    void Update()
    {
        Log.Debug("Player.Update");

        float speed = 1.0f;

        Vector3 position = transform.Position;
        Log.Debug($"he go fast ! {position}");
        position.y += speed;
        transform.Position = position;
    }
}

namespace SampleScripts
{
    public class PlayerController : SadBehaviour
    {
        void Awake()
        {
            Log.Debug("PlayerController.Awake");
        }

        void Update()
        {
            Log.Debug("PlayerController.Update");
        }
    }
}
