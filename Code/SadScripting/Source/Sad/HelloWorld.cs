using System;

namespace Sad 
{
    public class HelloWorld
    {
        public float HelloFloat = 5.0f;

        public void PrintFloatVariable()
        {
            Console.WriteLine("Hello World! This float is {0:F}", HelloFloat);
        }
    }
}