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
