#define KAILLERA_DLL 
#include "kailleraclient.h"
#define _WIN32_WINNT 0x0500 
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <commctrl.h>
#include <tchar.h>// For TCHAR and string functions.
#include <richedit.h>
#include <time.h>
#include "kailleramessages.h"

using namespace std;
kailleraInfos kInfo;
HWND mainHwnd;
#define SC_SUPRARECVK 27888
#define SC_SUPRARECV3D 27889
#define SC_SUPRARECVWAITING 27890
#define SC_SUPRARECPING 27891

extern "C" { 

	DLLEXP kailleraGetVersion(char *version){ 
		return (int)strcpy(version, myVersion); 
	} 

    DLLEXP kailleraInit(){ 
		
		ifstream config;
		char temp[2048];
		char temp1[1024];
		int i;
		char strI[1024];
		POINT p;

		//Set Initially.  If values are in .ini, value will change appropriately.
		joinChatValue = 1;
		createValue = 1;
		joinChatGameValue = 1;
		beepValue = 1;
		useScreenChatValue = 1;
		gameChatLogValue = 1;
		chatLogValue = 1;
		blinkValue = 1;
		useCacheValue = 1;
		errorValue = 1;
		lastTabServer = 0;
		wsprintf(maxUsersG, "%i", 16);
		wsprintf(maxPingG, "%i", 200);
		showChatroomFirst = true;
		recentCount = -1;
		favoriteCount = -1;
		dropValue = 1;
		awayMessageCount = 0;
		joinDblValue = 1;
		emulinkerSFValue = 0;
		fakeP2PValue = 0;
		//wsprintf(p2pPort, "%i", 7159);
		//strcpy(p2pServer, "127.0.0.1");

		//Open File
		config.open("supraclient.ini", ios::in);

		if (config.good()) {
			//Get Contents
			while (!config.eof()) {
				config.getline(temp, 128, '\n');

				//Server Tab
				if (strncmp(temp, "ServerTab=0", 11) == 0)
					lastTabServer = 0;
				else if (strncmp(temp, "ServerTab=1", 11) == 0)
					lastTabServer = 1;
				else if (strncmp(temp, "ServerTab=2", 11) == 0)
					lastTabServer = 2;
				else if (strncmp(temp, "ServerTab=3", 11) == 0)
					lastTabServer = 3;
				else if (strncmp(temp, "ServerTab=4", 11) == 0)
					lastTabServer = 4;
				//Show Chatroom First?
				else if (strncmp(temp, "ShowChatroomFirst=1", 19) == 0)
					showChatroomFirst = true;
				else if (strncmp(temp, "ShowChatroomFirst=0", 19) == 0)
					showChatroomFirst = false;
				//chkJoinChat
				else if (strncmp(temp, "Join/Left=0", 11) == 0)
					joinChatValue = 0;
				else if (strncmp(temp, "Join/Left=1", 11) == 0)
					joinChatValue = 1;
				//chkJoinChatGame
				else if (strncmp(temp, "Join/Left_Game=0", 16) == 0)
					joinChatGameValue = 0;
				else if (strncmp(temp, "Join/Left_Game=1", 16) == 0)
					joinChatGameValue = 1;
				//chkBeep
				else if (strncmp(temp, "Beep=0", 6) == 0)
					beepValue = 0;
				else if (strncmp(temp, "Beep=1", 6) == 0)
					beepValue = 1;
				//chkDrop
				else if (strncmp(temp, "Drop=0", 6) == 0)
					dropValue = 0;
				else if (strncmp(temp, "Drop=1", 6) == 0)
					dropValue = 1;
				//chkDrop
				else if (strncmp(temp, "Join_Double_Click=0", 19) == 0)
					joinDblValue = 0;
				else if (strncmp(temp, "Join_Double_Click=1", 19) == 0)
					joinDblValue = 1;
				//chkShowError
				else if (strncmp(temp, "Join_Error=0", 12) == 0)
					errorValue = 0;
				else if (strncmp(temp, "Join_Error=1", 12) == 0)
					errorValue = 1;

				//EmulinkerSF MaxUsers
				else if (strncmp(temp, "EmuSF_MaxUsers=", 15) == 0)
					strcpy(maxUsersG, &temp[15]);
				//EmulinkerSF MaxPing
				else if (strncmp(temp, "EmuSF_MaxPing=", 14) == 0)
					strcpy(maxPingG, &temp[14]);
				//EmulinkerSF FakeP2P
				else if (strncmp(temp, "EmuSF_FakeP2P=1", 15) == 0)
					fakeP2PValue = 1;
				else if (strncmp(temp, "EmuSF_FakeP2P=0", 15) == 0)
					fakeP2PValue = 0;
				//EmulinkerSF REmulator
				else if (strncmp(temp, "EmuSF_Emulator=1", 16) == 0)
					emuResValue = 1;
				else if (strncmp(temp, "EmuSF_Emulator=0", 16) == 0)
					emuResValue = 0;
				//EmulinkerSF RConnection Type 
				else if (strncmp(temp, "EmuSF_Connection=1", 18) == 0)
					connResValue = 1;
				else if (strncmp(temp, "EmuSF_Connection=0", 18) == 0)
					connResValue = 0;

				//EmulinkerSF REmulator
				else if (strncmp(temp, "Userlist_Column=0", 17) == 0)
					lstUserlistColumn = 0;
				else if (strncmp(temp, "Userlist_Column=1", 17) == 0)
					lstUserlistColumn = 1;
				else if (strncmp(temp, "Userlist_Column=2", 17) == 0)
					lstUserlistColumn = 2;
				else if (strncmp(temp, "Userlist_Column=3", 17) == 0)
					lstUserlistColumn = 3;
				else if (strncmp(temp, "Userlist_Column=4", 17) == 0)
					lstUserlistColumn = 4;
				else if (strncmp(temp, "Userlist_Column_Order=0", 23) == 0)
					userlistSwitch = false;
				else if (strncmp(temp, "Userlist_Column_Order=1", 23) == 0)
					userlistSwitch = true;

				//P2P
				//else if(strncmp(temp, "P2P_Server=", 11) == 0)
					//strcpy(p2pServer, &temp[11]);
				//else if(strncmp(temp, "P2P_Port=", 9) == 0)
					//strcpy(p2pPort, &temp[9]);

				//chkUseCache
				else if (strncmp(temp, "Cache=0", 7) == 0)
					useCacheValue = 0;
				else if (strncmp(temp, "Cache=1", 7) == 0)
					useCacheValue = 1;
				//chkEmulinkerSF
				else if (strncmp(temp, "Use_EmuSF=0", 11) == 0)
					emulinkerSFValue = 0;
				else if (strncmp(temp, "Use_EmuSF=1", 11) == 0)
					emulinkerSFValue = 1;
				//chkKeepGameChatLogs
				else if (strncmp(temp, "Keep_Gameroom_Logs=0", 20) == 0)
					gameChatLogValue = 0;
				else if (strncmp(temp, "Keep_Gameroom_Logs=1", 20) == 0)
					gameChatLogValue = 1;
				//chkKeepChatLogs
				else if (strncmp(temp, "Keep_Chatroom_Logs=0", 20) == 0)
					chatLogValue = 0;
				else if (strncmp(temp, "Keep_Chatroom_Logs=1", 20) == 0)
					chatLogValue = 1;
				//chkBlink
				else if (strncmp(temp, "Blink=0", 7) == 0)
					blinkValue = 0;
				else if (strncmp(temp, "Blink=1", 7) == 0)
					blinkValue = 1;
				//chkUseScreenChat
				else if (strncmp(temp, "Use_Screen_Chat=0", 17) == 0)
					useScreenChatValue = 0;
				else if (strncmp(temp, "Use_Screen_Chat=1", 17) == 0)
					useScreenChatValue = 1;
				//chkCreate
				else if (strncmp(temp, "Create/Close=0", 14) == 0)
					createValue = 0;
				else if (strncmp(temp, "Create/Close=1", 14) == 0)
					createValue = 1;
				//ServerIP
				else if (strncmp(temp, "ServerIP=", 9) == 0)
					strcpy(serverIP, &temp[9]); //ServerIP=
				//Nick
				else if (strncmp(temp, "Nick=", 5) == 0) {
					strcpy(username, &temp[5]); //Nick=
					username[31] = '\0';
				}
				//Connection
				else if (strncmp(temp, "Connection=", 11) == 0) { //Connection=
					if (strncmp(temp, "Connection=1", 12) == 0)
						connectionType = lan;
					else if (strncmp(temp, "Connection=2", 12) == 0)
						connectionType = excellent;
					else if (strncmp(temp, "Connection=3", 12) == 0)
						connectionType = good;
					else if (strncmp(temp, "Connection=4", 12) == 0)
						connectionType = average;
					else if (strncmp(temp, "Connection=5", 12) == 0)
						connectionType = low;
					else if (strncmp(temp, "Connection=6", 12) == 0)
						connectionType = bad;
					else
						connectionType = good;
				}
				//Quit
				else if (strncmp(temp, "Quit=", 5) == 0) {
					strcpy(quit, &temp[5]); //Quit=
				}


				//Away Messages
				else if (strncmp(temp, "Away_Subject", 12) == 0) {
					for (i = 1; i < AWAY_MESSAGE_MAX; i++) {
						wsprintf(strI, "%i", i);
						if (strncmp(&temp[12], strI, 1) == 0) {
							awayMessageCount = awayMessageCount + 1;
							strcpy(awayMessages[i].subject, &temp[14]);
							awayMessages[i].save = true;
							break;
						}
					}
				}
				else if (strncmp(temp, "Away_Message", 12) == 0) {
					for (i = 1; i < AWAY_MESSAGE_MAX; i++) {
						wsprintf(strI, "%i", i);
						if (strncmp(&temp[12], strI, 1) == 0) {
							strcpy(awayMessages[i].message, &temp[14]);
							break;
						}
					}
				}
				//Recent Servers
				else if (strncmp(temp, "Recent_Name", 11) == 0) {
					for (i = 0; i < RECENT_MAX; i++) {
						wsprintf(strI, "%02i", i);
						if (strncmp(&temp[11], strI, 2) == 0) {
							strcpy(recentServers[i].server, &temp[14]);
							break;
						}
					}
				}
				else if (strncmp(temp, "Recent_IP", 9) == 0) {
					for (i = 0; i < RECENT_MAX; i++) {
						wsprintf(strI, "%02i", i);
						if (strncmp(&temp[9], strI, 2) == 0) {
							strcpy(recentServers[i].ip, &temp[12]);
							recentCount = recentCount + 1;
							break;
						}
					}
				}
				else if (strncmp(temp, "Recent_Location", 15) == 0) {
					for (i = 0; i < RECENT_MAX; i++) {
						wsprintf(strI, "%02i", i);
						if (strncmp(&temp[15], strI, 2) == 0) {
							strcpy(recentServers[i].location, &temp[18]);
							break;
						}
					}
				}


				//Favorite Servers
				else if (strncmp(temp, "Favorite_Name", 13) == 0) {
					for (i = 0; i < FAVORITE_MAX; i++) {
						wsprintf(strI, "%02i", i);
						if (strncmp(&temp[13], strI, 2) == 0) {
							strcpy(favoriteServers[i].server, &temp[16]);
							break;
						}
					}
				}
				else if (strncmp(temp, "Favorite_IP", 11) == 0) {
					for (i = 0; i < FAVORITE_MAX; i++) {
						wsprintf(strI, "%02i", i);
						if (strncmp(&temp[11], strI, 2) == 0) {
							strcpy(favoriteServers[i].ip, &temp[14]);
							favoriteCount = favoriteCount + 1;
							break;
						}
					}

				}
				else if (strncmp(temp, "Favorite_Location", 17) == 0) {
					for (i = 0; i < FAVORITE_MAX; i++) {
						wsprintf(strI, "%02i", i);
						if (strncmp(&temp[17], strI, 2) == 0) {
							strcpy(favoriteServers[i].location, &temp[20]);
							break;
						}
					}
				}
				else if (strncmp(temp, "Favorite_Comments", 17) == 0) {
					for (i = 0; i < FAVORITE_MAX; i++) {
						wsprintf(strI, "%02i", i);
						if (strncmp(&temp[17], strI, 2) == 0) {
							strcpy(favoriteServers[i].comments, &temp[20]);
							favoriteCount = favoriteCount + 1;
							break;
						}
					}
				}


				//X position
				else if (strncmp(temp, "xPos=", 5) == 0) {
					strcpy(temp1, &temp[5]);
					p.x = cStrToInt(temp1);
					xPos = p.x;
				}
				//Y position
				else if (strncmp(temp, "yPos=", 5) == 0) {
					strcpy(temp1, &temp[5]);
					p.y = cStrToInt(temp1);
					yPos = p.y;
				}
				//LastGameToPlay1
				else if (strncmp(temp, "LastGameToPlay1=", 16) == 0)
					strcpy(lastGameToPlay1, &temp[16]);
				//LastGameToPlay2
				else if (strncmp(temp, "LastGameToPlay2=", 16) == 0)
					strcpy(lastGameToPlay2, &temp[16]);
				//LastGameToPlay3
				else if (strncmp(temp, "LastGameToPlay3=", 16) == 0)
					strcpy(lastGameToPlay3, &temp[16]);
			}

			//If Initial Display is not Visible, Set it back to the Origin.
			if (MonitorFromPoint(p, MONITOR_DEFAULTTONULL) == NULL) {
				xPos = 0;
				yPos = 0;
			}
		}

		config.close();


		ZeroMemory(myBuff, MESSAGE_LENGTH * MESSAGE_SIZE);
		//Server Lists
		strcpy(anti3DServerList.host, "kaillerareborn.2manygames.fr");
		strcpy(anti3DServerList.link, "server_list.php");
		strcpy(anti3DServerList.wglink, "game_list.php");
		anti3DServerList.port = 80;

		strcpy(kailleraServerList.host, "www.kaillera.com");
		strcpy(kailleraServerList.link, "raw_server_list2.php?wg=1&version=0.9");
		kailleraServerList.port = 80;
		
		return 0; 
    } 

    DLLEXP kailleraShutdown(){
		DestroyWindow(form1);
		DestroyWindow(mainHwnd);
		return 0; 
    } 

    DLLEXP kailleraSetInfos(kailleraInfos *infosp){
		long i;
		short strSize;
		int w = 0;
		char temp[2048];
		
		kInfo = *infosp;

		//Get Emulator Name
		strcpy(emulator, kInfo.appName);
		
		//Get Game List
		for(i = 0; i < 65536; i++){
			strSize = strlen((kInfo.gameList) + w);
			if(strSize == 0)
				break;
			strcpy(gameList[i].game, &kInfo.gameList[w]);
			w = w + strSize + 1;
			totalGames = totalGames + 1;
			if(i % 1000 == 0)
				DoEvents();
		}


		//Sort Games by First Character
		for(i = 0; i < totalGames; i++){
			for(w = 0; w < totalGames; w++){
				if(gameList[w].game[0] > gameList[i].game[0]){
					strcpy(temp, gameList[i].game);
					strcpy(gameList[i].game, gameList[w].game);
					strcpy(gameList[w].game, temp);
				}
				if(w % 100 == 0)
					DoEvents();
			}
		}
	
		//Create Menu
		gameMenu = CreatePopupMenu();
		AppendMenu(gameMenu, MF_STRING | MF_POPUP, (UINT)0xFFFD, lastGameToPlay1);
		AppendMenu(gameMenu, MF_STRING | MF_POPUP, (UINT)0xFFFE, lastGameToPlay2);
		AppendMenu(gameMenu, MF_STRING | MF_POPUP, (UINT)0xFFFF, lastGameToPlay3);
		AppendMenu(gameMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		//Add Catagories
		for(i = 0; i < totalGames; i++){
			//Add Catagory
			if(gameList[i].game[0] > gameList[i - 1].game[0]){
				strncpy(temp, gameList[i].game, 1);
				temp[1] = '\0';
				gameSubMenu = CreatePopupMenu();
				AppendMenu(gameMenu, MF_STRING | MF_POPUP, (UINT)gameSubMenu, temp);
			}
			//Add Game Under Catagory
			AppendMenu(gameSubMenu, MF_STRING, i + 1, gameList[i].game);
			if(i % 100 == 0)
				DoEvents();
		}

		//Favorite List Menu
		favoritelistMenu = CreatePopupMenu();
		favoritelistSubMenu = CreatePopupMenu();
		AppendMenu(favoritelistMenu, MF_STRING | MF_POPUP, 0xFFD8, "Connect");
		AppendMenu(favoritelistMenu, MF_STRING | MF_POPUP, 0xFFD5, "Edit Entry");
		AppendMenu(favoritelistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(favoritelistMenu, MF_STRING | MF_POPUP, 0xFFD9, "Remove");
		AppendMenu(favoritelistMenu, MF_STRING | MF_POPUP, 0xFFDA, "Remove All");
		AppendMenu(favoritelistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(favoritelistMenu, MF_STRING | MF_POPUP, (UINT)favoritelistSubMenu, "Copy");
		AppendMenu(favoritelistSubMenu, MF_STRING, 0xFFDB, "Server Name");
		AppendMenu(favoritelistSubMenu, MF_STRING, 0xFFDC, "IP Address");		
		AppendMenu(favoritelistSubMenu, MF_STRING, 0xFFDD, "Location");
		AppendMenu(favoritelistSubMenu, MF_STRING, 0xFFDE, "Comments");

		//Recent List Menu
		recentlistMenu = CreatePopupMenu();
		recentlistSubMenu = CreatePopupMenu();
		AppendMenu(recentlistMenu, MF_STRING | MF_POPUP, 0xFFE1, "Add to Favorites");
		AppendMenu(recentlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(recentlistMenu, MF_STRING | MF_POPUP, 0xFFE2, "Connect");
		AppendMenu(recentlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(recentlistMenu, MF_STRING | MF_POPUP, 0xFFE3, "Remove");
		AppendMenu(recentlistMenu, MF_STRING | MF_POPUP, 0xFFE0, "Remove All");
		AppendMenu(recentlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(recentlistMenu, MF_STRING | MF_POPUP, (UINT)recentlistSubMenu, "Copy");
		AppendMenu(recentlistSubMenu, MF_STRING, 0xFFE4, "Server Name");
		AppendMenu(recentlistSubMenu, MF_STRING, 0xFFE5, "IP Address");		
		AppendMenu(recentlistSubMenu, MF_STRING, 0xFFE6, "Location");

		//Waiting List Menu
		waitinglistMenu = CreatePopupMenu();
		waitinglistSubMenu = CreatePopupMenu();
		AppendMenu(waitinglistMenu, MF_STRING | MF_POPUP, 0xFFDF, "Add to Favorites");
		AppendMenu(waitinglistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(waitinglistMenu, MF_STRING | MF_POPUP, 0xFFE7, "Refresh Waiting Games");
		AppendMenu(waitinglistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(waitinglistMenu, MF_STRING | MF_POPUP, 0xFFE8, "Connect");
		AppendMenu(waitinglistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(waitinglistMenu, MF_STRING | MF_POPUP, (UINT)waitinglistSubMenu, "Copy");
		AppendMenu(waitinglistSubMenu, MF_STRING, 0xFFE9, "Game Name");
		AppendMenu(waitinglistSubMenu, MF_STRING, 0xFFEA, "Emulator");		
		AppendMenu(waitinglistSubMenu, MF_STRING, 0xFFEB, "Username");
		AppendMenu(waitinglistSubMenu, MF_STRING, 0xFFEC, "Server Name");
		AppendMenu(waitinglistSubMenu, MF_STRING, 0xFFED, "IP Address");
		AppendMenu(waitinglistSubMenu, MF_STRING, 0xFFEE, "Location");

		//Server List Menu
		serverlistMenu = CreatePopupMenu();
		serverlistSubMenu = CreatePopupMenu();
		AppendMenu(serverlistMenu, MF_STRING | MF_POPUP, 0xFFE6, "Add to Favorites");
		AppendMenu(serverlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(serverlistMenu, MF_STRING | MF_POPUP, 0xFFEF, "Refresh Server List");
		AppendMenu(serverlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(serverlistMenu, MF_STRING | MF_POPUP, 0xFFD7, "Ping");
		AppendMenu(serverlistMenu, MF_STRING | MF_POPUP, 0xFFD6, "Stop");
		AppendMenu(serverlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(serverlistMenu, MF_STRING | MF_POPUP, 0xFFF0, "Connect");
		AppendMenu(serverlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(serverlistMenu, MF_STRING | MF_POPUP, (UINT)serverlistSubMenu, "Copy");
		AppendMenu(serverlistSubMenu, MF_STRING, 0xFFF1, "Server Name");
		AppendMenu(serverlistSubMenu, MF_STRING, 0xFFF2, "IP Address");		
		AppendMenu(serverlistSubMenu, MF_STRING, 0xFFF3, "Location");

		//Gamelist Menu
		gamelistMenu = CreatePopupMenu();
		gamelistSubMenu = CreatePopupMenu();
		AppendMenu(gamelistMenu, MF_STRING | MF_POPUP, 0xFFF7, "Join");
		AppendMenu(gamelistMenu, MF_STRING | MF_POPUP, 0xDDD9, "Close Game");
		if(kInfo.moreInfosCallback != NULL){
			AppendMenu(gamelistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
			AppendMenu(gamelistMenu, MF_STRING | MF_POPUP, 0xFFF8, "More Information");
		}
		AppendMenu(gamelistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(gamelistMenu, MF_STRING | MF_POPUP, (UINT)gamelistSubMenu, "Copy");
		AppendMenu(gamelistSubMenu, MF_STRING, 0xFFF9, "Game ID");
		AppendMenu(gamelistSubMenu, MF_STRING, 0xFFFA, "Game");		
		AppendMenu(gamelistSubMenu, MF_STRING, 0xFFFB, "Emulator");
		AppendMenu(gamelistSubMenu, MF_STRING, 0xFFFC, "Owner");

		//Game Userlist Menu
		gameUserlistMenu = CreatePopupMenu();
		gameUserlistSubMenu = CreatePopupMenu();
		AppendMenu(gameUserlistMenu, MF_STRING | MF_POPUP, 0xFFF4, "Kick");

		AppendMenu(gameUserlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);

		AppendMenu(gameUserlistMenu, MF_STRING | MF_POPUP, (UINT)gameUserlistSubMenu, "Mute Player");
		AppendMenu(gameUserlistSubMenu, MF_STRING, 0xDDDD, "Mute");
		AppendMenu(gameUserlistSubMenu, MF_STRING, 0xDDDC, "Unmute");

		AppendMenu(gameUserlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);

		gameUserlistSubMenu = CreatePopupMenu();
		AppendMenu(gameUserlistMenu, MF_STRING | MF_POPUP, (UINT)gameUserlistSubMenu, "Copy");
		AppendMenu(gameUserlistSubMenu, MF_STRING, 0xFFF5, "User ID");
		AppendMenu(gameUserlistSubMenu, MF_STRING, 0xFFF6, "Username");
		

		//txtChatroom Menu
		txtChatroomMenu = CreatePopupMenu();
		txtChatroomSubMenu = CreatePopupMenu();
		txtChatroomPmSubMenu = CreatePopupMenu();
		AppendMenu(txtChatroomMenu, MF_STRING | MF_POPUP, 0xDDD6, "Get My IP Address");
		AppendMenu(txtChatroomMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);

		AppendMenu(txtChatroomMenu, MF_STRING | MF_POPUP, (UINT)txtChatroomPmSubMenu, "Private Messages");
		AppendMenu(txtChatroomPmSubMenu, MF_STRING, 0xDDD4, "Off");
		AppendMenu(txtChatroomPmSubMenu, MF_STRING, 0xDDD5, "On");
		AppendMenu(txtChatroomMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);

		AppendMenu(txtChatroomMenu, MF_STRING | MF_POPUP, (UINT)txtChatroomSubMenu, "Stealth Mode");
		AppendMenu(txtChatroomSubMenu, MF_STRING, 0xDDD8, "On");
		AppendMenu(txtChatroomSubMenu, MF_STRING, 0xDDD7, "Off");		

		
		//Userlist Menu
		userlistMenu = CreatePopupMenu();
		strcpy(userlistCommands[0].commands, "/kick ");

		strcpy(userlistCommands[1].commands, "/silence ");
		strcpy(userlistCommands[2].commands, "/silence ");
		strcpy(userlistCommands[3].commands, "/silence ");
		strcpy(userlistCommands[4].commands, "/silence ");
		strcpy(userlistCommands[5].commands, "/silence ");
		strcpy(userlistCommands[6].commands, "/silence ");
		strcpy(userlistCommands[7].commands, "/silence ");
		strcpy(userlistCommands[8].commands, "/silence ");
		strcpy(userlistCommands[9].commands, "/silence ");
		strcpy(userlistCommands[10].commands, "/silence ");
		userlistCommands[1].time = 5;
		userlistCommands[2].time = 15;
		userlistCommands[3].time = 30;
		userlistCommands[4].time = 45;
		userlistCommands[5].time = 60;
		userlistCommands[6].time = 180;
		userlistCommands[7].time = 300;
		userlistCommands[8].time = 1440;
		userlistCommands[9].time = 10080;
		userlistCommands[10].time = 30000;


		strcpy(userlistCommands[11].commands, "/ban ");
		strcpy(userlistCommands[12].commands, "/ban ");
		strcpy(userlistCommands[13].commands, "/ban ");
		strcpy(userlistCommands[14].commands, "/ban ");
		strcpy(userlistCommands[15].commands, "/ban ");
		strcpy(userlistCommands[16].commands, "/ban ");
		strcpy(userlistCommands[17].commands, "/ban ");
		strcpy(userlistCommands[18].commands, "/ban ");
		strcpy(userlistCommands[19].commands, "/ban ");
		strcpy(userlistCommands[20].commands, "/ban ");
		userlistCommands[11].time = 5;
		userlistCommands[12].time = 15;
		userlistCommands[13].time = 30;
		userlistCommands[14].time = 45;
		userlistCommands[15].time = 60;
		userlistCommands[16].time = 180;
		userlistCommands[17].time = 300;
		userlistCommands[18].time = 1440;
		userlistCommands[19].time = 10080;
		userlistCommands[20].time = 30000;

		strcpy(userlistCommands[21].commands, "/tempadmin ");
		strcpy(userlistCommands[22].commands, "/tempadmin ");
		strcpy(userlistCommands[23].commands, "/tempadmin ");
		strcpy(userlistCommands[24].commands, "/tempadmin ");
		strcpy(userlistCommands[25].commands, "/tempadmin ");
		strcpy(userlistCommands[26].commands, "/tempadmin ");
		strcpy(userlistCommands[27].commands, "/tempadmin ");
		strcpy(userlistCommands[28].commands, "/tempadmin ");
		strcpy(userlistCommands[29].commands, "/tempadmin ");
		strcpy(userlistCommands[30].commands, "/tempadmin ");
		userlistCommands[21].time = 5;
		userlistCommands[22].time = 15;
		userlistCommands[23].time = 30;
		userlistCommands[24].time = 45;
		userlistCommands[25].time = 60;
		userlistCommands[26].time = 180;
		userlistCommands[27].time = 300;
		userlistCommands[28].time = 1440;
		userlistCommands[29].time = 10080;
		userlistCommands[30].time = 30000;

		strcpy(userlistCommands[41].commands, "/tempmoderator ");
		strcpy(userlistCommands[42].commands, "/tempmoderator ");
		strcpy(userlistCommands[43].commands, "/tempmoderator ");
		strcpy(userlistCommands[44].commands, "/tempmoderator ");
		strcpy(userlistCommands[45].commands, "/tempmoderator ");
		strcpy(userlistCommands[46].commands, "/tempmoderator ");
		strcpy(userlistCommands[47].commands, "/tempmoderator ");
		strcpy(userlistCommands[48].commands, "/tempmoderator ");
		strcpy(userlistCommands[49].commands, "/tempmoderator ");
		strcpy(userlistCommands[50].commands, "/tempmoderator ");
		userlistCommands[41].time = 5;
		userlistCommands[42].time = 15;
		userlistCommands[43].time = 30;
		userlistCommands[44].time = 45;
		userlistCommands[45].time = 60;
		userlistCommands[46].time = 180;
		userlistCommands[47].time = 300;
		userlistCommands[48].time = 1440;
		userlistCommands[49].time = 10080;
		userlistCommands[50].time = 30000;

		strcpy(userlistCommands[31].commands, "/tempelevated ");
		strcpy(userlistCommands[32].commands, "/tempelevated ");
		strcpy(userlistCommands[33].commands, "/tempelevated ");
		strcpy(userlistCommands[34].commands, "/tempelevated ");
		strcpy(userlistCommands[35].commands, "/tempelevated ");
		strcpy(userlistCommands[36].commands, "/tempelevated ");
		strcpy(userlistCommands[37].commands, "/tempelevated ");
		strcpy(userlistCommands[38].commands, "/tempelevated ");
		strcpy(userlistCommands[39].commands, "/tempelevated ");
		strcpy(userlistCommands[40].commands, "/tempelevated ");
		userlistCommands[31].time = 5;
		userlistCommands[32].time = 15;
		userlistCommands[33].time = 30;
		userlistCommands[34].time = 45;
		userlistCommands[35].time = 60;
		userlistCommands[36].time = 180;
		userlistCommands[37].time = 300;
		userlistCommands[38].time = 1440;
		userlistCommands[39].time = 10080;
		userlistCommands[40].time = 30000;
		strcpy(userlistCommands[51].commands, "/finduser ");

		//Find User
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)0xCCCC, "Find User");

		AppendMenu(userlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);

		//PM
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)0xB2DD, "Private Message");

		AppendMenu(userlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);

		//Kick
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)0xFFFF, "Kick");
		
		//Silence
		userlistSubMenu = CreatePopupMenu();
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)userlistSubMenu, "Silence");
		AppendMenu(userlistSubMenu, MF_STRING, 1, "5 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 2, "15 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 3, "30 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 4, "45 minutes");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 5, "1 hour");
		AppendMenu(userlistSubMenu, MF_STRING, 6, "3 hours");
		AppendMenu(userlistSubMenu, MF_STRING, 7, "5 hours");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 8, "1 day");
		AppendMenu(userlistSubMenu, MF_STRING, 9, "1 week");
		AppendMenu(userlistSubMenu, MF_STRING, 10, "For a very long time...");
	
		//Ban
		userlistSubMenu = CreatePopupMenu();
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)userlistSubMenu, "Ban");
		AppendMenu(userlistSubMenu, MF_STRING, 11, "5 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 12, "15 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 13, "30 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 14, "45 minutes");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 15, "1 hour");
		AppendMenu(userlistSubMenu, MF_STRING, 16, "3 hours");
		AppendMenu(userlistSubMenu, MF_STRING, 17, "5 hours");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 18, "1 day");
		AppendMenu(userlistSubMenu, MF_STRING, 19, "1 week");
		AppendMenu(userlistSubMenu, MF_STRING, 20, "For a very long time...");

		AppendMenu(userlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);

		//Temp Elevated
		userlistSubMenu = CreatePopupMenu();
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)userlistSubMenu, "Temp Elevated");
		AppendMenu(userlistSubMenu, MF_STRING, 31, "5 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 32, "15 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 33, "30 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 34, "45 minutes");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 35, "1 hour");
		AppendMenu(userlistSubMenu, MF_STRING, 36, "3 hours");
		AppendMenu(userlistSubMenu, MF_STRING, 37, "5 hours");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 38, "1 day");
		AppendMenu(userlistSubMenu, MF_STRING, 39, "1 week");
		AppendMenu(userlistSubMenu, MF_STRING, 40, "For a very long time...");

		//Temp Moderator
		userlistSubMenu = CreatePopupMenu();
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)userlistSubMenu, "Temp Moderator");
		AppendMenu(userlistSubMenu, MF_STRING, 41, "5 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 42, "15 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 43, "30 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 44, "45 minutes");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 45, "1 hour");
		AppendMenu(userlistSubMenu, MF_STRING, 46, "3 hours");
		AppendMenu(userlistSubMenu, MF_STRING, 47, "5 hours");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 48, "1 day");
		AppendMenu(userlistSubMenu, MF_STRING, 49, "1 week");
		AppendMenu(userlistSubMenu, MF_STRING, 50, "For a very long time...");

		//Temp Admin
		userlistSubMenu = CreatePopupMenu();
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)userlistSubMenu, "Temp Admin");
		AppendMenu(userlistSubMenu, MF_STRING, 21, "5 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 22, "15 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 23, "30 minutes");
		AppendMenu(userlistSubMenu, MF_STRING, 24, "45 minutes");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 25, "1 hour");
		AppendMenu(userlistSubMenu, MF_STRING, 26, "3 hours");
		AppendMenu(userlistSubMenu, MF_STRING, 27, "5 hours");
		AppendMenu(userlistSubMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		AppendMenu(userlistSubMenu, MF_STRING, 28, "1 day");
		AppendMenu(userlistSubMenu, MF_STRING, 29, "1 week");
		AppendMenu(userlistSubMenu, MF_STRING, 30, "For a very long time...");

		AppendMenu(userlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		userlistSubMenu = CreatePopupMenu();
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)userlistSubMenu, "Ignore User");
		AppendMenu(userlistSubMenu, MF_STRING, 0xDDDB, "Ignore");
		AppendMenu(userlistSubMenu, MF_STRING, 0xDDDA, "Unignore");


		AppendMenu(userlistMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
		userlistSubMenu = CreatePopupMenu();
		AppendMenu(userlistMenu, MF_STRING | MF_POPUP, (UINT)userlistSubMenu, "Copy");
		AppendMenu(userlistSubMenu, MF_STRING, 0xFFFD, "User ID");
		AppendMenu(userlistSubMenu, MF_STRING, 0xFFFE, "Username");

		return 0; 
    } 

    DLLEXP kailleraSelectServerDialog(HWND parent){ 
		MSG msg;//Used to get messages from the thread's message queue
		WNDCLASSEX wcex; //Used to register window class information    
		hInstance = (HINSTANCE)GetModuleHandle(NULL);
		hDefaultFont = (HFONT)CreateFont(-11, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Microsoft Sans Serif");//GetStockObject(DEFAULT_GUI_FONT);
		mainHwnd = parent;
		
		wcex.cbSize         = sizeof(WNDCLASSEX);
		wcex.style          = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW; 
		wcex.lpfnWndProc    = (WNDPROC)WndProc;
		wcex.cbClsExtra     = 0;
		wcex.cbWndExtra     = 0;
		wcex.hInstance      = hInstance; 
		wcex.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);
		wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground  = (HBRUSH)COLOR_BTNFACE + 2;
		wcex.lpszMenuName   = NULL;
		wcex.lpszClassName  = "Supraclient";
		wcex.hIconSm        = NULL;

		//Class must be registered with the system before we can create the window
		RegisterClassEx(&wcex);
		LoadLibrary("Riched32.dll");
		InitCommonControlsEx(0);
		

		//Create Child Window
		form1 = CreateWindow("Supraclient", "SupraclientCPPE https://god-weapon.github.io", formProperties, xPos, yPos, 800, 600, NULL, NULL, hInstance, NULL);
		
		kailleraInit();
		createChatroom();
		createInitialWindow();

		Serverlist3DAdditem("Right Click to get EmuLinker Server List.", NULL, NULL, NULL, NULL, NULL, NULL);
		kServerlistAdditem("Right Click to get Kaillera Server List.", NULL, NULL, NULL, NULL, NULL, NULL);
		waitinglistAdditem("Right Click to get EmuLinker Waiting Games.", NULL, NULL, NULL, NULL, NULL, NULL);

		//Adjust Parent Window
		EnableWindow(parent, FALSE);
		ShowWindow(parent, SW_HIDE);
		
		//Start Winsock
		WSACleanup();
		WSAStartup(MAKEWORD(2,0), &startupInfo);

		//Show GUI
		ShowWindow(form1, SW_SHOWNORMAL);
		UpdateWindow(form1);
        
		//This is the message pump that retrieve all 
        //message from the calling thread's message queue 
		BOOL bRet;
		while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0){
			if (bRet != -1){
				TranslateMessage(&msg);
				DispatchMessage(&msg);//Dispatches the message to our WndProc
			}
			else{
				MessageBox(parent, "Error in Message Loop", "Message Loop Error!", NULL);
				break;
			}
		}

		return msg.wParam; //return exit code to the OS 
	} 

   DLLEXP kailleraModifyPlayValues(void *values, int size){ 
		int i;
		long w;
		
		//Exit Block
		if(sizeOfEinput == -1){
			Sleep(1);
			return sizeOfEinput;
		}
		//Game has Loaded. Wait Until Everyone is Ready.
		else if(stage == 0){
			inputSize = size;
			gameInit();
		}

		//Get Input from Emulator
		memcpy(&vInput[lastPosFrameSend], values, size);
		lastPosFrameSend += size;
		
		//Send Data
		frameSend++;
		if(frameSend == connectionType){
			frameSend = 0;
			lastPosFrameSend = 3;
			gameDataSend();
		}      

		//Initial Delay Setup
		frameRecv++;

		//Tweak for Spectating Mode
		if(totalPlayers == 0){
			returnInputSize = true;
		}
		//After Initial Delay Setup
		else if(frameRecv == inputFrame){
			frameRecv = 0;
			i = GetTickCount();
			w = 0;
			//Wait for Game Data if none is available
			while(frameCount < connectionType){
				//Exit Block
				if (sizeOfEinput == -1){
					return sizeOfEinput;
				}
				else if(GetTickCount() - i >= 1000){
					w = w + 1;
					i = GetTickCount();
					constructPacket(0, 0, 0x00);
				}
				else if(w == 15 && returnInputSize == true){
					sizeOfEinput = -1;
					MessageBox(form1, "Game Ended!  Didn't receive a response for 15s.", "Game Ended!", NULL);
					return sizeOfEinput;
				}
				DoEvents();
				Sleep(1);
			}
			inputFrame = connectionType;
			returnInputSize = true;
		}

		if(returnInputSize == true){
			//Return Values for Current Frame to Emulator
			//ex). MAME32k is 2 bytes of input/frame.  If there are 4 players, return 8 bytes
			for(i = 0; i < sizeOfEinput; i++){
				((char*)values)[i] = eInput[lastPosFrameRecv];
				lastPosFrameRecv++;
				if(lastPosFrameRecv == eSize){
					lastPosFrameRecv = 0;
				}
			}


			//Just used a frame.
			frameCount--;
			
			//Return Size of Input for ALL Players for ONE frame.  
			//ex). MAME32k is 2 bytes of input/frame.  If there are 4 players, return 8 bytes. 
			return sizeOfEinput;
		}
		
		//During Initial Delay Setup
		return 0;
   } 

   DLLEXP kailleraEndGame() {
	   dropGameRequest();
	   exitGameThread();
	   return 0;
   }

   DLLEXP kailleraChatSend(char *text){
	   char dataToBeSent[1024];
	   int i;

	  dataToBeSent[0] = '\0';
	  i = 0;
	  while(text[i] != '\0'){
		dataToBeSent[i + 1] = text[i];
		i++;
	  }
	  dataToBeSent[i + 1] = '\0';

	  //strcpy(&dataToBeSent[1], text);

	  constructPacket(dataToBeSent, (1 + i + 1), 0x08);//Game Chat Request
      return 0; 
   } 

};


