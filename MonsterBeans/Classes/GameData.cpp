#include "GameData.h"
int ReadGameData(const char *key)
{
    int data = CCUserDefault::sharedUserDefault()->getIntegerForKey(key,0);

    if(data)
    {
        int salt1 = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SALT_1);
        int salt2 = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SALT_2);
        int salt3 = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SALT_3);
        
        int a = salt1 / 11111 + salt2 / 7;
        int b = salt1 / 3 + salt3 / 17 + 77356;
        int c = salt2 * 11 - salt3;
        int d = (a + b + c) % RAND_MAX;
        
        data = data - d;
    }
    
    return data;
}

void WriteGameData(const char *key, int data)
{
    int value = 0;
    
    if(data)
    {
        int salt1 = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SALT_1);
        int salt2 = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SALT_2);
        int salt3 = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_SALT_3);
        
        int a = salt1 / 11111 + salt2 / 7;
        int b = salt1 / 3 + salt3 / 17 + 77356;
        int c = salt2 * 11 - salt3;
        int d = (a + b + c) % RAND_MAX;
        
        value = data + d;
    }
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(key, value);
    CCUserDefault::sharedUserDefault()->flush();
}

void ReadGameDataString(const char *key,char *result)
{
	string data_str=CCUserDefault::sharedUserDefault()->getStringForKey(key,"");
	strcpy(result,data_str.c_str());
}


void WriteGameDataString(const char *key, const char *data)
{
    string str(data);
    CCUserDefault::sharedUserDefault()->setStringForKey(key, data);
    CCUserDefault::sharedUserDefault()->flush();
}

void firstPlay(StartLayer* pA,PTRFUN pFun)
{
	if(!ReadGameData(KEY_FIRST_TIME_PLAY))
    {
        srand(time(0));
        int i = 0;
        while(i < 3)
        {
            int r = rand();
            if(r > 5000)
            {
                switch(i)
                {
                    case 0:
                        CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_SALT_1, r);
                        break;
                    case 1:
                        CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_SALT_2, r);
                        break;
                    case 2:
                        CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_SALT_3, r);
                        break;
                }
                
                ++i;
            }
        }
        
        CCUserDefault::sharedUserDefault()->flush();
        WriteGameData(KEY_FIRST_TIME_PLAY, time(0));


		(pA->*pFun)(); 
    }
}

int getRanking(int sorce,int level,int mode)
{
	if(mode==MODE_TRADITION)
	{
		int thirdSorce=ReadGameData("mode1_hightSorce_3");
		int secondSorce=ReadGameData("mode1_hightSorce_2");
		int firstSorce=ReadGameData("mode1_hightSorce_1");
		if(sorce<thirdSorce)
			return -1;
		else if(sorce<secondSorce)
		{
			char *name=new char;
			ReadGameDataString("user_selected",name);
			WriteGameDataString("mode1_username_3",name);
			WriteGameData("mode1_hightSorce_3",sorce);
			WriteGameData("mode1_hightLevel_3",level);
			return 3;
		}
		else if(sorce<firstSorce)
		{
			char utmp[20];
			ReadGameDataString("mode1_username_2",utmp);
			WriteGameDataString("mode1_username_3",utmp);
			WriteGameData("mode1_hightSorce_3",ReadGameData("mode1_hightSorce_2"));
			WriteGameData("mode1_hightLevel_3",ReadGameData("mode1_hightLevel_2"));

			char *name=new char;
			ReadGameDataString("user_selected",name);
			WriteGameDataString("mode1_username_2",name);
			WriteGameData("mode1_hightSorce_2",sorce);
			WriteGameData("mode1_hightLevel_2",level);
			return 2;
		}
		else
		{
			char utmp[20];
			ReadGameDataString("mode1_username_2",utmp);
			WriteGameDataString("mode1_username_3",utmp);
			WriteGameData("mode1_hightSorce_3",ReadGameData("mode1_hightSorce_2"));
			WriteGameData("mode1_hightLevel_3",ReadGameData("mode1_hightLevel_2"));

			char utmp1[20];
			ReadGameDataString("mode1_username_1",utmp1);
			WriteGameDataString("mode1_username_2",utmp1);
			WriteGameData("mode1_hightSorce_2",ReadGameData("mode1_hightSorce_1"));
			WriteGameData("mode1_hightLevel_2",ReadGameData("mode1_hightLevel_1"));

			char *name=new char;
			ReadGameDataString("user_selected",name);
			WriteGameDataString("mode1_username_1",name);
			WriteGameData("mode1_hightSorce_1",sorce);
			WriteGameData("mode1_hightLevel_1",level);
			return 1;
		}
	}
	else if(mode==MODE_STEP)
	{
		int thirdSorce=ReadGameData("mode2_hightSorce_3");
		int secondSorce=ReadGameData("mode2_hightSorce_2");
		int firstSorce=ReadGameData("mode2_hightSorce_1");
		if(sorce<thirdSorce)
			return -1;
		else if(sorce<secondSorce)
		{
			char *name=new char;
			ReadGameDataString("user_selected",name);
			WriteGameDataString("mode2_username_3",name);
			WriteGameData("mode2_hightSorce_3",sorce);
			WriteGameData("mode2_hightLevel_3",level);
			return 3;
		}
		else if(sorce<firstSorce)
		{
			char utmp[20];
			ReadGameDataString("mode2_username_2",utmp);
			WriteGameDataString("mode2_username_3",utmp);
			WriteGameData("mode2_hightSorce_3",ReadGameData("mode2_hightSorce_2"));
			WriteGameData("mode2_hightLevel_3",ReadGameData("mode2_hightLevel_2"));

			char *name=new char;
			ReadGameDataString("user_selected",name);
			WriteGameDataString("mode2_username_2",name);
			WriteGameData("mode2_hightSorce_2",sorce);
			WriteGameData("mode2_hightLevel_2",level);
			return 2;
		}
		else
		{
			char utmp[20];
			ReadGameDataString("mode2_username_2",utmp);
			WriteGameDataString("mode2_username_3",utmp);
			WriteGameData("mode2_hightSorce_3",ReadGameData("mode2_hightSorce_2"));
			WriteGameData("mode2_hightLevel_3",ReadGameData("mode2_hightLevel_2"));

			char utmp1[20];
			ReadGameDataString("mode2_username_1",utmp1);
			WriteGameDataString("mode2_username_2",utmp1);
			WriteGameData("mode2_hightSorce_2",ReadGameData("mode2_hightSorce_1"));
			WriteGameData("mode2_hightLevel_2",ReadGameData("mode2_hightLevel_1"));

			char *name=new char;
			ReadGameDataString("user_selected",name);
			WriteGameDataString("mode2_username_1",name);
			WriteGameData("mode2_hightSorce_1",sorce);
			WriteGameData("mode2_hightLevel_1",level);
			return 1;
		}
	}
	return -1;
}

