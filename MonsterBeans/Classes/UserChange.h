#ifndef __USERCHANGE_H__
#define __USERCHANGE_H__
#include "StartLayer.h"
#include "GameData.h"
#include "GUI/CCEditBox/CCEditBox.h"
//#include "GUI/CCControlExtension/CCControlButton.h"
//#include "GUI/CCControlExtension/CCScale9Sprite.h"
using namespace CocosDenshion;
using namespace cocos2d::extension;
using namespace cocos2d;

#define USER_COUNT 5
struct UserItem
{
	CCMenuItemImage *user_bg;
	CCMenuItemImage *user_edit;
	CCLabelTTF *uname;
    char username[20];
};

class UserChange:public cocos2d::CCLayer
{
public:
	virtual bool init();
	CREATE_FUNC(UserChange);
	
private:
	float _winWidth;
	float _winHeight;
	UserItem *_user_items[USER_COUNT];
	int _user_count;
	CCMenu *_menu;
	CCSprite * _select;
	CCEditBox* m_pEditName;
    CCSprite *background;
	char *_oldname;
	void editUser(CCObject*);
	void selectUser(CCObject*);
	void clickAddButton(CCObject *node);
	void close(CCObject*);
	void preLayer(CCObject*);
	void initView();
	void editView(char*);
	void insertView();
	void DeleteUser(CCObject*);
	void AddUser(CCObject*);
	void UpdateUser(CCObject *);
	void changeUserLabel(const char*);
	void clickSound();
};

#endif