<p align="center">
    <img src="../.github/assets/blob-love.png" alt="style" height="100">
</p>
<h3 align="center">Style!</h3>
<p align="center">
    💕 <i>Cause putting a bit of love into the code we write helps!</i> 💕
</p>

---

This could be much, much, much longer - but as discussed, we'll limit this to some hard/fast rules that are good practices but will make the code look a little clean/consistent.

### TLDR
If you need to **push and get a review** fast, follow these six steps and honestly - we won't care about anything else.

1. Place all curly braces on a new line.
2. Classes and method names should start with a capital.
3. Use `nullptr` instead of `NULL`.
4. Avoid using macros or globals where possible.
5. Member (private) variables should **always** be prefixed with `m_` to indicate ownership.
6. Avoid unnecessary **dead code**. Long blocks of commented, unused, or old code is not pog.

### Commenting
Comment where you deem necessary! No need to comment absolutely everything, but if an operation/method is long or ambiguous feel free to comment for clarity.

In lieu of Doxygen support, set the *Generated Doxygen Comments Style* option in Visual Studio to `Doxygen (/**)`.

Similar to line comments, use the Doxygen comments where you deem necessary (ie: A basic 'getter' for a private variable may not need an entire `@brief`).

### Files and Extensions
Each source file should have an associated header file. Prefer `.h` and `.cpp` for file extensions.

If the source or header is an **ECS** component or system, files should be suffixed with `Component` and `System` respectively.

```
/* Transform.h */
struct Transform				// Not Pog
{ }

/* TransformComponent.h */
struct TransformComponent		// Pog
{ }
```

Prefer using `#pragma once` as a header guard over `#ifndef GUARD`.

### Dead Code
**Early note:** If code is dead, let it stay dead.

This is obviously 110% discretionary - sometimes leaving a few lines commented works. Obviously if it's something that is getting reverted or is required by the next feature in line, leave it.

However, entire implementations or previous ways of doing things should be removed and not commented (we have git for a reason).

### Bracket Style
Avoid Egyptian braces wherever possible! Placing brackets on a new line in almost all cases adds some space and makes C/C++ code happier to read.

```
int Foo()
{
    // Pog
}

int Foo() {
    // Not pog
}
```

If statements can be condensed to one-line if the coinciding logic isn't also condensed.

```
if (foo)
    return; // Pog

if (foo)
    int i = 2; i += 2; return i; // Not Pog
```

### Order of Includes
Prioritize the source header and any precompiled headers when ordering includes. Then use the following:

- C/C++ System Files
- Library Headers
- Other Project Headers
- Same Project Headers

```
#include "PrecompiledHeader.h" // Precompiled headers

#include "MainClass.h" // Header file associated with source file MainClass.cpp

#include <iostream> // C/C++ System Files

#include <lib/LibHeader.h> // Library Headers

#include <proj/ProjHeader.h> // Headers from other projects (ie: Game, Engine, Core, etc.)

#include "OtherClass.h" // Headers associated with other source files in the same project
```

### Classes/Functions Name
Start them with a capital, prefer CapitalCamelCase for more words. Keep access modifiers inline with the class definition. Make them descriptive.

CapitalCamelCase can be used for public variables. Use the following prefixes for class variables.

- `m_` - Member Variables
- `s_` - Static Variables

```
class LogManager            // Pog
{
public:                     // Pog
    void Debug();           // Pog

    int Count;              // Pog
    static int s_Count;     // Pog

private:
    int m_Count;            // Pog
};

class manager               // Not Pog, what does a 'manager' do?
{
    public:                 // Ultra Not Pog
        void debug();       // Mega Ultra Not Pog
};
```

### Namespaces
Prefer lowerCamelCase for namespace names. Try to keep namespace names as short and descriptive as possible.

```
namespace sad
{
    // Pog
}

namespace sadEngineEditor
{
    // Not Pog
}
```

Also prefer using complete namespaces in source (.cpp) and header files.

```
namespace sad
{
	void SomeMethod()    // Not Pog
	{ }
}

void sad::SomeMethod()   // Pog
{ }
```

```
namespace sad::rad
{
	// Pog
}

namespace sad
{
	namespace rad
	{
		// Not Pog
	}
}
```

### nullptr
Please just use `nullptr` over `NULL`. Thanks.

### Globals and Macros
If you can avoid using them, please do.

If you have to, follow `ALL_CAPS` for macros and `g_GlobalVariableName` for globals.

### Reference Arguments
If you pass references as an argument, label tham as `const`.

```
void Foo(const std::string& bar);
```

### const
Use it where it makes sense! If you aren't going to modify something or don't want anyone to change it, just make it const.

If its safer to just make something const, feel free to do it.

### auto
Let's avoid using it unless we're repeating long and obvious return types.

```
// I'm cool if you don't want to repeat that namespace... (5/6 deep)
auto rendererMeshType = sad::Engine::RenderingFramework::Mesh::Type();

// Let's not be cool with this...
auto num = 2;
```

Prefer not using it though.

### Casting
Prefer C++ casts.

```
float x = 1.2;
int y = static_cast<int>(x); // Pog
int z = (int) x; // Not Pog

Check this out for a deeper dive.
http://cppblogs.blogspot.com/2013/08/c-style-casts.html
```

### Using Statements
Avoid using statements if possible.

```
// Let's avoid this where possible...
using namespace sad::Engine;

// Abbreviating namespaces is more pog and makes sense if you're using this long namespace a lot
namespace SEMC = sad::Engine::Mesh::Container;

// Ultra not pog, if you don't absolutely need to do this please don't
using namespace std;
```
