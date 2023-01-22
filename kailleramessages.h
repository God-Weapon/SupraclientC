//Prototypes
//0x01 - User Quit Notification
void userQuitNotification(unsigned short position, int slot);
void userQuitRequest();
//0x02 - User Joined
void userJoined(unsigned short position, int slot);
//0x03 - User Login Information
void userLoginInformation(short lenUsername, short lenEmulator);
//0x04 - Server Status
void serverStatus(unsigned short position, int slot);
//0x05 - ACK
void serverToClientAck();
//0x07 - Global Chat Notification
void globalChatNotification(unsigned short position, int slot);
void globalChatRequest();
//0x08 - Game Chat Notification
void gameChatNotification(unsigned short position, int slot);
void gameChatRequest();
//0x09 - Client Keep Alive
void clientKeepAlive();
//0x0A - Create Game Notification
void createGameNotification(unsigned short position, int slot);
void createGameRequest();
//0x0B - Quit Game Notification
void quitGameNotification(unsigned short position, int slot);
void quitGameRequest();
//0x0C - Join Game Notification
void joinGameNotification(unsigned short position, int slot);
void joinGameRequest();
//0x0D - Player Information
void playerInformation(unsigned short position, int slot);
//0x0E - Update Game Status
void updateGameStatus(unsigned short position, int slot);
//0x0F - Kick Request
void kickRequest();
//0x10 - Close Game Notification
void closeGameNotification(unsigned short position, int slot);
//0x11 - Start Game Notification
void startGameNotification(unsigned short position, int slot);
void startGameRequest();
//0x12 - Game Data
void gameDataRecv(unsigned short position, int slot);
void gameDataSend();
//0x13 - Game Cache
void gameCacheRecv(unsigned short position, int slot);
//void gameCacheSend(char pos);
//0x14 - Drop Game Notification
void dropGameNotification(unsigned short position, int slot);
void dropGameRequest();
//0x15 - Ready to Play Notification
void readyToPlayNotificaiton();
void readyToPlayRequest();
//0x16 - Connection Rejected Notification
void connectionRejectedNotification(unsigned short position, int slot);
//0x17 - Server Information Message
void serverInformationMessage(unsigned short position, int slot);


struct RECENT_SERVERS{
	char server[1024];
	char ip[1024];
	char location[1024];
};
int const RECENT_MAX = 25;
RECENT_SERVERS recentServers[RECENT_MAX];
int recentCount = -1;


struct FAVORITE_SERVERS{
	char server[1024];
	char ip[1024];
	char location[1024];
	char comments[1024];
};
int const FAVORITE_MAX = 25;
FAVORITE_SERVERS favoriteServers[FAVORITE_MAX];
int favoriteCount = -1;




//void p2pGameData(char *data, int dLen);
//void p2pGameChat(char *data, int dLen);
//void p2pGameStart(char *data, int dLen);
//void p2pGameDrop(char *data, int dLen);
//void p2pGameReady(char *data, int dLen);




