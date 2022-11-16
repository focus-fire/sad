using Sad;

public class TestClass
{
    private int m_Counter = 1;
    private string m_TestString = "You just sent ";

    public void Increment(int i)
    {
        m_Counter += i;
        Log.Debug($"Increment called, counter is now at {m_Counter}");
    }

    public void AddNumbers(int i, int j) 
    {
        m_Counter = m_Counter + i + j; 
        Log.Debug($"AddNumbers called, counter is now at {m_Counter}");
    }

    public void AddStrings(string i)
    {
        m_TestString += i;
        Log.Debug($"AddStrings called, test string is now {m_TestString}");
    }

    public void TestMethod()
    {
        // IO Test
        // System.IO.File.WriteAllLines($"SadCSFrameworkTest{m_Counter}.txt", new string[] { "pog!", $"Integer -> {m_Counter}", $"{m_TestString}" });
    
        Log.Debug($"This is a test log from C# with a variable {m_Counter}");
        Log.Warn($"This is a warning message from C#");
        Log.Error($"This is an error message from C#");
    }
}