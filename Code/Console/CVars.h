#pragma once

class CCVars final
{
public:
	CCVars() = default;
	CCVars(CCVars const&) = delete;
	CCVars(CCVars&&) = delete;
	CCVars& operator=(CCVars const&) = delete;
	CCVars& operator=(CCVars&&) = delete;

	void RegisterVariables();
	void UnregisterVariables();

	int m_debugai;
	int m_debugactor;
};

extern CCVars g_cvars;