namespace Sad
{
    public class Time
    {
        /// <summary>
        /// Time just delta'ed
        /// </summary>
        public static float dt
        {
            get => Internal.Misc.Getdt();
        }
    }
}
