#include "sadpch.h"

// Import engines stuff
#include <Game/Time.h>
#include <Engine/Application.h>
#include <Engine/AudioManager.h>
#include <Engine/ResourceManager.h>
#include <Engine/InputManager.h>

#include "ScriptingBridge.h"

#include <mono/metadata/object.h>

/// Refer to SadCSFramework.Internal.cs for a reference on currently implemented internal methods
/// In order to add a new method to the scripting api, use this macro with the className (in the 'Sad.Internal' namespace) and the method name
/// Note: Parameters can be added to the 'mono_add_internal_call' signature in order to define an overload
/// ie: mono_add_internal_call("Sad.Internal.Log::Debug(string)") vs mono_add_internal_call("Sad.Internal.Log::Debug(Vector3)")
#define SAD_CSF_ADD_INTERNAL(className, method) mono_add_internal_call("Sad.Internal." className "::" #method, (const void*) method)

namespace sad::cs
{
	///////////
	/////////// Log
	///////////

	static void Info(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Info, "{}", cString);

		mono_free(cString);
	}

	static void Debug(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Debug, "{}", cString);

		mono_free(cString);
	}

	static void Warn(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Warn, "{}", cString);

		mono_free(cString);
	}

	static void Error(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Error, "{}", cString);

		mono_free(cString);
	}

	///////
	/////// Audio
	///////

	static void PlaySFX(MonoString* audioString)
	{
		char* cAudioString = mono_string_to_utf8(audioString);

		sad::AudioResource* audio = sad::ResourceManager::GetResource<sad::AudioResource>(cAudioString);

		sad::AudioManager::PlaySFX(audio);

		mono_free(cAudioString);
	}

	static void PlayMusic(MonoString* audioString, int times)
	{
		char* cAudioString = mono_string_to_utf8(audioString);

		sad::AudioResource* audio = sad::ResourceManager::GetResource<sad::AudioResource>(cAudioString);

		sad::AudioManager::PlayMusic(audio, times);

		mono_free(cAudioString);
	}

	static void SetVolume(MonoString* audioString, int volume)
	{
		char* cAudioString = mono_string_to_utf8(audioString);

		sad::AudioResource* audio = sad::ResourceManager::GetResource<sad::AudioResource>(cAudioString);

		sad::AudioManager::SetVolume(audio, volume);

		mono_free(cAudioString);
	}

	///////
	/////// Input
	///////

	static bool GetButton(sad::ControllerButton button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetButton(button);
	}

	static bool GetButtonPressed(sad::ControllerButton button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetButtonPressed(button);
	}

	static bool GetButtonReleased(sad::ControllerButton button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetButtonReleased(button);
	}

	static bool GetKey(sad::KeyCode key)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetKey(key);
	}

	static bool GetKeyPressed(sad::KeyCode key)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetKeyPressed(key);
	}

	static bool GetKeyReleased(sad::KeyCode key)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetKeyReleased(key);
	}

	static bool GetMouseButton(int button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMouseButton(button);
	}

	static bool GetMouseButtonPressed(int button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMouseButtonPressed(button);
	}

	static bool GetMouseButtonReleased(int button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMouseButtonReleased(button);
	}

	static SDL_Point GetMousePosition()
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMousePosition();
	}

	static bool IsControllerActive()
	{
		InputManager& input = InputManager::GetInstance();

		return input.IsControllerActive();
	}

	static bool IsControllerConnected()
	{
		InputManager& input = InputManager::GetInstance();

		return input.IsControllerConnected();
	}

	///////
	/////// Misc
	///////

	static float Getdt()
	{
		return pog::Time::GetDeltaTime();
	}
}

void sad::cs::ScriptingBridge::SetupEngineAPIFunctions()
{
	// Log
	SAD_CSF_ADD_INTERNAL("Log", Debug);
	SAD_CSF_ADD_INTERNAL("Log", Warn);
	SAD_CSF_ADD_INTERNAL("Log", Error);

	// Audio
	SAD_CSF_ADD_INTERNAL("Audio", PlaySFX);
	SAD_CSF_ADD_INTERNAL("Audio", PlayMusic);
	SAD_CSF_ADD_INTERNAL("Audio", SetVolume);

	// Inputs
	SAD_CSF_ADD_INTERNAL("Input", GetButton);
	SAD_CSF_ADD_INTERNAL("Input", GetButtonPressed);
	SAD_CSF_ADD_INTERNAL("Input", GetButtonReleased);

	SAD_CSF_ADD_INTERNAL("Input", GetKey);
	SAD_CSF_ADD_INTERNAL("Input", GetKeyPressed);
	SAD_CSF_ADD_INTERNAL("Input", GetKeyReleased);

	SAD_CSF_ADD_INTERNAL("Input", GetMouseButton);
	SAD_CSF_ADD_INTERNAL("Input", GetMouseButtonPressed);
	SAD_CSF_ADD_INTERNAL("Input", GetMouseButtonReleased);

	SAD_CSF_ADD_INTERNAL("Input", GetMousePosition);

	SAD_CSF_ADD_INTERNAL("Input", IsControllerActive);
	SAD_CSF_ADD_INTERNAL("Input", IsControllerConnected);

	SAD_CSF_ADD_INTERNAL("Misc", Getdt);

	// Entities

	// Math when transforms done

	// raycast when it's done

	// line renderer

	// instantiate -- scripting runtime -- getcurrentlevel -- destroyentitybyname
}
