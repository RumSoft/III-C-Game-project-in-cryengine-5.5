#include <StdAfx.h>

#include "CVars.h"
#include <CrySystem/ISystem.h>

CCVars g_cvars;

void CCVars::RegisterVariables()
{
	//CVARS VARIABLES TEMPLATE:
	//m_position = REGISTER_STRING("myposition", "0, 0, 0", VF_CHEAT, "some starting position");
	//REGISTER_CVAR2("starthealth", &m_starthealth, 100, VF_CHEAT, "starting player health");

	REGISTER_CVAR2("debug_ai", &m_debugai, false, VF_CHEAT, "displays some information about AIs");
	REGISTER_CVAR2("debug_actor", &m_debugactor, false, VF_CHEAT, "display some information for ACTOR components");
	
}

void CCVars::UnregisterVariables()
{
	//remove commands if any registered
}