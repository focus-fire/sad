using Sad;
using System;

public class HelloWorld
{
    public int m_Counter = 1;
    public string m_TestString = "You just sent ";

    public void Increment(int i) => m_Counter += i;

    public void AddNumbers(int i, int j) => m_Counter = m_Counter + i + j; 

    public void AddStrings(string i) => m_TestString += i;

    public void PrintIntegerVariable()
    {
        System.IO.File.WriteAllLines($"SadCSFrameworkTest{m_Counter}.txt", 
            new string[] { "pog!", $"Integer -> {m_Counter}", $"{m_TestString}" });

        Console.WriteLine($"Hello World! This int is {m_Counter}");
        
        Core.Log($"This is a test log from C# with a variable {m_Counter}");
    }
}