//maxPing Subclass
long CALLBACK SubProcTxtMaxPing(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	char data[1024];
	int dLen;
	static bool controlKey = false;

	switch(message){
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
				case VK_RETURN:{
					//Max Ping
					GetWindowText(txtMaxPing, maxPingG, GetWindowTextLength(txtMaxPing) + 1);
					if(atoi(maxPingG) < 1 || atoi(maxPingG) > 1000){
						wsprintf(maxPingG, "%i", 16);
						SetWindowText(txtMaxPing, "200");
					}
					if(myGameID != -1 && imOwner == true && emulinkerSFValue == BST_CHECKED){
						data[0] = 'd';
						strcpy(&data[1], "/maxping ");
						strcat(data, maxPingG);
						dLen = strlen(data) + 1;
						data[0] = '\0';
						constructPacket(data, dLen, 0x08);
					}
					return 0;
				}
				case VK_CONTROL:{
					controlKey = true;
					return 0;
				}
				case 65:{
					if(controlKey == true){
						SendMessage(txtMaxPing, EM_SETSEL, 0, -1);
						SetFocus(txtMaxPing);
						return 0;
					}
				}
			}
		}

		case WM_KEYUP:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = false;
					return 0;
				}
			}
		}
		default:{
			if(wParam == VK_RETURN)
				return 0;
			else if(wParam == VK_CONTROL)
				return 0;
			return CallWindowProc(EditProcTxtMaxPing, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}


//maxUsers Subclass
long CALLBACK SubProcTxtMaxUsers(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	char data[1024];
	int dLen;
	static bool controlKey = false;

	switch(message){
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
				case VK_RETURN:{
					//Max Users
					GetWindowText(txtMaxUsers, maxUsersG, GetWindowTextLength(txtMaxUsers) + 1);
					if(atoi(maxUsersG) < 1 || atoi(maxUsersG) > 100){
						wsprintf(maxUsersG, "%i", 16);
						SetWindowText(txtMaxUsers, "16");
					}
					if(myGameID != -1 && imOwner == true && emulinkerSFValue == BST_CHECKED){
						data[0] = 'd';
						strcpy(&data[1], "/maxusers ");
						strcat(data, maxUsersG);
						dLen = strlen(data) + 1;
						data[0] = '\0';
						constructPacket(data, dLen, 0x08);
					}
					return 0;
				}
				case VK_CONTROL:{
					controlKey = true;
					return 0;
				}
				case 65:{
					if(controlKey == true){
						SendMessage(txtMaxUsers, EM_SETSEL, 0, -1);
						SetFocus(txtMaxUsers);
						return 0;
					}
				}
			}
		}
		case WM_KEYUP:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = false;
					return 0;
				}
			}
		}
		default:{
			if(wParam == VK_RETURN)
				return 0;
			else if(wParam == VK_CONTROL)
				return 0;
			return CallWindowProc(EditProcTxtMaxUsers, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}

//txtUsername Subclass
long CALLBACK SubProcTxtNick(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	static bool controlKey = false;

	switch(message){
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = true;
					return 0;
				}
				case 65:{
					if(controlKey == true){
						SendMessage(txtUsername, EM_SETSEL, 0, -1);
						SetFocus(txtUsername);
						return 0;
					}
				}
			}
		}
		case WM_KEYUP:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = false;
					return 0;
				}
			}
		}
		default:{
			return CallWindowProc(EditProcTxtNick, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}

//txtQuit Subclass
long CALLBACK SubProcTxtIP(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	static bool controlKey = false;

	switch(message){
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = true;
					return 0;
				}
				case 65:{
					if(controlKey == true){
						SendMessage(txtServerIP, EM_SETSEL, 0, -1);
						SetFocus(txtServerIP);
						return 0;
					}
				}
			}
		}
		case WM_KEYUP:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = false;
					return 0;
				}
			}
		}
		default:{
			return CallWindowProc(EditProcTxtIP, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}

//txtQuit Subclass
long CALLBACK SubProcTxtQuit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	static bool controlKey = false;

	switch(message){
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = true;
					return 0;
				}
				case 65:{
					if(controlKey == true){
						SendMessage(txtQuit, EM_SETSEL, 0, -1);
						SetFocus(txtQuit);
						return 0;
					}
				}
			}
		}
		case WM_KEYUP:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = false;
					return 0;
				}
			}
		}
		default:{
			return CallWindowProc(EditProcTxtQuit, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}


//Chatroom Subclass
long CALLBACK SubProcTxtChatroom(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
		case WM_RBUTTONUP:{
			popupMenu(9);
			return 0;
		}
		default:{
			return CallWindowProc(EditProcTxtChatroom, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}


//Chat Subclass
long CALLBACK SubProcTxtChat(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	static bool controlKey = false;

	switch(message){
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
				case VK_RETURN:{
					globalChatRequest();
					return 0;
				}
				case VK_CONTROL:{
					controlKey = true;
					return 0;
				}
				case 65:{
					if(controlKey == true){
						SendMessage(txtChat, EM_SETSEL, 0, -1);
						SetFocus(txtChat);
						return 0;
					}
				}
			}
		}
		case WM_KEYUP:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = false;
					return 0;
				}
			}
		}
		default:{
			if(wParam == VK_RETURN)
				return 0;
			return CallWindowProc(EditProcTxtChat, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}

//Game Chat Subclass
long CALLBACK SubProcTxtGameChat(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	static bool controlKey = false;

	switch(message){
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
				case VK_RETURN:{
					gameChatRequest();
					return 0;
				}
				case VK_CONTROL:{
					controlKey = true;
					return 0;
				}
				case 65:{
					if(controlKey == true){
						SendMessage(txtGameChat, EM_SETSEL, 0, -1);
						SetFocus(txtGameChat);
						return 0;
					}
				}
			}
		}
		case WM_KEYUP:{
			switch(LOWORD(wParam)){
				case VK_CONTROL:{
					controlKey = false;
					return 0;
				}
			}
		}
		default:{
			if(wParam == VK_RETURN)
				return 0;
			return CallWindowProc(EditProcTxtGameChat, hwnd, message, wParam, lParam);
		}
	}
	return TRUE;
}

//******************************************************************

            //This is where all the messages will be processed
