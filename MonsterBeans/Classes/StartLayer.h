#ifndef __MONSTERBEANS_SCENE_H__
#define __MONSTERBEANS_SCENE_H__

#include "Common.h"
using namespace CocosDenshion;
using namespace cocos2d;

#define TOTALRESOURCENUM 7
#define MENU 8
#define MENU_USER 9


class StartLayer : public cocos2d::CCLayer
{
private:
	float _winWidth;
	float _winHeight;
	int _loadedResources;

	CCSprite *_cloud;
	CCProgressTimer *_progressBar;
	CCMenuItemImage *_startItem;
	CCMenuItemImage *_comItem;
	CCMenuItemImage *_recordItem;
	CCMenuItemImage *_userItem;
	CCLabelTTF *_nameLabel;
	CCMenu *_menu;

	void StartItemPressed(CCObject *sender);
	void ComItemPressed(CCObject *sender);
	void RecordItemPressed(CCObject *sender);
	void UserItemPressed(CCObject *sender);
	void SoundItemPressed(CCObject *sender);
	void FirstPlayFun();

	void LoadResources();
	void LoadingCallBack(CCObject *sender);
	void ResourcesLoaded(CCNode *target, void *data);

public:
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(StartLayer);

	StartLayer();
	~StartLayer();
};

extern bool IsSoundOff;
extern void clickSound();
#endif