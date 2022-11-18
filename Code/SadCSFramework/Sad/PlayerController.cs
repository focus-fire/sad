//using Sad;
using Sad.Internal;

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
            // INPUT TESTS
            if (Input.GetButton(ControllerButton.South))
            {
                Log.Warn("South Button On!!!");
            }

            // Isn't working properly, possibly related to delay/syncing across scripting-engine communication
            if (Input.GetKeyPressed(KeyCode.Space))
            {
                Log.Warn("Space Key Pressed!!!");
            }

            // Isn't working properly, possibly related to delay/syncing across scripting-engine communication
            if (Input.GetKeyReleased(KeyCode.Space))
            {
                Log.Warn("Space Key Released!!!");
            }

            if (Input.GetMouseButton(MouseButton.Right))
            {
                Log.Warn("Right Mouse Pressed!!!");
            }

            //Log.Warn($"{Misc.Getdt()}");

            // Works
            // Log.Warn(Input.GetMousePosition().x.ToString());
        }
    }
}
