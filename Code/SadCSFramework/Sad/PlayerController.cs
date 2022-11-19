using Sad;

public class Player : SadBehaviour 
{
    void Awake()
    {
        Log.Debug($"Player.Awake => {GUID}");

        bool hasTransform = HasComponent<Transform>();
        string yay = hasTransform ? "a transform" : "F";
        Log.Debug($"Player has.... {yay}");
    }

    void Update()
    {
        Log.Debug("Player.Update");

        float speed = 0.001f;
        transform.Translate(new Vector3(speed, speed, speed));
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