int getGameUsers(char* a[])
{
	int userNum= ReadGameData("user_number");
	for(int i=1;i<=userNum;i++)
	{
		char namestr[20];
		sprintf(namestr,"user_name_%d",i);
		ReadGameDataString(namestr,a[i]);
	}
	return userNum;
}

void SaveGameData(int level,int scorce,char gamedata[101],int step)
{
	if (scorce==0) return;
	char *username=new char;
	ReadGameDataString("user_selected",username);
	if(step<0)
	{
		char levelstr[40];
		sprintf(levelstr,"user_mode1_level_%s",username);
		WriteGameData(levelstr,level);
		char scorcestr[40];
		sprintf(scorcestr,"user_mode1_scorce_%s",username);
		WriteGameData(scorcestr,scorce);
		char gamedatastr[40];
		sprintf(gamedatastr,"user_mode1_gameData_%s",username);
		WriteGameDataString(gamedatastr,gamedata);
		char tmp[40];
		sprintf(tmp,"is_save_mode1_%s",username);
		CCUserDefault::sharedUserDefault()->setBoolForKey(tmp,true);
	}
	else
	{

		char levelstr[40];
		sprintf(levelstr,"user_mode2_level_%s",username);
		WriteGameData(levelstr,level);
		char scorcestr[40];
		sprintf(scorcestr,"user_mode2_scorce_%s",username);
		WriteGameData(scorcestr,scorce);
		char gamedatastr[40];
		sprintf(gamedatastr,"user_mode2_gameData_%s",username);
		WriteGameDataString(gamedatastr,gamedata);

		char stepstr[40];
		sprintf(stepstr,"user_mode2_step_%s",username);
		WriteGameData(stepstr,step);

		char tmp[40];
		sprintf(tmp,"is_save_mode2_%s",username);
		CCUserDefault::sharedUserDefault()->setBoolForKey(tmp,true);
	}
}

void RecoverGameData(int &level,int &scorce,char* gamedata,int &magicNum,int mode, int *step)
{

	 char *username=new char;
	ReadGameDataString("user_selected",username);
	if(mode==MODE_TRADITION)
	{
		char levelstr[40];
		sprintf(levelstr,"user_mode1_level_%s",username);
		level=ReadGameData(levelstr);
		char scorcestr[40];
		sprintf(scorcestr,"user_mode1_scorce_%s",username);
		scorce=ReadGameData(scorcestr);
		char magicNumstr[40];
		sprintf(magicNumstr,"user_mode1_magicNum_%s",username);
		magicNum=ReadGameData(magicNumstr);

		char gamedatastr[40];
		sprintf(gamedatastr,"user_mode1_gameData_%s",username);

		ReadGameDataString(gamedatastr,gamedata);
		
	}
	else
	{

		char levelstr[40];
		sprintf(levelstr,"user_mode2_level_%s",username);
		level=ReadGameData(levelstr);

		char scorcestr[40];
		sprintf(scorcestr,"user_mode2_scorce_%s",username);
		scorce=ReadGameData(scorcestr);

		char magicNumstr[40];
		sprintf(magicNumstr,"user_mode2_magicNum_%s",username);
		magicNum=ReadGameData(magicNumstr);

		char gamedatastr[40];
		sprintf(gamedatastr,"user_mode2_gameData_%s",username);
		ReadGameDataString(gamedatastr,gamedata);

		char stepstr[40];
		sprintf(stepstr,"user_mode2_step_%s",username);
		*step=ReadGameData(stepstr);
	}
}



