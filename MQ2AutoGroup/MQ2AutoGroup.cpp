#include "../MQ2Plugin.h" 

#include <sstream>

PreSetup("MQ2AutoGroup");

//Setup stuff 
bool AutoGroupOn = true;
bool AutoGroupGuild = true; 
int CurrentMaxNames = 0;
int TotalNames = 0;
int MAXLIST = 20;
char Name[100][64];
#define COLOR_YELLOW "\ay" 
#define COLOR_GREEN "\ag" 
#define COLOR_RED "\ar" 
#define COLOR_OFF "\ax" 

void WriteChatColor(string s)
{
	char *cstr = new char[s.length() + 1];
	std::strcpy(cstr, s.c_str());
	WriteChatColor(cstr);
	delete cstr;
}

void WriteChatColor(ostringstream &oss)
{
	WriteChatColor(oss.str());
}

//Show help 
VOID ShowHelp(VOID)
{
	string on = COLOR_GREEN "on" COLOR_OFF;
	string off = COLOR_RED "off" COLOR_OFF;
	WriteChatColor(COLOR_RED "+++ AutoGroup - Usage +++" COLOR_OFF);
	WriteChatColor("Autogroup is Currently: " + (AutoGroupOn ? on : off));
	WriteChatColor("/AutoGroup " COLOR_YELLOW "CharName" COLOR_OFF ": This will add a Character name to the AutoGroup.ini that you will auto accept invites from.");
	//    WriteChatColor("/AutoGroup < Guild > Guildies on/off"); 
	WriteChatColor("/AutoGroup " + on + ": Auto group on!");
	WriteChatColor("/AutoGroup " + off + ": Auto group off!");
	return;
}

//Write ini if there is none 
VOID WriteINI(VOID)
{
	WritePrivateProfileString("Settings", "AutoGroup", "on", INIFileName);
	WritePrivateProfileString("Settings", "AutoGroupGuild", "on", INIFileName);
	ShowHelp();
	//   WritePrivateProfileString("Names","Name1","Nada",INIFileName); 
}

//Load ini when player enters game 
VOID LoadINI(VOID)
{
	char szTemp[MAX_STRING];
	TotalNames = 0;
	if (GetPrivateProfileString("Settings", "AutoGroup", "", szTemp, MAX_STRING, INIFileName))
	{
		if (!strcmp(szTemp, "on"))
			AutoGroupOn = true;
		else
			AutoGroupOn = false;
	}
	else
		WriteINI();
	if(GetPrivateProfileString("Settings","AutoGroupGuild","",szTemp,MAX_STRING,INIFileName)) 
	{ 
	   if (!strcmp(szTemp,"on")) 
	      AutoGroupGuild = true;
	   else 
	      AutoGroupGuild = false; 
	} 
	for (int i = 0; i<(MAXLIST + 1); i++)
	{
		sprintf(szTemp, "Name%i", i);
		if (!GetPrivateProfileString("Names", szTemp, "", Name[i], 64, INIFileName))
			return;
		else
			TotalNames++;
	}
}

VOID DoAutoGroupOn(PSPAWNINFO pChar, PCHAR szLine)
{
	char szTemp[MAX_STRING];
	if (gGameState == GAMESTATE_INGAME)
	{
		if (szLine[0] == 0)
		{
			//         WriteChatf("AutoGroup is now: %s", AutoGroupOn?"\agOn":"\arOff"); 
			//         WriteChatf("AutoGroupGuild is now: %s", AutoGroupGuild?"\agOn":"\arOff"); 
			ShowHelp();
			return;
		}
		if (!strcmpi(szLine, "on"))
			AutoGroupOn = true;
		else if (!strcmpi(szLine, "off"))
			AutoGroupOn = false;
		//      else if (!strcmpi(szLine,"guild")) 
		//         AutoGroupGuild=!AutoGroupGuild; 
		else
		{
			WriteChatf("Adding \ay< %s >\ax to list of people you'll auto-join", szLine);
			sprintf(szTemp, "Name%i", TotalNames);
			WritePrivateProfileString("Names", szTemp, szLine, INIFileName);
			TotalNames++;
			LoadINI();
			return;
		}
		WriteChatf("AutoGroup is now: %s", AutoGroupOn ? "\agOn" : "\arOff");
		//      WriteChatf("AutoGroupGuild is now: %s", AutoGroupGuild?"\agOn":"\arOff"); 
		LoadINI();
	}
}

BOOL CheckNames(char *inviterName)
{
	if (AutoGroupGuild) {
		int myGuild = GetCharInfo()->GuildID;
		EQClasses::EQPlayer *inviter = pSpawnManager->GetSpawnByName(inviterName);
		if (inviter != NULL) {
			int inviterGuild = inviter->Data.GuildID;
			if (myGuild == inviterGuild) {
				return true;
			}
		}
	}

	for (int i = 0; i<(TotalNames + 1); i++)
	{
		if (!strcmpi(inviterName, Name[i]))
			return true;
	}
	return false;
}

PLUGIN_API DWORD OnIncomingChat(PCHAR Line, DWORD Color)
{
	CHAR szName[MAX_STRING];
	CHAR szMsg[MAX_STRING];
	if (strstr(Line, "invites you to join a group.")) {
		GetArg(szName, Line, 1);
		if (CheckNames(szName)) {
			DoCommand(GetCharInfo()->pSpawn, "/timed 3s /keypress invite_follow");
			sprintf(szMsg, "Accepted group invite from \ay< %s >\ax", szName);
			WriteChatColor(szMsg);
		}
	}
	if (strstr(Line, "invites you to join a raid")) {
		GetArg(szName, Line, 1);
		if (CheckNames(szName)) {
			DoCommand(GetCharInfo()->pSpawn, "/timed 3s /raidaccept");
			sprintf(szMsg, "Accepted raid invite from \ay< %s >\ax", szName);
			WriteChatColor(szMsg);
		}
	}
	return 0;
}


// Called once, when the plugin is to initialize 
PLUGIN_API VOID InitializePlugin(VOID)
{
	DebugSpewAlways("Initializing MQ2AutoGroup");
	LoadINI();
	AddCommand("/autogroup", DoAutoGroupOn);
}

// Called once, when the plugin is to shutdown 
PLUGIN_API VOID ShutdownPlugin(VOID)
{
	DebugSpewAlways("Shutting down MQ2AutoGroup");
	RemoveCommand("/autogroup");
}