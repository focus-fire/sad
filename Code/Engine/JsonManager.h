#pragma once

namespace sad
{
	class JsonManager
	{
	public:
		bool static ImportLevel(int saveFile = 1);
		bool static ExportLevel(int saveFile = 1);
	};
}