bool addUser(const char *name)
{
	int userNum= ReadGameData("user_number");
	for(int i=1;i<=userNum;i++)
	{
		char namestr[20];
		sprintf(namestr,"user_name_%d",i);
		char tmp_name[20];
		ReadGameDataString(namestr,tmp_name);
		if(strcmp(tmp_name,name)==0)
		{
			return false;
		}
	}
	char namestr[20];
	WriteGameData("user_number",++userNum);
	sprintf(namestr,"user_name_%d",userNum);
	WriteGameDataString(namestr,name);
	WriteGameDataString("user_selected",name);
	return true;
}

bool deleteUser(const char *name)
{
	int userNum= ReadGameData("user_number");
	if(userNum==1)
		return false;
	int index=userNum;
	for(;index>=1;index--)
	{
		char namestr[20];
		sprintf(namestr,"user_name_%d",index);
		char tmp_name[20];
		ReadGameDataString(namestr,tmp_name);
		if(strcmp(tmp_name,name)==0)
		{
			break;
		}
	}
	for(int i=index;i<userNum;i++)
	{
		char namestr[20];
		sprintf(namestr,"user_name_%d",i+1);
		char next_name[20];
		ReadGameDataString(namestr,next_name);
		char cur_namestr[20];
		sprintf(cur_namestr,"user_name_%d",i);
		WriteGameDataString(cur_namestr,next_name);
	}
	WriteGameData("user_number",--userNum);
	char tmp_name[20];
	ReadGameDataString("user_selected",tmp_name);
	if(strcmp(tmp_name,name)==0)
	{
		ReadGameDataString("user_name_1",tmp_name);
		WriteGameDataString("user_selected",tmp_name);
	}
	
	return true;
}

bool updateUser(const char *oldname,const char *newname)
{
	int userNum= ReadGameData("user_number");

	for(int i=1;i<=userNum;i++)
	{
		char namestr[20];
		sprintf(namestr,"user_name_%d",i);
		char tmp_name[20];
		ReadGameDataString(namestr,tmp_name);
		if(strcmp(tmp_name,newname)==0)
		{
			return false;
		}
	}

	int index=userNum;
	for(;index>=1;index--)
	{
		char namestr[20];
		sprintf(namestr,"user_name_%d",index);
		char tmp_name[20];
		ReadGameDataString(namestr,tmp_name);
		if(strcmp(tmp_name,oldname)==0)
		{
			WriteGameDataString(namestr,newname);
			char select_name[20];
			ReadGameDataString("user_selected",select_name);
			if(strcmp(select_name,oldname)==0)
			{
				WriteGameDataString("user_selected",newname);
			}
			return true;
		}
	}
	return false;
}

void getSelectedUser(char *result)
{
	ReadGameDataString("user_selected",result);
}

 bool setMofaNumber(int count,int mode)
{
	char username[20];
	ReadGameDataString("user_selected",username);
	char tmp[20];
	if(mode==MODE_TRADITION)
	{
		sprintf(tmp,"mode1_mofa_%s",username);
	}
	else
	{
		sprintf(tmp,"mode2_mofa_%s",username);
	}
	int num= ReadGameData(tmp);
	num+=count;
	if(num<0)
		return false;
	WriteGameData(tmp,num);
    return true;
}

 int getMofaNumber(int mode)
{
	char username[20];
	ReadGameDataString("user_selected",username);
	char tmp[20];
	if(mode==MODE_TRADITION)
	{
		sprintf(tmp,"mode1_mofa_%s",username);
	}
	else
	{
		sprintf(tmp,"mode2_mofa_%s",username);
	}
	int num= ReadGameData(tmp);
	return num;
}

  bool isSaveGame(int mode)
  {
	  char username[20];
	ReadGameDataString("user_selected",username);
	char tmp[40];
	if(mode==MODE_TRADITION)
	{
		sprintf(tmp,"is_save_mode1_%s",username);
	}
	else
	{
		sprintf(tmp,"is_save_mode2_%s",username);
	}
	bool b= CCUserDefault::sharedUserDefault()->getBoolForKey(tmp,false);
	return b;
  }

   void deleteSave(int mode)
  {
	  char username[20];
	ReadGameDataString("user_selected",username);
	char tmp[40];
	if(mode==MODE_TRADITION)
	{
		sprintf(tmp,"is_save_mode1_%s",username);
	}
	else
	{
		sprintf(tmp,"is_save_mode2_%s",username);
	}
	 CCUserDefault::sharedUserDefault()->setBoolForKey(tmp,false);
  }