#pragma once
	
namespace sad 
{
	enum class EngineMode
	{
		Editor, // Editor inputs allowed (moving a floating camera around, moving objects, etc.
		Game    // Game should start and editor actions should be killed
	};
}