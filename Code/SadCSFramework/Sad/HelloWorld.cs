using System;

namespace Sad 
{
    public class HelloWorld
    {
        public int counter = 1;

        public void Increment(int i) => counter += i;

        public void PrintIntegerVariable()
        {
            System.IO.File.WriteAllLines($"SadCSFrameworkTest{counter}.txt", 
                new string[] { "You triggered this from a C# script, pog!", $"Integer -> {counter}" });

            Console.WriteLine($"Hello World! This int is {counter}");
        }
    }
}