//******************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	HWND hwndCtl = (HWND)lParam;
	char temp[65536];
	char temp1[1024];
	char dataToBeSent[1024];
	char server[1024];
	char ip[1024];
	char location[1024];
	char comments[1024];
	int i;
	int iSelect;
	LVITEM b;
	LVFINDINFO c;

	switch(message){
		case WM_COMMAND:{
			switch(wParam){
				case BN_CLICKED:{
					if(hwndCtl == btnLogin){
						if (loggedIn == true)
							break;

						loginToServer();
					
						//Display
						GetWindowText(txtServerIP, ip, 256);

						c.psz = ip;
						c.flags = LVFI_STRING;
						int pos = SendMessage(lstRecentList, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);

						if(pos > -1){
							//Get IP Address
							b.mask = LVIF_TEXT;
							b.iItem = pos;
							b.iSubItem = 1;
							b.pszText = server;
							b.cchTextMax = 1024;
							SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
							if(strcmp(server, "NA") == 0){
								strcpy(myServer, ip);
								SetWindowText(form1, ip);
							}
							else{
								strcpy(myServer, server);
								SetWindowText(form1, server);
							}
						}
						else{
							strcpy(myServer, ip);
							SetWindowText(form1, ip);

							int pos = SendMessage(lstFavoriteList, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);
	
							if(pos > -1){
								//Get IP Address
								b.mask = LVIF_TEXT;
								b.iItem = pos;
								b.iSubItem = 1;
								b.pszText = server;
								b.cchTextMax = 1024;
								SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
								if(strcmp(server, "NA") == 0){
									strcpy(myServer, ip);
									SetWindowText(form1, ip);
								}
								else{
									strcpy(myServer, server);
									SetWindowText(form1, server);
								}
							}
							else{
								strcpy(myServer, ip);
								SetWindowText(form1, ip);
							}
						}

						recentlistAdditem("NA", ip, "NA");
					}
					else if(hwndCtl == btnEOkay){
						editFavorite = true;
					}
					else if(hwndCtl == chkJoinChat){
						joinChatValue = SendMessage(chkJoinChat, BM_GETCHECK, 0, 0);
					}
					else if(hwndCtl == chkCreate){
						createValue = SendMessage(chkCreate, BM_GETCHECK, 0, 0);
					}

					else if(hwndCtl == chkUseScreenChat){
						useScreenChatValue = SendMessage(chkUseScreenChat, BM_GETCHECK, 0, 0);
					}
					else if(hwndCtl == chkEmulinkerSF){
						emulinkerSFValue = SendMessage(chkEmulinkerSF, BM_GETCHECK, 0, 0);
					}


					/*else if(hwndCtl == btnP2PServer){
						/* Open windows connection *
						if (WSAStartup(0x0101, &startupInfo) != 0){
							MessageBox(NULL, "Could not open Windows connection.", "WSAStartup Error", NULL);
						}
						/* Open a datagram socket *
						p2pSocket = socket(AF_INET, SOCK_DGRAM, 0);
						if (p2pSocket == INVALID_SOCKET){
							MessageBox(NULL, "Could not create socket!.", "Socket Error", NULL);
							WSACleanup();
						}
						/* Set family and port *
						p2pServerInfo.sin_family = AF_INET;
						p2pServerInfo.sin_port = htons(7159);
						p2pServerInfo.sin_addr.s_addr = inet_addr("127.0.0.1");

						/* Bind address to socket *
						if (bind(p2pSocket, (struct sockaddr *)&p2pServerInfo, sizeof(struct sockaddr_in)) == -1){
							MessageBox(NULL, "Could not bind to socket!.", "Bind Error", NULL);
							closesocket(p2pSocket);
							WSACleanup();
						}

						p2pClientLength = (int)sizeof(struct sockaddr_in);
						p2pThread = CreateThread(NULL, 0, p2pLoop, NULL, 0, NULL);
					}
					else if(hwndCtl == btnP2PConnect){
						supraCleanup(4, 0);

						p2pSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
						
						//Length of ServerIP:Port
						short lenServerIP = (short)GetWindowTextLength(txtP2PServer);

						if(lenServerIP < 1){
							MessageBox(form1,"Invalid Address Format!", "Address Error!", NULL);
							return 0;
						}

						//Get ServerIP:Port
						GetWindowText(txtP2PServer, ip, lenServerIP + 1);
						//Find:
						char* c = strstr(ip, ":");

						if(c != NULL){
							//Split Port
							strcpy(temp, c + 1);
							//Split Address and Port
							ZeroMemory(server, 512);
							strncpy(server, ip, (lenServerIP - strlen(temp) - 1));
							i = (u_short)atoi(temp);
						}
						else{
							ZeroMemory(server, 512);
							GetWindowText(txtP2PServer, server, lenServerIP + 1);
							i = 7159;
						}
						
						p2pServerInfo.sin_family = AF_INET;
						p2pServerInfo.sin_port = htons(i);
						
						unsigned long addr;

						if(inet_addr(server) == INADDR_NONE){
							hp = gethostbyname(server);
							if(hp == NULL){
								closesocket(p2pSocket);
								MessageBox(form1, "Error Resolving!", "Error!", NULL);
								return 0;
							}
							p2pServerInfo.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
						}
						else{
							p2pServerInfo.sin_addr.s_addr = inet_addr(myAddress);
						}

 
						p2pThread = CreateThread(NULL, 0, p2pLoop, NULL, 0, NULL);

						sendto(p2pSocket, "GAME_START",  10, NULL, (sockaddr *) &p2pServerInfo, sizeof(p2pServerInfo));


					}
					else if(hwndCtl == btnP2PStart){

					}*/


					else if(hwndCtl == chkKeepChatLogs){
						chatLogValue = SendMessage(chkKeepChatLogs, BM_GETCHECK, 0, 0);
					}
					else if(hwndCtl == chkKeepGameChatLogs){
						gameChatLogValue = SendMessage(chkKeepGameChatLogs, BM_GETCHECK, 0, 0);
					}
					else if(hwndCtl == chkFakeP2P){
						fakeP2PValue = SendMessage(chkFakeP2P, BM_GETCHECK, 0, 0);
						if(myGameID != -1 && imOwner == true){
							temp[0] = 'd';
							if(fakeP2PValue == BST_CHECKED)
								strcpy(&temp[1], "/p2pon");
							else
								strcpy(&temp[1], "/p2poff");
							i = strlen(temp) + 1;
							temp[0] = '\0';
							constructPacket(temp, i, 0x08);							
						}
					}
					else if (hwndCtl == chkEmuRes) {
						emuResValue = SendMessage(chkEmuRes, BM_GETCHECK, 0, 0);
						if (myGameID != -1 && imOwner == true) {
							temp[0] = 'd';
							if (emuResValue == BST_CHECKED) {
								strcpy(&temp[1], "/setemu ");
								strcat(temp, emulator);
							}
							else
								strcpy(&temp[1], "/setemu any");
							i = strlen(temp) + 1;
							temp[0] = '\0';
							constructPacket(temp, i, 0x08);
						}
					}
					else if (hwndCtl == chkConnRes) {
						connResValue = SendMessage(chkConnRes, BM_GETCHECK, 0, 0);
						if (myGameID != -1 && imOwner == true) {
							temp[0] = 'd';
							if (connResValue == BST_CHECKED)
								strcpy(&temp[1], "/setconn");
							else
								strcpy(&temp[1], "/setconn any");
							i = strlen(temp) + 1;
							temp[0] = '\0';
							constructPacket(temp, i, 0x08);
						}
					}
					else if(hwndCtl == btnVersion && myUserID != -1){
						temp[0] = 'd';
						strcpy(&temp[1], "/version");
						i = strlen(temp) + 1;
						temp[0] = '\0';
						constructPacket(temp, i, 0x07);							
					}
					else if(hwndCtl == btnLagStat){
						temp[0] = 'd';
						strcpy(&temp[1], "/lagstat");
						i = strlen(temp) + 1;
						temp[0] = '\0';
						constructPacket(temp, i, 0x08);							
					}
					else if(hwndCtl == btnDrop){
						dropGameRequest();
						exitGameThread();
						//if (gamePlaying == true)
							//constructPacket("\0Game Ended Manually!", 22, 0x08);
					}
					else if(hwndCtl == btnCreateAway)
						updateAway();
					else if(hwndCtl == btnComments){
						cComments = true;
						SendMessage(txtComments, WM_GETTEXT, 1024, (LPARAM) fComments);
						DestroyWindow(frmComments);
					}
					else if(hwndCtl == btnPM){
						cPM = true;
						SendMessage(txtPM, WM_GETTEXT, 1024, (LPARAM) fPM);
						DestroyWindow(frmPM);
					}
					else if(hwndCtl == btnChat)
						globalChatRequest();
					else if(hwndCtl == btnGameChat)
						gameChatRequest();
					else if(hwndCtl == btnAway){
						if(imAway == false)
							popupAway();
						else{
							imAway = false;
							showAway();
						}
					}
					else if(hwndCtl == btnRemoveAway){
						if(SendMessage(lstAway, LVM_GETITEMCOUNT, 0, 0) != 0){
							iSelect = SendMessage(lstAway, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
							if(iSelect > -1){
								b.mask = LVIF_TEXT;
								b.iItem = iSelect;
								b.iSubItem = 2;
								b.pszText = temp;
								b.cchTextMax = 1024;
								SendMessage(lstAway, LVM_GETITEM, (WPARAM)iSelect, (LPARAM)&b);
								i = cStrToInt(temp);
								awayMessages[i].save = false;
								SendMessage(lstAway, LVM_DELETEITEM, (WPARAM) iSelect, 0);
							}
						}
					}
					else if(hwndCtl == btnLogoff){
						userQuitRequest();
					}
					else if(hwndCtl == btnJoin){
						if(swapp == false)
							joinGameRequest();
						else{
							//0x07 - Global Chat Request
							if(strcmp(gOwner, username) != 0){
								strcpy(temp, "A<");
								strcat(temp, gOwner);
								strcat(temp, "> ");

								strcat(temp, currentGame);
							}
							else{
								strcpy(temp, "A");
								strcat(temp, currentGame);
							}

							strcat(temp, " [");
							strcat(temp, gEmulator);
							strcat(temp,"]");
							i = strlen(temp);
							temp[0] = '\0';
							constructPacket(temp, 1 + i, 0x07);

						}
					}
					else if (hwndCtl == btnGameLeave) {
						exitGameThread();
						quitGameRequest();
					}
					else if(hwndCtl == btnCreate){
						if(swapp == false){
							popupMenu(0);
						}
						else{
							if(gameroom == true)
								showGameroom(false);
							else
								showGameroom(true);
						}
					}
					else if(hwndCtl == btnChatroom){
						if(chatroom == true)
							showChatroom(false);
						else
							showChatroom(true);
					}
					else if(hwndCtl == btnGameKick && imOwner)
						kickRequest();
					else if(hwndCtl == btnGameStart && imOwner)
						startGameRequest();
					return 0;
				}
			}
			return 0;
		}
		case WM_NOTIFY:{
			NMHDR* pnmh = (NMHDR*)lParam;
			NMLISTVIEW* pnmlv = (NMLISTVIEW*)lParam;
			NMITEMACTIVATE* pnmia = (NMITEMACTIVATE*) lParam;
			ENLINK * tEN = (ENLINK *)pnmh;
			char tLink[1024];
			TEXTRANGE g;
			
			//Tab Control Login Info
			if((pnmh->hwndFrom == tTab) && (pnmh->code == TCN_SELCHANGE )){ 
				i = TabCtrl_GetCurSel(tTab);

				if(i == 0)
					showOptions(1);
				else if(i == 1)
					showOptions(0);
				else if(i == 2)
					showOptions(2);
				else if(i == 3)
					showOptions(3);
				else if(i == 4)
					showOptions(4);
				return 0;
			}

			//Tab Control Server List
			if((pnmh->hwndFrom == sTab) && (pnmh->code == TCN_SELCHANGE )){ 
				i = TabCtrl_GetCurSel(sTab);

				if(i == 0){
					showServerlistK();
				}
				else if(i == 1){
					showServerlist3D();
				}
				else if(i == 2)
					showRecentlist();
				else if(i == 3)
					showFavoritelist();
				else if(i == 4)
					showWaitinglist();

				lastTabServer = SendMessage(sTab, TCM_GETCURSEL, 0, 0);
				return 0;
			}

			//Chatroom Link Support
            if((pnmh->hwndFrom == txtChatroom) && (pnmh->code == EN_LINK)){  
				if(tEN->msg == WM_LBUTTONDOWN){
					g.chrg = tEN->chrg;
					g.lpstrText = tLink;
					SendMessage(txtChatroom, EM_GETTEXTRANGE, 0, (LPARAM) &g);
					ShellExecute(txtChatroom, NULL, tLink, NULL, NULL, SW_SHOWNORMAL);
					tEN->msg = 0;
					return 0;
				}
			}
			//Game Chatroom Link Support
            else if((pnmh->hwndFrom == txtGameChatroom) && (pnmh->code == EN_LINK)){  
				if(tEN->msg == WM_LBUTTONDOWN){
					g.chrg = tEN->chrg;
					g.lpstrText = tLink;
					SendMessage(txtGameChatroom, EM_GETTEXTRANGE, 0, (LPARAM) &g);
					ShellExecute(txtGameChatroom, NULL, tLink, NULL, NULL, SW_SHOWNORMAL);
					tEN->msg = 0;
					return 0;
				}
			}

			//lstUserlist
            if((pnmh->hwndFrom == lstUserlist) && (pnmh->code == LVN_ITEMACTIVATE)){    
				//Get Currently Selected item
				iSelect = SendMessage(lstUserlist, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
				if(iSelect < 0)
					return 0;

				//Get UserID
				b.mask = LVIF_TEXT;
				b.iItem = iSelect;
				b.iSubItem = 0;
				b.pszText = temp1;
				b.cchTextMax = 1024;
				SendMessage(lstUserlist, LVM_GETITEM, 0, (LPARAM) &b);


				/*strcpy(temp, userlistCommands[0].commands);
				strcat(temp, temp1);
				//Send
				SendMessage(txtChat, WM_SETTEXT, 0, (LPARAM) temp);
				globalChatRequest();*/

				EnableWindow(form1, FALSE);
				createPMWindow();

				if (strlen(fPM) == NULL)
					return 0;

				temp[0] = 'd';
				strcpy(&temp[1], "/msg ");
				strcat(temp, temp1);
				strcat(&temp[1], " ");
				strcat(temp, fPM);
				i = strlen(temp) + 1;
				temp[0] = '\0';

				constructPacket(temp, i, 0x07);

                return 0;
            }
            else if((pnmh->hwndFrom == lstUserlist) && (pnmh->code == NM_RCLICK)){    
                popupMenu(2);
                return 0;
            }
            else if((pnmh->hwndFrom == lstUserlist) && (pnmh->code == LVN_COLUMNCLICK)){
                // The user clicked on one of the columns.  Here we will save
                // the index of the subitem that the column refers to.  Then
                // call ListView_SortItemsEx() to sort the list items.
				if(userlistSwitch == false)
					userlistSwitch = true;
				else
					userlistSwitch = false;
                lstUserlistColumn = pnmlv->iSubItem;
                ListView_SortItemsEx(lstUserlist, lstUserlistCompareFunc, 0);
                return 0;
            }
            else if((pnmh->hwndFrom == lstUserlist) && (pnmh->code == LVN_ITEMCHANGED)){
                // See if the state changed on the item in question.
                if(pnmlv->uChanged & LVIF_STATE){
                    // We compare the old state vs. the new state to see how
                    // the item is changing.
                    if(((pnmlv->uOldState & LVIS_FOCUSED) == 0) && (pnmlv->uNewState & LVIS_FOCUSED)){
                        // This item is getting the focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_FOCUSED) == 0) && (pnmlv->uOldState & LVIS_FOCUSED)){
                        // This item is losing focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uOldState & LVIS_SELECTED)){
                        // This item is being selected
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uNewState & LVIS_SELECTED)){
                        // This item is being unselected
                        return 0;
                    }
                }
            }

			//lstGameUserlist
            if((pnmh->hwndFrom == lstGameUserlist) && (pnmh->code == LVN_ITEMACTIVATE) && imOwner){
                //kickRequest();
                return 0;
            }
            else if((pnmh->hwndFrom == lstGameUserlist) && (pnmh->code == NM_RCLICK)){    
                popupMenu(4);
                return 0;
            }

			//lstGamelist
            if((pnmh->hwndFrom == lstGamelist) && (pnmh->code == LVN_ITEMACTIVATE)){
				if(myGameID == -1 && SendMessage(chkJoinDbl, BM_GETCHECK, 0, 0) == BST_CHECKED)
					joinGameRequest();
                return 0;
            }
            else if((pnmh->hwndFrom == lstGamelist) && (pnmh->code == NM_RCLICK)){    
                popupMenu(3);
                return 0;
            }
            else if((pnmh->hwndFrom == lstGamelist) && (pnmh->code == LVN_COLUMNCLICK)){
                // The user clicked on one of the columns.  Here we will save
                // the index of the subitem that the column refers to.  Then
                // call ListView_SortItemsEx() to sort the list items.
				if(gamelistSwitch == false)
					gamelistSwitch = true;
				else
					gamelistSwitch = false;
                lstGamelistColumn = pnmlv->iSubItem;
                ListView_SortItemsEx(lstGamelist, lstGamelistCompareFunc, 0);
                return 0;
            }
            else if((pnmh->hwndFrom == lstGamelist) && (pnmh->code == LVN_ITEMCHANGED)){
                // See if the state changed on the item in question.
                if(pnmlv->uChanged & LVIF_STATE){
                    // We compare the old state vs. the new state to see how
                    // the item is changing.
                    if(((pnmlv->uOldState & LVIS_FOCUSED) == 0) && (pnmlv->uNewState & LVIS_FOCUSED)){
                        // This item is getting the focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_FOCUSED) == 0) && (pnmlv->uOldState & LVIS_FOCUSED)){
                        // This item is losing focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uOldState & LVIS_SELECTED)){
                        // This item is being selected
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uNewState & LVIS_SELECTED)){
                        // This item is being unselected
                        return 0;
                    }
                }
            }

			//lstServerListK
            if((pnmh->hwndFrom == lstServerListK) && (pnmh->code == LVN_ITEMACTIVATE)){
				userQuitRequest();
				//Get Currently Selected item
				iSelect = SendMessage(lstServerListK, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
				//Get IP Address
				b.mask = LVIF_TEXT;
				b.iItem = iSelect;
				b.cchTextMax = 1024;

				b.iSubItem = 0;
				b.pszText = server;
				SendMessage(lstServerListK, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 1;
				b.pszText = ip;
				SendMessage(lstServerListK, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 3;
				b.pszText = location;
				SendMessage(lstServerListK, LVM_GETITEM, 0, (LPARAM) &b);

				//Display
				SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) ip);

				loginToServer();

				//Display
				strcpy(myServer, server);
				SetWindowText(form1, server);

				recentlistAdditem(server, ip, location);
                return 0;
            }
            else if((pnmh->hwndFrom == lstServerListK) && (pnmh->code == NM_RCLICK)){    
                popupMenu(5);
                return 0;
            }
            else if((pnmh->hwndFrom == lstServerListK) && (pnmh->code == LVN_COLUMNCLICK)){
                // The user clicked on one of the columns.  Here we will save
                // the index of the subitem that the column refers to.  Then
                // call ListView_SortItemsEx() to sort the list items.
				if (!pingingK) {
					if (kailleraSwitch == false)
						kailleraSwitch = true;
					else
						kailleraSwitch = false;
					lstServerlistKColumn = pnmlv->iSubItem;
					ListView_SortItemsEx(lstServerListK, lstServerlistKCompareFunc, 0);
				}
                return 0;
            }
            else if((pnmh->hwndFrom == lstServerListK) && (pnmh->code == LVN_ITEMCHANGED)){
                // See if the state changed on the item in question.
                if(pnmlv->uChanged & LVIF_STATE){
                    // We compare the old state vs. the new state to see how
                    // the item is changing.
                    if(((pnmlv->uOldState & LVIS_FOCUSED) == 0) && (pnmlv->uNewState & LVIS_FOCUSED)){
                        // This item is getting the focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_FOCUSED) == 0) && (pnmlv->uOldState & LVIS_FOCUSED)){
                        // This item is losing focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uOldState & LVIS_SELECTED)){
                        // This item is being selected
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uNewState & LVIS_SELECTED)){
                        // This item is being unselected
                        return 0;
                    }
                }
            }

			//lstServerList3D
            if((pnmh->hwndFrom == lstServerList3D) && (pnmh->code == LVN_ITEMACTIVATE)){
				userQuitRequest();
				//Get Currently Selected item
				iSelect = SendMessage(lstServerList3D, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				//Get IP Address
				b.mask = LVIF_TEXT;
				b.iItem = iSelect;
				b.iSubItem = 1;
				b.pszText = ip;
				b.cchTextMax = 1024;
				SendMessage(lstServerList3D, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 0;
				b.pszText = server;
				SendMessage(lstServerList3D, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 3;
				b.pszText = location;
				SendMessage(lstServerList3D, LVM_GETITEM, 0, (LPARAM) &b);

				//Display
				SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) ip);

				loginToServer();

				//Display
				strcpy(myServer, server);
				SetWindowText(form1, server);

				recentlistAdditem(server, ip, location);
                return 0;
            }
            else if((pnmh->hwndFrom == lstServerList3D) && (pnmh->code == NM_RCLICK)){    
                popupMenu(5);
                return 0;
            }
            else if((pnmh->hwndFrom == lstServerList3D) && (pnmh->code == LVN_COLUMNCLICK)){
                // The user clicked on one of the columns.  Here we will save
                // the index of the subitem that the column refers to.  Then
                // call ListView_SortItemsEx() to sort the list items.
				if (!pinging3D) {
					if (anti3DSwitch == false)
						anti3DSwitch = true;
					else
						anti3DSwitch = false;
					lstServerlist3DColumn = pnmlv->iSubItem;
					ListView_SortItemsEx(lstServerList3D, lstServerlist3DCompareFunc, 0);
				}
                return 0;
            }
            else if((pnmh->hwndFrom == lstServerList3D) && (pnmh->code == LVN_ITEMCHANGED)){
                // See if the state changed on the item in question.
                if(pnmlv->uChanged & LVIF_STATE){
                    // We compare the old state vs. the new state to see how
                    // the item is changing.
                    if(((pnmlv->uOldState & LVIS_FOCUSED) == 0) && (pnmlv->uNewState & LVIS_FOCUSED)){
                        // This item is getting the focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_FOCUSED) == 0) && (pnmlv->uOldState & LVIS_FOCUSED)){
                        // This item is losing focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uOldState & LVIS_SELECTED)){
                        // This item is being selected
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uNewState & LVIS_SELECTED)){
                        // This item is being unselected
                        return 0;
                    }
                }
            }


			//lstWaitingList
            if((pnmh->hwndFrom == lstWaitingList) && (pnmh->code == LVN_ITEMACTIVATE)){
				userQuitRequest();
				//Get Currently Selected item
				iSelect = SendMessage(lstWaitingList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				//Get IP Address
				b.mask = LVIF_TEXT;
				b.iItem = iSelect;
				b.iSubItem = 4;
				b.pszText = ip;
				b.cchTextMax = 1024;
				SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 3;
				b.pszText = server;
				SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 5;
				b.pszText = location;
				SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &b);

				//Display
				SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) ip);

				loginToServer();

				//Display
				strcpy(myServer, server);
				SetWindowText(form1, server);

				recentlistAdditem(server, ip, location);
                return 0;
            }
            else if((pnmh->hwndFrom == lstWaitingList) && (pnmh->code == NM_RCLICK)){    
				popupMenu(6);
                return 0;
            }
            else if((pnmh->hwndFrom == lstWaitingList) && (pnmh->code == LVN_COLUMNCLICK)){
                // The user clicked on one of the columns.  Here we will save
                // the index of the subitem that the column refers to.  Then
                // call ListView_SortItemsEx() to sort the list items.
				if(waitingSwitch == false)
					waitingSwitch = true;
				else
					waitingSwitch = false;
                lstWaitinglistColumn = pnmlv->iSubItem;
                ListView_SortItemsEx(lstWaitingList, lstWaitinglistCompareFunc, 0);
                return 0;
            }
            else if((pnmh->hwndFrom == lstWaitingList) && (pnmh->code == LVN_ITEMCHANGED)){
                // See if the state changed on the item in question.
                if(pnmlv->uChanged & LVIF_STATE){
                    // We compare the old state vs. the new state to see how
                    // the item is changing.
                    if(((pnmlv->uOldState & LVIS_FOCUSED) == 0) && (pnmlv->uNewState & LVIS_FOCUSED)){
                        // This item is getting the focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_FOCUSED) == 0) && (pnmlv->uOldState & LVIS_FOCUSED)){
                        // This item is losing focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uOldState & LVIS_SELECTED)){
                        // This item is being selected
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uNewState & LVIS_SELECTED)){
                        // This item is being unselected
                        return 0;
                    }
                }
            }

			//lstFavoriteList
            if((pnmh->hwndFrom == lstFavoriteList) && (pnmh->code == LVN_ITEMACTIVATE)){
				userQuitRequest();
				i = SendMessage(lstFavoriteList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				b.mask = LVIF_TEXT;
				b.iItem = i;
				b.cchTextMax = 1024;

				b.iSubItem = 1;
				b.pszText = server;
				SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 0;
				b.pszText = ip;
				SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 2;
				b.pszText = location;
				SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 3;
				b.pszText = comments;
				SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);

				//Display
				SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) ip);

				SendMessage(lstFavoriteList, LVM_DELETEITEM, (WPARAM) i, 0);

				favoritelistAdditem(server, ip, location, comments);

				loginToServer();

				//Display
				strcpy(myServer, server);
				SetWindowText(form1, server);

				recentlistAdditem(server, ip, location);
            }
            else if((pnmh->hwndFrom == lstFavoriteList) && (pnmh->code == NM_RCLICK)){    
				popupMenu(8);
                return 0;
            }
            else if((pnmh->hwndFrom == lstFavoriteList) && (pnmh->code == LVN_COLUMNCLICK)){
                // The user clicked on one of the columns.  Here we will save
                // the index of the subitem that the column refers to.  Then
                // call ListView_SortItemsEx() to sort the list items.
				if(favoriteSwitch == false)
					favoriteSwitch = true;
				else
					favoriteSwitch = false;
                lstFavoritelistColumn = pnmlv->iSubItem;
                ListView_SortItemsEx(lstFavoriteList, lstFavoritelistCompareFunc, 0);
                return 0;
            }
            else if((pnmh->hwndFrom == lstFavoriteList) && (pnmh->code == LVN_ITEMCHANGED)){
                // See if the state changed on the item in question.
                if(pnmlv->uChanged & LVIF_STATE){
                    // We compare the old state vs. the new state to see how
                    // the item is changing.
                    if(((pnmlv->uOldState & LVIS_FOCUSED) == 0) && (pnmlv->uNewState & LVIS_FOCUSED)){
                        // This item is getting the focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_FOCUSED) == 0) && (pnmlv->uOldState & LVIS_FOCUSED)){
                        // This item is losing focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uOldState & LVIS_SELECTED)){
                        // This item is being selected
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uNewState & LVIS_SELECTED)){
                        // This item is being unselected
                        return 0;
                    }
                }
            }

			//lstRecentList
            if((pnmh->hwndFrom == lstRecentList) && (pnmh->code == LVN_ITEMACTIVATE)){
				userQuitRequest();
				i = SendMessage(lstRecentList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				b.mask = LVIF_TEXT;
				b.iItem = i;
				b.cchTextMax = 1024;

				b.iSubItem = 1;
				b.pszText = server;
				SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 0;
				b.pszText = ip;
				SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
				b.iSubItem = 2;
				b.pszText = location;
				SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);

				//Display
				SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) ip);

				SendMessage(lstRecentList, LVM_DELETEITEM, (WPARAM) i, 0);

				loginToServer();

				//Display
				strcpy(myServer, server);
				SetWindowText(form1, server);

				recentlistAdditem(server, ip, location);
            }
            else if((pnmh->hwndFrom == lstRecentList) && (pnmh->code == NM_RCLICK)){    
				popupMenu(7);
                return 0;
            }
            else if((pnmh->hwndFrom == lstRecentList) && (pnmh->code == LVN_COLUMNCLICK)){
                // The user clicked on one of the columns.  Here we will save
                // the index of the subitem that the column refers to.  Then
                // call ListView_SortItemsEx() to sort the list items.
				if(recentSwitch == false)
					recentSwitch = true;
				else
					recentSwitch = false;
                lstRecentlistColumn = pnmlv->iSubItem;
                ListView_SortItemsEx(lstRecentList, lstRecentlistCompareFunc, 0);
                return 0;
            }
            else if((pnmh->hwndFrom == lstRecentList) && (pnmh->code == LVN_ITEMCHANGED)){
                // See if the state changed on the item in question.
                if(pnmlv->uChanged & LVIF_STATE){
                    // We compare the old state vs. the new state to see how
                    // the item is changing.
                    if(((pnmlv->uOldState & LVIS_FOCUSED) == 0) && (pnmlv->uNewState & LVIS_FOCUSED)){
                        // This item is getting the focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_FOCUSED) == 0) && (pnmlv->uOldState & LVIS_FOCUSED)){
                        // This item is losing focus.
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uOldState & LVIS_SELECTED)){
                        // This item is being selected
                        return 0;
                    }
                    if(((pnmlv->uNewState & LVIS_SELECTED) == 0) && (pnmlv->uNewState & LVIS_SELECTED)){
                        // This item is being unselected
                        return 0;
                    }
                }
            }
            return 0;

		}
		case WM_CLOSE:{
			DestroyWindow(hWnd);
            return 0;
		}
		case WM_DESTROY:{
			if(supraCleanup(3, hWnd) == true){
				ShowWindow(mainHwnd, SW_SHOW);
				EnableWindow(mainHwnd, TRUE);
				UpdateWindow(mainHwnd);
				PostQuitMessage(0);
			}
			return 0;
		}
		//For Waiting Games List
		case SC_SUPRARECVWAITING:{
			switch(WSAGETSELECTEVENT(lParam)){
				case FD_CONNECT:{
					waiting = true;
					//Http GET Request
					strcpy(dataToBeSent, "GET /");
					strcat(dataToBeSent, anti3DServerList.wglink);
					strcat(dataToBeSent, " HTTP/1.0\r\n");
					strcat(dataToBeSent, "Host: ");
					strcat(dataToBeSent, anti3DServerList.host);
					strcat(dataToBeSent,"\r\n\r\n");
					send(mySocketWaiting, dataToBeSent, strlen(dataToBeSent) + 1, NULL);
					return 0;
				}
				case FD_READ:{
					bytesRecvWaiting = recv(mySocketWaiting, myBuffWaiting, sizeof(myBuffWaiting), NULL);
					parseWaitingGames();
					return 0;
				}				
			}
			return 0;
		}
		//For Anti3D Server List
		case SC_SUPRARECV3D:{
			switch(WSAGETSELECTEVENT(lParam)){
				case FD_CONNECT:{
					//Http GET Request
					strcpy(dataToBeSent, "GET /");
					strcat(dataToBeSent, anti3DServerList.link);
					strcat(dataToBeSent, " HTTP/1.0\r\n");
					strcat(dataToBeSent, "Host: ");
					strcat(dataToBeSent, anti3DServerList.host);
					strcat(dataToBeSent,"\r\n\r\n");
					send(mySocket3D, dataToBeSent, strlen(dataToBeSent) + 1, NULL);
					return 0;
				}
				case FD_READ:{
					bytesRecvServerList3D = recv(mySocket3D, myBuffServerList3D, sizeof(myBuffServerList3D), NULL);
					parseServerList3D();
					return 0;
				}				
			}
			return 0;
		}
		//For Kaillera Server List
		case SC_SUPRARECVK:{
			switch(WSAGETSELECTEVENT(lParam)){
				case FD_CONNECT:{
					//Http GET Request
					strcpy(dataToBeSent, "GET /");
					strcat(dataToBeSent, kailleraServerList.link);
					strcat(dataToBeSent, " HTTP/1.0\r\n");
					strcat(dataToBeSent, "Host: ");
					strcat(dataToBeSent, kailleraServerList.host);
					strcat(dataToBeSent,"\r\n\r\n");
					send(mySocketK, dataToBeSent, strlen(dataToBeSent) + 1, NULL);
					return 0;
				}
				case FD_READ:{
					bytesRecvServerListK = recv(mySocketK, myBuffServerListK, sizeof(myBuffServerListK), NULL);
					parseServerListK();
					return 0;
				}				
			}
			return 0;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
//*******************************************************************




void createInitialWindow(){
	LVCOLUMN LvCol;
	TCITEM v;

	//Create Favoritelist
	lstFavoriteList = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 10, 30, 770, 440, form1, NULL, hInstance, NULL);
	SendMessage(lstFavoriteList, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	LvCol.pszText = "IP Address\0";
	LvCol.cx = 150;
	SendMessage(lstFavoriteList, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "Server Name\0";
	LvCol.cx = 250;
	SendMessage(lstFavoriteList, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Location\0";
	LvCol.cx = 140;
	SendMessage(lstFavoriteList, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	LvCol.pszText = "Comments\0";
	LvCol.cx = 205;
	SendMessage(lstFavoriteList, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	SendMessage(lstFavoriteList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Create Waitinglist
	lstWaitingList = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 10, 30, 770, 440, form1, NULL, hInstance, NULL);
	SendMessage(lstWaitingList, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.pszText = "Game Name\0";
	LvCol.cx = 250;
	SendMessage(lstWaitingList, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "Emulator\0";
	LvCol.cx = 125;
	SendMessage(lstWaitingList, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Username\0";
	LvCol.cx = 100;
	SendMessage(lstWaitingList, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	LvCol.pszText = "Server Name\0";
	LvCol.cx = 100;
	SendMessage(lstWaitingList, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	LvCol.pszText = "IP Address\0";
	LvCol.cx = 100;
	SendMessage(lstWaitingList, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol);
	LvCol.pszText = "Location\0";
	LvCol.cx = 100;
	SendMessage(lstWaitingList, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol);
	LvCol.pszText = "# of Players\0";
	LvCol.cx = 80;
	SendMessage(lstWaitingList, LVM_INSERTCOLUMN, 6, (LPARAM)&LvCol);
	SendMessage(lstWaitingList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Create Recentlist
	lstRecentList = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 10, 30, 770, 440, form1, NULL, hInstance, NULL);
	SendMessage(lstRecentList, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.pszText = "IP Address\0";
	LvCol.cx = 150;
	SendMessage(lstRecentList, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "Server Name\0";
	LvCol.cx = 350;
	SendMessage(lstRecentList, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Location\0";
	LvCol.cx = 245;
	SendMessage(lstRecentList, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	SendMessage(lstRecentList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Create Serverlist
	lstServerListK = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 10, 30, 770, 440, form1, NULL, hInstance, NULL);
	SendMessage(lstServerListK, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.pszText = "Server Name\0";
	LvCol.cx = 250;
	SendMessage(lstServerListK, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "IP Address\0";
	LvCol.cx = 125;
	SendMessage(lstServerListK, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Ping\0";
	LvCol.cx = 75;
	SendMessage(lstServerListK, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	LvCol.pszText = "Location\0";
	LvCol.cx = 100;
	SendMessage(lstServerListK, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	LvCol.pszText = "Users\0";
	LvCol.cx = 65;
	SendMessage(lstServerListK, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol);
	LvCol.pszText = "Games\0";
	LvCol.cx = 65;
	SendMessage(lstServerListK, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol);
	LvCol.pszText = "Version\0";
	LvCol.cx = 65;
	SendMessage(lstServerListK, LVM_INSERTCOLUMN, 6, (LPARAM)&LvCol);
	SendMessage(lstServerListK, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Create Serverlist
	lstServerList3D = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 10, 30, 770, 440, form1, NULL, hInstance, NULL);
	SendMessage(lstServerList3D, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.pszText = "Server Name\0";
	LvCol.cx = 250;
	SendMessage(lstServerList3D, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "IP Address\0";
	LvCol.cx = 125;
	SendMessage(lstServerList3D, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Ping\0";
	LvCol.cx = 75;
	SendMessage(lstServerList3D, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	LvCol.pszText = "Location\0";
	LvCol.cx = 100;
	SendMessage(lstServerList3D, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	LvCol.pszText = "Users\0";
	LvCol.cx = 65;
	SendMessage(lstServerList3D, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol);
	LvCol.pszText = "Games\0";
	LvCol.cx = 65;
	SendMessage(lstServerList3D, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol);
	LvCol.pszText = "Version\0";
	LvCol.cx = 65;
	SendMessage(lstServerList3D, LVM_INSERTCOLUMN, 6, (LPARAM)&LvCol);
	SendMessage(lstServerList3D, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Login Info Frame
	tTab = CreateWindowEx(controlStyles, "SysTabControl32", NULL, tabProperties, 5, 478, 780, 85, form1, NULL, hInstance, NULL);
	SendMessage(tTab, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	v.mask = TCIF_TEXT | TCIF_IMAGE;
	v.iImage = -1;
	v.pszText = "Login Info";
	TabCtrl_InsertItem(tTab, 0, &v);
	v.pszText = "Chatroom Options";
	TabCtrl_InsertItem(tTab, 1, &v);
	v.pszText = "Gameroom Options";
	TabCtrl_InsertItem(tTab, 2, &v);
	v.pszText = "Extended EmulinkerSF v0.90.0+ Gameroom Options";
	TabCtrl_InsertItem(tTab, 3, &v);
	//v.pszText = "P2P Options";
	//TabCtrl_InsertItem(tTab, 4, &v);
	//Login Servers
	sTab = CreateWindowEx(controlStyles, "SysTabControl32", NULL, tabProperties, 5, 5, 780, 470, form1, NULL, hInstance, NULL);
	SendMessage(sTab, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	v.mask = TCIF_TEXT | TCIF_IMAGE;
	v.iImage = -1;
	v.pszText = "Kaillera Server List";
	TabCtrl_InsertItem(sTab, 0, &v);
	v.pszText = "EmuLinker Server List";
	TabCtrl_InsertItem(sTab, 1, &v);
	v.pszText = "Recent List";
	TabCtrl_InsertItem(sTab, 2, &v);
	v.pszText = "Favorites List";
	TabCtrl_InsertItem(sTab, 3, &v);
	v.pszText = "EmuLinker Waiting Games List";
	TabCtrl_InsertItem(sTab, 4, &v);


	if (atoi(maxUsersG) < 1 || atoi(maxUsersG) > 100)
		wsprintf(maxUsersG, "%i", 16);
	txtMaxUsers = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", maxUsersG, textboxProperties, 10, 520, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtMaxUsers, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	lblMaxUsers = CreateWindowEx(controlStyles, "STATIC", "Max Users:", labelProperties, 10, 505, 60, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblMaxUsers, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	if (atoi(maxPingG) < 1 || atoi(maxPingG) > 1000)
		wsprintf(maxPingG, "%i", 200);
	txtMaxPing = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", maxPingG, textboxProperties, 75, 520, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtMaxPing, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	lblMaxPing = CreateWindowEx(controlStyles, "STATIC", "Max Ping:", labelProperties, 75, 505, 60, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblMaxPing, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Use EmulinkerSF Options
	chkEmulinkerSF = CreateWindowEx(controlStyles, "BUTTON", "Use Extended Options?", checkProperties, 325, 535, 140, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkEmulinkerSF, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkEmulinkerSF, BM_SETCHECK, emulinkerSFValue, 0);
	//Fake P2P Options
	chkFakeP2P = CreateWindowEx(controlStyles, "BUTTON", "Use Fake P2P?", checkProperties, 325, 505, 100, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkFakeP2P, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkFakeP2P, BM_SETCHECK, fakeP2PValue, 0);
	//Emulator Restriction
	chkEmuRes = CreateWindowEx(controlStyles, "BUTTON", "Restrict Emulator?", checkProperties, 150, 505, 105, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkEmuRes, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkEmuRes, BM_SETCHECK, emuResValue, 0);
	//Connection Restriction
	chkConnRes = CreateWindowEx(controlStyles, "BUTTON", "Restrict Connection Type?", checkProperties, 150, 535, 145, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkConnRes, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkConnRes, BM_SETCHECK, connResValue, 0);
	//Version
	btnVersion = CreateWindowEx(controlStyles, "BUTTON", "Server Version", buttonProperties, 500, 530, 80, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnVersion, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));


	//P2P
	/*txtP2PServer = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", p2pServer, textboxProperties, 10, 520, 210,25, form1, NULL, hInstance, NULL);
	SendMessage(txtP2PServer, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	lblP2PServer = CreateWindowEx(controlStyles, "STATIC", "Server IP:", labelProperties, 10, 505, 210, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblP2PServer, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//P2P Port Textbox
	txtP2PPort = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", p2pPort, textboxProperties, 225, 520, 60,25, form1, NULL, hInstance, NULL);
	SendMessage(txtP2PPort, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	lblP2PPort = CreateWindowEx(controlStyles, "STATIC", "Port#:", labelProperties, 225, 505, 60, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblP2PPort, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Login Button
	btnP2PStart = CreateWindowEx(controlStyles, "BUTTON", "Start", buttonProperties, 500, 530, 60,25, form1, NULL, hInstance, NULL);
	SendMessage(btnP2PStart, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Connect
	btnP2PConnect = CreateWindowEx(controlStyles, "BUTTON", "Connect to Server", buttonProperties,290, 520, 100, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnP2PConnect, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Server
	btnP2PServer = CreateWindowEx(controlStyles, "BUTTON", "Start as Server", buttonProperties, 395, 520, 100, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnP2PServer, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));*/



	//Quit Textbox
	txtQuit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", quit, textboxProperties, 245, 530, 205, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtQuit, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	lblQuit = CreateWindowEx(controlStyles, "STATIC", "Quit msg:", labelProperties, 196, 535, 46, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblQuit, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Stats Label
	lblStats = CreateWindowEx(controlStyles, "STATIC", cVersion, labelProperties, 510, 510, 270, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblStats, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Server IP Textbox
	txtServerIP = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", serverIP, textboxProperties, 40, 505, 150, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtServerIP, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	lblServerIP = CreateWindowEx(controlStyles, "STATIC", "IP:", labelProperties, 23, 510, 14, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblServerIP, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Username Textbox
	txtUsername = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", username, textboxProperties, 40, 530, 150, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtUsername, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	lblUsername = CreateWindowEx(controlStyles, "STATIC", "Nick:", labelProperties, 11, 535, 26, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblUsername, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Show Chatroom
	btnChatroom = CreateWindowEx(controlStyles, "BUTTON", "Servers", buttonProperties, 590, 530, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnChatroom, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Login Button
	btnLogin = CreateWindowEx(controlStyles, "BUTTON", "Login", buttonProperties, 655, 530, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnLogin, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Logoff Button
	btnLogoff = CreateWindowEx(controlStyles, "BUTTON", "Logoff", buttonProperties, 720, 530, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnLogoff, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Connection Type Combobox
	cmbConnectionType = CreateWindow("COMBOBOX", NULL, comboboxProperties, 495, 535, 90, 500, form1, NULL, hInstance, NULL);
	SendMessage(cmbConnectionType, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(cmbConnectionType, CB_ADDSTRING, 0, (LPARAM)"LAN");
	SendMessage(cmbConnectionType, CB_ADDSTRING, 0, (LPARAM)"Excellent");
	SendMessage(cmbConnectionType, CB_ADDSTRING, 0, (LPARAM)"Good");
	SendMessage(cmbConnectionType, CB_ADDSTRING, 0, (LPARAM)"Average");
	SendMessage(cmbConnectionType, CB_ADDSTRING, 0, (LPARAM)"Low");
	SendMessage(cmbConnectionType, CB_ADDSTRING, 0, (LPARAM)"Bad");
	SendMessage(cmbConnectionType, CB_SETCURSEL, connectionType - 1, 0);
	lblConnectionType = CreateWindowEx(controlStyles, "STATIC", "Type:", labelProperties, 462, 538, 30, 15, form1, NULL, hInstance, NULL);
	SendMessage(lblConnectionType, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	//Chatroom Options
	chkJoinChat = CreateWindowEx(controlStyles, "BUTTON", "Show Join/Left Notifications?", checkProperties, 10, 505, 150, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkJoinChat, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkJoinChat, BM_SETCHECK, joinChatValue, 0);

	chkCreate = CreateWindowEx(controlStyles, "BUTTON", "Show Create/Close Notifications?", checkProperties, 10, 535, 180, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkCreate, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkCreate, BM_SETCHECK, createValue, 0);

	chkKeepChatLogs = CreateWindowEx(controlStyles, "BUTTON", "Keep Logs?", checkProperties, 350, 505, 100, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkKeepChatLogs, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkKeepChatLogs, BM_SETCHECK, chatLogValue, 0);

	chkShowError = CreateWindowEx(controlStyles, "BUTTON", "Show Rom/Emulator Errors?", checkProperties, 180, 505, 155, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkShowError, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkShowError, BM_SETCHECK, errorValue, 0);

	chkDrop = CreateWindowEx(controlStyles, "BUTTON", "Show Dropped Packet Alerts?", checkProperties, 400, 535, 165, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkDrop, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkDrop, BM_SETCHECK, dropValue, 0);

	chkJoinDbl = CreateWindowEx(controlStyles, "BUTTON", "Allow double click to join game?", checkProperties, 210, 535, 170, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkJoinDbl, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkJoinDbl, BM_SETCHECK, joinDblValue, 0);

	//Gameroom Options
	chkJoinChatGame = CreateWindowEx(controlStyles, "BUTTON", "Show Join/Left Notifications?", checkProperties, 10, 505, 170, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkJoinChatGame, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkJoinChatGame, BM_SETCHECK, joinChatGameValue, 0);


	chkBeep = CreateWindowEx(controlStyles, "BUTTON", "Beep when User Joins?", checkProperties, 10, 535, 170, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkBeep, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkBeep, BM_SETCHECK, beepValue, 0);

	chkUseScreenChat = CreateWindowEx(controlStyles, "BUTTON", "Use Screen Chat?", checkProperties, 180, 535, 150, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkUseScreenChat, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkUseScreenChat, BM_SETCHECK, useScreenChatValue, 0);

	chkKeepGameChatLogs = CreateWindowEx(controlStyles, "BUTTON", "Keep Gameroom Logs?", checkProperties, 180, 505, 140, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkKeepGameChatLogs, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkKeepGameChatLogs, BM_SETCHECK, gameChatLogValue, 0);

	chkBlink = CreateWindowEx(controlStyles, "BUTTON", "Blink when User Joins?", checkProperties, 330, 505, 140, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkBlink, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkBlink, BM_SETCHECK, blinkValue, 0);

	chkUseCache = CreateWindowEx(controlStyles, "BUTTON", "Use Cache? [Experiment for best Performance]", checkProperties, 330, 535, 260, 25, form1, NULL, hInstance, NULL);
	SendMessage(chkUseCache, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(chkUseCache, BM_SETCHECK, useCacheValue, 0);


	int i;

	//Recent
	for (i = recentCount; i >= 0; i--) {
		if (/*strlen(recentServers[i].server) > 0 && */strlen(recentServers[i].ip) > 0/* && strlen(recentServers[i].location) > 0*/)
			recentlistAdditem(recentServers[i].server, recentServers[i].ip, recentServers[i].location);
	}

	//Favorites
	for (i = favoriteCount; i >= 0; i--) {
		if (/*strlen(favoriteServers[i].server) > 0 && */strlen(favoriteServers[i].ip) > 0/* && strlen(favoriteServers[i].location) > 0*/)
			favoritelistAdditem(favoriteServers[i].server, favoriteServers[i].ip, favoriteServers[i].location, favoriteServers[i].comments);
	}

	showOptions(1);
	displayAndAutoScrollRichEdit(txtChatroom, initText, RGB(205, 133, 0));

	if (showChatroomFirst == true)
		showChatroom(true);
	else
		showChatroom(false);

	EditProcTxtMaxUsers = (WNDPROC)GetWindowLongPtr(txtMaxUsers, GWLP_WNDPROC);
	SetWindowLongPtr(txtMaxUsers, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtMaxUsers);

	EditProcTxtMaxPing = (WNDPROC)GetWindowLongPtr(txtMaxPing, GWLP_WNDPROC);
	SetWindowLongPtr(txtMaxPing, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtMaxPing);

	EditProcTxtIP = (WNDPROC)GetWindowLongPtr(txtServerIP, GWLP_WNDPROC);
	SetWindowLongPtr(txtServerIP, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtIP);

	EditProcTxtNick = (WNDPROC)GetWindowLongPtr(txtUsername, GWLP_WNDPROC);
	SetWindowLongPtr(txtUsername, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtNick);

	EditProcTxtQuit = (WNDPROC)GetWindowLongPtr(txtQuit, GWLP_WNDPROC);
	SetWindowLongPtr(txtQuit, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtQuit);

	EditProcTxtChatroom = (WNDPROC)GetWindowLongPtr(txtChatroom, GWLP_WNDPROC);
	SetWindowLongPtr(txtChatroom, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtChatroom);
}



void popupAway(){
	int i;
	RECT b;

	//Get Window Position
	GetWindowRect(form1, &b);


	//Make List
	awayMenu = CreatePopupMenu();
	AppendMenu(awayMenu, MF_STRING | MF_POPUP, 0xFFFF, "Create/Remove Away Messages");
	AppendMenu(awayMenu, MF_SEPARATOR | MF_POPUP, NULL, NULL);
	
	strcpy(awayMessages[0].subject, "Default Away Message");
	strcpy(awayMessages[0].message, "I'm currently away from my computer.");
	awayMessages[0].save = true;
	for(i = 0; i <= awayMessageCount; i++){
		if(awayMessages[i].save == true)
			AppendMenu(awayMenu, MF_STRING | MF_POPUP, i + 1, awayMessages[i].subject);
	}

	//Popup Menu at Appropriate Location
	i = TrackPopupMenu(awayMenu, TPM_RETURNCMD, b.left + 725, b.top + 330, 0, form1, NULL);

	//Error Check
	if(i == 0)
		return;
	//Create Away Message
	else if(i == 0xFFFF){
		EnableWindow(form1, FALSE);
		createAwayWindow();
		RedrawWindow(form1, 0 , NULL, RDW_INVALIDATE);
	}
	else{
		imAway = true;
		awayValue = i - 1;
		showAway();
	}
}

void showAway(){
	char temp[2048];

	if(imAway == true){
		SetWindowText(btnAway, "I'm Back");
		strcpy(temp, "[");
		strcat(temp, awayMessages[awayValue].subject);
		strcat(temp, "] ");
		strcat(temp, awayMessages[awayValue].message);
		SetWindowText(txtGameChat, temp);
		kailleraChatSend("Status: Away");
	}
	else{
		SetWindowText(btnAway, "Away");
		kailleraChatSend("Status: Returned");
		SetWindowText(txtGameChat, "");
	}
}

void updateAway(){
	if(awayMessageCount == (AWAY_MESSAGE_MAX - 1)){
		MessageBox(frmAway, "You have reached the maximum# of away messages allowed!", "Max Away Messages!", NULL);
		return;
	}

	awayMessageCount = awayMessageCount + 1;

	SendMessage(txtAwayMessageSubject, WM_GETTEXT, 1024, (LPARAM) awayMessages[awayMessageCount].subject);
	SendMessage(txtAwayMessage, WM_GETTEXT, 1024, (LPARAM) awayMessages[awayMessageCount].message);
	
	awayAdditem(awayMessages[awayMessageCount].subject, awayMessages[awayMessageCount].message, awayMessageCount);
	awayMessages[awayMessageCount].save = true;
	//Clear Chat Box
	SendMessage(txtAwayMessage, WM_SETTEXT, 0, (LPARAM) "");
	SendMessage(txtAwayMessageSubject, WM_SETTEXT, 0, (LPARAM) "");
}

void awayAdditem(char subject[], char message[], int value){
	LVITEM LvItem;
	char temp[1024];

	memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem.mask = LVIF_TEXT;   // Text Style
	LvItem.cchTextMax = 1024; // Max size of test
	LvItem.iItem = 0;          // choose item  
	LvItem.iSubItem = 0;       // Put in first coluom
	LvItem.pszText = subject; // Text to display (can be from a char variable) (Items)
	SendMessage(lstAway,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview

	LvItem.iSubItem = 1;
	LvItem.pszText = message;
	SendMessage(lstAway,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 2;
	wsprintf(temp, "%i", value);
	LvItem.pszText = temp;
	SendMessage(lstAway,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
}

void createEditFavoritesWindow(){
	RECT b;
	LVITEM q;
	int iSelect;
	char strIP [1024];
	char strServer [1024];
	char strLocation [1024];
	char strComments [1024];

	iSelect = SendMessage(lstFavoriteList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
	if(iSelect < 0)
		return;

	//IP
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 0;
	q.pszText = strIP;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
	//Server
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 1;
	q.pszText = strServer;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
	//Location
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 2;
	q.pszText = strLocation;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
	//Comments
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 3;
	q.pszText = strComments;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);


	//Get Window Position
	GetWindowRect(form1, &b);

	frmEditFavorites = CreateWindow("Supraclient", "Edit Favorite Entry", formOtherProperties, b.left + 110, b.top + 200, 330, 150, form1, NULL, hInstance, NULL);
	//IP
	txtEIP = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", strIP, textboxProperties, 5, 10, 250, 25, frmEditFavorites, NULL, hInstance, NULL);
	SendMessage(txtEIP, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Name
	txtEServer = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", strServer, textboxProperties, 5, 35, 250, 25, frmEditFavorites, NULL, hInstance, NULL);
	SendMessage(txtEServer, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Location
	txtELocation = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", strLocation, textboxProperties, 5, 60, 250, 25, frmEditFavorites, NULL, hInstance, NULL);
	SendMessage(txtELocation, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Comments
	txtEComments = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", strComments, textboxProperties, 5, 85, 250, 25, frmEditFavorites, NULL, hInstance, NULL);
	SendMessage(txtEComments, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	//Create New
	btnEOkay = CreateWindowEx(controlStyles, "BUTTON", "Okay", buttonProperties, 260, 10, 60, 25, frmEditFavorites, NULL, hInstance, NULL);
	SendMessage(btnEOkay, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	editFavorite = false;
	while(editFavorite == false){
		GetWindowText(txtEIP, strIP, 1024);
		GetWindowText(txtEServer, strServer, 1024);
		GetWindowText(txtELocation, strLocation, 1024);
		GetWindowText(txtEComments, strComments, 1024);
		
		SwitchToThread();
		DoEvents();
		Sleep(1);
	}

	//IP
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 0;
	q.pszText = strIP;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_SETITEMTEXT, (WPARAM)iSelect, (LPARAM)&q);
	//Server
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 1;
	q.pszText = strServer;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_SETITEMTEXT, (WPARAM)iSelect, (LPARAM)&q);
	//Location
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 2;
	q.pszText = strLocation;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_SETITEMTEXT, (WPARAM)iSelect, (LPARAM)&q);
	//Comments
	q.mask = LVIF_TEXT;
	q.iItem = iSelect;
	q.iSubItem = 3;
	q.pszText = strComments;
	q.cchTextMax = 1024;
	SendMessage(lstFavoriteList, LVM_SETITEMTEXT, (WPARAM)iSelect, (LPARAM)&q);
	
	DestroyWindow(frmEditFavorites);
}


void createCommentsWindow(){
	RECT b;

	//Get Window Position
	GetWindowRect(form1, &b);

	frmComments = CreateWindow("Supraclient", "Any Comments?", formOtherProperties, b.left + 110, b.top + 200, 330, 80, form1, NULL, hInstance, NULL);
	//Subject
	txtComments = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, textboxProperties, 5, 10, 250, 25, frmComments, NULL, hInstance, NULL);
	SendMessage(txtComments, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Create New
	btnComments = CreateWindowEx(controlStyles, "BUTTON", "Okay", buttonProperties, 260, 10, 60, 25, frmComments, NULL, hInstance, NULL);
	SendMessage(btnComments, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	cComments = false;
	strcpy(fComments,"NA");
	while(cComments == false){
		SwitchToThread();
		DoEvents();
		Sleep(1);
	}
}


void createPMWindow(){
	RECT b;

	//Get Window Position
	GetWindowRect(form1, &b);

	frmPM = CreateWindow("Supraclient", "Enter a message:", formOtherProperties, b.left + 110, b.top + 200, 330, 80, form1, NULL, hInstance, NULL);
	//Subject
	txtPM = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, textboxProperties, 5, 10, 250, 25, frmPM, NULL, hInstance, NULL);
	SendMessage(txtPM, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Create New
	btnPM = CreateWindowEx(controlStyles, "BUTTON", "Okay", buttonProperties, 260, 10, 60, 25, frmPM, NULL, hInstance, NULL);
	SendMessage(btnPM, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	cPM = false;
	strcpy(fPM,"");

	while(cPM == false){
		SwitchToThread();
		DoEvents();
		Sleep(1);
	}
}


void createAwayWindow(){
	int i;
	RECT b;
	LVCOLUMN LvCol;

	//Get Window Position
	GetWindowRect(form1, &b);

	frmAway = CreateWindow("Supraclient", "Create Away Message", formOtherProperties, b.left + 110, b.top + 80, 440, 340, form1, NULL, hInstance, NULL);
	//Subject
	txtAwayMessageSubject = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Enter Subject", textboxProperties, 5, 10, 250, 25, frmAway, NULL, hInstance, NULL);
	SendMessage(txtAwayMessageSubject, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Message
	txtAwayMessage = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Enter Message", textboxProperties, 5, 40, 250, 25, frmAway, NULL, hInstance, NULL);
	SendMessage(txtAwayMessage, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Create New
	btnCreateAway = CreateWindowEx(controlStyles, "BUTTON", "Create", buttonProperties, 260, 40, 60, 25, frmAway, NULL, hInstance, NULL);
	SendMessage(btnCreateAway, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Remove
	btnRemoveAway = CreateWindowEx(controlStyles, "BUTTON", "Remove", buttonProperties, 325, 40, 60, 25, frmAway, NULL, hInstance, NULL);
	SendMessage(btnRemoveAway, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	//Create Away List
	lstAway = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 5, 80, 420, 220, frmAway, NULL, hInstance, NULL);
	SendMessage(lstAway, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol,0,sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	LvCol.pszText="Subject\0";
	LvCol.cx=150;
	SendMessage(lstAway,LVM_INSERTCOLUMN,0,(LPARAM)&LvCol);
	LvCol.pszText="Message\0";
	LvCol.cx=245;
	SendMessage(lstAway,LVM_INSERTCOLUMN,1,(LPARAM)&LvCol);
	SendMessage(lstAway, LVM_SETEXTENDEDLISTVIEWSTYLE, 0 , (LPARAM) listviewStyles);
	LvCol.pszText = "#\0";
	LvCol.cx=0;
	SendMessage(lstAway,LVM_INSERTCOLUMN,2,(LPARAM)&LvCol);
	SendMessage(lstAway, LVM_SETEXTENDEDLISTVIEWSTYLE, 0 , (LPARAM) listviewStyles);
	
	for(i = 1; i <= awayMessageCount; i++){
		if(awayMessages[i].save == true)
			awayAdditem(awayMessages[i].subject, awayMessages[i].message, i);
	}
}


void parseWaitingGames(){
	char game[1024];
	char emulator[1024];
	char username[1024];
	char server[1024];
	char ip[1024];
	char location[1024];
	char numPlayers[1024];
	char strWaiting[1024];
	int numWaiting;
	long i;
	int q;
	int w;
	char* s;
	long total;

	if(waiting == false)
		return;

	total = bytesRecvWaiting;

	//Contains Header for HTTP Request.
	s = strstr(myBuffWaiting, "\r\n\r\n");

	//Doesn't Contain Header for HTTP Request 
	if(s == NULL){
		s = myBuffWaiting;
	}
	else{
		q = ((long)s + 4) - (long)myBuffWaiting;
		s = &myBuffWaiting[q];
		total = total - q;
	}

	//Find First NULL.  Anything before this is the Waiting Games List
	i = 0;
	while(s[i] != '\n' && i < total){
		i = i + 1;
		waiting = false;
	}
	total = i;


	//Parse
	for(i = 0; i < total; i++){
		//Game
		w = 0;
		while(s[i] != '|' && i < total){
			game[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		game[w] = '\0';

		//IP Address
		w = 0;
		while(s[i] != '|' && i < total){
			ip[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		ip[w] = '\0';

		//Username
		w = 0;
		while(s[i] != '|' && i < total){
			username[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		username[w] = '\0';
		
		//Emulator
		w = 0;
		while(s[i] != '|' && i < total){
			emulator[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		emulator[w] = '\0';

		//# of Players
		w = 0;
		while(s[i] != '|' && i < total){
			numPlayers[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		numPlayers[w] = '\0';

		//Server
		w = 0;
		while(s[i] != '|' && i < total){
			server[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		server[w] = '\0';

		//Location
		w = 0;
		while(s[i] != '|' && i < total){
			location[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		location[w] = '\0';

		waitinglistAdditem(game, emulator, username, server, ip, location, numPlayers);
	}

	numWaiting = SendMessage(lstWaitingList, LVM_GETITEMCOUNT, 0, 0);
	wsprintf(strWaiting, "%i Waiting Games Total", numWaiting);
	SetWindowText(form1, strWaiting);
}

void parseServerList3D(){
	char serverName[1024];
	char ipAddress[1024];
	char location[1024];
	char users[1024];
	char games[1024];
	char version[1024];
	char strServers[1024];
	int numOfServers;
	int i;
	int q;
	int w;
	char* s;
	long total;

	
	total = bytesRecvServerList3D;

	//Contains Header for HTTP Request.
	s = strstr(myBuffServerList3D, "\r\n\r\n");

	//Doesn't Contain Header for HTTP Request 
	if(s == NULL){
		s = myBuffServerList3D;
	}
	else{
		q = ((long)s + 4) - (long)myBuffServerList3D;
		s = &myBuffServerList3D[q];
		total = total - q;
	}


	//Find First NULL.  Anything before this is the Waiting Games List
	i = 0;
	//while(s[i] != '\n' && i < total){
	//	i = i + 1;
	//}
	//i = i + 1;

	//Parse Server List
	for(i = i; i < total; i++){
		//Server Name
		w = 0;
		while(s[i] != '\n' && i < total){
			serverName[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		serverName[w] = '\0';

		//IP Address
		w = 0;
		while(s[i] != ';' && i < total){
			ipAddress[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		ipAddress[w] = '\0';
		
		//Users
		w = 0;
		while(s[i] != ';' && i < total){
			users[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		users[w] = '\0';

		//Games
		w = 0;
		while(s[i] != ';' && i < total){
			games[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		games[w] = '\0';

		//Version
		w = 0;
		while(s[i] != ';' && i < total){
			version[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		version[w] = '\0';

		//Location
		w = 0;
		while(s[i] != '\n' && i < total){
			location[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		location[w] = '\0';
		

		Serverlist3DAdditem(serverName, ipAddress, "NA", location, users, games, version);
	}

	numOfServers = SendMessage(lstServerList3D, LVM_GETITEMCOUNT, 0, 0);
	wsprintf(strServers, "%i EmuLinker Servers Total", numOfServers);
	SetWindowText(form1, strServers);

}

void parseServerListK(){
	char serverName[1024];
	char ipAddress[1024];
	char location[1024];
	char users[1024];
	char games[1024];
	char version[1024];
	char strServers[1024];
	int numOfServers;
	int i;
	int q;
	int w;
	char* s;
	long total;

	
	total = bytesRecvServerListK;

	//Contains Header for HTTP Request.
	s = strstr(myBuffServerListK, "\r\n\r\n");

	//Doesn't Contain Header for HTTP Request 
	if(s == NULL){
		s = myBuffServerListK;
	}
	else{
		q = ((long)s + 4) - (long)myBuffServerListK;
		s = &myBuffServerListK[q];
		total = total - q;
	}


	//Find First NULL.  Anything before this is the Waiting Games List
	i = 0;
	while(s[i] != '\n' && i < total){
		i = i + 1;
	}
	i = i + 1;

	//Parse Server List
	for(i = i; i < total; i++){
		//Server Name
		w = 0;
		while(s[i] != '\n' && i < total){
			serverName[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		serverName[w] = '\0';

		//IP Address
		w = 0;
		while(s[i] != ';' && i < total){
			ipAddress[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		ipAddress[w] = '\0';
		
		//Users
		w = 0;
		while(s[i] != ';' && i < total){
			users[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		users[w] = '\0';

		//Games
		w = 0;
		while(s[i] != ';' && i < total){
			games[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		games[w] = '\0';

		//Version
		w = 0;
		while(s[i] != ';' && i < total){
			version[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		i = i + 1;
		version[w] = '\0';

		//Location
		w = 0;
		while(s[i] != '\n' && i < total){
			location[w] = s[i];
			w = w + 1;
			i = i + 1;
		}
		location[w] = '\0';
		
		//skips spam servers
		if (strstr(serverName, "kofip") || 
			strcmp(version, "0.97") == 0 || 
			strcmp(version, "0.98") == 0 || 
			strcmp(version, "0.99") == 0 || 
			strcmp(version, "1.00") == 0 || 
			strcmp(version, "1.01") == 0) continue;
		kServerlistAdditem(serverName, ipAddress, "NA", location, users, games, version);
	}

	numOfServers = SendMessage(lstServerListK, LVM_GETITEMCOUNT, 0, 0);
	wsprintf(strServers, "%i Kaillera Servers Total", numOfServers);
	SetWindowText(form1, strServers);
}


DWORD WINAPI pingKailleraServers(LPVOID lpParam){
	unsigned short port;
	int buff;
	int i;
	char str[1024];
	char strPing [1024];
	char strIP[1024];
	char strPort[1024];
	char strAddress[1024];
	int total;
	char myBuff[1024];
	LVITEM b;
	hostent* hp;

	total = SendMessage(lstServerListK, LVM_GETITEMCOUNT, 0, 0);
	
	i = SendMessage(lstServerListK, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
	if(i < 0)
		i = 0;
	pingingK = true;
	for(i = i; i < total; i++){
		bool sError = false;
		
		if(lastTabServer == 0 && pinging3D == true){
			sprintf(str, "Pinging Kaillera Server %i of %i", i + 1, total);
			SetWindowText(form1, str);
		}
		else if(pinging3D == false){
			sprintf(str, "Pinging Kaillera Server %i of %i", i + 1, total);
			SetWindowText(form1, str);
		}

		//Get Address
		b.mask = LVIF_TEXT;
		b.iItem = i;
		b.iSubItem = 1;
		b.pszText = strAddress;
		b.cchTextMax = 1024;
		ZeroMemory(strAddress, 1024);
		SendMessage(lstServerListK, LVM_GETITEM, 0, (LPARAM) &b);

		//Length of ServerIP:Port
		short lenAddress = strlen(strAddress);

		char* c = strstr(strAddress, ":");

		if(c != NULL){
			//Split Port
			strcpy(strPort, c + 1);
			//Split Address and Port
			ZeroMemory(strIP, 1024);
			strncpy(strIP, strAddress, (lenAddress - strlen(strPort) - 1));
			port = (u_short)atoi(strPort);
		}
		else{
			ZeroMemory(strIP, 1024);
			strcpy(strIP, strAddress);
			port = 27888;
		}

		closesocket(mySocketK);
		mySocketK = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		socketInfoK.sin_family = AF_INET;
		socketInfoK.sin_port = htons(port);

		if(strlen(strAddress) < 1){
			closesocket(mySocketK);
			b.iItem = i;
			b.iSubItem = 2;
			b.pszText = "ERR";
			SendMessage(lstServerListK, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
			sError = true;
		}
		
		if(sError == false){
			if(inet_addr(strIP) == INADDR_NONE){
				hp = gethostbyname(strIP);
				if(hp == NULL){
					closesocket(mySocketK);
					b.iItem = i;
					b.iSubItem = 2;
					b.pszText = "ERR";
					SendMessage(lstServerListK, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
					sError = true;
				} else socketInfoK.sin_addr = *(struct in_addr*)(hp->h_addr_list[0]);
			}
			else{
				socketInfoK.sin_addr.s_addr = inet_addr(strIP);
			}
		}
		
		if(sError == false){
			int iMode = 1;
			bool here = false;

			ioctlsocket(mySocketK, FIONBIO, (u_long FAR*) &iMode);
			sendto(mySocketK, "PING\0",  5, NULL, (sockaddr *) &socketInfoK, sizeof(socketInfoK));

			//Check the socket
			int timeout = GetTickCount();
			buff = 0;
			while(buff < 1 && GetTickCount() - timeout <= 1000){
				buff = recv(mySocketK, myBuff, sizeof(myBuff), NULL);
				if(buff > 1){
					if(strncmp(myBuff, "PONG", 4) == 0){
						sprintf(strPing, "%ims",GetTickCount() - timeout);
						b.iItem = i;
						b.iSubItem = 2;
						b.pszText = strPing;
						SendMessage(lstServerListK, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
						here = true;
						break;
					}
				}
				Sleep(1);
			}
			if(here == false){
				sprintf(strPing, "%i",GetTickCount() - timeout);
				b.iItem = i;
				b.iSubItem = 2;
				b.pszText = ">1s";
				SendMessage(lstServerListK, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
			}
		}
	}

	kailleraSwitch = false;
	lstServerlistKColumn = 2;
	ListView_SortItemsEx(lstServerListK, lstServerlistKCompareFunc, 0);
	lstServerlistKColumn = 4;
	kailleraSwitch = true;

	SetWindowText(form1, "Kaillera Pinging Finished!");
	exitPingKThread();
	closesocket(mySocketK);
	return 0;
}

DWORD WINAPI ping3DServers(LPVOID lpParam){
	unsigned short port;
	int buff;
	int i;
	char str[1024];
	char strPing [1024];
	char strIP[1024];
	char strPort[1024];
	char strAddress[1024];
	int total;
	char myBuff[1024];
	LVITEM b;
	hostent* hp;

	total = SendMessage(lstServerList3D, LVM_GETITEMCOUNT, 0, 0);
	
	i = SendMessage(lstServerList3D, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
	if(i < 0)
		i =0;
	pinging3D = true;

	for(i = i; i < total; i++){
		bool sError = false;
		
		if(lastTabServer == 1 && pingingK == true){
			sprintf(str, "Pinging EmuLinker Server %i of %i", i + 1, total);
			SetWindowText(form1, str);
		}
		else if(pingingK == false){
			sprintf(str, "Pinging EmuLinker Server %i of %i", i + 1, total);
			SetWindowText(form1, str);
		}

		//Get Address
		b.mask = LVIF_TEXT;
		b.iItem = i;
		b.iSubItem = 1;
		b.pszText = strAddress;
		b.cchTextMax = 1024;
		ZeroMemory(strAddress, 1024);
		SendMessage(lstServerList3D, LVM_GETITEM, 0, (LPARAM) &b);

		//Length of ServerIP:Port
		short lenAddress = strlen(strAddress);

		char* c = strstr(strAddress, ":");

		if(c != NULL){
			//Split Port
			strcpy(strPort, c + 1);
			//Split Address and Port
			ZeroMemory(strIP, 1024);
			strncpy(strIP, strAddress, (lenAddress - strlen(strPort) - 1));
			port = (u_short)atoi(strPort);
		}
		else{
			ZeroMemory(strIP, 1024);
			strcpy(strIP, strAddress);
			port = 27888;
		}

		closesocket(mySocket3D);
		mySocket3D = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		socketInfo3D.sin_family = AF_INET;
		socketInfo3D.sin_port = htons(port);

		if(strlen(strAddress) < 1){
			closesocket(mySocket3D);
			b.iItem = i;
			b.iSubItem = 2;
			b.pszText = "ERR";
			SendMessage(lstServerList3D, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
			sError = true;
		}
		
		if(sError == false){		
			if(inet_addr(strIP) == INADDR_NONE){
				hp = gethostbyname(strIP);
				if(hp == NULL){
					closesocket(mySocket3D);
					b.iItem = i;
					b.iSubItem = 2;
					b.pszText = "ERR";
					SendMessage(lstServerList3D, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
					sError = true;
				} else socketInfoK.sin_addr = *(struct in_addr*)(hp->h_addr_list[0]);
			}
			else{
				socketInfo3D.sin_addr.s_addr = inet_addr(strIP);
			}
		}
		
		if(sError == false){
			int iMode = 1;
			bool here = false;

			ioctlsocket(mySocket3D, FIONBIO, (u_long FAR*) &iMode);
			sendto(mySocket3D, "PING\0",  5, NULL, (sockaddr *) &socketInfo3D, sizeof(socketInfo3D));

			//Check the socket
			int timeout = GetTickCount();
			buff = 0;
			while(buff < 1 && GetTickCount() - timeout <= 1000){
				buff = recv(mySocket3D, myBuff, sizeof(myBuff), NULL);
				if(buff > 1){
					if(strncmp(myBuff, "PONG", 4) == 0){
						sprintf(strPing, "%ims",GetTickCount() - timeout);
						b.iItem = i;
						b.iSubItem = 2;
						b.pszText = strPing;
						SendMessage(lstServerList3D, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
						here = true;
						break;
					}
				}
				Sleep(1);
			}
			if(here == false){
				sprintf(strPing, "%i",GetTickCount() - timeout);
				b.iItem = i;
				b.iSubItem = 2;
				b.pszText = ">1s";
				SendMessage(lstServerList3D, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&b);
			}
		}
	}

	anti3DSwitch = false;
	lstServerlist3DColumn = 2;
	ListView_SortItemsEx(lstServerList3D, lstServerlist3DCompareFunc, 0);
	lstServerlist3DColumn = 4;
	anti3DSwitch = true;

	SetWindowText(form1, "EmuLinker Pinging Finished!");
	exitPing3DThread();
	closesocket(mySocket3D);
	return 0;
}




void gameInit(){
    unsigned long i;
	int w;
	char t[1024];
	char temp[2048];

    stage = 1;
    gameCount = 60;
    frameRecv = 0;
	frameSend = 0;
	
	returnInputSize = false;
    ePos = 0;
    totalInput = 0;
    lastPosFrameRecv = 0;
    lastPosFrameSend = 3;
    gamePlaying = false;
    sizeOfEinput = inputSize * totalPlayers;
    inputFrame = connectionType * (frameDelay + 1);

	frameCount = 0;
    sizeOfVinput = connectionType * inputSize;
	returnFrameSize = inputSize * totalPlayers;
    outCachePos = 0;
    inCachePos = 0;

	eSize = sizeOfEinput * connectionType * MAX_INCOMING_BUFFER;

	if(SendMessage(chkUseCache, BM_GETCHECK, 0, 0) == BST_CHECKED)
		useCache = true;
	else
		useCache = false;

	//Tweak for Spectating Mode
	if(totalPlayers == 0)
		return;

	EnableWindow(chkUseCache, FALSE);

    //Game Loaded. Ready to Start.
    readyToPlayRequest();
	wsprintf(t, "%i", gameCount);
	strcpy(temp,"Waiting...Press ESC to cancel! ");
	strcat(temp, t);
	strcat(temp, "s");
	displayGameChatroomAsServer(temp);

	//Bring New Window on Top
	HWND gameHandle;

	gameHandle = GetWindow(mainHwnd, GW_HWNDPREV);
	if(gameHandle != NULL && gameHandle != mainHwnd)
		SetWindowPos(gameHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(mainHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


    i = GetTickCount();
	w = 0;
    while(gamePlaying == false){
        if(GetAsyncKeyState(27) == -32767)
			sizeOfEinput = -1;
        if(gameCount == 0 || sizeOfEinput == -1){            
			MessageBox(form1, "Never received ready signal. Game Ended!", "gameInit()", NULL);
			sizeOfEinput = -1;
            return;
        }
        if(GetTickCount() - i >= 1000){
			w = w + 1;
            i = GetTickCount();
            gameCount--;
			wsprintf(t, "%i", gameCount);
			strcpy(temp,"Waiting...Press ESC to cancel! ");
			strcat(temp, t);
			strcat(temp, "s");
			displayGameChatroomAsServer(temp);
		}
		//If game didnt' start in 5s, try sending again...
		if(w == 5){
			w = 0;
			readyToPlayRequest();
		}

		Sleep(1);
    }
}


void createChatroom(){
	LVCOLUMN LvCol;

	//Create Form
	//Create Server List Frame
	//Create Main Chatroom
	txtChatroom = CreateWindowEx(WS_EX_CLIENTEDGE, "RichEdit", NULL, richTextboxProperties, 5, 5, 480, 260, form1, NULL, hInstance, NULL);
	SendMessage(txtChatroom, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(txtChatroom, EM_SHOWSCROLLBAR, SB_VERT, true);
	SendMessage(txtChatroom, EM_SETREADONLY, true, 0);
	WORD mask1 = SendMessage(txtChatroom, EM_GETEVENTMASK, 0, 0);
	SendMessage(txtChatroom, EM_SETEVENTMASK, 0, mask1 | ENM_LINK);
	SendMessage(txtChatroom, EM_AUTOURLDETECT, (WPARAM)true, 0);//URL detect

	//Create Game Chatroom
	txtGameChatroom = CreateWindowEx(WS_EX_CLIENTEDGE, "RichEdit", "<New Session>\r\n", richTextboxProperties, 5, 330, 480, 115, form1, NULL, hInstance, NULL);
	SendMessage(txtGameChatroom, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	SendMessage(txtGameChatroom, EM_SHOWSCROLLBAR, SB_VERT, true);
	SendMessage(txtGameChatroom, EM_SETREADONLY, true, 0);
	WORD mask2 = SendMessage(txtGameChatroom, EM_GETEVENTMASK, 0, 0);
	SendMessage(txtGameChatroom, EM_SETEVENTMASK, 0, mask2 | ENM_LINK);
	SendMessage(txtGameChatroom, EM_AUTOURLDETECT, (WPARAM)true, 0);//URL detect
	//Game Chat Textbox
	txtGameChat = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, textboxProperties, 5, 450, 350, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtGameChat, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Game Chat Button
	btnGameChat = CreateWindowEx(controlStyles, "BUTTON", "Chat", buttonProperties, 360, 450, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnGameChat, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Lagstat Button
	btnLagStat = CreateWindowEx(controlStyles, "BUTTON", "Lag Stat", buttonProperties, 425, 450, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnLagStat, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	//Game Start Button
	btnGameStart = CreateWindowEx(controlStyles, "BUTTON", "Start", buttonProperties, 490, 300, 50, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnGameStart, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Game Kick Button
	btnGameKick = CreateWindowEx(controlStyles, "BUTTON", "Kick", buttonProperties, 550, 300, 50, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnGameKick, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Game Drop Button
	btnDrop = CreateWindowEx(controlStyles, "BUTTON", "Drop", buttonProperties, 610, 300, 50, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnDrop, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Game Away Button
	btnAway = CreateWindowEx(controlStyles, "BUTTON", "Away", buttonProperties, 670, 300, 50, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnAway, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Game Leave Button
	btnGameLeave = CreateWindowEx(controlStyles, "BUTTON", "Leave", buttonProperties, 730, 300, 50, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnGameLeave, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));

	//Game Name Textbox
	txtGame = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Not in Game!", buttonProperties | ES_READONLY | WS_BORDER, 5, 300, 480, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtGame, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Create GameUserlist
	lstGameUserlist = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 490, 330, 295, 145, form1, NULL, hInstance, NULL);
	SendMessage(lstGameUserlist, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.pszText = "UserID\0";
	LvCol.cx = 45;
	SendMessage(lstGameUserlist, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "Nick\0";
	LvCol.cx = 135;
	SendMessage(lstGameUserlist, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Ping\0";
	LvCol.cx = 35;
	SendMessage(lstGameUserlist, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	LvCol.pszText = "Type\0";
	LvCol.cx = 59;
	SendMessage(lstGameUserlist, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	SendMessage(lstGameUserlist, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Create Userlist
	lstUserlist = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 490, 5, 295, 290, form1, NULL, hInstance, NULL);
	SendMessage(lstUserlist, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.pszText = "UserID\0";
	LvCol.cx = 45;
	SendMessage(lstUserlist, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "Nick\0";
	LvCol.cx = 105;
	SendMessage(lstUserlist, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Ping\0";
	LvCol.cx = 35;
	SendMessage(lstUserlist, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	LvCol.pszText = "Type\0";
	LvCol.cx = 43;
	SendMessage(lstUserlist, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	LvCol.pszText = "Status\0";
	LvCol.cx = 46;
	SendMessage(lstUserlist, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol);
	SendMessage(lstUserlist, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Create Gamelist
	lstGamelist = CreateWindowEx(WS_EX_CLIENTEDGE, "SysListView32", NULL, listviewProperties, 5, 300, 780, 175, form1, NULL, hInstance, NULL);
	SendMessage(lstGamelist, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.pszText = "GameID\0";
	LvCol.cx = 55;
	SendMessage(lstGamelist, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
	LvCol.pszText = "Game\0";
	LvCol.cx = 290;
	SendMessage(lstGamelist, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);
	LvCol.pszText = "Emulator\0";
	LvCol.cx = 204;
	SendMessage(lstGamelist, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);
	LvCol.pszText = "Owner\0";
	LvCol.cx = 105;
	SendMessage(lstGamelist, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	LvCol.pszText = "Status\0";
	LvCol.cx = 55;
	SendMessage(lstGamelist, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol);
	LvCol.pszText = "#Users\0";
	LvCol.cx = 50;
	SendMessage(lstGamelist, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol);
	SendMessage(lstGamelist, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)listviewStyles);
	//Chat Textbox
	txtChat = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, textboxProperties, 5, 270, 285, 25, form1, NULL, hInstance, NULL);
	SendMessage(txtChat, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Chat Button
	btnChat = CreateWindowEx(controlStyles, "BUTTON", "Chat", buttonProperties, 295, 270, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnChat, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Create Button
	btnCreate = CreateWindowEx(controlStyles, "BUTTON", "Create", buttonProperties, 360, 270, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnCreate, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));
	//Join Button
	btnJoin = CreateWindowEx(controlStyles, "BUTTON", "Join", buttonProperties, 425, 270, 60, 25, form1, NULL, hInstance, NULL);
	SendMessage(btnJoin, WM_SETFONT, (WPARAM)hDefaultFont, MAKELPARAM(FALSE, 0));


	//SubClass txtChat
	EditProcTxtChat = (WNDPROC)GetWindowLongPtr(txtChat, GWLP_WNDPROC);
	SetWindowLongPtr(txtChat, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtChat);
	//SubClass txtGameChat
	EditProcTxtGameChat = (WNDPROC)GetWindowLongPtr(txtGameChat, GWLP_WNDPROC);
	SetWindowLongPtr(txtGameChat, GWLP_WNDPROC, (DWORD_PTR)SubProcTxtGameChat);
}

void fixLastGameToPlay(){
	if(strcmp(lastGameToPlay1, "LastGameToPlay1") == 0){
		strcpy(lastGameToPlay1, currentGame);
		ModifyMenu(gameMenu, (UINT)0xFFFD, MF_BYCOMMAND, (UINT)0xFFFD, (LPCSTR)lastGameToPlay1);
	}
	else if(strcmp(lastGameToPlay2, "LastGameToPlay2") == 0){
		if(strcmp(lastGameToPlay1, currentGame) == 0)
			return;
		strcpy(lastGameToPlay2, lastGameToPlay1);
		strcpy(lastGameToPlay1, currentGame);
		ModifyMenu(gameMenu, (UINT)0xFFFD, MF_BYCOMMAND, (UINT)0xFFFD, (LPCSTR)lastGameToPlay1);
		ModifyMenu(gameMenu, (UINT)0xFFFE, MF_BYCOMMAND, (UINT)0xFFFE, (LPCSTR)lastGameToPlay2);
	}
	else if(strcmp(lastGameToPlay3, "LastGameToPlay3") == 0){
		if(strcmp(lastGameToPlay1, currentGame) == 0)
			return;
		else if(strcmp(lastGameToPlay2, currentGame) == 0)
			return;
		strcpy(lastGameToPlay3, lastGameToPlay2);
		strcpy(lastGameToPlay2, lastGameToPlay1);
		strcpy(lastGameToPlay1, currentGame);
		ModifyMenu(gameMenu, (UINT)0xFFFD, MF_BYCOMMAND, (UINT)0xFFFD, (LPCSTR)lastGameToPlay1);
		ModifyMenu(gameMenu, (UINT)0xFFFE, MF_BYCOMMAND, (UINT)0xFFFE, (LPCSTR)lastGameToPlay2);
		ModifyMenu(gameMenu, (UINT)0xFFFF, MF_BYCOMMAND, (UINT)0xFFFF, (LPCSTR)lastGameToPlay3);
	}
	else{
		if(strcmp(lastGameToPlay1, currentGame) == 0)
			return;
		else if(strcmp(lastGameToPlay2, currentGame) == 0){
			strcpy(lastGameToPlay2, lastGameToPlay1);
			strcpy(lastGameToPlay1, currentGame);
		}
		else{
			strcpy(lastGameToPlay3, lastGameToPlay2);
			strcpy(lastGameToPlay2, lastGameToPlay1);
			strcpy(lastGameToPlay1, currentGame);
		}
		ModifyMenu(gameMenu, (UINT)0xFFFD, MF_BYCOMMAND, (UINT)0xFFFD, (LPCSTR)lastGameToPlay1);
		ModifyMenu(gameMenu, (UINT)0xFFFE, MF_BYCOMMAND, (UINT)0xFFFE, (LPCSTR)lastGameToPlay2);
		ModifyMenu(gameMenu, (UINT)0xFFFF, MF_BYCOMMAND, (UINT)0xFFFF, (LPCSTR)lastGameToPlay3);
	}
	DrawMenuBar(form1);
}


//Very Messy here. Good Luck!
void popupMenu(char num){
	unsigned long i;
	RECT b;	
	POINT c;
	char temp[2048];
	char ID[1024];
	char userNick[1024];
	char server[1024];
	char ip[1024];
	char location[1024];
	int iSelect;
	char str[1024];
	LVITEM q;
	HWND tempS;
	
	//Get Window Position
	GetWindowRect(form1, &b);

	//Create Game
	if(num == 0){
		//Popup Menu at Appropriate Location
		i = TrackPopupMenu(gameMenu, TPM_RETURNCMD, b.left + 425, b.top + 300, 0, form1, NULL);
		
		if(i == 0xFFFD && strcmp(lastGameToPlay1, "LastGameToPlay1") != 0){
			strcpy(currentGame, lastGameToPlay1);
			createGameRequest();
			imOwner = true;
		}
		else if(i == 0xFFFE && strcmp(lastGameToPlay2, "LastGameToPlay2") != 0){
			strcpy(currentGame, lastGameToPlay2);
			createGameRequest();
			imOwner = true;
		}
		else if(i == 0xFFFF && strcmp(lastGameToPlay3, "LastGameToPlay3") != 0){
			strcpy(currentGame, lastGameToPlay3);
			createGameRequest();
			imOwner = true;
		}
		else if(i > 0 && i <= totalGames){
			strcpy(currentGame, gameList[i - 1].game);
			createGameRequest();
			imOwner = true;
		}
	}
	//User List
	else if(num == 2){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(userlistMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;
		else if(SendMessage(lstUserlist, LVM_GETITEMCOUNT, 0, 0) == 0)
			return;

		//Get Currently Selected item
		iSelect = SendMessage(lstUserlist, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
		if(iSelect < 0)
			return;

		//Get UserID
		q.mask = LVIF_TEXT;
		q.iItem = iSelect;
		q.iSubItem = 0;
		q.pszText = ID;
		q.cchTextMax = 1024;
		SendMessage(lstUserlist, LVM_GETITEM, 0, (LPARAM) &q);

		
		//Get Username
		q.mask = LVIF_TEXT;
		q.iItem = iSelect;
		q.iSubItem = 1;
		q.pszText = userNick;
		q.cchTextMax = 1024;
		SendMessage(lstUserlist, LVM_GETITEM, 0, (LPARAM) &q);

		//Find User
		if(i == 0xCCCC){
			strcpy(temp, userlistCommands[51].commands);
			strcat(temp, userNick);
		}
		//PM
		else if(i == 0xB2DD){
			EnableWindow(form1, FALSE);
			createPMWindow();

			if(strlen(fPM)==NULL)
				return;

			temp[0] = 'd';
			strcpy(&temp[1],"/msg ");
			strcat(temp, ID);
			strcat(&temp[1]," ");
			strcat(temp, fPM);
			i = strlen(temp) + 1;
			temp[0] = '\0';

			constructPacket(temp, i, 0x07);
			return;
		}
		//For KICK only
		else if(i == 0xFFFF){
			strcpy(temp, userlistCommands[0].commands);
			strcat(temp, ID);
		}
		//Ignore
		else if(i == 0xDDDB){
			temp[0] = 'd';
			strcpy(&temp[1], "/ignore ");
			strcat(temp, ID);
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
		//Unignore
		else if(i == 0xDDDA){
			temp[0] = 'd';
			strcpy(&temp[1], "/unignore ");
			strcat(temp, ID);
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
		//COPY User ID
		else if(i == 0xFFFD){
			copyToClipboard(ID);
			return;
		}
		//COPY Username
		else if(i == 0xFFFE){
			//Get UserID
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstUserlist, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//Find Whitespace.  For SILENCE, BAN, TEMP ADMIN, TEMP ELEVATED
		else{
			strcpy(temp, userlistCommands[i].commands);
			strcat(temp, ID);
			wsprintf(str, " %i", userlistCommands[i].time);
			strcat(temp, str);
		}

		//Send
		SendMessage(txtChat, WM_SETTEXT, 0, (LPARAM) temp);
		globalChatRequest();
	}
	//Game List
	else if(num == 3){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(gamelistMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;
		else if(SendMessage(lstGamelist, LVM_GETITEMCOUNT, 0, 0) == 0)
			return;

		//Get Currently Selected item
		iSelect = SendMessage(lstGamelist, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
		if(iSelect < 0)
			return;

		//Get Game ID
		q.mask = LVIF_TEXT;
		q.iItem = iSelect;
		q.iSubItem = 0;
		q.pszText = ID;
		q.cchTextMax = 1024;
		SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &q);

		//JOIN
		if(i == 0xFFF7){
			if(myGameID == -1)
				joinGameRequest();
			return;
		}
		else if(i == 0xDDD9){
			temp[0] = 'd';
			strcpy(&temp[1], "/closegame ");
			strcat(temp, ID);
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
		//Kaillera More Information
		else if(i == 0xFFF8){
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &q);
			if(kInfo.moreInfosCallback != NULL)
				kInfo.moreInfosCallback(str);
			return;
		}
		//COPY Game ID
		else if(i == 0xFFF9){
			copyToClipboard(ID);
			return;
		}
		//COPY Game
		else if(i == 0xFFFA){
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Version
		else if(i == 0xFFFB){
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 2;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Owner
		else if(i == 0xFFFC){
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 3;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}

	}
	//Game Userlist
	else if(num == 4){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(gameUserlistMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;
		else if(SendMessage(lstGameUserlist, LVM_GETITEMCOUNT, 0, 0) == 0)
			return;

		//Get Currently Selected item
		iSelect = SendMessage(lstGameUserlist, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
		if(iSelect < 0)
			return;

		//Get UserID
		q.mask = LVIF_TEXT;
		q.iItem = iSelect;
		q.iSubItem = 0;
		q.pszText = ID;
		q.cchTextMax = 1024;
		SendMessage(lstGameUserlist, LVM_GETITEM, 0, (LPARAM) &q);

		//For KICK only
		if(i == 0xFFF4 && imOwner){
			kickRequest();
			return;
		}
		//Mute
		else if(i == 0xDDDD){
			temp[0] = 'd';
			strcpy(&temp[1], "/mute ");
			strcat(temp, ID);
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x08);
			return;
		}
		//Unmute
		else if(i == 0xDDDC){
			temp[0] = 'd';
			strcpy(&temp[1], "/unmute ");
			strcat(temp, ID);
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x08);
			return;
		}
		//COPY User ID
		else if(i == 0xFFF5){
			copyToClipboard(ID);
			return;
		}
		//COPY Username
		else if(i == 0xFFF6){
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstGameUserlist, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
	}
	//Server List Menu
	else if(num == 5){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(serverlistMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;

		if(SendMessage(sTab, TCM_GETCURSEL, 0, 0) == 0){
			tempS = lstServerListK;
			iSelect = SendMessage(tempS, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
			
		}
		else{
			tempS = lstServerList3D;
			iSelect = SendMessage(tempS, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
		}

		if(SendMessage(tempS, LVM_GETITEMCOUNT, 0, 0) == 0){
			if(i != 0xFFEF)
				return;
		}			

		//Add to Favorites
		if(i == 0xFFE6){
			if(iSelect < 0)
				return;

			//Get IP Address
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.cchTextMax = 1024;

			q.iSubItem = 0;
			q.pszText = server;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 1;
			q.pszText = ip;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 3;
			q.pszText = location;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);

			EnableWindow(form1, FALSE);
			createCommentsWindow();
			favoritelistAdditem(server, ip, location, fComments);
		}
		//Get Server List
		else if(i == 0xFFEF){
			if(tempS == lstServerListK){
				getServerListK();
			}
			else{
				getServerList3D();
			}
		}
		//Ping
		else if(i == 0xFFD7){
			if (tempS == lstServerListK) {
				if (pingingK == false) {
					exitPingKThread();
					SetWindowText(form1, "Pinging Kaillera Servers...");
					kailleraPingThread = CreateThread(NULL, 0, pingKailleraServers, NULL, 0, NULL);
				}
			}
			else {
				if (pinging3D == false) {
					exitPing3DThread();
					SetWindowText(form1, "Pinging EmuLinker Servers...");
					ping3DThread = CreateThread(NULL, 0, ping3DServers, NULL, 0, NULL);
				}
			}
		}
		//Stop
		else if(i == 0xFFD6){
			if(tempS == lstServerListK){
				exitPingKThread();
				SetWindowText(form1, "Kaillera Pinging Stopped!");
			}
			else{
				exitPing3DThread();
				SetWindowText(form1, "EmuLinker Pinging Stopped!");
			}
		}	
		//For Connect
		else if(i == 0xFFF0){
			//Get Currently Selected item
			iSelect = SendMessage(tempS, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
			if(iSelect < 0)
				return;
			userQuitRequest();
			//Get IP Address
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.cchTextMax = 1024;

			q.iSubItem = 0;
			q.pszText = server;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 1;
			q.pszText = ip;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 3;
			q.pszText = location;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);

			//Display
			SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) ip);

			loginToServer();

			//Display
			strcpy(myServer, server);
			SetWindowText(form1, server);

			recentlistAdditem(server, ip, location);
		}
		//COPY Server Name
		else if(i == 0xFFF1){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 0;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY IP Address
		else if(i == 0xFFF2){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Location
		else if(i == 0xFFF3){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 3;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(tempS, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
	}
	//Waiting Games Menu
	else if(num == 6){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(waitinglistMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;

		iSelect = SendMessage(lstWaitingList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

		if(SendMessage(lstWaitingList, LVM_GETITEMCOUNT, 0, 0) == 0){
			if(i != 0xFFE7)
				return;
		}			

		//Get Waiting Games
		if(i == 0xFFE7){
			getWaitingGames();
		}
		//Add to Favorites
		else if(i == 0xFFDF){
			if(iSelect < 0)
				return;
			//Get IP Address
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.cchTextMax = 1024;

			q.iSubItem = 3;
			q.pszText = server;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 4;
			q.pszText = ip;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 5;
			q.pszText = location;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);

			EnableWindow(form1, FALSE);
			createCommentsWindow();
			favoritelistAdditem(server, ip, location, fComments);
		}
		//For Connect
		else if(i == 0xFFE8){
			//Get Currently Selected item
			iSelect = SendMessage(lstWaitingList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
			if(iSelect < 0)
				return;
			userQuitRequest();
			//Get IP Address
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 4;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			//Display
			SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) str);

			q.iSubItem = 3;
			q.pszText = server;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 4;
			q.pszText = ip;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 5;
			q.pszText = location;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);

			recentlistAdditem(server, ip, location);

			loginToServer();


			q.iSubItem = 3;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			//Display
			strcpy(myServer, server);
			SetWindowText(form1, server);
		}
		//COPY Game Name
		else if(i == 0xFFE9){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 0;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Emulator
		else if(i == 0xFFEA){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Username
		else if(i == 0xFFEB){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 2;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Server Name
		else if(i == 0xFFEC){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 3;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY IP Address
		else if(i == 0xFFED){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 4;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Location
		else if(i == 0xFFEE){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 5;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstWaitingList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
	}
	//Recent List Menu
	else if(num == 7){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(recentlistMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;
		else if(SendMessage(lstRecentList, LVM_GETITEMCOUNT, 0, 0) == 0)
			return;

		iSelect = SendMessage(lstRecentList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);			

		//Add to Favorites
		if(i == 0xFFE1){
			//Get Currently Selected item
			iSelect = SendMessage(lstRecentList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
			if(iSelect < 0)
				return;
			//Get IP Address
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.cchTextMax = 1024;

			q.iSubItem = 1;
			q.pszText = server;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 0;
			q.pszText = ip;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 2;
			q.pszText = location;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);

			EnableWindow(form1, FALSE);
			createCommentsWindow();
			favoritelistAdditem(server, ip, location, fComments);
		}
		//For Connect
		else if(i == 0xFFE2){
			if(iSelect < 0)
				return;
			userQuitRequest();
			//Get IP Address
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 0;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			//Display
			SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) str);


			q.iSubItem = 0;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);

			q.iSubItem = 0;
			q.pszText = ip;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 1;
			q.pszText = server;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 2;
			q.pszText = location;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);


			SendMessage(lstRecentList, LVM_DELETEITEM, (WPARAM) i, 0);

			recentlistAdditem(server, ip, location);

			loginToServer();

			q.iSubItem = 1;
			q.pszText = str;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			if(strcmp(str, "NA") == 0){
				q.iSubItem = 0;
				SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			}
				
			//Display
			strcpy(myServer, server);
			SetWindowText(form1, server);
		}
		//Remove
		else if(i == 0xFFE3){
			if(iSelect < 0)
				return;
			SendMessage(lstRecentList, LVM_DELETEITEM, (WPARAM) iSelect, 0);
		}
		//Remove ALL
		else if(i == 0xFFE0){
			SendMessage(lstRecentList, LVM_DELETEALLITEMS, 0, 0);
		}
		//COPY Server Name
		else if(i == 0xFFE4){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY IP Address
		else if(i == 0xFFE5){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 0;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Location
		else if(i == 0xFFE6){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 2;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
	}
	//Favorite List Menu
	else if(num == 8){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(favoritelistMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;
		else if(SendMessage(lstFavoriteList, LVM_GETITEMCOUNT, 0, 0) == 0)
			return;

		iSelect = SendMessage(lstFavoriteList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);			
		if(iSelect < 0)
			return;

		//For Connect
		if(i == 0xFFD8){
			userQuitRequest();
			//Get IP Address
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 0;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			//Display
			SendMessage(txtServerIP, WM_SETTEXT, 0, (LPARAM) str);

			q.iSubItem = 0;
			q.pszText = ip;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 1;
			q.pszText = server;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			q.iSubItem = 2;
			q.pszText = location;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);

			loginToServer();

			recentlistAdditem(server, ip, location);
			q.iSubItem = 1;
			q.pszText = str;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);

			if(strcmp(str, "NA") == 0){
				q.iSubItem = 0;
				SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			}
				
			//Display
			strcpy(myServer, server);
			SetWindowText(form1, server);
		}
		//Edit Entry
		else if(i == 0xFFD5){
			EnableWindow(form1, FALSE);
			createEditFavoritesWindow();
		}
		//Remove
		else if(i == 0xFFD9){
			if(iSelect < 0)
				return;
			SendMessage(lstFavoriteList, LVM_DELETEITEM, (WPARAM) iSelect, 0);
		}
		//Remove ALL
		else if(i == 0xFFDA){
			SendMessage(lstFavoriteList, LVM_DELETEALLITEMS, 0, 0);
		}
		//COPY Server Name
		else if(i == 0xFFDB){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 1;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY IP Address
		else if(i == 0xFFDC){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 0;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Location
		else if(i == 0xFFDD){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 2;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
		//COPY Comments
		else if(i == 0xFFDE){
			if(iSelect < 0)
				return;
			q.mask = LVIF_TEXT;
			q.iItem = iSelect;
			q.iSubItem = 2;
			q.pszText = str;
			q.cchTextMax = 1024;
			SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &q);
			copyToClipboard(str);
			return;
		}
	}
	//txtChatroom
	else if(num == 9){
		//Popup Menu at Appropriate Location
		GetCursorPos(&c);
		i = TrackPopupMenu(txtChatroomMenu, TPM_RETURNCMD, c.x, c.y, 0, form1, NULL);
		
		//Error Check
		if(i == 0)
			return;

		//Msg Off
		if(i == 0xDDD4){
			temp[0] = 'd';
			strcpy(&temp[1], "/msgoff");
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
		//Msg On
		else if(i == 0xDDD5){
			temp[0] = 'd';
			strcpy(&temp[1], "/msgon");
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
		//Stealth On
		if(i == 0xDDD8){
			temp[0] = 'd';
			strcpy(&temp[1], "/stealthon");
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
		//Stealth Off
		else if(i == 0xDDD7){
			temp[0] = 'd';
			strcpy(&temp[1], "/stealthoff");
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
		//My IP Address
		else if(i == 0xDDD6){
			temp[0] = 'd';
			strcpy(&temp[1], "/myip");
			i = strlen(temp) + 1;
			temp[0] = '\0';
			constructPacket(temp, i, 0x07);
			return;
		}
	}
}


void copyToClipboard(LPSTR lpCmdLine){
	HGLOBAL clipbuffer;
	char * buffer;
	
	if(OpenClipboard(0)){
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(lpCmdLine) + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, lpCmdLine);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
}

void displayChatroomAsServer(char *msg){
	char temp[12800];

	//Previous (CRLF) 
	//<Server> Message (CRLF)
	strcpy(temp, "<Client> ");
	strcat(temp, msg);
	strcat(temp,"\r\n");
	
	//Display
	displayAndAutoScrollRichEdit(txtChatroom, temp, RGB(34, 139, 34));
}

void displayStats(){
	int numUsers;
	int numGames;
	char temp[2048];
	char strUsers[1024];
	char strGames[1024];

	numUsers = SendMessage(lstUserlist, LVM_GETITEMCOUNT, 0, 0);
	numGames = SendMessage(lstGamelist, LVM_GETITEMCOUNT, 0, 0);
	wsprintf(strUsers, "%i", numUsers);
	wsprintf(strGames, "%i", numGames);

	strcpy(temp, cVersion);
	strcat(temp, "     #Users: ");
	strcat(temp, strUsers);
	strcat(temp, "     #Games: ");
	strcat(temp, strGames);

	SendMessage(lblStats, WM_SETTEXT, 0, (LPARAM) temp);
}
			


int saveConfig(){
	ofstream config("supraclient.ini", ios::out);
	char temp[2048];

	if(!config.good()){
		MessageBox(form1, "Error occurred while saving to supraclient.ini", "Error Saving!", NULL);
		return 1;
	}

	config << "[SupraclientCPPE Config File]" << "\n\n";

	//Server
	GetWindowText(txtServerIP, temp, GetWindowTextLength(txtServerIP) + 1);
	config << "ServerIP=" << temp << "\n";
		
	//Nick
	GetWindowText(txtUsername, temp, GetWindowTextLength(txtUsername) + 1);
	config << "Nick=" << temp << "\n";

	//Connection
	GetWindowText(cmbConnectionType, temp, GetWindowTextLength(cmbConnectionType) + 1);		
	if(strcmp(temp,"LAN") == 0)
		config << "Connection=1" << "\n";
	else if(strcmp(temp,"Excellent") == 0)
		config << "Connection=2" << "\n";
	else if(strcmp(temp,"Good") == 0)
		config << "Connection=3" << "\n";
	else if(strcmp(temp,"Average") == 0)
		config << "Connection=4" << "\n";
	else if(strcmp(temp,"Low") == 0)
		config << "Connection=5" << "\n";
	else if(strcmp(temp,"Bad") == 0)
		config << "Connection=6" << "\n";
	else
		config << "Connection=3" << "\n";

	//Quit
	GetWindowText(txtQuit, temp, GetWindowTextLength(txtQuit) + 1);
	config << "Quit=" << temp << "\n";
	
	//Window Position
	RECT q;
	GetWindowRect(form1, &q);
	config << "xPos=" << q.left << "\n";
	config << "yPos=" << q.top << "\n";

	//LastGameToPlay1
	config << "LastGameToPlay1=" << lastGameToPlay1 << "\n";
	//LastGameToPlay2
	config << "LastGameToPlay2=" << lastGameToPlay2 << "\n";
	//LastGameToPlay3
	config << "LastGameToPlay3=" << lastGameToPlay3 << "\n";

	//UserListColumn
	config << "Userlist_Column=" << lstUserlistColumn << "\n";
	config << "Userlist_Column_Order=" << userlistSwitch << "\n";

	int v; 
	//chkKeepGameChatLogs
	v = SendMessage(chkKeepGameChatLogs, BM_GETCHECK, 0, 0);
	config << "Keep_Gameroom_Logs=" << v << "\n";
	//chkKeepChatLogs
	v = SendMessage(chkKeepChatLogs, BM_GETCHECK, 0, 0);
	config << "Keep_Chatroom_Logs=" << v << "\n";
	//chkJoinChat
	v = SendMessage(chkJoinChat, BM_GETCHECK, 0, 0);
	config << "Join/Left=" << v << "\n";
	//chkCreate
	v = SendMessage(chkCreate, BM_GETCHECK, 0, 0);
	config << "Create/Close=" << v << "\n";
	//chkEmulinkerSF
	v = SendMessage(chkEmulinkerSF, BM_GETCHECK, 0, 0);
	config << "Use_EmuSF=" << v << "\n";
	//chkBlink
	v = SendMessage(chkBlink, BM_GETCHECK, 0, 0);
	config << "Blink=" << v << "\n";
	//chkUseScreenChat
	v = SendMessage(chkUseScreenChat, BM_GETCHECK, 0, 0);
	config << "Use_Screen_Chat=" << v << "\n";
	//chkDrop
	v = SendMessage(chkDrop, BM_GETCHECK, 0, 0);
	config << "Drop=" << v << "\n";
	//chkBeep
	v = SendMessage(chkBeep, BM_GETCHECK, 0, 0);
	config << "Beep=" << v << "\n";
	//chkJoinDbl
	v = SendMessage(chkJoinDbl, BM_GETCHECK, 0, 0);
	config << "Join_Double_Click=" << v << "\n";
	//chkUseCache
	v = SendMessage(chkUseCache, BM_GETCHECK, 0, 0);
	config << "Cache=" << v << "\n";
	//chkShowError
	v = SendMessage(chkShowError, BM_GETCHECK, 0, 0);
	config << "Join_Error=" << v << "\n";
	

	//P2P Server
	/*GetWindowText(txtP2PServer, p2pServer, GetWindowTextLength(txtP2PServer) + 1);
	config << "P2P_Server=" << p2pServer << "\n";
	//P2P Port
	GetWindowText(txtP2PPort, p2pPort, GetWindowTextLength(txtP2PPort) + 1);
	config << "P2P_Port=" << p2pPort << "\n";*/


	//EmuSF_Emulator
	v = SendMessage(chkEmuRes, BM_GETCHECK, 0, 0);
	config << "EmuSF_Emulator=" << v << "\n";

	//EmuSF_Connection Type
	v = SendMessage(chkConnRes, BM_GETCHECK, 0, 0);
	config << "EmuSF_Connection=" << v << "\n";

	//EmuSF_MaxUsers
	GetWindowText(txtMaxUsers, maxUsersG, GetWindowTextLength(txtMaxUsers) + 1);
	if(atoi(maxUsersG) < 1 || atoi(maxUsersG) > 100){
		wsprintf(maxUsersG, "%i", 16);
	}
	config << "EmuSF_MaxUsers=" << maxUsersG << "\n";

	//EmuSF_MaxUsers
	GetWindowText(txtMaxPing, maxPingG, GetWindowTextLength(txtMaxPing) + 1);
	if(atoi(maxPingG) < 1 || atoi(maxPingG) > 1000){
		wsprintf(maxPingG, "%i", 200);
	}
	config << "EmuSF_MaxPing=" << maxPingG << "\n";
	//chkFakeP2P
	v = SendMessage(chkFakeP2P, BM_GETCHECK, 0, 0);
	config << "EmuSF_FakeP2P=" << v << "\n";


	//chkJoinChatGame
	v = SendMessage(chkJoinChatGame, BM_GETCHECK, 0, 0);
	config << "Join/Left_Game=" << v << "\n";
	//lastTabServer
	config << "ServerTab=" << SendMessage(sTab, TCM_GETCURSEL, 0, 0) << "\n";
	//lastTabServer
	config << "ShowChatroomFirst=" << 0 << "\n";

	LVITEM b;
	int t;
	char strI[1024];

	//Recent
	t = SendMessage(lstRecentList, LVM_GETITEMCOUNT, 0, 0);
	if(t > RECENT_MAX)
		t = RECENT_MAX;
	for(v = 0; v < t; v++){
		b.mask = LVIF_TEXT;
		b.iItem = v;
		b.cchTextMax = 1024;
		b.iSubItem = 0;
		b.pszText = temp;
		SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
		wsprintf(strI, "%02i", v);
		config << "Recent_IP" << strI << "=" << temp << endl;

		b.iSubItem = 1;
		b.pszText = temp;
		SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
		config << "Recent_Name" << strI << "=" << temp << endl;

		b.iSubItem = 2;
		b.pszText = temp;
		SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
		config << "Recent_Location" << strI << "=" << temp << endl;
	}

	//Favorite
	t = SendMessage(lstFavoriteList, LVM_GETITEMCOUNT, 0, 0);
	if(t > FAVORITE_MAX)
		t = FAVORITE_MAX;
	for(v = 0; v < t; v++){
		b.mask = LVIF_TEXT;
		b.iItem = v;
		b.cchTextMax = 1024;
		b.iSubItem = 0;
		b.pszText = temp;
		SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
		wsprintf(strI, "%02i", v);
		config << "Favorite_IP" << strI << "=" << temp << endl;

		b.iSubItem = 1;
		b.pszText = temp;
		SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
		config << "Favorite_Name" << strI << "=" << temp << endl;

		b.iSubItem = 2;
		b.pszText = temp;
		SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
		config << "Favorite_Location" << strI << "=" << temp << endl;

		b.iSubItem = 3;
		b.pszText = temp;
		SendMessage(lstFavoriteList, LVM_GETITEM, 0, (LPARAM) &b);
		config << "Favorite_Comments" << strI << "=" << temp << endl;
	}

	//Away Messages
	for(v = 1; v <= awayMessageCount; v++){
		if(awayMessages[v].save == true){
			config << "Away_Subject" << v << "=" << awayMessages[v].subject << endl;
			config << "Away_Message" << v << "=" << awayMessages[v].message << endl;
		}
	}

	config << "Version=" << myVersion << endl;

	config.close();

	return 0;
}


void displayGameChatroomAsServer(char *msg){
	char temp[2048];

	//Previous (CRLF) 
	//<Server> Message (CRLF)
	strcpy(temp, "<Server> ");
	strcat(temp, msg);
	strcat(temp,"\r\n");
	
	//Display
	displayAndAutoScrollRichEdit(txtGameChatroom, temp, RGB(34, 139, 34));
}


void loginToServer(){
	char serverIP[1024];
	char port[1024];

	supraCleanup(0, 0);
	showChatroom(true);

	mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	//Length of ServerIP:Port
	short lenServerIP = (short)GetWindowTextLength(txtServerIP);

	if(lenServerIP < 1){
		MessageBox(form1,"Invalid Address Format!", "Address Error!", NULL);
		displayChatroomAsServer("Invalid Address Format!  ex). 127.0.0.1:27888");
		return;
	}

	//Get ServerIP:Port
	GetWindowText(txtServerIP, serverIP, lenServerIP + 1);
	//Find:
	char* c = strstr(serverIP, ":");

	if(c != NULL){
		//Split Port
		strcpy(port, c + 1);
		//Split Address and Port
		ZeroMemory(myAddress, 512);
		strncpy(myAddress, serverIP, (lenServerIP - strlen(port) - 1));
		myPort = (u_short)atoi(port);
	}
	else{
		ZeroMemory(myAddress, 512);
		GetWindowText(txtServerIP, myAddress, lenServerIP + 1);
		myPort = 27888;
	}
	
	socketInfo.sin_family = AF_INET;
    socketInfo.sin_port = htons(myPort);

	
	displayChatroomAsServer("Resolving host...");
	DoEvents();

	if(inet_addr(myAddress) == INADDR_NONE){
		hp = gethostbyname(myAddress);
		if(hp == NULL){
			closesocket(mySocket);
			displayChatroomAsServer("Error Resolving!");
			return;
		}
		socketInfo.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	}
	else{
		socketInfo.sin_addr.s_addr = inet_addr(myAddress);
	}

	displayChatroomAsServer("Connecting...");
	DoEvents();

	connection = true;

    //memset(&(socketInfo.sin_zero), 0, 8); 
	recvLoopThread = CreateThread(NULL, 0, recvLoop, NULL, 0, NULL);
	continuousLoopThread = CreateThread(NULL, 0, continuousLoop, NULL, 0, NULL);

	sendto(mySocket, entryMsg,  10, NULL, (sockaddr *) &socketInfo, sizeof(socketInfo));
}


void DoEvents(){
	MSG msg;
    long sts;
    do{
		if(sts = PeekMessage(&msg, (HWND) NULL, 0, 0, PM_REMOVE)){
    		TranslateMessage(&msg);
    		DispatchMessage(&msg);
    	}
    }while (sts);
} 



DWORD WINAPI callGameCallback(LPVOID lpParam){

	int i = kInfo.gameCallback(currentGame, myPlayerNumber, totalPlayers);
	
	return 0;
}


DWORD WINAPI continuousLoop(LPVOID lpParam){
	for(; ;){
		if(gamePlaying == false && loggedIn)
			clientKeepAlive();
		Sleep(20000);		
	}
	return 0;
}


/*DWORD WINAPI p2pLoop(LPVOID lpParam){
	char temp[5000];
	int rBytes;

	for( ; ; ){
		/* Receive bytes from client *
		rBytes = recvfrom(p2pSocket, temp, 5000, 0, (struct sockaddr *)&p2pClientInfo, &p2pClientLength);

		if(rBytes > 0){
			if(strncmp(temp, "GAME_DATA", 9)){
				p2pGameData(&temp[9], rBytes - 9);
			}
			else if(strncmp(temp, "GAME_CHAT", 9)){
				p2pGameChat(&temp[9], rBytes - 9);
			}
			else if(strncmp(temp, "GAME_READY", 10)){
				p2pGameReady(&temp[9], rBytes - 10);
			}
			else if(strncmp(temp, "GAME_START", 10)){
				p2pGameStart(&temp[9], rBytes - 10);
				MessageBox(NULL,"Start",NULL,NULL);
			}
			else if(strncmp(temp, "GAME_DROP", 9)){
				p2pGameDrop(&temp[9], rBytes - 9);
				return 0;
			}
		}
	}

	return 0;
}

void p2pGameReady(char *data, int dLen){

}

void p2pGameData(char *data, int dLen){

}

void p2pGameChat(char *data, int dLen){

}

void p2pGameStart(char *data, int dLen){

}

void p2pGameDrop(char *data, int dLen){

}*/


DWORD WINAPI recvLoop(LPVOID lpParam){
    char str[1024];
	unsigned char msgType;

	for(; ;){
		//Check the socket
		myBuffCount = (myBuffCount + 1) % MAX_INCOMING_BUFFER;
		mainBytesRecv = recv(mySocket, myBuff[myBuffCount].myBuff, sizeof(myBuff[myBuffCount].myBuff), NULL);
		
		if(mainBytesRecv > 0){
			//Check for Message Type Range
			msgType = *(unsigned char *) &myBuff[myBuffCount].myBuff[5];
			if(msgType >= 0x01 && msgType <= 0x17){
				parseData(myBuffCount, msgType);
			}
			else if (strncmp(&myBuff[myBuffCount].myBuff[0], "HELLOD00D", 9) == 0){ 
				strncpy(str,&myBuff[myBuffCount].myBuff[9],6);
				myPort = cStrToInt(str);
				socketInfo.sin_family = AF_INET;
				socketInfo.sin_port = htons(myPort);

				if(inet_addr(myAddress) == INADDR_NONE){
					hp = gethostbyname(myAddress);
					socketInfo.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
				}
				else
					socketInfo.sin_addr.s_addr = inet_addr(myAddress); 

				//memset(&(socketInfo.sin_zero), 0, 8); 

				short lenUsername = (short)GetWindowTextLength(txtUsername);
				short lenEmulator = (short)strlen(emulator);
				short lenConnectionType = (short)GetWindowTextLength(cmbConnectionType);
				char temp[16];
				
				GetWindowText(cmbConnectionType, temp, lenConnectionType + 1);
				
				if(strcmp(temp,"LAN") == 0)
					connectionType = lan;
				else if(strcmp(temp,"Excellent") == 0)
					connectionType = excellent;
				else if(strcmp(temp,"Good") == 0)
					connectionType = good;
				else if(strcmp(temp,"Average") == 0)
					connectionType = average;
				else if(strcmp(temp,"Low") == 0)
					connectionType = low;
				else if(strcmp(temp,"Bad") == 0)
					connectionType = bad;
				else
					connectionType = good;

				GetWindowText(txtUsername, username, lenUsername + 1);
				

				if(lenUsername > 0){
					userLoginInformation(lenUsername, lenEmulator);
				}
				else
					MessageBox(form1,"Please supply a username!", "Error UserLoginInformation",NULL);
			}
			else if(strncmp(&myBuff[myBuffCount].myBuff[0], "TOO",3) == 0){
				displayChatroomAsServer("Server is Full!");
				MessageBox(form1,"Server is full!","Login Error!",NULL);
				supraCleanup(0, 0);
			}
			else if(strncmp(&myBuff[myBuffCount].myBuff[0], "VER",3) == 0){
				displayChatroomAsServer("Invalid Version!");
				MessageBox(form1,"Invalid Version!","Login Error!",NULL);
				supraCleanup(0, 0);
			}
			//else{
			//	displayChatroomAsServer("Unknown Message Received!");
			//	MessageBox(form1,myBuff[myBuffCount].myBuff,"Unknown Message Received!",NULL);
			//	supraCleanup(0, 0);
			//}
		}
	}
	return 0;
}


void parseData(int slot, unsigned char msgType){
    short i;
    unsigned short temp;
    unsigned char numOfMessages;
    unsigned short msgNum;
	long msgNumCount;
       
    //How many messages are in packet
	msgNum = *(unsigned short *) &myBuff[slot].myBuff[1];

	//Determine how many Messages are needed
	if(msgNum > serversLastMessage)
		msgNumCount = msgNum - serversLastMessage;
	//Check for Out of Order Messages
	else if(msgNum <= serversLastMessage){
		//Max is 65535 or 0xFFFF...reset. 20 Message backup should be enough.
		if(serversLastMessage >= 65515){
			//Determine how many Messages are needed
			msgNumCount = msgNum - (65536 - serversLastMessage);
		}
		//Out of Order Message
		else{
			return;
			//We probably should check to see if we needed this packet.
		}
	}

	//Don't Recover any Messages.
	if(msgNumCount < 2){
		serversLastMessage = msgNum;
		gotoMessageType(6, msgType, slot);
	}
	//Recover
	else{
		numOfMessages = *(unsigned char *) &myBuff[slot].myBuff[0];
		//Can't Recover some Messages =(
        if(msgNumCount > numOfMessages){
			if(SendMessage(chkDrop, BM_GETCHECK, 0, 0) == BST_CHECKED)
				displayAndAutoScrollRichEdit(txtChatroom, "<ALERT> PACKET(S) DROPPED. Desynch may occur or server updates may be missed.\n", RGB(255, 0, 0));
			msgNumCount = numOfMessages;
		}

		serversLastMessage = msgNum;

		//Recover needed Messages
		temp = 1; //temp = 0 is numOfMessages
        for(i = 0; i < msgNumCount; i++){
			//serverMessage[i].msgNum = *(unsigned short *) &myBuff[slot].myBuff[temp];
            temp += 2;
			serverMessage[i].msgLen = *(unsigned short *) &myBuff[slot].myBuff[temp];
            temp += 2;
            serverMessage[i].msgType = *(unsigned char *) &myBuff[slot].myBuff[temp];
			if(i != 0)
				serverMessage[i].msgPos = 4 + serverMessage[i - 1].msgLen + serverMessage[i - 1].msgPos;
            temp += serverMessage[i].msgLen;//includes msgType		
		}

        for (i = (msgNumCount - 1); i >= 0; i--){
			if(i != 0)
				gotoMessageType(6 + serverMessage[i].msgPos, serverMessage[i].msgType, slot);
			else
				gotoMessageType(6, msgType, slot);
		}
    }
}



/*
void parseData(){
    char i;
    unsigned long w;
    unsigned short temp;
    unsigned char numOfMessages;
    unsigned short msgNum;
	unsigned char msgType;
       
    //how many messages are in packet
    numOfMessages = *(unsigned char *) &myBuff[slot].myBuff[0];
	msgNum = *(unsigned short *) &myBuff[slot].myBuff[1];
    msgType = *(unsigned char *) &myBuff[slot].myBuff[5];

    if(msgType < 0x01 || msgType > 0x17){
        MessageBox(form1,"Received Unknown Message!", "Error in parseData()", NULL);
        return;
    }

    if(msgNum > serversLastMessage || serversLastMessage == 0){
        //increases speed to not process every message
        if(msgNum > 0)
			numOfMessages = msgNum - serversLastMessage;
                
        if(numOfMessages > myBuff[0])
			numOfMessages = myBuff[0];

        temp = 1;
        for(i = 0; i < numOfMessages; i++){
			serverMessage[i].msgNum = *(unsigned short *) &myBuff[slot].myBuff[temp];
            temp = temp + 2;
			serverMessage[i].msgLen = *(unsigned short *) &myBuff[slot].myBuff[temp];
            temp = temp + 2;
            serverMessage[i].msgType = *(unsigned char *) &myBuff[slot].myBuff[temp];
            temp = temp + 1;
            for(w = 0; w < serverMessage[i].msgLen; w++)
                serverMessage[i].msgData[w] = myBuff[temp + w];
            temp = temp + serverMessage[i].msgLen;
		}

        if(numOfMessages == 1){
            serversLastMessage = msgNum;
            gotoMessageType(0, serverMessage[0].msgType);
            return;
		}
        else{
            if((msgNum - serversLastMessage) > myBuff[0]){
				//Trigger Server Retry Algorithm by sending the last packet with the same Message Number
				sendto(mySocket, globalPacket, globalPacketSize, NULL, (sockaddr *) &socketInfo, sizeof(socketInfo));
				//MessageBox(form1,"Client dropped packet(s).  Desynch may occur or server updates may be missed.  Please relogin!", "Error - Dropped Packet(s)", NULL);
				//displayChatroomAsServer("Client dropped packet(s).  Desynch may occur or server updates may be missed.  Please relogin!");
				return;
			}

            i = i - 1;
            //store current message number in last message for next time.
            temp = serversLastMessage;
            serversLastMessage = msgNum;
            //recover messages
            while(i >= 0){
                if(temp <= serverMessage[i].msgNum)
                    gotoMessageType(i, serverMessage[i].msgType);
                i = i - 1;
            }
		}
    }
}
*/



void constructPacket(char *data, unsigned short lenData, char msgType){
    int i;
	int j;

	//unsigned short normalPacketSize = 0;
	unsigned short mySize = 0;

	if (msgType != 0) {

		normalPacketSize = 0;

		//Increase Message Count Each Send
		msgCount++;

		//Number of Messages
		if (msgCount > (NORMAL_PACKETS - 2))
			globalPacket[0] = NORMAL_PACKETS;
		else
			globalPacket[0] = msgCount + 1;

		normalPacketSize++;

		//Fill
		for (i = (globalPacket[0] - 1); i >= 0; i--) {
			if (i != 0) {
				//Data
				mySize = 4 + *(unsigned short*)& myPackets[i - 1].packet[2];
				memcpy(&myPackets[i].packet, &myPackets[i - 1].packet, mySize);
				normalPacketSize += mySize;
			}
			else {
				//Message Number
				myPackets[i].packet[0] = msgCount & 255;
				myPackets[i].packet[1] = (msgCount >> 8) & 255;
				//Message Length
				myPackets[i].packet[2] = (lenData + 1) & 255;
				myPackets[i].packet[3] = ((lenData + 1) >> 8) & 255;
				//Message Type
				myPackets[i].packet[4] = msgType;

				//Data
				memcpy(&myPackets[i].packet[5], data, lenData);
				normalPacketSize += lenData + 4 + 1;
			}
		}


		//Prepare Packet to Send
		j = 1;
		for (i = 0; i < globalPacket[0]; i++) {
			mySize = 4 + *(unsigned short*)& myPackets[i].packet[2];
			memcpy(&globalPacket[j], &myPackets[i].packet, mySize);
			j += mySize;
		}

		sendto(mySocket, globalPacket, normalPacketSize, NULL, (sockaddr*)& socketInfo, sizeof(socketInfo));

		//Reset
		if (msgCount == 65535)
			msgCount = -1;

	} else sendto(mySocket, globalPacket, normalPacketSize, NULL, (sockaddr*)& socketInfo, sizeof(socketInfo));

}



void gotoMessageType(unsigned short position, char msgType, int slot){
    
	//Moved Game Functions Up here for speed.
	//0x12 - Game Data
    if(msgType == 0x12)
        gameDataRecv(position, slot);
    //0x13 - Game Cache
    else if(msgType == 0x13)
        gameCacheRecv(position, slot);


    //0x01 - User Quit Notification
    else if(msgType == 0x01)
        userQuitNotification(position, slot);
    //0x02 - User Joined
    else if(msgType == 0x02)
        userJoined(position, slot);
    //0x04 - Server Status
    else if(msgType == 0x4)
        serverStatus(position, slot);
    //0x05 - Server to Client ACK
    else if(msgType == 0x05)
        serverToClientAck();
    //0x07 - Global Chat Notification
    else if(msgType == 0x07)
        globalChatNotification(position, slot);
    //0x08 - Game Chat Notification
    else if(msgType == 0x08)
        gameChatNotification(position, slot);
    //0x0A - Create Game Notification
    else if(msgType == 0x0A)
        createGameNotification(position, slot);
    //0x0B - Quit Game Notification
    else if(msgType == 0x0B)
        quitGameNotification(position, slot);
    //0x0C - Join Game Notification
    else if(msgType == 0x0C)
        joinGameNotification(position, slot);
    //0x0D - Player Information
    else if(msgType == 0x0D)
		playerInformation(position, slot);
    //0x0E - Update Game Status
    else if(msgType == 0x0E)
        updateGameStatus(position, slot);
    //0x10 - Close Game Notification
    else if(msgType == 0x10)
        closeGameNotification(position, slot);
    //0x11 - Start Game Notification
    else if(msgType == 0x11)
		startGameNotification(position, slot);
    //0x14 - Drop Game Notification
    else if(msgType == 0x14)
		dropGameNotification(position, slot);
    //0x15 - Ready to Play Notification
    else if(msgType == 0x15)
		readyToPlayNotificaiton();
    //0x16 - Connection Rejected Notification
    else if(msgType == 0x16)
        connectionRejectedNotification(position, slot);
    //0x17 - Server Information Message
    else if(msgType == 0x17)
        serverInformationMessage(position, slot);
}


void exitGameThread(){
	if (startedGame) {
		startedGame = false;
		sizeOfEinput = -1;
		gamePlaying = false;
		Sleep(20);
		EnableWindow(chkUseCache, TRUE);
		UpdateWindow(form1);
		TerminateThread(gameThread, 0);
		CloseHandle(gameThread);
	}
}

void exitThreads(){
	if (connection) {
		connection = false;
		TerminateThread(recvLoopThread, 0);
		CloseHandle(recvLoopThread);
		TerminateThread(continuousLoopThread, 0);
		CloseHandle(continuousLoopThread);
	}
}

void exitPingKThread() {
	if (pingingK) {
		pingingK = false;
		TerminateThread(kailleraPingThread, 0);
		CloseHandle(kailleraPingThread);
	}
}

void exitPing3DThread() {
	if (pinging3D) {
		pinging3D = false;
		TerminateThread(ping3DServers, 0);
		CloseHandle(ping3DThread);
	}
}


bool supraCleanup(char type, HWND h){
	//Complete
	if(type == 0){
		saveConfig();
		//Reset Winsock
		exitGameThread();
		exitThreads();
		closesocket(mySocket);
		myGameID = -1;
		myUserID = -1;
		myBuffCount = -1;
		tempUserCount = 0;
		tempGameCount = 0;
		imOwner = false;
		globalPacketSize = 0;
		//Reset Message Count
		msgCount = -1;
		sendMax = false;
		gameroom = false;
		iQuit = false;
		serversLastMessage = -1;
		//showGameroom(false);
		showChatroom(false);
		//Clear Textboxes and Listboxes
		SendMessage(txtChatroom, WM_SETTEXT, 0, (LPARAM) NULL);
		SendMessage(txtGameChatroom, WM_SETTEXT, 0, (LPARAM) NULL);
		SendMessage(txtChat, WM_SETTEXT, 0, (LPARAM) NULL);
		SendMessage(txtGameChat, WM_SETTEXT, 0, (LPARAM) NULL);
		SendMessage(lstGamelist, LVM_DELETEALLITEMS, 0, 0);
		SendMessage(lstUserlist, LVM_DELETEALLITEMS, 0, 0);
		SendMessage(lstGameUserlist, LVM_DELETEALLITEMS, 0, 0);
		//Display
		//SendMessage(form1, WM_SETTEXT, 0, (LPARAM) "SupraclientCPPE https://god-weapon.github.io");
		strcpy(myServer, "Not in a Server\0");
		displayChatroomAsServer("Disconnected!");
		displayStats();
	}
	//Gameroom
	else if(type == 1){
		exitGameThread();
		myGameID = -1;
		SendMessage(txtGame, WM_SETTEXT, 0, (LPARAM) "Not in Game!");
		SendMessage(lstGameUserlist, LVM_DELETEALLITEMS, 0, 0);
		SendMessage(txtGameChatroom, WM_SETTEXT, 0, (LPARAM) NULL);
		SendMessage(txtGameChat, WM_SETTEXT, 0, (LPARAM) NULL);
	}
	//UnSubclass
	else if(type == 2){
		SetWindowLongPtr(txtChat, GWLP_WNDPROC, 0);
		SetWindowLongPtr(txtGameChat, GWLP_WNDPROC, 0);
	}
	//Destroy
	else if(type == 3){
		if(h == form1){
			userQuitRequest();
			SendMessage(lstRecentList, LVM_DELETEALLITEMS, 0, 0);
			SendMessage(lstFavoriteList, LVM_DELETEALLITEMS, 0, 0);
			SendMessage(lstServerListK, LVM_DELETEALLITEMS, 0, 0);
			SendMessage(lstServerList3D, LVM_DELETEALLITEMS, 0, 0);
			SendMessage(lstWaitingList, LVM_DELETEALLITEMS, 0, 0);
			WSACleanup();
			exitPingKThread();
			exitPing3DThread();
			closesocket(mySocket);
			closesocket(mySocket3D);
			closesocket(mySocketK);
			closesocket(mySocketWaiting);
			//closesocket(p2pSocket);
			//TerminateThread(p2pThread, 0);
			//CloseHandle(p2pThread);
			totalGames = 0;
			return true;
		}
		else if(h == frmAway){
			ShowWindow(frmAway, SW_HIDE);
			EnableWindow(form1, TRUE);
			SetFocus(form1);
			UpdateWindow(form1);
			return false;
		}
		else if(h == frmPM){
			cPM = true;
			ShowWindow(frmPM, SW_HIDE);
			EnableWindow(form1, TRUE);
			SetFocus(form1);
			UpdateWindow(form1);
			return false;
		}
		else if(h == frmComments){
			cComments = true;
			ShowWindow(frmComments, SW_HIDE);
			EnableWindow(form1, TRUE);
			SetFocus(form1);
			UpdateWindow(form1);
			return false;
		}
		else if(h == frmEditFavorites){
			editFavorite = true;
			ShowWindow(frmEditFavorites, SW_HIDE);
			EnableWindow(form1, TRUE);
			SetFocus(form1);
			UpdateWindow(form1);
			return false;
		}
	}
	//P2P
	/*else if(type == 4){
		closesocket(p2pSocket);
		TerminateThread(p2pThread, 0);
		CloseHandle(p2pThread);
	}*/
	return false;
}

void kServerlistAdditem(char *serverName, char *ip, char *ping, char *location, char *users, char *games, char *version){
	LVITEM LvItem;
	memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem.mask = LVIF_TEXT;   // Text Style
	LvItem.cchTextMax = 1024; // Max size of test
	LvItem.iItem = 0;          // choose item  
	LvItem.iSubItem = 0;       // Put in first coluom
	LvItem.pszText = serverName; // Text to display (can be from a char variable) (Items)
	SendMessage(lstServerListK,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview
	LvItem.iSubItem = 1;
	LvItem.pszText = ip;
	SendMessage(lstServerListK,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 2;
	LvItem.pszText = ping;
	SendMessage(lstServerListK,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 3;
	LvItem.pszText = location;
	SendMessage(lstServerListK,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 4;
	LvItem.pszText = users;
	SendMessage(lstServerListK,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 5;
	LvItem.pszText = games;
	SendMessage(lstServerListK,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 6;
	LvItem.pszText = version;
	SendMessage(lstServerListK,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems

	ListView_SortItemsEx(lstServerListK, lstServerlistKCompareFunc, 0);
}

void Serverlist3DAdditem(char *serverName, char *ip, char *ping, char *location, char *users, char *games, char *version){
	LVITEM LvItem;
	memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members

	//  Setting properties Of members:
	LvItem.mask = LVIF_TEXT;   // Text Style
	LvItem.cchTextMax = 1024; // Max size of test
	LvItem.iItem = 0;          // choose item  
	LvItem.iSubItem = 0;       // Put in first coluom
	LvItem.pszText = serverName; // Text to display (can be from a char variable) (Items)
	SendMessage(lstServerList3D,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview
	LvItem.iSubItem = 1;
	LvItem.pszText = ip;
	SendMessage(lstServerList3D,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 2;
	LvItem.pszText = ping;
	SendMessage(lstServerList3D,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 3;
	LvItem.pszText = location;
	SendMessage(lstServerList3D,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 4;
	LvItem.pszText = users;
	SendMessage(lstServerList3D,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 5;
	LvItem.pszText = games;
	SendMessage(lstServerList3D,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 6;
	LvItem.pszText = version;
	SendMessage(lstServerList3D,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems

	ListView_SortItemsEx(lstServerList3D, lstServerlist3DCompareFunc, 0);
}

void userlistAdditem(char *nick, char *ping, char *connection, char *userID, char *status){
	LVITEM LvItem;
	memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem.mask = LVIF_TEXT;   // Text Style
	LvItem.cchTextMax = 1024; // Max size of test
	LvItem.iItem = 0;          // choose item  
	LvItem.iSubItem = 0;       // Put in first coluom
	LvItem.pszText = userID; // Text to display (can be from a char variable) (Items)
	SendMessage(lstUserlist,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview

	LvItem.iSubItem = 1;
	LvItem.pszText = nick;
	SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 2;
	LvItem.pszText = ping;
	SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 3;
	LvItem.pszText = connection;
	SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 4;
	LvItem.pszText = status;
	SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems

	ListView_SortItemsEx(lstUserlist, lstUserlistCompareFunc, 0);
}


void userlistAdditemAll(){
	int w;
	LVITEM LvItem;

	for(w = 0; w < tempUserCount; w++){
		memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members
		//  Setting properties Of members:
		LvItem.mask = LVIF_TEXT;   // Text Style
		LvItem.cchTextMax = 1024; // Max size of test
		LvItem.iItem = 0;          // choose item  
		LvItem.iSubItem = 0;       // Put in first coluom
		LvItem.pszText = tempUsers[w].userID; // Text to display (can be from a char variable) (Items)
		SendMessage(lstUserlist,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview

		LvItem.iSubItem = 1;
		LvItem.pszText = tempUsers[w].nick;
		SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
		LvItem.iSubItem = 2;
		LvItem.pszText = tempUsers[w].ping;
		SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
		LvItem.iSubItem = 3;
		LvItem.pszText = tempUsers[w].type;
		SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
		LvItem.iSubItem = 4;
		LvItem.pszText = tempUsers[w].status;
		SendMessage(lstUserlist,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	}

	ListView_SortItemsEx(lstUserlist, lstUserlistCompareFunc, 0);
}


int CALLBACK lstRecentlistCompareFunc(LPARAM i1, LPARAM i2, LPARAM){
    TCHAR tsz1[1024];
    TCHAR tsz2[1024];

    // Get the text of the two items.  Notice we get the text
    // for the column that we selected, specified by lstUserlistColumn.

    ListView_GetItemText(lstRecentList, i1, lstRecentlistColumn, tsz1, 1024);
    ListView_GetItemText(lstRecentList, i2, lstRecentlistColumn, tsz2, 1024);

    if(lstRecentlistColumn > -1 && lstRecentlistColumn < 4){
		if(recentSwitch == false){
			return _tcscmp(strToLower(tsz1), strToLower(tsz2));
		}
		else{
			return _tcscmp(strToLower(tsz2), strToLower(tsz1));
		}
    }
    return 0;
}

void recentlistAdditem(char *server, char *ip, char *location){
	LVFINDINFO c;
	LVITEM b;
	char tempServer[1024];
	char tempLocation[1024];
	LVITEM LvItem;

	strcpy(tempServer, server);
	strcpy(tempLocation, location);

	memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem.mask = LVIF_TEXT;   // Text Style
	LvItem.cchTextMax = 1024; // Max size of test
	LvItem.iItem = 0;          // choose item 

	c.psz = ip;
	c.flags = LVFI_STRING;
	int pos = SendMessage(lstRecentList, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);
	
	if(pos > -1){
		if(strcmp(server, "NA") == 0){
			b.mask = LVIF_TEXT;
			b.iItem = pos;
			b.iSubItem = 1;
			b.pszText = tempServer;
			b.cchTextMax = 1024;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
		}
		if(strcmp(location, "NA") == 0){
			b.mask = LVIF_TEXT;
			b.iItem = pos;
			b.iSubItem = 2;
			b.cchTextMax = 1024;
			b.pszText = tempLocation;
			SendMessage(lstRecentList, LVM_GETITEM, 0, (LPARAM) &b);
		}

		SendMessage(lstRecentList, LVM_DELETEITEM, (WPARAM) pos, 0);
	}



	LvItem.iSubItem = 0;
	LvItem.pszText = ip;
	SendMessage(lstRecentList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 1;       // Put in first coluom
	LvItem.pszText = tempServer; // Text to display (can be from a char variable) (Items)
	SendMessage(lstRecentList,LVM_SETITEM,0,(LPARAM)&LvItem); // Send info to the Listview
	LvItem.iSubItem = 2;
	LvItem.pszText = tempLocation;
	SendMessage(lstRecentList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems

	ListView_SortItemsEx(lstRecentList, lstRecentlistCompareFunc, 0);
}

int CALLBACK lstWaitinglistCompareFunc(LPARAM i1, LPARAM i2, LPARAM){
    TCHAR tsz1[1024];
    TCHAR tsz2[1024];

    // Get the text of the two items.  Notice we get the text
    // for the column that we selected, specified by lstUserlistColumn.

    ListView_GetItemText(lstWaitingList, i1, lstWaitinglistColumn, tsz1, 1024);
    ListView_GetItemText(lstWaitingList, i2, lstWaitinglistColumn, tsz2, 1024);

    if(lstWaitinglistColumn > -1 && lstWaitinglistColumn < 7){
		if(waitingSwitch == false){
			return _tcscmp(strToLower(tsz1), strToLower(tsz2));
		}
		else{
			return _tcscmp(strToLower(tsz2), strToLower(tsz1));
		}
    }
    return 0;
}

void waitinglistAdditem(char *game, char *emulator, char *username, char *server, char *ip, char *location, char *numPlayers){
	LVITEM LvItem;
	memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem.mask = LVIF_TEXT;   // Text Style
	LvItem.cchTextMax = 1024; // Max size of test
	LvItem.iItem = 0;          // choose item  
	LvItem.iSubItem = 0;       // Put in first coluom
	LvItem.pszText = game; // Text to display (can be from a char variable) (Items)
	SendMessage(lstWaitingList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview
	LvItem.iSubItem = 1;
	LvItem.pszText = emulator;
	SendMessage(lstWaitingList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 2;
	LvItem.pszText = username;
	SendMessage(lstWaitingList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 3;
	LvItem.pszText = server;
	SendMessage(lstWaitingList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 4;
	LvItem.pszText = ip;
	SendMessage(lstWaitingList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 5;
	LvItem.pszText = location;
	SendMessage(lstWaitingList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 6;
	LvItem.pszText = numPlayers;
	SendMessage(lstWaitingList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems

	ListView_SortItemsEx(lstWaitingList, lstWaitinglistCompareFunc, 0);
}


int CALLBACK lstFavoritelistCompareFunc(LPARAM i1, LPARAM i2, LPARAM){
    TCHAR tsz1[1024];
    TCHAR tsz2[1024];

    // Get the text of the two items.  Notice we get the text
    // for the column that we selected, specified by lstUserlistColumn.

    ListView_GetItemText(lstFavoriteList, i1, lstFavoritelistColumn, tsz1, 1024);
    ListView_GetItemText(lstFavoriteList, i2, lstFavoritelistColumn, tsz2, 1024);

    if(lstFavoritelistColumn > -1 && lstFavoritelistColumn < 4){
		if(favoriteSwitch == false){
			return _tcscmp(strToLower(tsz1), strToLower(tsz2));
		}
		else{
			return _tcscmp(strToLower(tsz2), strToLower(tsz1));
		}
    }
    return 0;
}

void favoritelistAdditem(char *server, char *ip, char *location, char *comments){
	LVFINDINFO c;

	c.psz = ip;
	c.flags = LVFI_STRING;
	int pos = SendMessage(lstFavoriteList, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);
	
	if(pos > -1)
		SendMessage(lstFavoriteList, LVM_DELETEITEM, (WPARAM) pos, 0);


	LVITEM LvItem;
	memset(&LvItem,0,sizeof(LvItem)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem.mask = LVIF_TEXT;   // Text Style
	LvItem.cchTextMax = 1024; // Max size of test
	LvItem.iItem = 0;          // choose item  

	LvItem.iSubItem = 0;       // Put in first coluom
	LvItem.pszText = ip; // Text to display (can be from a char variable) (Items)
	SendMessage(lstFavoriteList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send info to the Listview
	LvItem.iSubItem = 1;
	LvItem.pszText = server;
	SendMessage(lstFavoriteList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 2;
	LvItem.pszText = location;
	SendMessage(lstFavoriteList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
	LvItem.iSubItem = 3;
	LvItem.pszText = comments;
	SendMessage(lstFavoriteList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems

	ListView_SortItemsEx(lstFavoriteList, lstFavoritelistCompareFunc, 0);
}


void gameUserlistAdditem(char nick[], char ping[], char connection[], char userID[]){
	LVITEM LvItem2;
	memset(&LvItem2,0,sizeof(LvItem2)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem2.mask = LVIF_TEXT;   // Text Style
	LvItem2.cchTextMax = 1024; // Max size of test
	LvItem2.iItem = 0;          // choose item  
	LvItem2.iSubItem = 0;       // Put in first coluom
	LvItem2.pszText = userID; // Text to display (can be from a char variable) (Items)
	SendMessage(lstGameUserlist,LVM_INSERTITEM,0,(LPARAM)&LvItem2); // Send info to the Listview
	LvItem2.iSubItem = 1;
	LvItem2.pszText = nick;
	SendMessage(lstGameUserlist,LVM_SETITEM,0,(LPARAM)&LvItem2); // Enter text to SubItems
	LvItem2.iSubItem = 2;
	LvItem2.pszText = ping;
	SendMessage(lstGameUserlist,LVM_SETITEM,0,(LPARAM)&LvItem2); // Enter text to SubItems
	LvItem2.iSubItem = 3;
	LvItem2.pszText = connection;
	SendMessage(lstGameUserlist,LVM_SETITEM,0,(LPARAM)&LvItem2); // Enter text to SubItems
}

void gamelistAdditemAll(){
	LVITEM LvItem1;
	int w;

	for(w = 0; w < tempGameCount; w++){
		memset(&LvItem1,0,sizeof(LvItem1)); // Zero struct's Members
		//  Setting properties Of members:
		LvItem1.mask = LVIF_TEXT;   // Text Style
		LvItem1.cchTextMax = 1024; // Max size of test
		LvItem1.iItem = 0;          // choose item  
		LvItem1.iSubItem = 0;       // Put in first coluom
		LvItem1.pszText = tempGames[w].gameID; // Text to display (can be from a char variable) (Items)
		SendMessage(lstGamelist,LVM_INSERTITEM,0,(LPARAM)&LvItem1); // Send info to the Listview
		LvItem1.iSubItem = 1;
		LvItem1.pszText = tempGames[w].game;
		SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
		LvItem1.iSubItem = 2;
		LvItem1.pszText = tempGames[w].emulator;
		SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
		LvItem1.iSubItem = 3;
		LvItem1.pszText = tempGames[w].owner;
		SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
		LvItem1.iSubItem = 4;
		LvItem1.pszText = tempGames[w].status;
		SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
		LvItem1.iSubItem = 5;
		LvItem1.pszText = tempGames[w].users;
		SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
	}

	ListView_SortItemsEx(lstGamelist, lstGamelistCompareFunc, 0);
}


void gamelistAdditem(char *game, char *version, char *owner, char *status, char *numUsers, char *gameID){
	LVITEM LvItem1;
	memset(&LvItem1,0,sizeof(LvItem1)); // Zero struct's Members
	//  Setting properties Of members:
	LvItem1.mask = LVIF_TEXT;   // Text Style
	LvItem1.cchTextMax = 1024; // Max size of test
	LvItem1.iItem = 0;          // choose item  
	LvItem1.iSubItem = 0;       // Put in first coluom
	LvItem1.pszText = gameID; // Text to display (can be from a char variable) (Items)
	SendMessage(lstGamelist,LVM_INSERTITEM,0,(LPARAM)&LvItem1); // Send info to the Listview
	LvItem1.iSubItem = 1;
	LvItem1.pszText = game;
	SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
	LvItem1.iSubItem = 2;
	LvItem1.pszText = version;
	SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
	LvItem1.iSubItem = 3;
	LvItem1.pszText = owner;
	SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
	LvItem1.iSubItem = 4;
	LvItem1.pszText = status;
	SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems
	LvItem1.iSubItem = 5;
	LvItem1.pszText = numUsers;
	SendMessage(lstGamelist,LVM_SETITEM,0,(LPARAM)&LvItem1); // Enter text to SubItems

	ListView_SortItemsEx(lstGamelist, lstGamelistCompareFunc, 0);
}

void showRecentlist(){
		chatroom = false;
		ShowWindow(lstGamelist, SW_HIDE);
		ShowWindow(txtGameChatroom, SW_HIDE);
		ShowWindow(txtChatroom, SW_HIDE);
		ShowWindow(lstUserlist, SW_HIDE);
		ShowWindow(btnChat, SW_HIDE);
		ShowWindow(txtChat, SW_HIDE);
		ShowWindow(lstGameUserlist, SW_HIDE);
		ShowWindow(txtGameChat, SW_HIDE);
		ShowWindow(btnGameChat, SW_HIDE);
		ShowWindow(btnGameStart, SW_HIDE);
		ShowWindow(btnGameKick, SW_HIDE);
		ShowWindow(btnGameLeave, SW_HIDE);
		ShowWindow(txtGame, SW_HIDE);
		ShowWindow(btnCreate, SW_HIDE);
		ShowWindow(btnJoin, SW_HIDE);
		ShowWindow(btnAway, SW_HIDE);

		ShowWindow(lstRecentList, SW_SHOW);
		ShowWindow(lstWaitingList, SW_HIDE);
		ShowWindow(lstFavoriteList, SW_HIDE);
		ShowWindow(lstServerListK, SW_HIDE);
		ShowWindow(lstServerList3D, SW_HIDE);
		ShowWindow(sTab, SW_SHOW);

		if(pingingK == true || pinging3D == true)
			return;

		int num;
		char str[1024];
		num = SendMessage(lstRecentList, LVM_GETITEMCOUNT, 0, 0);
		if(num < 1){
			SetWindowText(form1, "SupraclientCPPE https://god-weapon.github.io");
		}
		else{
			wsprintf(str, "%i Recent Servers", num);
			SetWindowText(form1, str);
		}
}


void showWaitinglist(){
		chatroom = false;
		ShowWindow(lstGamelist, SW_HIDE);
		ShowWindow(txtGameChatroom, SW_HIDE);
		ShowWindow(txtChatroom, SW_HIDE);
		ShowWindow(lstUserlist, SW_HIDE);
		ShowWindow(btnChat, SW_HIDE);
		ShowWindow(txtChat, SW_HIDE);
		ShowWindow(lstGameUserlist, SW_HIDE);
		ShowWindow(txtGameChat, SW_HIDE);
		ShowWindow(btnGameChat, SW_HIDE);
		ShowWindow(btnGameStart, SW_HIDE);
		ShowWindow(btnGameKick, SW_HIDE);
		ShowWindow(btnGameLeave, SW_HIDE);
		ShowWindow(txtGame, SW_HIDE);
		ShowWindow(btnCreate, SW_HIDE);
		ShowWindow(btnJoin, SW_HIDE);
		ShowWindow(btnAway, SW_HIDE);

		ShowWindow(lstRecentList, SW_HIDE);
		ShowWindow(lstWaitingList, SW_SHOW);
		ShowWindow(lstFavoriteList, SW_HIDE);
		ShowWindow(lstServerListK, SW_HIDE);
		ShowWindow(lstServerList3D, SW_HIDE);
		ShowWindow(sTab, SW_SHOW);

		if(pingingK == true || pinging3D == true)
			return;

		int num;
		char str[1024];
		num = SendMessage(lstWaitingList, LVM_GETITEMCOUNT, 0, 0);
		if(num < 2){
			SetWindowText(form1, "SupraclientCPPE https://god-weapon.github.io");
		}
		else{
			wsprintf(str, "%i Waiting Games", num);
			SetWindowText(form1, str);
		}
}


void showFavoritelist(){
		chatroom = false;
		ShowWindow(lstGamelist, SW_HIDE);
		ShowWindow(txtGameChatroom, SW_HIDE);
		ShowWindow(txtChatroom, SW_HIDE);
		ShowWindow(lstUserlist, SW_HIDE);
		ShowWindow(btnChat, SW_HIDE);
		ShowWindow(txtChat, SW_HIDE);
		ShowWindow(lstGameUserlist, SW_HIDE);
		ShowWindow(txtGameChat, SW_HIDE);
		ShowWindow(btnGameChat, SW_HIDE);
		ShowWindow(btnGameStart, SW_HIDE);
		ShowWindow(btnGameKick, SW_HIDE);
		ShowWindow(btnGameLeave, SW_HIDE);
		ShowWindow(txtGame, SW_HIDE);
		ShowWindow(btnCreate, SW_HIDE);
		ShowWindow(btnJoin, SW_HIDE);
		ShowWindow(btnAway, SW_HIDE);

		ShowWindow(lstRecentList, SW_HIDE);
		ShowWindow(lstWaitingList, SW_HIDE);
		ShowWindow(lstFavoriteList, SW_SHOW);
		ShowWindow(lstServerList3D, SW_HIDE);
		ShowWindow(lstServerListK, SW_HIDE);
		ShowWindow(sTab, SW_SHOW);

		if(pingingK == true || pinging3D == true)
			return;

		int num;
		char str[1024];
		num = SendMessage(lstFavoriteList, LVM_GETITEMCOUNT, 0, 0);
		if(num < 1){
			SetWindowText(form1, "SupraclientCPPE https://god-weapon.github.io");
		}
		else{
			wsprintf(str, "%i Favorite Servers", num);
			SetWindowText(form1, str);
		}
}

void showServerlistK(){
		chatroom = false;
		ShowWindow(lstGamelist, SW_HIDE);
		ShowWindow(txtGameChatroom, SW_HIDE);
		ShowWindow(txtChatroom, SW_HIDE);
		ShowWindow(lstUserlist, SW_HIDE);
		ShowWindow(btnChat, SW_HIDE);
		ShowWindow(txtChat, SW_HIDE);
		ShowWindow(lstGameUserlist, SW_HIDE);
		ShowWindow(txtGameChat, SW_HIDE);
		ShowWindow(btnGameChat, SW_HIDE);
		ShowWindow(btnGameStart, SW_HIDE);
		ShowWindow(btnGameKick, SW_HIDE);
		ShowWindow(btnGameLeave, SW_HIDE);
		ShowWindow(txtGame, SW_HIDE);
		ShowWindow(btnCreate, SW_HIDE);
		ShowWindow(btnJoin, SW_HIDE);
		ShowWindow(btnAway, SW_HIDE);

		ShowWindow(lstRecentList, SW_HIDE);
		ShowWindow(lstWaitingList, SW_HIDE);
		ShowWindow(lstFavoriteList, SW_HIDE);
		ShowWindow(lstServerListK, SW_SHOW);
		ShowWindow(lstServerList3D, SW_HIDE);
		ShowWindow(sTab, SW_SHOW);

		if(pingingK == true || pinging3D == true)
			return;

		int num;
		char str[1024];
		num = SendMessage(lstServerListK, LVM_GETITEMCOUNT, 0, 0);
		if(num < 2){
			SetWindowText(form1, "SupraclientCPPE https://god-weapon.github.io");
		}
		else{
			wsprintf(str, "%i Kaillera Servers", num);
			SetWindowText(form1, str);
		}
}

void showServerlist3D(){
		chatroom = false;
		ShowWindow(lstGamelist, SW_HIDE);
		ShowWindow(txtGameChatroom, SW_HIDE);
		ShowWindow(txtChatroom, SW_HIDE);
		ShowWindow(lstUserlist, SW_HIDE);
		ShowWindow(btnChat, SW_HIDE);
		ShowWindow(txtChat, SW_HIDE);
		ShowWindow(lstGameUserlist, SW_HIDE);
		ShowWindow(txtGameChat, SW_HIDE);
		ShowWindow(btnGameChat, SW_HIDE);
		ShowWindow(btnGameStart, SW_HIDE);
		ShowWindow(btnGameKick, SW_HIDE);
		ShowWindow(btnGameLeave, SW_HIDE);
		ShowWindow(txtGame, SW_HIDE);
		ShowWindow(btnCreate, SW_HIDE);
		ShowWindow(btnJoin, SW_HIDE);
		ShowWindow(btnAway, SW_HIDE);

		ShowWindow(lstRecentList, SW_HIDE);
		ShowWindow(lstWaitingList, SW_HIDE);
		ShowWindow(lstFavoriteList, SW_HIDE);
		ShowWindow(lstServerListK, SW_HIDE);
		ShowWindow(lstServerList3D, SW_SHOW);
		ShowWindow(sTab, SW_SHOW);

		if(pinging3D == true || pingingK == true)
			return;

		int num;
		char str[1024];
		num = SendMessage(lstServerList3D, LVM_GETITEMCOUNT, 0, 0);
		if(num < 2){
			SetWindowText(form1, "SupraclientCPPE https://god-weapon.github.io");
		}
		else{
			wsprintf(str, "%i EmuLinker Servers", num);
			SetWindowText(form1, str);
		}
}


void showChatroom(bool show){
	if(show == true){
		ShowWindow(lstServerListK, SW_HIDE);
		ShowWindow(lstServerList3D, SW_HIDE);
		ShowWindow(lstWaitingList, SW_HIDE);
		ShowWindow(lstFavoriteList, SW_HIDE);
		ShowWindow(lstRecentList, SW_HIDE);
		ShowWindow(txtChatroom, SW_SHOW);
		ShowWindow(lstUserlist, SW_SHOW);
		ShowWindow(btnChat, SW_SHOW);
		ShowWindow(txtChat, SW_SHOW);
		ShowWindow(btnCreate, SW_SHOW);
		ShowWindow(btnJoin, SW_SHOW);
		lastTabServer = SendMessage(sTab, TCM_GETCURSEL, 0, 0);
		ShowWindow(sTab, SW_HIDE);
		if(gameroom == true)
			showGameroom(true);
		else
			showGameroom(false);
		SetWindowText(btnChatroom, "Servers");
		SetWindowText(form1, myServer);
		chatroom = true;
	}
	else{
		ShowWindow(lstGamelist, SW_HIDE);
		ShowWindow(txtGameChatroom, SW_HIDE);
		ShowWindow(txtChatroom, SW_HIDE);
		ShowWindow(lstUserlist, SW_HIDE);
		ShowWindow(btnChat, SW_HIDE);
		ShowWindow(txtChat, SW_HIDE);
		ShowWindow(lstGameUserlist, SW_HIDE);
		ShowWindow(txtGameChat, SW_HIDE);
		ShowWindow(btnLagStat, SW_HIDE);
		ShowWindow(btnGameChat, SW_HIDE);
		ShowWindow(btnDrop, SW_HIDE);
		ShowWindow(btnGameStart, SW_HIDE);
		ShowWindow(btnGameKick, SW_HIDE);
		ShowWindow(btnGameLeave, SW_HIDE);
		ShowWindow(txtGame, SW_HIDE);
		ShowWindow(btnCreate, SW_HIDE);
		ShowWindow(btnJoin, SW_HIDE);

		ShowWindow(sTab, SW_SHOW);
		SendMessage(sTab, TCM_SETCURSEL, (WPARAM) lastTabServer, 0);
		
		if(lastTabServer == 0)
			showServerlistK();
		else if(lastTabServer == 1)
			showServerlist3D();
		else if(lastTabServer == 2)
			showRecentlist();
		else if(lastTabServer == 3)
			showFavoritelist();
		else if(lastTabServer == 4)
			showWaitinglist();

		SetWindowText(btnChatroom, "Chatroom");
		chatroom = false;
	}
}

void showGameroom(bool show){
	if(show == true){
		ShowWindow(lstGamelist, SW_HIDE);
		ShowWindow(txtGameChatroom, SW_SHOW);
		ShowWindow(lstGameUserlist, SW_SHOW);
		ShowWindow(txtGameChat, SW_SHOW);
		ShowWindow(btnGameChat, SW_SHOW);
		ShowWindow(btnGameStart, SW_SHOW);
		ShowWindow(btnGameKick, SW_SHOW);
		ShowWindow(btnGameLeave, SW_SHOW);
		ShowWindow(btnDrop, SW_SHOW);
		ShowWindow(txtGame, SW_SHOW);
		ShowWindow(btnAway, SW_SHOW);
		ShowWindow(btnLagStat, SW_SHOW);

		SetWindowText(btnCreate, "Swap");
		SetWindowText(btnJoin, "Ad");
		gameroom = true;
		swapp = true;
	}
	else{
		ShowWindow(lstGamelist, SW_SHOW);
		ShowWindow(txtGameChatroom, SW_HIDE);
		ShowWindow(lstGameUserlist, SW_HIDE);
		ShowWindow(txtGameChat, SW_HIDE);
		ShowWindow(btnGameChat, SW_HIDE);
		ShowWindow(btnDrop, SW_HIDE);
		ShowWindow(btnGameStart, SW_HIDE);
		ShowWindow(btnGameKick, SW_HIDE);
		ShowWindow(btnGameLeave, SW_HIDE);
		ShowWindow(btnAway, SW_HIDE);
		ShowWindow(txtGame, SW_HIDE);
		ShowWindow(btnLagStat, SW_HIDE);
		if(myGameID == -1){
			SetWindowText(btnCreate, "Create");
			SetWindowText(btnJoin, "Join");
			swapp = false;
		}
		gameroom = false;
	}
}


unsigned long cStrToInt(char *str){
	unsigned long num = 0;
    for (unsigned long i = 0; str[i] != '\0'; i++){
		num = num * 10;
        num = num + (str[i] - '0');
     }
     return(num); 
}

//0x01 - User Quit Notification
void userQuitNotification(unsigned short position, int slot){
	unsigned short userID;
	char strUserID[1024];
	char temp[2048];
	char nick[1024];
	char quit[1024];
	short strSize;
	short i;

	
	i = position;
	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	
	//UserID
    userID = *(unsigned short *) &myBuff[slot].myBuff[i];
	wsprintf(strUserID, "%i", userID);
	i = i + 2;

	//Quit Message
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(quit, &myBuff[slot].myBuff[i], strSize + 1);
	

	//Find UserID in Userlist
	LVFINDINFO c;
	c.psz = strUserID;
	c.flags = LVFI_STRING;
	int pos = SendMessage(lstUserlist, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);
	
	//Remove Game from Userlist
	if(pos > -1)
		SendMessage(lstUserlist, LVM_DELETEITEM, (WPARAM) pos, 0);

	
	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> Quit the Server: ");
	strcat(temp, quit);
	strcat(temp,"\r\n");
	

	//Display
	if(joinChatValue == BST_CHECKED)
		displayAndAutoScrollRichEdit(txtChatroom, temp, RGB(122, 122, 122));
	displayStats();

	//Log
	saveChatroomLog(temp);
}


//0x01 - User Quit Request
void userQuitRequest(){
	if (loggedIn == true) {
		char dataToBeSent[1024];

		dataToBeSent[0] = 0x00;
		dataToBeSent[1] = 0xFF;
		dataToBeSent[2] = 0xFF;

		short lenQuit = (short)GetWindowTextLength(txtQuit);
		GetWindowText(txtQuit, &dataToBeSent[3], lenQuit + 1);

		constructPacket(dataToBeSent, 3 + lenQuit + 1, 0x01);

		imAway = false;
		loggedIn = false;
	}
	//cleanup
	supraCleanup(0, 0);
}


//0x02 - User Joined
void userJoined(unsigned short position, int slot){
	char nick[1024];
	char strUserID[1024];
	char temp[2048];
	unsigned short userID;
	unsigned long ping;
	char strPing[1024];
	unsigned char connectionType;
	char strConnectionType[1024];
	short strSize;
	short i;
	
	i = position;
	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	
	//UserID
    userID = *(unsigned short *) &myBuff[slot].myBuff[i];
	wsprintf(strUserID, "%i", userID);
	i = i + 2;

	//Ping
	ping = *(unsigned long *) &myBuff[slot].myBuff[i];
	wsprintf(strPing, "%i", ping);
	


	if(myUserID == -1){
		myUserID = userID;
		myPing = ping;
		displayChatroomAsServer("Connected!");
		SetWindowText(form1, myServer);
		userlistAdditemAll();
		gamelistAdditemAll();
		saveChatroomLog("####################\r\n");
		saveChatroomLog("NEW SESSION CHATROOM\r\n");
		saveChatroomLog("####################\r\n");
	}

	//Connection Type
	connectionType =  *(unsigned char *) &myBuff[slot].myBuff[i + 4];
	if(connectionType == lan)
		memcpy(strConnectionType, "LAN\0", 4);
	else if(connectionType == excellent)
		memcpy(strConnectionType, "Excellent\0", 10);
	else if(connectionType == good)
		memcpy(strConnectionType, "Good\0", 5);
	else if(connectionType == average)
		memcpy(strConnectionType, "Average\0", 8);
	else if(connectionType == low)
		memcpy(strConnectionType, "Low\0", 4);
	else if(connectionType == bad)
		memcpy(strConnectionType, "Bad\0", 4);
	else
		memcpy(strConnectionType, "ERROR\0", 5);	

	
	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> Joined the Server\r\n");
	

	//Display
	if(joinChatValue == BST_CHECKED)
		displayAndAutoScrollRichEdit(txtChatroom, temp, RGB(122, 122, 122));

	//Add to Userlist
	userlistAdditem(nick, strPing, strConnectionType, strUserID, "Idle");
	displayStats();

	//Log
	saveChatroomLog(temp);
}


//0x03 - User Login Information
void userLoginInformation(short lenUsername, short lenEmulator){
	char dataToBeSent[1024];

	//Get/Set Username
	GetWindowText(txtUsername, dataToBeSent, lenUsername + 1);
	GetWindowText(txtUsername, username, lenUsername + 1);
	
	//Get Emulator
	strcpy(&dataToBeSent[lenUsername + 1], emulator);

	//Get Connection Type
	dataToBeSent[lenUsername + 1 + lenEmulator + 1] = connectionType;
	
    constructPacket(dataToBeSent, (lenUsername + 1 + lenEmulator + 1 + 1), 0x03);
}


//0x04 - Server Status
void serverStatus(unsigned short position, int slot){
    unsigned long numOfUsers;
    unsigned long numOfGames;
    unsigned short i;
    unsigned short w;
	short strSize;

    char nick[1024];
    unsigned long ping;
	char strPing[1024];
    unsigned char status;
	char strStatus[1024];
    unsigned short userID;
	char strUserID[1024];
    unsigned char connectionType;
	char strConnectionType[1024];

    char game[1024];
    unsigned long gameID;
	char strGameID[1024];
    char owner[1024];
    char emulator[1024];
    char maxUsers[1024];

	loggedIn = true;

    ///Get Number of Users
	i = position + 1;
    numOfUsers = *(unsigned long *) &myBuff[slot].myBuff[i];
    i = i + 4;
	
	//Get Number of Games
    numOfGames = *(unsigned long *) &myBuff[slot].myBuff[i];
    i = i + 4;

//----USERLIST----
    //For Each User
    for(w = 0; w < numOfUsers; w++){
        //Nick
		strSize = strlen(&myBuff[slot].myBuff[i]);
		memcpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
        i = i + strSize + 1;

        //Ping
		ping = *(unsigned long *) &myBuff[slot].myBuff[i];
        wsprintf(strPing, "%i", ping);
		i = i + 4;
            
        //Connection Status
        status = *(unsigned char *) &myBuff[slot].myBuff[i];
		i = i + 1;
		if(status == 0)
			memcpy(strStatus,"Playing\0", 8); //Netsync
		else if(status == 1)
			memcpy(strStatus, "Idle\0", 5);
		else if(status == 2)
			memcpy(strStatus, "Playing\0", 8);
		else
			memcpy(strStatus, "ERROR\0", 6);

        //UserID
		userID = *(unsigned short *) &myBuff[slot].myBuff[i];
        wsprintf(strUserID, "%i", userID);
		i = i + 2;
            
        //Connection Type
        connectionType = *(unsigned char *) &myBuff[slot].myBuff[i];
        i = i + 1;
		if(connectionType == lan)
			memcpy(strConnectionType, "LAN\0", 4);
		else if(connectionType == excellent)
			memcpy(strConnectionType, "Excellent\0", 10);
		else if(connectionType == good)
			memcpy(strConnectionType, "Good\0", 5);
		else if(connectionType == average)
			memcpy(strConnectionType, "Average\0", 8);
		else if(connectionType == low)
			memcpy(strConnectionType, "Low\0", 4);			
		else if(connectionType == bad)
			memcpy(strConnectionType, "Bad\0", 4);
		else
			memcpy(strStatus, "ERROR", 5);

		//Add User
		strcpy(tempUsers[tempUserCount].nick, nick);
		strcpy(tempUsers[tempUserCount].ping, strPing);
		strcpy(tempUsers[tempUserCount].status, strStatus);
		strcpy(tempUsers[tempUserCount].userID, strUserID);
		strcpy(tempUsers[tempUserCount].type, strConnectionType);
		tempUserCount = tempUserCount + 1;
		//userlistAdditem(nick, strPing, strConnectionType, strUserID, strStatus);
    }


//----GAMELIST----

    //For Each Game
    for(w = 0; w < numOfGames; w++){
        //Game
        strSize = strlen(&myBuff[slot].myBuff[i]);
		memcpy(game, &myBuff[slot].myBuff[i], strSize + 1);
        i = i + strSize + 1;
            
        //GameID
		gameID = *(unsigned long*) &myBuff[slot].myBuff[i];
		wsprintf(strGameID, "%i", gameID);
		i = i + 4;
            
       //Emulator
        strSize = strlen(&myBuff[slot].myBuff[i]);
		memcpy(emulator, &myBuff[slot].myBuff[i], strSize + 1);
        i = i + strSize + 1;
            
        //Owner
        strSize = strlen(&myBuff[slot].myBuff[i]);
		memcpy(owner, &myBuff[slot].myBuff[i], strSize + 1);
        i = i + strSize + 1;
            
        //Users/Maxusers
        strSize = strlen(&myBuff[slot].myBuff[i]);
		memcpy(maxUsers, &myBuff[slot].myBuff[i], strSize + 1);
        i = i + strSize + 1;

        //Status
        status = *(unsigned char *) &myBuff[slot].myBuff[i];
        i = i + 1;
		if(status == 0)
			memcpy(strStatus, "Waiting\0", 8);
		else if(status == 1)
			memcpy(strStatus, "Playing\0", 8);
		else  if(status == 2)
			memcpy(strStatus, "Playing\0", 8);//Netsync
		else
			memcpy(strStatus, "ERROR\0", 6);

		//Add Game
		strcpy(tempGames[tempGameCount].game, game);
		strcpy(tempGames[tempGameCount].emulator, emulator);
		strcpy(tempGames[tempGameCount].gameID, strGameID);
		strcpy(tempGames[tempGameCount].owner, owner);
		strcpy(tempGames[tempGameCount].status, strStatus);
		strcpy(tempGames[tempGameCount].users, maxUsers);
		tempGameCount = tempGameCount + 1;
		//gamelistAdditem(game, emulator, owner, strStatus, maxUsers, strGameID);
	}	
	//displayStats();
	displayChatroomAsServer("Gathering Users and Games...");
}




//0x05 - Server to Client ACK
void serverToClientAck(){
	char ack[17];

    ack[0] = 0;
    
    ack[1] = 0;
    ack[2] = 0;
    ack[3] = 0;
    ack[4] = 0;
    
    ack[5] = 1;
    ack[6] = 0;
    ack[7] = 0;
    ack[8] = 0;
    
    ack[9] = 2;
    ack[10] = 0;
    ack[11] = 0;
    ack[12] = 0;
    
    ack[13] = 3;
    ack[14] = 0;
    ack[15] = 0;
    ack[16] = 0;
	//Sleep(94);
    constructPacket(ack, 17, 0x06);
	//clientToServerAck();
}

/*
//0x05 - Client to Server ACK
void clientToServerAck(){
	char ack[17];

    ack[0] = 0;
    
    ack[1] = 0;
    ack[2] = 0;
    ack[3] = 0;
    ack[4] = 0;
    
    ack[5] = 1;
    ack[6] = 0;
    ack[7] = 0;
    ack[8] = 0;
    
    ack[9] = 2;
    ack[10] = 0;
    ack[11] = 0;
    ack[12] = 0;
    
    ack[13] = 3;
    ack[14] = 0;
    ack[15] = 0;
    ack[16] = 0;
	//Sleep(10);
    constructPacket(ack, 17, 0x06);
}
*/

//0x07 - Global Chat Notification
void globalChatNotification(unsigned short position, int slot){
	char temp[2048];
	char nick[1024];
	char message[1024];
	short strSize;
	unsigned short i;
	
  
	i = position;
	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	
	//Message
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(message, &myBuff[slot].myBuff[i], strSize + 1);
	
	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> ");
	strcat(temp,message);
	strcat(temp,"\r\n");
	
	//Display
	displayAndAutoScrollRichEdit(txtChatroom, temp, RGB(0, 0, 0));

	//Log
	saveChatroomLog(temp);
}

void showOptions(char show){
	//Chatroom Options
	if(show == 0){
		ShowWindow(lblServerIP, SW_HIDE);
		ShowWindow(txtServerIP, SW_HIDE);
		ShowWindow(txtUsername, SW_HIDE);
		ShowWindow(lblUsername, SW_HIDE);
		ShowWindow(lblQuit, SW_HIDE);
		ShowWindow(txtQuit, SW_HIDE);
		ShowWindow(lblConnectionType, SW_HIDE);
		ShowWindow(cmbConnectionType, SW_HIDE);


		//ShowWindow(lblStats, SW_HIDE);
		ShowWindow(btnLogoff, SW_SHOW);
		ShowWindow(btnLogin, SW_SHOW);
		
		ShowWindow(btnChatroom, SW_SHOW);

		ShowWindow(chkBeep, SW_HIDE);
		ShowWindow(chkJoinChatGame, SW_HIDE);
		ShowWindow(chkUseCache, SW_HIDE);
		ShowWindow(chkUseScreenChat, SW_HIDE);
		ShowWindow(chkKeepGameChatLogs, SW_HIDE);
		ShowWindow(chkKeepChatLogs, SW_SHOW);
		ShowWindow(chkShowError, SW_SHOW);
		ShowWindow(chkDrop, SW_SHOW);
		ShowWindow(chkBlink, SW_HIDE);
		ShowWindow(chkJoinChat, SW_SHOW);
		ShowWindow(chkCreate, SW_SHOW);
		ShowWindow(chkJoinDbl, SW_SHOW);

		ShowWindow(txtMaxUsers, SW_HIDE);
		ShowWindow(lblMaxUsers, SW_HIDE);
		ShowWindow(txtMaxPing, SW_HIDE);
		ShowWindow(lblMaxPing, SW_HIDE);
		ShowWindow(chkFakeP2P, SW_HIDE);
		ShowWindow(chkEmulinkerSF, SW_HIDE);
		ShowWindow(chkEmuRes, SW_HIDE);
		ShowWindow(chkConnRes, SW_HIDE);
		ShowWindow(btnVersion, SW_HIDE);

		/*ShowWindow(lblP2PServer, SW_HIDE);
		ShowWindow(txtP2PServer, SW_HIDE);
		ShowWindow(lblP2PPort, SW_HIDE);
		ShowWindow(txtP2PPort, SW_HIDE);
		ShowWindow(btnP2PStart, SW_HIDE);
		ShowWindow(btnP2PServer, SW_HIDE);
		ShowWindow(btnP2PConnect, SW_HIDE);*/
	}
	//Login Info
	else if(show == 1){
		ShowWindow(lblServerIP, SW_SHOW);
		ShowWindow(txtServerIP, SW_SHOW);
		ShowWindow(txtUsername, SW_SHOW);
		ShowWindow(lblUsername, SW_SHOW);
		ShowWindow(lblQuit, SW_SHOW);
		ShowWindow(txtQuit, SW_SHOW);
		ShowWindow(lblConnectionType, SW_SHOW);
		ShowWindow(cmbConnectionType, SW_SHOW);

		ShowWindow(lblStats, SW_SHOW);
		ShowWindow(btnLogoff, SW_SHOW);
		ShowWindow(btnLogin, SW_SHOW);

		ShowWindow(btnChatroom, SW_SHOW);
		ShowWindow(chkKeepGameChatLogs, SW_HIDE);
		ShowWindow(chkShowError, SW_HIDE);
		ShowWindow(chkKeepChatLogs, SW_HIDE);
		ShowWindow(chkDrop, SW_HIDE);
		ShowWindow(chkUseCache, SW_HIDE);
		ShowWindow(chkBlink, SW_HIDE);
		ShowWindow(chkBeep, SW_HIDE);
		ShowWindow(chkJoinChatGame, SW_HIDE);
		ShowWindow(chkJoinChat, SW_HIDE);
		ShowWindow(chkUseScreenChat, SW_HIDE);
		ShowWindow(chkCreate, SW_HIDE);
		ShowWindow(chkJoinDbl, SW_HIDE);

		ShowWindow(txtMaxUsers, SW_HIDE);
		ShowWindow(lblMaxUsers, SW_HIDE);
		ShowWindow(txtMaxPing, SW_HIDE);
		ShowWindow(lblMaxPing, SW_HIDE);
		ShowWindow(chkFakeP2P, SW_HIDE);
		ShowWindow(chkEmulinkerSF, SW_HIDE);
		ShowWindow(chkEmuRes, SW_HIDE);
		ShowWindow(chkConnRes, SW_HIDE);
		ShowWindow(btnVersion, SW_HIDE);

		/*ShowWindow(lblP2PServer, SW_HIDE);
		ShowWindow(txtP2PServer, SW_HIDE);
		ShowWindow(lblP2PPort, SW_HIDE);
		ShowWindow(txtP2PPort, SW_HIDE);
		ShowWindow(btnP2PStart, SW_HIDE);
		ShowWindow(btnP2PServer, SW_HIDE);
		ShowWindow(btnP2PConnect, SW_HIDE);*/
	}
	//Gameroom Options
	else if(show == 2){
		ShowWindow(lblServerIP, SW_HIDE);
		ShowWindow(txtServerIP, SW_HIDE);
		ShowWindow(txtUsername, SW_HIDE);
		ShowWindow(lblUsername, SW_HIDE);
		ShowWindow(lblQuit, SW_HIDE);
		ShowWindow(txtQuit, SW_HIDE);
		ShowWindow(lblConnectionType, SW_HIDE);
		ShowWindow(cmbConnectionType, SW_HIDE);
		ShowWindow(chkShowError, SW_HIDE);
		ShowWindow(chkDrop, SW_HIDE);
		ShowWindow(btnChatroom, SW_SHOW);
		ShowWindow(chkKeepGameChatLogs, SW_SHOW);
		ShowWindow(chkKeepChatLogs, SW_HIDE);
		ShowWindow(chkUseCache, SW_SHOW);
		ShowWindow(chkBlink, SW_SHOW);

		//ShowWindow(lblStats, SW_HIDE);
		ShowWindow(btnLogoff, SW_SHOW);
		ShowWindow(btnLogin, SW_SHOW);

		ShowWindow(chkBeep, SW_SHOW);
		ShowWindow(chkJoinChatGame, SW_SHOW);
		ShowWindow(chkUseScreenChat, SW_SHOW);
		ShowWindow(chkJoinChat, SW_HIDE);
		ShowWindow(chkCreate, SW_HIDE);
		ShowWindow(chkJoinDbl, SW_HIDE);

		ShowWindow(txtMaxUsers, SW_HIDE);
		ShowWindow(lblMaxUsers, SW_HIDE);
		ShowWindow(txtMaxPing, SW_HIDE);
		ShowWindow(lblMaxPing, SW_HIDE);
		ShowWindow(chkFakeP2P, SW_HIDE);
		ShowWindow(chkEmulinkerSF, SW_HIDE);
		ShowWindow(chkEmuRes, SW_HIDE);
		ShowWindow(chkConnRes, SW_HIDE);
		ShowWindow(btnVersion, SW_HIDE);

		/*ShowWindow(lblP2PServer, SW_HIDE);
		ShowWindow(txtP2PServer, SW_HIDE);
		ShowWindow(lblP2PPort, SW_HIDE);
		ShowWindow(txtP2PPort, SW_HIDE);
		ShowWindow(btnP2PStart, SW_HIDE);
		ShowWindow(btnP2PServer, SW_HIDE);
		ShowWindow(btnP2PConnect, SW_HIDE);*/
	}
	//Emulinker Options
	else if(show == 3){
		ShowWindow(lblServerIP, SW_HIDE);
		ShowWindow(txtServerIP, SW_HIDE);
		ShowWindow(txtUsername, SW_HIDE);
		ShowWindow(lblUsername, SW_HIDE);
		ShowWindow(lblQuit, SW_HIDE);
		ShowWindow(txtQuit, SW_HIDE);
		ShowWindow(lblConnectionType, SW_HIDE);
		ShowWindow(cmbConnectionType, SW_HIDE);
		ShowWindow(chkShowError, SW_HIDE);
		ShowWindow(chkDrop, SW_HIDE);
		ShowWindow(btnChatroom, SW_SHOW);
		ShowWindow(chkKeepGameChatLogs, SW_HIDE);
		ShowWindow(chkKeepChatLogs, SW_HIDE);
		ShowWindow(chkUseCache, SW_HIDE);
		ShowWindow(chkBlink, SW_HIDE);

		//ShowWindow(lblStats, SW_HIDE);
		ShowWindow(btnLogoff, SW_SHOW);
		ShowWindow(btnLogin, SW_SHOW);

		ShowWindow(chkBeep, SW_HIDE);
		ShowWindow(chkJoinChatGame, SW_HIDE);
		ShowWindow(chkUseScreenChat, SW_HIDE);
		ShowWindow(chkJoinChat, SW_HIDE);
		ShowWindow(chkCreate, SW_HIDE);
		ShowWindow(chkJoinDbl, SW_HIDE);
		
		ShowWindow(txtMaxUsers, SW_SHOW);
		ShowWindow(lblMaxUsers, SW_SHOW);
		ShowWindow(txtMaxPing, SW_SHOW);
		ShowWindow(lblMaxPing, SW_SHOW);
		ShowWindow(chkFakeP2P, SW_SHOW);
		ShowWindow(chkEmulinkerSF, SW_SHOW);
		ShowWindow(chkEmuRes, SW_SHOW);
		ShowWindow(chkConnRes, SW_SHOW);
		ShowWindow(btnVersion, SW_SHOW);

		/*ShowWindow(lblP2PServer, SW_HIDE);
		ShowWindow(txtP2PServer, SW_HIDE);
		ShowWindow(lblP2PPort, SW_HIDE);
		ShowWindow(txtP2PPort, SW_HIDE);
		ShowWindow(btnP2PStart, SW_HIDE);
		ShowWindow(btnP2PServer, SW_HIDE);
		ShowWindow(btnP2PConnect, SW_HIDE);*/
	}
	//P2P Options
	/*else if(show == 4){
		ShowWindow(txtServerIP, SW_HIDE);
		ShowWindow(txtUsername, SW_HIDE);
		ShowWindow(txtQuit, SW_HIDE);
		ShowWindow(cmbConnectionType, SW_HIDE);
		ShowWindow(chkShowError, SW_HIDE);
		ShowWindow(chkDrop, SW_HIDE);
		ShowWindow(btnChatroom, SW_SHOW);
		ShowWindow(chkKeepGameChatLogs, SW_HIDE);
		ShowWindow(chkKeepChatLogs, SW_HIDE);
		ShowWindow(chkUseCache, SW_HIDE);
		ShowWindow(chkBlink, SW_HIDE);

		//ShowWindow(lblStats, SW_HIDE);
		ShowWindow(btnLogoff, SW_HIDE);
		ShowWindow(btnLogin, SW_HIDE);

		ShowWindow(chkBeep, SW_HIDE);
		ShowWindow(chkJoinChatGame, SW_HIDE);
		ShowWindow(chkUseScreenChat, SW_HIDE);
		ShowWindow(chkJoinChat, SW_HIDE);
		ShowWindow(chkCreate, SW_HIDE);
		ShowWindow(chkJoinDbl, SW_HIDE);
		
		ShowWindow(txtMaxUsers, SW_HIDE);
		ShowWindow(lblMaxUsers, SW_HIDE);
		ShowWindow(txtMaxPing, SW_HIDE);
		ShowWindow(lblMaxPing, SW_HIDE);
		ShowWindow(chkFakeP2P, SW_HIDE);
		ShowWindow(chkEmulinkerSF, SW_HIDE);
		ShowWindow(chkEmuRes, SW_HIDE);
		ShowWindow(chkConnRes, SW_HIDE);
		ShowWindow(btnVersion, SW_HIDE);

		ShowWindow(lblP2PServer, SW_SHOW);
		ShowWindow(txtP2PServer, SW_SHOW);
		ShowWindow(lblP2PPort, SW_SHOW);
		ShowWindow(txtP2PPort, SW_SHOW);
		ShowWindow(btnP2PStart, SW_SHOW);
		ShowWindow(btnP2PServer, SW_SHOW);
		ShowWindow(btnP2PConnect, SW_SHOW);
	}*/

}

void displayAndAutoScrollRichEdit(HWND rEdit, char *temp, COLORREF rgbColor){
	CHARFORMAT d;
	int min;
	int max;
	int i;
	int f;
	CHARRANGE b;
	SETTEXTEX q;

	//Move Caret to Bottom
	b.cpMin = -1;
	b.cpMax = -1;
	SendMessage(rEdit, EM_EXSETSEL, 0, (LPARAM)&b);

	//Set Text Properties
	d.cbSize = sizeof(CHARFORMAT);
	d.crTextColor = rgbColor;
	d.dwEffects = 0;
	d.dwMask = CFM_COLOR;
	SendMessage(rEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &d);


	GetScrollRange(rEdit,	SB_VERT, &min, &max);
	i = GetScrollPos(rEdit, SB_VERT);
	
	//Display
	//SendMessage(rEdit, EM_REPLACESEL, 0, (LPARAM) temp);
	q.codepage = CP_ACP; 
	q.flags = ST_SELECTION;
	SendMessage(rEdit, EM_SETTEXTEX, (WPARAM) &q, (LPARAM) temp);
	
	//Height
	if(rEdit == txtChatroom)
		f = 260;
	else
		f = 115;

	//Auto Scroll
	if((i + f) >= max)
		SendMessage(rEdit, WM_VSCROLL, SB_BOTTOM, 0);
}


//0x07 - Global Chat Request
void globalChatRequest(){
	char dataToBeSent[1024];
	
	if(myUserID == -1)
		return;

	dataToBeSent[0] = '\0';


	int lenChat = SendMessage(txtChat, WM_GETTEXT, 255, (LPARAM) &dataToBeSent[1]);

	if(lenChat < 1)
		return;

	lenChat = lenChat + 1;

	dataToBeSent[lenChat] = '\0';

	//Clear Chat Box
	SendMessage(txtChat, WM_SETTEXT, 0, (LPARAM) "");

    constructPacket(dataToBeSent, (1 + lenChat), 0x07);
}



//0x08 - Game Chat Notification
void gameChatNotification(unsigned short position, int slot){
	char nick[1024];
	char message[1024];
	char temp[2048];
	short strSize;
	short i;
	
	i = position;
	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;

	//Message
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(message, &myBuff[slot].myBuff[i], strSize + 1);

	//Game Callback
	if(gamePlaying == true && kInfo.chatReceivedCallback != NULL){
		if (useScreenChatValue == BST_CHECKED)
			kInfo.chatReceivedCallback(nick, message);
	}

	//Show Green if it's <Server>
	if(nick[0] == 'S' && nick[1] == 'e' && nick[2] == 'r' && nick[3] == 'v' && nick[4] == 'e' && nick[5] == 'r' && nick[6] == '\0'){
		displayGameChatroomAsServer(message);
		return;
	}

	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> ");
	strcat(temp,message);
	strcat(temp,"\r\n");

	//Display
	displayAndAutoScrollRichEdit(txtGameChatroom, temp, RGB(0, 0, 0));

	//Log
	saveGameroomLog(temp);
}


//0x08 - Game Chat Request
void gameChatRequest(){
	char dataToBeSent[1024];
	
	dataToBeSent[0] = '\0';

	int lenChat = SendMessage(txtGameChat, WM_GETTEXT, 255, (LPARAM) &dataToBeSent[1]);

	if(lenChat < 1)
		return;

	lenChat = lenChat + 1;

	dataToBeSent[lenChat] = '\0';

	//Clear Chat Box
	SendMessage(txtGameChat, WM_SETTEXT, 0, (LPARAM) "");

	//Away Message
	if(imAway == true){
		imAway = false;
		showAway();
	}

    constructPacket(dataToBeSent, (1 + lenChat), 0x08);
}


//0x09 - Client Keep Alive
void clientKeepAlive(){
	char dataToBeSent[1];

	dataToBeSent[0] = 0;
	constructPacket(dataToBeSent, 1, 0x09);
}


//0x0A - Create Game Notification
void createGameNotification(unsigned short position, int slot){
	char owner[1024];
	char game[1024];
	char strGameID[1024];
	char emulator[1024];
	char temp[2048];
	unsigned long gameID;
	short strSize;
	short i;
	    
	
	i = position;
	//Owner
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(owner, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	
	//Game
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(game, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;

	//Emulator
    strSize = strlen(&myBuff[slot].myBuff[i]);
	memcpy(emulator, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;

	//GameID
    gameID = *(unsigned long*) &myBuff[slot].myBuff[i];
	wsprintf(strGameID, "%i", gameID);
	

	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp, "<");
	strcat(temp, owner);
	strcat(temp, "> Created Game: ");
	strcat(temp, game);
	strcat(temp, "\r\n");
	

	//Display
	if(createValue == BST_CHECKED)
		displayAndAutoScrollRichEdit(txtChatroom, temp, RGB(153, 51, 0));
	
	//Add to Gamelist
	gamelistAdditem(game, emulator, owner, "Waiting", "1/N", strGameID);
	displayStats();

	//Log
	saveChatroomLog(temp);
}


//0x0A - Create Game Request
void createGameRequest(){
	char dataToBeSent[1024];
    int gameLength;
    
	if(myUserID == -1)
		return;

	dataToBeSent[0] = 0x00;
	
	//Get Game
	//strcpy(currentGame, "GodWeapon Good Pings: 69.31.15.190:27888");
	strcpy(&dataToBeSent[1], currentGame);
	gameLength = strlen(currentGame);

	dataToBeSent[1 + gameLength + 1] = 0x00;
	dataToBeSent[1 + gameLength + 2] = 0xFF;
	dataToBeSent[1 + gameLength + 3] = 0xFF;
	dataToBeSent[1 + gameLength + 4] = 0xFF;
	dataToBeSent[1 + gameLength + 5] = 0xFF;

    constructPacket(dataToBeSent, (1 + gameLength + 1 + 5), 0x0A);
	showGameroom(true);
	SendMessage(txtGame, WM_SETTEXT, 0, (LPARAM) currentGame);

	myGameID = 0xFFFF;
	strcpy(gOwner, username);
	strcpy(gEmulator, emulator);
	fixLastGameToPlay();
	saveConfig();
}


//0x0B - Quit Game Notification
void quitGameNotification(unsigned short position, int slot){
	unsigned short userID;
	char strUserID[1024];
	char nick[1024];
	char temp[2048];
	short strSize;
	short i;
		   
	i = position;
	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	strncpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	
	//UserID
    userID = *(unsigned short *) &myBuff[slot].myBuff[i];
	wsprintf(strUserID, "%i", userID);
	i = i + 2;

	//Find UserID in GameUserlist
	LVFINDINFO c;
	c.psz = strUserID;
	c.flags = LVFI_STRING;
	int pos = SendMessage(lstGameUserlist, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);
	
	//Remove Game from Gamelist
	if(pos > -1)
		SendMessage(lstGameUserlist, LVM_DELETEITEM, (WPARAM) pos, 0);

	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> Left the Game\r\n");
	
	//Display
	if(SendMessage(chkJoinChatGame, BM_GETCHECK, 0, 0) == BST_CHECKED)
		displayAndAutoScrollRichEdit(txtGameChatroom, temp, RGB(122, 122, 122));

	if(userID == myUserID){
		if(iQuit == false)
			displayAndAutoScrollRichEdit(txtChatroom, "<ALERT> YOU HAVE BEEN KICKED FROM THE GAME!\n", RGB(255, 0, 0));
		iQuit = false;
		supraCleanup(1, 0);
		showGameroom(false);
		imAway = false;
		imOwner = false;
		showAway();
	}

	//Log
	saveGameroomLog(temp);
}


//0x0B - Quit Game Request
void quitGameRequest(){
	char dataToBeSent[3];

	dataToBeSent[0] = 0x00;
	dataToBeSent[1] = 0xFF;
	dataToBeSent[2] = 0xFF;

	iQuit = true;

	constructPacket(dataToBeSent, 3, 0x0B);

	supraCleanup(1, 0);
	showGameroom(false);
	saveConfig();
}


//0x0C - Join Game Notification
void joinGameNotification(unsigned short position, int slot){
	char nick[1024];
	char strUserID[1024];
	char temp[2048];
	unsigned short userID;
	unsigned long ping;
	char strPing[1024];
	unsigned char connectionType;
	char strConnectionType[1024];
	short strSize;
	unsigned long pGame;
	short i;
	
	
	//Pointer to Game on Server...has no real use
	i = position + 1;
	pGame = *(unsigned long *) &myBuff[slot].myBuff[i];
	i = i + 4;
	    
	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	strncpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	
	//Ping
	ping = *(unsigned long *) &myBuff[slot].myBuff[i];
	wsprintf(strPing, "%i", ping);
	i = i + 4;

	//UserID
    userID = *(unsigned short *) &myBuff[slot].myBuff[i];
	wsprintf(strUserID, "%i", userID);
	
	//Connection Type
	connectionType = *(char *) &myBuff[slot].myBuff[i + 2];
	if(connectionType == lan)
		strcpy(strConnectionType, "LAN");
	else if(connectionType == excellent)
		strcpy(strConnectionType, "Excellent");
	else if(connectionType == good)
		strcpy(strConnectionType, "Good");
	else if(connectionType == average)
		strcpy(strConnectionType, "Average");
	else if(connectionType == low)
		strcpy(strConnectionType, "Low");
	else if(connectionType == bad)
		strcpy(strConnectionType, "Bad");
	else
		strcpy(strConnectionType, "ERROR");	


	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> Joined the Game\r\n");
	
	//Display
	if(SendMessage(chkJoinChatGame, BM_GETCHECK, 0, 0) == BST_CHECKED)
		displayAndAutoScrollRichEdit(txtGameChatroom, temp, RGB(122, 122, 122));

	//Add to Userlist
	gameUserlistAdditem(nick, strPing, strConnectionType, strUserID);

	FLASHWINFO g;

	if(userID != myUserID){
		if(SendMessage(chkBeep, BM_GETCHECK, 0, 0) == BST_CHECKED)
			MessageBeep(MB_OK);
		if(SendMessage(chkBlink, BM_GETCHECK, 0, 0) == BST_CHECKED){
			g.cbSize = sizeof(FLASHWINFO);
			g.hwnd = form1;
			g.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
			g.uCount = 100;
			g.dwTimeout = 0;
			FlashWindowEx(&g);
		}
		if(imAway == true){
			strcpy(temp, "Away: ");
			strcat(temp, awayMessages[awayValue].message);
			kailleraChatSend(temp);
		}
	}
	else{
		saveGameroomLog("####################\r\n");
		saveGameroomLog("NEW SESSION GAMEROOM\r\n");
		saveGameroomLog("####################\r\n");
		if(imOwner == true && emulinkerSFValue == BST_CHECKED){
			//Max Users
			GetWindowText(txtMaxUsers, maxUsersG, GetWindowTextLength(txtMaxUsers) + 1);
			if(atoi(maxUsersG) < 1 || atoi(maxUsersG) > 100){
				wsprintf(maxUsersG, "%i", 16);
				SetWindowText(txtMaxUsers, "16");
			}
			char data[1024];
			int dLen;
			data[0] = 'd';
			strcpy(&data[1], "/maxusers ");
			strcat(data, maxUsersG);
			dLen = strlen(data) + 1;
			data[0] = '\0';
			constructPacket(data, dLen, 0x08);

			//Max Ping
			GetWindowText(txtMaxPing, maxPingG, GetWindowTextLength(txtMaxPing) + 1);
			if(atoi(maxPingG) < 1 || atoi(maxPingG) > 1000){
				wsprintf(maxPingG, "%i", 200);
				SetWindowText(txtMaxPing, "200");
			}
			data[0] = 'd';
			strcpy(&data[1], "/maxping ");
			strcat(data, maxPingG);
			dLen = strlen(data) + 1;
			data[0] = '\0';
			constructPacket(data, dLen, 0x08);


			//Fake P2P
			data[0] = 'd';
			if(fakeP2PValue == BST_CHECKED)
				strcpy(&data[1], "/p2pon");
			else
				strcpy(&data[1], "/p2poff");
			i = strlen(data) + 1;
			data[0] = '\0';
			constructPacket(data, i, 0x08);	

			//Restrict Emulator
			data[0] = 'd';
			if (emuResValue == BST_CHECKED) {
				strcpy(&data[1], "/setemu ");
				strcat(data, emulator);
			}
			else
				strcpy(&data[1], "/setemu any");
			i = strlen(data) + 1;
			data[0] = '\0';
			constructPacket(data, i, 0x08);

			//Restrict Conn Type
			data[0] = 'd';
			if (connResValue == BST_CHECKED)
				strcpy(&data[1], "/setconn");
			else
				strcpy(&data[1], "/setconn any");
			i = strlen(data) + 1;
			data[0] = '\0';
			constructPacket(data, i, 0x08);
		}
	}

	//Log
	saveGameroomLog(temp);
}


//0x0C - Join Game Request
void joinGameRequest(){
	char dataToBeSent[13];
	unsigned long gameID;
	int iSelect = -1;
	int w;
	int i;
	char strEmu[1024];
	char strOwner[1024];
	char strStatus[1024];
	char strGameID[1024];

	if(myUserID == -1)
		return;

	//Get Currently Selected item
	iSelect = SendMessage(lstGamelist, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
	if(iSelect < 0)
		return;

	//Get GameID
	LVITEM b;
	b.mask = LVIF_TEXT;
	b.iItem = iSelect;
	b.iSubItem = 0;
	b.pszText = strGameID;
	b.cchTextMax = 1024;
	SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &b);
	gameID = cStrToInt(strGameID);

	//Get Game
	b.mask = LVIF_TEXT;
	b.iItem = iSelect;
	b.iSubItem = 1;
	b.pszText = currentGame;
	b.cchTextMax = 1024;
	SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &b);

	//Get Owner
	b.mask = LVIF_TEXT;
	b.iItem = iSelect;
	b.iSubItem = 3;
	b.pszText = strOwner;
	b.cchTextMax = 1024;
	SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &b);
	strcpy(gOwner, strOwner);

	//Get Emulator
	b.mask = LVIF_TEXT;
	b.iItem = iSelect;
	b.iSubItem = 2;
	b.pszText = strEmu;
	b.cchTextMax = 1024;
	SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &b);
	strcpy(gEmulator, strEmu);

	//Check for Rom/Emulator
	//if(SendMessage(chkShowError, BM_GETCHECK, 0, 0) == BST_CHECKED){

		//Get Status
		b.mask = LVIF_TEXT;
		b.iItem = iSelect;
		b.iSubItem = 4;
		b.pszText = strStatus;
		b.cchTextMax = 1024;
		SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &b);

		//Do we have this game?
		for(i = 0; i < totalGames; i++){
			if(strcmp(gameList[i].game, currentGame) == 0)
				break;
			else if(i == (totalGames - 1)){
				w = MessageBox(form1, "Don't have Game!  Do you still want to join?", "Game Not Available!", MB_YESNO); 
				if(w == IDYES)
					break;
				else
					return;
			}
			
			if(i % 100 == 0)
				DoEvents();
		}

		//Same Emulator?
		if(strcmp(strEmu, emulator) != 0){
			w = MessageBox(form1, "Different Emulators!  Do you still want to join?", "Emulator Not Compatible!", MB_YESNO); 
			if(w == IDNO)
				return;
		}

		//Status Playing?
		if(strcmp(strStatus, "Playing") == 0){
			w = MessageBox(form1, "Currently Playing!  Do you still want to join?", "Game is Playing!", MB_YESNO); 
			if(w == IDNO)
				return;
		}
	//}
			
	//Setup Packet
	dataToBeSent[0] = 0;
	dataToBeSent[1] = gameID & 255;
	dataToBeSent[2] = (gameID >> 8) & 255;
	dataToBeSent[3] = (gameID >> 16) & 255;
	dataToBeSent[4] = (gameID >> 24) & 255;
	dataToBeSent[5] = 0x00;
	dataToBeSent[6] = 0x00;
	dataToBeSent[7] = 0x00;
	dataToBeSent[8] = 0x00;
	dataToBeSent[9] = 0x00;
	dataToBeSent[10] = 0xFF;
	dataToBeSent[11] = 0xFF;
	dataToBeSent[12] = connectionType;
	constructPacket(dataToBeSent, 13, 0x0C);	
	
	//Switch to Gameroom View
	showGameroom(true);
	myGameID = gameID;//must be after showGameroom(true); or it will reset
	fixLastGameToPlay();
	SendMessage(txtGame, WM_SETTEXT, 0, (LPARAM) currentGame);
	saveConfig();
	
}


//0x0D - Player Information
void playerInformation(unsigned short position, int slot){
    unsigned long numOfUsers;
    short i;
    int w;
	short strSize;
    char nick[1024];
    unsigned long ping;
	char strPing[1024];
	char strStatus[1024];
    unsigned short userID;
	char strUserID[1024];
    unsigned char connectionType;
	char strConnectionType[1024];
       
    ///Get Number of Users
	i = position + 1;
    numOfUsers = *(unsigned long *) &myBuff[slot].myBuff[i];
	i = i + 4;
  
	//For Each User
    for(w = 0; w < numOfUsers; w++){
        //Nick
		strSize = strlen(&myBuff[slot].myBuff[i]);
		strncpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
        i = i + strSize + 1;
		
        //Ping
		ping = *(unsigned long *) &myBuff[slot].myBuff[i];
        wsprintf(strPing, "%i", ping);
		i = i + 4;

        //UserID
		userID = *(unsigned short *) &myBuff[slot].myBuff[i];
        wsprintf(strUserID, "%i", userID);
		i = i + 2;
            
        //Connection Type
        connectionType = *(unsigned char *) &myBuff[slot].myBuff[i];
        i = i + 1;
		if(connectionType == lan)
			strcpy(strConnectionType, "LAN");
		else if(connectionType == excellent)
			strcpy(strConnectionType, "Excellent");
		else if(connectionType == good)
			strcpy(strConnectionType, "Good");
		else if(connectionType == average)
			strcpy(strConnectionType, "Average");
		else if(connectionType == low)
			strcpy(strConnectionType, "Low");			
		else if(connectionType == bad)
			strcpy(strConnectionType, "Bad");
		else
			strcpy(strStatus, "ERROR");

		//Add User
		gameUserlistAdditem(nick, strPing, strConnectionType, strUserID);
    }
}


//0x0E - Update Game Status
void updateGameStatus(unsigned short position, int slot){
	int gameID;
	char strMaxUsers[1024];
	unsigned char status;
	char strGameID[1024];
	char strUsers[1024];
	char strStatus[1024];
	unsigned char users;
	unsigned char maxUsers;
	char temp[2048];
	short i;

	//GameID
	i = position + 1;
    gameID = *(int *) &myBuff[slot].myBuff[i];
	wsprintf(strGameID, "%i", gameID);
	i = i + 4;

	//Status
	status = *(unsigned char *) &myBuff[slot].myBuff[i];
	if(status == 0)
		memcpy(strStatus, "Waiting\0", 8);
	else if(status == 1)
		memcpy(strStatus, "Playing\0", 8);
	else  if(status == 2)
		memcpy(strStatus, "Playing\0", 8);//Netsync
	else
		memcpy(strStatus, "ERROR\0", 6);

	//Users/Maxusers
	users =  *(unsigned char *) &myBuff[slot].myBuff[i + 1];
	wsprintf(strUsers, "%i", users);
	maxUsers =  *(unsigned char *) &myBuff[slot].myBuff[i + 2];
	wsprintf(strMaxUsers, "%i", maxUsers);
	strcpy(temp, strUsers);
	strcat(temp, "/");
	strcat(temp, strMaxUsers);

	//Find GameID in Gamelist
	LVFINDINFO c;
	c.psz = strGameID;
	c.flags = LVFI_STRING;
	int pos = SendMessage(lstGamelist, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);
	
	if(pos > -1){
		LVITEM b;
		//Status
		b.mask = LVIF_TEXT;
		b.iItem = pos;
		b.iSubItem = 4;
		b.pszText = strStatus;
		b.cchTextMax = 1024;
		SendMessage(lstGamelist, LVM_SETITEMTEXT, (WPARAM)pos, (LPARAM)&b);
		//#Users
		b.mask = LVIF_TEXT;
		b.iItem = pos;
		b.iSubItem = 5;
		b.pszText = temp;
		b.cchTextMax = 1024;
		SendMessage(lstGamelist, LVM_SETITEMTEXT, (WPARAM)pos, (LPARAM)&b);

		ListView_SortItemsEx(lstGamelist, lstGamelistCompareFunc, 0);
	}
}


//0x0F - Kick Request
void kickRequest(){
	char dataToBeSent[3];
	unsigned short userID;
	char temp[2048];
	int iSelect;
	LVITEM b;

	dataToBeSent[0] = 0x00;

	//Get Currently Selected item
	iSelect = SendMessage(lstGameUserlist, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

	//Get UserID
	b.mask = LVIF_TEXT;
	b.iItem = iSelect;
	b.iSubItem = 0;
	b.pszText = temp;
	b.cchTextMax = 1024;
	SendMessage(lstGameUserlist, LVM_GETITEM, 0, (LPARAM) &b);

	userID = cStrToInt(temp);

    //UserID
	dataToBeSent[1]= userID & 255;
	dataToBeSent[2]= (userID >> 8) & 255;

	constructPacket(dataToBeSent, 3, 0x0F);
}

//0x10 - Close Game Notification
void closeGameNotification(unsigned short position, int slot){
	unsigned long gameID;
	char owner[1024];
	char strGameID[1024];
	char temp[2048];

	//GameID
    gameID = *(unsigned long*) &myBuff[slot].myBuff[position + 1];
	wsprintf(strGameID, "%i", gameID);
	
	//Find GameID in Gamelist
	LVFINDINFO c;
	c.psz = strGameID;
	c.flags = LVFI_STRING;
	int pos = SendMessage(lstGamelist, LVM_FINDITEM, (WPARAM) -1, (LPARAM) &c);

	//Remove Game from Gamelist
	if(pos > -1){
		//Get Nick
		LVITEM b;
		b.mask = LVIF_TEXT;
		b.iItem = pos;
		b.iSubItem = 3;
		b.pszText = owner;
		b.cchTextMax = 1024;
		SendMessage(lstGamelist, LVM_GETITEM, 0, (LPARAM) &b);
		SendMessage(lstGamelist, LVM_DELETEITEM, (LPARAM) pos, 0);

		
		//Previous (CRLF) 
		//<Nick> Message (CRLF)
		strcpy(temp,"<");
		strcat(temp, owner);
		strcat(temp, "> ");
		strcat(temp,"Closed Game\r\n");
		
		
		//Display
		if(createValue == BST_CHECKED)
			displayAndAutoScrollRichEdit(txtChatroom, temp, RGB(153, 51, 0));
		displayStats();

		//Log
		saveChatroomLog(temp);
	}

	if(gameID == myGameID){
		supraCleanup(1, 0);
		showGameroom(false);
		imOwner = false;
	}
}


//0x11 - Start Game Notification
void startGameNotification(unsigned short position, int slot){
	if(startedGame == true)
		return;

    frameDelay = *(unsigned short *) &myBuff[slot].myBuff[position + 1];
    myPlayerNumber = myBuff[slot].myBuff[position + 3];
    totalPlayers = myBuff[slot].myBuff[position + 4];
    
	displayGameChatroomAsServer("The Game has Started!");

	//Log
	saveGameroomLog("<Server> The Game has Started!\r\n");
    
    stage = 0;
    sizeOfEinput = 0;

    startedGame = true;

	saveConfig();

	//kInfo.gameCallback(currentGame, myPlayerNumber, totalPlayers);
    gameThread = CreateThread(NULL, 0, callGameCallback, NULL, 0, NULL);
}


//0x11 - Start Game Request
void startGameRequest(){
	char dataToBeSent[5];

	dataToBeSent[0] = 0x00;
	dataToBeSent[1] = 0xFF;
	dataToBeSent[2] = 0xFF;
	dataToBeSent[3] = 0xFF;
	dataToBeSent[4] = 0xFF;
	constructPacket(dataToBeSent, 5, 0x11);
}

void testNum(long num){
	char a[1024];
	wsprintf(a,"%li",num);

	//Display
	displayAndAutoScrollRichEdit(txtChatroom, a, RGB(34, 139, 34));
}


//0x12 - Game Data Receive
void gameDataRecv(unsigned short position, int slot){
	unsigned long i;//, w;
    
	//Size of All Player's Input
    totalInput = *(unsigned short *) &myBuff[slot].myBuff[position + 1];

	//Emulator Values
    for(i = 0; i < totalInput; i++){
		eInput[ePos] =  myBuff[slot].myBuff[i + position + 3];
		ePos = (ePos + 1) % eSize;
    }
	//Add Frames
	frameCount += connectionType;

	//FIFO
	if(inCachePos >= cacheSize){
		//Left Shift Array for FIFO
		for(i = 0; i < cacheSize; i++){
			memcpy(&inCache[i * inCacheSize], &inCache[((i + 1) * inCacheSize)], totalInput);
		}
		memcpy(&inCache[lastInCachePos], &myBuff[slot].myBuff[position + 3], totalInput);
    }
	//Cache Values
    else{
        memcpy(&inCache[inCachePos * inCacheSize], &myBuff[slot].myBuff[position + 3], totalInput);
		inCachePos++;
	}
}


//0x12 - Game Data Send
void gameDataSend(){
	int i;
	char dataToBeSent[2];

	if(useCache == true){
        //Search Cache for Match	
		for(i = 0; i < outCachePos; i++){
			if(memcmp(&vInput[3], &outCache[i * outCacheSize], sizeOfVinput) == 0){
				//0x13 - Game Cache Send
				dataToBeSent[0] = 0x00;
				dataToBeSent[1] = i;
				constructPacket(dataToBeSent, 2, 0x13);
				return;				
			}
        }
  
        //FIFO
        if(outCachePos >= cacheSize){
			for(i = 0; i < cacheSize; i++)
				memcpy(&outCache[i * outCacheSize], &outCache[(i + 1) * outCacheSize], sizeOfVinput);
			memcpy(&outCache[lastOutCachePos], &vInput[3], sizeOfVinput);
		}
        //Add Until Cache is Full
        else{
			memcpy(&outCache[outCachePos * outCacheSize], &vInput[3], sizeOfVinput);
            outCachePos++;
		}
    }


	vInput[0] = 0x00;
	vInput[1] = sizeOfVinput & 255;
	vInput[2]= (sizeOfVinput >> 8) & 255;

    constructPacket(vInput, 3 + sizeOfVinput, 0x12);
}


//0x13 - Game Cache Receive
void gameCacheRecv(unsigned short position, int slot){
    int i;

    for(i = 0; i < totalInput; i++){
        eInput[ePos] = inCache[(*((unsigned char *) &myBuff[slot].myBuff[position + 1]) * inCacheSize) + i];
		ePos = (ePos + 1) % eSize;
    }
	
	//Add Frames
	frameCount += connectionType;
}


/*
//0x13 - Game Cache Send
void gameCacheSend(char pos){
	char dataToBeSent[2];

	dataToBeSent[0] = 0x00;
	dataToBeSent[1] = pos;

	constructPacket(dataToBeSent, 2, 0x13);
}
*/

//0x14 - Drop Game Notification
void dropGameNotification(unsigned short position, int slot){
	char nick[1024];
	unsigned char playerNumber;
	char temp[2048];
	short strSize;
	short i;


	i = position;    
	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	strncpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;

	//Player Number
	playerNumber = *(unsigned char *) &myBuff[slot].myBuff[i];
	
	if(kInfo.clientDroppedCallback != NULL)
		kInfo.clientDroppedCallback(nick, playerNumber);

	//Previous (CRLF) 
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> Dropped from Game\r\n");
	
	//Display
	displayAndAutoScrollRichEdit(txtGameChatroom, temp, RGB(34, 139, 34));

	//Log
	saveGameroomLog(temp);
}


//0x14 - Drop Game Request
void dropGameRequest(){
	if (startedGame) {
		char dataToBeSent[2];

		dataToBeSent[0] = '\0';
		dataToBeSent[1] = 0x00;
		constructPacket(dataToBeSent, 2, 0x14);
	}
}


//0x15 - Ready to Play Notification
void readyToPlayNotificaiton(){
	gamePlaying = true;

	//Display
	displayGameChatroomAsServer("All Players are Ready!");

	//Log
	saveGameroomLog("<Server> All Players are Ready!\r\n");
}


//0x15 - Ready to Play Request
void readyToPlayRequest(){
	char dataToBeSent[1];

	dataToBeSent[0] = 0x00;
	constructPacket(dataToBeSent, 1, 0x15);
}


//0x16 - Connection Rejected Notification
void connectionRejectedNotification(unsigned short position, int slot){
	char message[1024];
	short strSize;
	short i;
	    
	i = position;

	//Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	i = i + strSize + 1;

	//UserID 2bytes
	i = i + 2; 

	//Message
    strSize = strlen(&myBuff[slot].myBuff[i]);
	strncpy(message, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	
	MessageBox(form1, message, "Connection Rejected!", NULL);
	displayChatroomAsServer(message);
	supraCleanup(0, 0);
}


//0x17 - Server Information Message
void serverInformationMessage(unsigned short position, int slot){
	char temp[2048];
	char nick[1024];
	char message[1024];
	short strSize;
	short i;
	int j;

	i = position;

    //Nick
    strSize = strlen(&myBuff[slot].myBuff[i]);
	strncpy(nick, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;
	//Message
    strSize = strlen(&myBuff[slot].myBuff[i]);
	strncpy(message, &myBuff[slot].myBuff[i], strSize + 1);
	i = i + strSize + 1;

	if(strncmp(message, "VERSION:", 8) == 0){
		MessageBox(form1, &message[8], "Server Version", NULL);
		//Log
		saveChatroomLog(temp);
		return;
	}
	else if(strncmp(message, "sccppevercheck", 8) == 0){
		temp[0] = 'd';
		strcpy(&temp[1], cVersion);
		j = strlen(temp) + 1;
		temp[0] = '\0';
		constructPacket(temp, j, 0x07);
	}

	//Previous (CRLF)
	//<Nick> Message (CRLF)
	strcpy(temp,"<");
	strcat(temp, nick);
	strcat(temp,"> ");
	strcat(temp,message);
	strcat(temp,"\r\n");
	
	//Display
	displayAndAutoScrollRichEdit(txtChatroom, temp, RGB(34, 139, 34));

	//Log
	saveChatroomLog(temp);
}


char * strToLower(TCHAR *str){
	int dLen = strlen(str);

	for(int i = 0; i < dLen; i++)
		str[i] = tolower(str[i]);


	return str;
}


/*
    This callback function is called by the ListView to compare two items.
    This is done in response to ListView_SortItemsEx().  Though the description
    of this function in MSDN can be confusing, it's actually very simple
    to write.
    
    In this particular function, we first get the text of the two items we
    are comparing.  Then, based on the column that we are sorting for, we
    either compare the string value (column 0) or numeric value (column 1).
*/
int CALLBACK lstUserlistCompareFunc(LPARAM i1, LPARAM i2, LPARAM){
    TCHAR tsz1[1024];
    TCHAR tsz2[1024];
    int n1, n2;// For use in sorting by number.

    // Get the text of the two items.  Notice we get the text
    // for the column that we selected, specified by lstUserlistColumn.

    ListView_GetItemText(lstUserlist, i1, lstUserlistColumn, tsz1, 1024);
    ListView_GetItemText(lstUserlist, i2, lstUserlistColumn, tsz2, 1024);

    if(lstUserlistColumn == 0 || lstUserlistColumn == 2){
		n1 = _tcstol(tsz1, 0, 10);
		n2 = _tcstol(tsz2, 0, 10);

		if(userlistSwitch == false)
			return (n1 - n2);
		else
			return (n2 - n1);	
    }
    else if(lstUserlistColumn == 1 || lstUserlistColumn == 3 || lstUserlistColumn == 4){
		if(userlistSwitch == false){
			return _tcscmp(strToLower(tsz1), strToLower(tsz2));
		}
		else{
			return _tcscmp(strToLower(tsz2), strToLower(tsz1));
		}
    }
    return 0;
}


int CALLBACK lstServerlistKCompareFunc(LPARAM i1, LPARAM i2, LPARAM){
    TCHAR tsz1[1024];
    TCHAR tsz2[1024];
    int n1, n2;// For use in sorting by number.

    // Get the text of the two items.  Notice we get the text
    // for the column that we selected, specified by lstUserlistColumn.

    ListView_GetItemText(lstServerListK, i1, lstServerlistKColumn, tsz1, 1024);
    ListView_GetItemText(lstServerListK, i2, lstServerlistKColumn, tsz2, 1024);

    if(lstServerlistKColumn == 0 || lstServerlistKColumn == 1 || lstServerlistKColumn == 3 || lstServerlistKColumn == 6){
		if(kailleraSwitch == false){
			return _tcscmp(strToLower(tsz1), strToLower(tsz2));
		}
		else{
			return _tcscmp(strToLower(tsz2), strToLower(tsz1));
		}
    }
    else if( lstServerlistKColumn == 2 || lstServerlistKColumn == 4 || lstServerlistKColumn == 5){
		if (strstr(tsz1, ">1s")) n1 = 1000;
		else if (strstr(tsz1, "ERR")) n1 = 1001;
		else if (strstr(tsz1, "NA")) n1 = 1002;
		else n1 = _tcstol(tsz1, 0, 10);

		if (strstr(tsz2, ">1s")) n2 = 1000;
		else if (strstr(tsz2, "ERR")) n2 = 1001;
		else if (strstr(tsz2, "NA")) n2 = 1002;
		else n2 = _tcstol(tsz2, 0, 10);

		if(kailleraSwitch == false)
			return (n1 - n2);
		else
			return (n2 - n1);
    }
    return 0;
}

int CALLBACK lstServerlist3DCompareFunc(LPARAM i1, LPARAM i2, LPARAM){
    TCHAR tsz1[1024];
    TCHAR tsz2[1024];
    int n1, n2;// For use in sorting by number.

    // Get the text of the two items.  Notice we get the text
    // for the column that we selected, specified by lstUserlistColumn.

    ListView_GetItemText(lstServerList3D, i1, lstServerlist3DColumn, tsz1, 1024);
    ListView_GetItemText(lstServerList3D, i2, lstServerlist3DColumn, tsz2, 1024);

	if (lstServerlist3DColumn == 0 || lstServerlist3DColumn == 1 || lstServerlist3DColumn == 3 || lstServerlist3DColumn == 6) {
		if(anti3DSwitch == false){
			return _tcscmp(strToLower(tsz1), strToLower(tsz2));
		}
		else{
			return _tcscmp(strToLower(tsz2), strToLower(tsz1));
		}
    }
	else if (lstServerlist3DColumn == 2 || lstServerlist3DColumn == 4 || lstServerlist3DColumn == 5) {
		if (strstr(tsz1, ">1s")) n1 = 1000;
		else if (strstr(tsz1, "ERR")) n1 = 1001;
		else if (strstr(tsz1, "NA")) n1 = 1002;
		else n1 = _tcstol(tsz1, 0, 10);

		if (strstr(tsz2, ">1s")) n2 = 1000;
		else if (strstr(tsz2, "ERR")) n2 = 1001;
		else if (strstr(tsz2, "NA")) n2 = 1002;
		else n2 = _tcstol(tsz2, 0, 10);

		if(anti3DSwitch == false)
			return (n1 - n2);
		else
			return (n2 - n1);
    }
    return 0;
}

int CALLBACK lstGamelistCompareFunc(LPARAM i1, LPARAM i2, LPARAM){
    TCHAR tsz1[1024];
    TCHAR tsz2[1024];
    int n1, n2;// For use in sorting by number.

    // Get the text of the two items.  Notice we get the text
    // for the column that we selected, specified by lstUserlistColumn.

    ListView_GetItemText(lstGamelist, i1, lstGamelistColumn, tsz1, 1024);
    ListView_GetItemText(lstGamelist, i2, lstGamelistColumn, tsz2, 1024);

    if(lstGamelistColumn == 0){
		n1 = _tcstol(tsz1, 0, 10);
		n2 = _tcstol(tsz2, 0, 10);

		if(gamelistSwitch == false)
			return (n1 - n2);
		else
			return (n2 - n1);
    }
    else if(lstGamelistColumn > 0 && lstGamelistColumn < 6){
		if(gamelistSwitch == false){
			return _tcscmp(strToLower(tsz1), strToLower(tsz2));
		}
		else{
			return _tcscmp(strToLower(tsz2), strToLower(tsz1));
		}
    }
    return 0;
}



void getServerList3D(){	
	hostent* remoteHost;
	unsigned long addr;
	int x;

	closesocket(mySocket3D);
	exitPing3DThread();
	SendMessage(lstServerList3D, LVM_DELETEALLITEMS, 0, 0);

	mySocket3D = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	socketInfo3D.sin_family = AF_INET;
    socketInfo3D.sin_port = htons((u_short)anti3DServerList.port);

	if (inet_addr(anti3DServerList.host) == INADDR_NONE) {
		remoteHost = gethostbyname(anti3DServerList.host);
		if (remoteHost == NULL) {
			closesocket(mySocket3D);
			MessageBox(form1, "Could not resolve Server List!  Server may be down", "Error Resolving Address!", NULL);
			return;
		}
		else socketInfo3D.sin_addr.s_addr = *((unsigned long*)remoteHost->h_addr);
	}
	else {
		socketInfo3D.sin_addr.s_addr = inet_addr(anti3DServerList.host);
	}

	WSAAsyncSelect(mySocket3D, form1, SC_SUPRARECV3D, FD_WRITE | FD_CONNECT | FD_READ | FD_CLOSE);
    x = connect(mySocket3D, (sockaddr *) &socketInfo3D, sizeof(socketInfo3D));
	SetWindowText(form1, "Sending GET Request...If a list doesn't arrive within a few seconds, the host is probably down!");

}

void getWaitingGames(){
	hostent* remoteHost;
	unsigned long addr;
	int x;

	closesocket(mySocketWaiting);
	SendMessage(lstWaitingList, LVM_DELETEALLITEMS, 0, 0);

	mySocketWaiting = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	socketInfoWaiting.sin_family = AF_INET;
    socketInfoWaiting.sin_port = htons((u_short)anti3DServerList.port);

	if (inet_addr(anti3DServerList.host) == INADDR_NONE) {
		remoteHost = gethostbyname(anti3DServerList.host);
		if (remoteHost == NULL) {
			closesocket(mySocketWaiting);
			MessageBox(form1, "Could not resolve Server List!  Server may be down", "Error Resolving Address!", NULL);
			return;
		}
		else socketInfoWaiting.sin_addr.s_addr = *((unsigned long*)remoteHost->h_addr);
	}
	else {
		socketInfoWaiting.sin_addr.s_addr = inet_addr(anti3DServerList.host);
	}

	WSAAsyncSelect(mySocketWaiting, form1, SC_SUPRARECVWAITING, FD_WRITE | FD_CONNECT | FD_READ | FD_CLOSE);
    x = connect(mySocketWaiting, (sockaddr *) &socketInfoWaiting, sizeof(socketInfoWaiting));
	SetWindowText(form1, "Sending GET Request...If a list doesn't arrive within a few seconds, the host is probably down!");
}

void getServerListK(){
	hostent* remoteHost;
	unsigned long addr;
	int x;

	closesocket(mySocketK);
	exitPingKThread();
	SendMessage(lstServerListK, LVM_DELETEALLITEMS, 0, 0);

	mySocketK = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	socketInfoK.sin_family = AF_INET;
    socketInfoK.sin_port = htons((u_short)kailleraServerList.port);

	if (inet_addr(kailleraServerList.host) == INADDR_NONE) {
		remoteHost = gethostbyname(kailleraServerList.host);
		if (remoteHost == NULL) {
			closesocket(mySocketWaiting);
			MessageBox(form1, "Could not resolve Server List!  Server may be down", "Error Resolving Address!", NULL);
			return;
		}
		else socketInfoK.sin_addr.s_addr = *((unsigned long*)remoteHost->h_addr);
	}
	else {
		socketInfoK.sin_addr.s_addr = inet_addr(kailleraServerList.host);
	}

	WSAAsyncSelect(mySocketK, form1, SC_SUPRARECVK, FD_WRITE | FD_CONNECT | FD_READ | FD_CLOSE);
    x = connect(mySocketK, (sockaddr *) &socketInfoK, sizeof(socketInfoK));
	SetWindowText(form1, "Sending GET Request...If a list doesn't arrive within a few seconds, the host is probably down!");
}


void saveChatroomLog(char text[]){
	if(chatLogValue != BST_CHECKED)
		return;

	ofstream log("chatroom.log", ios::app);

	char date [24];
	char time [24];

	_strtime(time);
	_strdate(date);

	log << "[" << date << ": " << time << "] " << text;
	log.close();
}

void saveGameroomLog(char text[]){
	if(gameChatLogValue != BST_CHECKED)
		return;

	ofstream log("gameroom.log", ios::app);
	char date [24];
	char time [24];
  
	_strtime(time);
	_strdate(date);

	log << "[" << date << ": " << time << "] " << text;
	log.close();
}