void loginToServer();
void popupMenu(char num);
void gameInit();
void DoEvents();
void popupAway();
unsigned long cStrToInt(char *str);
void showGameroom(bool show);
bool supraCleanup(char type, HWND h);
void displayGameChatroomAsServer(char *msg);
void displayChatroomAsServer(char *msg);
void displayAndAutoScrollRichEdit(HWND rEdit, char *temp, COLORREF cColor);
void parseData(int slot, unsigned char msgType);
int saveConfig();
void constructPacket(char *data, unsigned short lenData, char msgNum);
void gotoMessageType(unsigned short position, char msgType, int slot);
void parseWaitingGames();
void parseServerList3D();
void displayStats();
void kServerlistAdditem(char *serverName, char *ip, char *ping, char *location, char *users, char *games, char *version);
void Serverlist3DAdditem(char *serverName, char *ip, char *ping, char *location, char *users, char *games, char *version);
void waitinglistAdditem(char *game, char *emulator, char *username, char *server, char *ip, char *location, char *numPlayers);
void recentlistAdditem(char *server, char *ip, char *location);
void favoritelistAdditem(char *server, char *ip, char *location, char *comments);
void gamelistAdditem(char *game, char *version, char *owner, char *status, char *numUsers, char *gameID);
void userlistAdditem(char *nick, char *ping, char *connection, char *userID, char *status);
void userlistAdditemAll();
void gamelistAdditemAll();
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI callGameCallback(LPVOID lpParam);
DWORD WINAPI recvLoop(LPVOID lpParam);
DWORD WINAPI pingKailleraServers(LPVOID lpParam);
DWORD WINAPI ping3DServers(LPVOID lpParam);
DWORD WINAPI continuousLoop(LPVOID lpParam);
//DWORD WINAPI p2pLoop(LPVOID lpParam);
//HANDLE p2pThread;
int CALLBACK lstUserlistCompareFunc(LPARAM i1, LPARAM i2, LPARAM);
int CALLBACK lstGamelistCompareFunc(LPARAM i1, LPARAM i2, LPARAM);
int CALLBACK lstServerlistKCompareFunc(LPARAM i1, LPARAM i2, LPARAM);
int CALLBACK lstServerlist3DCompareFunc(LPARAM i1, LPARAM i2, LPARAM);
int CALLBACK lstWaitinglistCompareFunc(LPARAM i1, LPARAM i2, LPARAM);
int CALLBACK lstRecentlistCompareFunc(LPARAM i1, LPARAM i2, LPARAM);
int CALLBACK lstFavoritelistCompareFunc(LPARAM i1, LPARAM i2, LPARAM);
long CALLBACK SubProcTxtChat(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtChat;
long CALLBACK SubProcTxtGameChat(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtGameChat;
long CALLBACK SubProcTxtMaxUsers(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtMaxUsers;
long CALLBACK SubProcTxtMaxPing(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtMaxPing;
long CALLBACK SubProcTxtChatroom(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtChatroom;
long CALLBACK SubProcTxtIP(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtIP;
long CALLBACK SubProcTxtQuit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtQuit;
long CALLBACK SubProcTxtNick(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC EditProcTxtNick;

char * strToLower(TCHAR *str);

//Control Properties
DWORD const formProperties = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION | WS_VISIBLE;
DWORD const formOtherProperties = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_VISIBLE;
DWORD const buttonProperties = BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP;
DWORD const frameProperties = BS_GROUPBOX | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE;
DWORD const textboxProperties = WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL;
DWORD const comboboxProperties = CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE;
DWORD const richTextboxProperties = ES_MULTILINE | WS_CHILD | WS_VISIBLE;
DWORD const listviewProperties = LVS_SHOWSELALWAYS | LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE | WS_TABSTOP;
DWORD const controlStyles = NULL;
DWORD const formStyles = WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE ;
DWORD const listviewStyles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
DWORD const labelProperties =  SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP;
DWORD const tabProperties = TCS_TABS | WS_CHILD | WS_VISIBLE | WS_TABSTOP;
DWORD const checkProperties = BS_AUTOCHECKBOX | BS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_TABSTOP;
HWND form1;
HWND txtGame;
HWND btnChatroom;
HWND lblQuit;
HWND txtQuit;
HWND tTab;
HWND chkShowError;
int errorValue;
HWND sTab;
HWND chkJoinChat;
HWND chkCreate;
HWND chkDrop;
int dropValue;
HWND chkBeep;
HWND btnDrop;
//HWND btnP2PServer;
//HWND btnP2PConnect;
HWND chkJoinChatGame;
HWND btnVersion;
//HWND txtP2PServer;
//HWND lblP2PServer;
//char p2pServer[1024];
//HWND txtP2PPort;
//HWND lblP2PPort;
//char p2pPort[1024];
//HWND btnP2PStart;
HWND chkUseScreenChat;
HWND chkKeepGameChatLogs;
HWND chkKeepChatLogs;
HWND btnAway;
int gameChatLogValue;
int chatLogValue;
int useScreenChatValue;
int joinChatGameValue;
int beepValue;

bool userlistSwitch = false;
bool gamelistSwitch = true;
bool gameUserlistSwitch = false;
bool kailleraSwitch = true;
bool anti3DSwitch = true;
bool recentSwitch = false;
bool favoriteSwitch = false;
bool waitingSwitch = false;

HWND btnLagStat;
int joinChatValue;
int createValue;
HWND serverListTab;
HWND fLoginInfo;
HWND lblUsername;
HWND txtUsername;
HWND chkEmulinkerSF;
int emulinkerSFValue;
HWND chkEmuRes;
int emuResValue;
HWND chkConnRes;
int connResValue;
HWND chkFakeP2P;
int fakeP2PValue;
HWND lblServerIP;
HWND txtServerIP;
HWND chkJoinDbl;
int joinDblValue;
HWND lstRecentList;
HWND lstWaitingList;
HWND lstFavoriteList;


struct AWAY_MESSAGES{
	char subject[1024];
	char message[1024];
	int item;
	bool save;
};
AWAY_MESSAGES awayMessages[10];
int awayMessageCount = 0;
int const AWAY_MESSAGE_MAX = 10;

//Away
HWND frmAway;
HWND txtAwayMessage;
HWND txtAwayMessageSubject;
HWND btnCreateAway;
HWND btnRemoveAway;
HWND chkUseCache;
int useCacheValue;
HWND chkBlink;
int blinkValue;
bool imOwner = false;
bool loggedIn = false;
bool connection = false;
void createAwayWindow();
void updateAway();
int awayValue;
HWND lstAway;
void awayAdditem(char subject[], char message[], int value);
void showAway();
bool imAway = false;

//PM
void createPMWindow();
HWND btnPM;
HWND frmPM;
HWND txtPM;
char fPM[1024];
bool cPM = false;

//Comments
void createCommentsWindow();
HWND btnComments;
HWND frmComments;
HWND txtComments;
char fComments[1024];
void createEditFavoritesWindow();
bool cComments = false;
HWND frmEditFavorites;
HWND txtEComments;
HWND txtEIP;
HWND txtELocation;
HWND txtEServer;
HWND btnEOkay;
bool editFavorite = false;

//Chatroom
HWND lstUserlist;
HWND lstGamelist;
HWND btnCreate;
HWND btnJoin;
HWND txtChatroom;
HWND txtChat;
HWND btnChat;
HWND lstGameUserlist;
HWND txtGameChatroom;
HWND txtGameChat;
HWND btnGameChat;
HWND btnGameStart;
HWND btnGameKick;
HWND btnGameLeave;
int chatroomCount = 0;

HWND btnLogin;
HWND txtMaxUsers;
HWND lblMaxUsers;
HWND txtMaxPing;
HWND lblMaxPing;
char maxUsersG[sizeof(int)];
char maxPingG[sizeof(int)];
HWND txtMSG;
HWND lblStats;
HWND btnLogoff;
HWND lstServerListK;
HWND lstServerList3D;
HWND btnKailleraServerList;
HWND btnAnti3dServerList;
HWND lblConnectionType;
HWND cmbConnectionType;

bool swapp;
HMENU awayMenu;
HWND btnSave;
HMENU gameMenu;
HMENU serverMenu;
HMENU serverSubMenu;
HMENU gameSubMenu;
HMENU userlistMenu;
HMENU userlistSubMenu;
HMENU gamelistMenu;
HMENU gamelistSubMenu;
HMENU gameUserlistMenu;
HMENU gameUserlistSubMenu;
HMENU serverlistMenu;
HMENU serverlistSubMenu;
HMENU waitinglistMenu;
HMENU waitinglistSubMenu;
HMENU recentlistMenu;
HMENU recentlistSubMenu;
HMENU favoritelistMenu;
HMENU favoritelistSubMenu;
HMENU txtChatroomMenu;
HMENU txtChatroomSubMenu;
HMENU txtChatroomPmSubMenu;

GETTEXTEX txtChatroomA;
GETTEXTEX txtGameChatroomA;
int lstUserlistColumn = 1;
int lstServerlistKColumn = 4;
int lstServerlist3DColumn = 4;
int lstGamelistColumn = 4;
int lstRecentlistColumn = 4;
int lstFavoritelistColumn = 4;
int lstWaitinglistColumn = 0;
bool gameroom = false;

HINSTANCE hInstance;
HFONT hDefaultFont;

//Winsock Stuff
WSADATA startupInfo;
int mySocket;
int mySocketK;
int mySocket3D;
//int p2pSocket;
int mySocketWaiting;
//sockaddr_in p2pServerInfo;
//int p2pClientLength;
//sockaddr_in p2pClientInfo;
sockaddr_in socketInfo;
sockaddr_in socketInfoK;
sockaddr_in socketInfo3D;
sockaddr_in socketInfoWaiting;

struct SERVER_LIST{
	char host[128];
	char link[128];
	char wglink[128];
	unsigned long port;
};

SERVER_LIST kailleraServerList;
SERVER_LIST anti3DServerList;
//http://master.anti3d.com/raw_server_list2.php
//http://kaillera.com/raw_server_list2.php?wg=1&version=0.9


struct USER_LIST_COMMANDS{
	char commands[128];
	int time;
};


USER_LIST_COMMANDS userlistCommands[60];

HANDLE gameThread;
HANDLE recvLoopThread;
HANDLE continuousLoopThread;
HANDLE kailleraPingThread;
HANDLE ping3DThread;
bool pingingK = false;
bool pinging3D = false;

//Kaillera Stuff
char entryMsg[11] = "HELLO0.83\0";
u_short myPort = 27888;
char myAddress[512];

//Outbound Data
char globalPacket[15000];
unsigned short globalPacketSize;
unsigned short normalPacketSize;

char myBuffServerListK[10000];
char myBuffServerList3D[10000];
char myBuffWaiting[10000];
long serversLastMessage;
//Inbound Data
long msgCount;
long bytesRecvServerListK;
long bytesRecvServerList3D;
long bytesRecvWaiting;
//Outbound Data
unsigned short const MESSAGE_SIZE = 15;
unsigned short const MESSAGE_LENGTH = 5000;
unsigned short const MAX_INCOMING_BUFFER = 15;


struct INCOMING_BUFFER{
	char myBuff[MESSAGE_LENGTH * MESSAGE_SIZE];
};

INCOMING_BUFFER myBuff[MAX_INCOMING_BUFFER];
int myBuffCount = -1;



struct Messages{
    //char msgData[MESSAGE_LENGTH];
    unsigned char msgType;
    unsigned short msgLen;
    //unsigned short msgNum;
	unsigned long msgPos;
};
Messages serverMessage[MESSAGE_SIZE];


enum ConnectionType{
    lan = 1,
    excellent = 2,
    good = 3,
	average = 4,
	low = 5,
	bad = 6
};



struct USERS{
	char nick[1024];
	char ping[1024];
	char status[1024];
	char userID[1024];
	char type[1024];
};
USERS tempUsers[1000];
int tempUserCount = 0;

struct GAMES{
	char game[1024];
	char gameID[1024];
	char emulator[1024];
	char owner[1024];
	char users[1024];
	char status[1024];
};
GAMES tempGames[1000];
int tempGameCount = 0;



struct Packets{
    char packet[6000];
};
const int MAX_PACKETS = 3;
const int NORMAL_PACKETS = 3;
Packets myPackets[MAX_PACKETS];



//Information
int xPos = 0;
int yPos = 0;
char myServer[1024] = "Not in a Server\0";
char username[1024] = "Please enter a username!\0";
char emulator[128];
char serverIP[1024] = "127.0.0.1:27888\0";
unsigned long myPing;
//################
char myVersion[17] = "SCCPPE v0.89.10\0";
char cVersion[26] = "SupraclientCPPE v0.89.10\0";
char initText[1024] = "Supraclient CPPE [C++ Edition]\r\nVersion: 0.89.10  Sunday Jan 22, 2023\r\nSpecial Thanks to: Trac, Moosehead, & r@z\r\nOther Testers: Morphus56K, Paramount, Agenda Suicide, & okaygo\r\nAuthor: SupraFast - Daniel Strusser\r\nEmail: Dynomite1234@aol.com\r\n\r\n#To get started, click on the button below that says [Servers]\n\0";
char quit[1024] = "I'm using SupraclientCPPE v0.89.10\0";
//################
bool chatroom;

int mainBytesRecv;
long myGameID = -1;
long myUserID = -1;
hostent* hp;

struct Games{
	char game[1024];
};
char lastGameToPlay1[1024] = "LastGameToPlay1\0";
char lastGameToPlay2[1024] = "LastGameToPlay2\0";
char lastGameToPlay3[1024] = "LastGameToPlay3\0";
Games gameList[65535];
unsigned short totalGames = 0;



void showOptions(char show);

char connectionType = 1;
unsigned short frameDelay;
unsigned short myPlayerNumber;
unsigned short totalPlayers;
unsigned short inputSize;
int sizeOfEinput;
bool startedGame = false;
bool iQuit = false;
char currentGame[128];// = "Marvel Vs. Capcom: Clash of Super Heroes (US 980123)";
char gOwner[1024];
char gEmulator[1024];
char stage = 0;
bool gamePlaying = false;
unsigned short totalInput = 1;
int returnFrameSize;
short frameCount;
int blockFrame;
short int gameCount;
unsigned short inputFrame;
unsigned short lastPosFrameRecv;
unsigned short lastPosFrameSend;
unsigned short sizeOfVinput;
short frameRecv;
unsigned short frameSend;
unsigned short ePos;
short outCachePos;
bool returnInputSize = false;
bool storeFirstValues;
short inCachePos;
unsigned short eSize = 4096;
unsigned short const maxPlayers = 8;
unsigned short const cacheSize = 256;
unsigned short const bufferSize = 32;
unsigned short const outCacheSize = bufferSize * (unsigned short)bad;
unsigned long const inCacheSize = maxPlayers * outCacheSize;
unsigned long const lastInCachePos = (cacheSize - 1) * inCacheSize;
unsigned short const lastOutCachePos = (cacheSize - 1) * outCacheSize;
char inCache[cacheSize * inCacheSize];
char outCache[cacheSize * outCacheSize];
char eInput[bufferSize * maxPlayers * bad * MAX_INCOMING_BUFFER];
char vInput[3 + outCacheSize];
bool sendMax = false;
bool useCache = false;
void testNum(long num);
void fixLastGameToPlay();
void showServerlistK();
void showServerlist3D();
void showRecentlist();
void showFavoritelist();
void showWaitinglist();
void showChatroom(bool show);
void getServerListK();
void getServerList3D();
void getWaitingGames();
void saveChatroomLog(char text[]);
void saveGameroomLog(char text[]);
void parseServerListK();
void createChatroom();
void createInitialWindow();
int lastTabServer = 0;
bool showChatroomFirst = true;
bool waiting;
void exitGameThread();
void exitThreads();
void exitPingKThread();
void exitPing3DThread();
void copyToClipboard(LPSTR lpCmdLine);


bool hosting = false;
