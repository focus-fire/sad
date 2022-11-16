using Sad;

namespace SampleScripts
{
    class SadBehaviour
    { }

    class PlayerController : SadBehaviour
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
