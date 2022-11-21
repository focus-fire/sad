using Sad.Internal;

namespace Sad
{
    public class Audio
    {
        public static void PlaySFX(string audioString) => Internal.Audio.PlaySFX(audioString);

        public static void PlayMusic(string audioString, int times) => Internal.Audio.PlayMusic(audioString, times);

        public static void SetVolume(string audioString, int volume) => Internal.Audio.SetVolume(audioString, volume);
    }
}
