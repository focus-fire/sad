#pragma once

namespace core
{

	void InitializeListener(const char* m_Name, void (* m_Event)(), int m_Amount, bool m_Persist);
	void RemoveListener(const char* m_Name);
	void SignalEvent(const char* m_Name);

	void UpdateEvents();

}