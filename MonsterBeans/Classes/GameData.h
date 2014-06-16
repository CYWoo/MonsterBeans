#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "Common.h"
using namespace cocos2d;
using namespace std;

#define KEY_SALT_1 "FBE42A" //ян1
#define KEY_SALT_2 "C837CD" //ян2
#define KEY_SALT_3 "EF34A1" //ян3
#define KEY_FIRST_TIME_PLAY "F14235"
#define MODE_TRADITION 1
#define MODE_STEP 2

class StartLayer;
typedef void (StartLayer::*PTRFUN)(); 

extern int ReadGameData(const char *key);


extern void WriteGameData(const char *key, int data);


extern  void ReadGameDataString(const char *key,char *result);


extern void WriteGameDataString(const char *key, const char *data);

extern void firstPlay(StartLayer* pA,PTRFUN pFun);

extern int getRanking(int sorce,int level,int mode);

extern void SaveGameData(int level,int scorce,char gamedata[101],int step);

extern void RecoverGameData(int &level,int &scorce,char gamedata[101],int &magicNum,int mode, int *step=NULL);

extern bool addUser(const char *name);

extern bool deleteUser(const char *name);

extern bool updateUser(const char *oldname,const char *newname);

extern void getSelectedUser(char *result);

extern bool setMofaNumber(int count,int mode);
extern int getMofaNumber(int mode);
extern bool isSaveGame(int mode);
extern void deleteSave(int mode);
#endif