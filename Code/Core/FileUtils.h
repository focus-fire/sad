#pragma once

#include <string>

namespace core
{
	/**
	 * @brief Utilities and helpers for dealing with files and file streams
	*/
	class FileUtils
	{
	public:
		/**
		 * @brief Reads the contents of a file into a string
		 * @param path Path for the file to be read
		 * @param addLineBreaks Indicates whether a line break should be appended to the end of each line
		 * @return String contents of the file
		*/
		static std::string ReadFile(const std::string& path, bool addLineBreaks = false);

		/**
		 * @brief Writes to a file provided a particular path and string contents 
		 * @param path Path for the file to be written to
		 * @param content String contents for the file being written
		 * @return True if the creation succeeded, false if creating the file failed
		*/
		static bool WriteFile(const std::string& path, const std::string& content);

		/**
		 * @brief Appends contents to the end of a file at a given path
		 * @param path Path for the file being added to
		 * @param content String contents being appended to the end of the file
		 * @return True if appending to the file succeeded, false if appending to the file failed
		*/
		static bool AppendFile(const std::string& path, const std::string& content);

		/**
		 * @brief Removes a file at the provided path 
		 * @param path Path for the file being removed
		 * @return True if removing the file succeeded, false if removing the file failed
		*/
		static bool RemoveFile(const std::string& path);

		/**
		 * @brief Reads the bytes of a file (or specifically a .dll) into a char array 
		 * @param filePath Path indicating the file to read
		 * @param outputSize Pointer to an unsigned int that will contain the size of the buffer allocated
		 * @return Allocated buffer containing the file's byte data
		 * @note Warning: This performs allocation, remember to `delete` returned data
		*/
		static char* ReadBytes(const std::string& filePath, uint32_t* outputSize);
		
		/**
		 * @brief Evaluates if a pass exists on the filesystem
		 * @param path Path to evaluate the existence of
		 * @return True if the path exists, false if it does not
		*/
		static bool PathExists(const std::string& path);

		/**
		 * @brief Retrieves a platform-specific absolute path for the root folder of the project ('sad')
		*/
		static std::string GetProjectDirectory();

		/**
		 * @brief Retrieves a platform-specific absolute path to the data directory of the engine ('sad/Data') 
		*/
		static std::string GetDataDirectory();

		/**
		 * @brief Retrieves a platform-specific absolute path to the code directory of the engine ('sad/Code') 
		*/
		static std::string GetCodeDirectory();

		/**
		 * @brief Converts a Unix path string to be OS-specific  
		 * @param path Unix path string to ensure remains OS-specific 
		 * @return String containing the OS-specific path
		 * @example ConvertOSPathString("sad/Data/Test") converts to "sad\Data\Test" on Windows and remains "sad/Data/Test" on Mac/Unix platforms
		*/
		static std::string ConvertOSPathString(const std::string& path);

		/**
		 * @brief Retrieves a platform-specific absolute path inside the data directory of the engine ('sad/Data/PATH')
		 * @param path Path for the file and/or directory path being retrieved
		*/
		static std::string GetPathInsideDataDirectory(const std::string& path);

		/**
		 * @brief Retrieves a platform-specific absolute path inside the code directory of the engine ('sad/Code/PATH')
		 * @param path Path for the file and/or directory path being retrieved 
		*/
		static std::string GetPathInsideCodeDirectory(const std::string& path);
	};
}
