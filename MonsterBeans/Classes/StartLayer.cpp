#include "StartLayer.h"
 bool IsSoundOff=false;

 void clickSound()
{
	if(!IsSoundOff)
        SimpleAudioEngine::sharedEngine()->playEffect("button_click.mp3");
}

StartLayer::StartLayer()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_winWidth = winSize.width;
	_winHeight = winSize.height;

	_loadedResources = 0;
}

StartLayer::~StartLayer()
{
}

CCScene* StartLayer::scene()
{
	CCScene *scene = CCScene::create();

	StartLayer *layer = StartLayer::create();
	scene->addChild(layer);

	return scene;
}

bool StartLayer::init()
{
	CCLayer::init();

	firstPlay(this,&StartLayer::FirstPlayFun);
	IsSoundOff=CCUserDefault::sharedUserDefault()->getBoolForKey("is_sound_off",false);
	char *selectedUser=new char;
	getSelectedUser(selectedUser);
	_nameLabel = CCLabelTTF::create(selectedUser, "Arial", 15);
    _nameLabel->setColor(ccRED);
    _nameLabel->setPosition(ccp(_winWidth/ 2+5,_winHeight / 2-210));
    _nameLabel->setScale(2*0.6);
	_nameLabel->setOpacity(0);
    this->addChild(_nameLabel,10,MENU_USER);


    CCSprite *background =NULL;
    if(IS_IPHONE5)
    {
        background=CCSprite::create("startScreen2.png");
    }
    else{
        background=CCSprite::create("startScreen.png");
    }
	background->setScale(2*0.5);
	background->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(background);


	_cloud = CCSprite::create("cloud.png");
	_cloud->setScale(2*0.5);
	_cloud->setPosition(ccp(_winWidth/ 2 - 28, 100));
	this->addChild(_cloud);

	_progressBar = CCProgressTimer::create(CCSprite::create("loadingProgress.png"));
	CCSize size = _progressBar->getContentSize();
	_progressBar->setScale(2*0.5);
	_progressBar->setType(kCCProgressTimerTypeBar);
	_progressBar->setMidpoint(ccp(0, 1));
	_progressBar->setBarChangeRate(ccp(1, 0));
	_progressBar->setPosition(ccp(_winWidth - size.width / 4-15, 50));
	this->addChild(_progressBar);

	_startItem = CCMenuItemImage::create(
		"startButtonNormal.png",
		"startButtonSelected.png",
		this,
		menu_selector(StartLayer::StartItemPressed));
	_startItem->setPosition(ccp(_winWidth / 2, _winHeight / 2 - 40));
	_startItem->setScale(2*0.5);
	_startItem->setEnabled(false);
	_startItem->setOpacity(0);
    

	_comItem = CCMenuItemImage::create(
		"comButtonNormal.png",
		"comButtonSelect.png",
		this,
		menu_selector(StartLayer::ComItemPressed));
	_comItem->setPosition(ccp(_winWidth / 2, _winHeight / 2 - 90));
	_comItem->setScale(2*0.5);
	_comItem->setEnabled(false);
	_comItem->setOpacity(0);

	_recordItem = CCMenuItemImage::create(
		"recordButtonNormal.png",
		"recordButtonSelect.png",
		this,
		menu_selector(StartLayer::RecordItemPressed));
	_recordItem->setPosition(ccp(_winWidth / 2, _winHeight / 2 - 140));
	_recordItem->setScale(2*0.5);
	_recordItem->setEnabled(false);
	_recordItem->setOpacity(0);

	_userItem = CCMenuItemImage::create(
		"user.png",
		"user.png",
		this,
		menu_selector(StartLayer::UserItemPressed));
	_userItem->setPosition(ccp(_winWidth / 2, _winHeight / 2 - 210));
	_userItem->setScale(2*0.5);
	_userItem->setEnabled(false);
	_userItem->setOpacity(1);


	

	CCMenuItemImage *soundOnItem = CCMenuItemImage::create("soundOn.png", NULL);
	CCMenuItemImage *soundOffItem = CCMenuItemImage::create("soundOff.png", NULL);

	CCMenuItemToggle *soundItem = CCMenuItemToggle::createWithTarget(
		this, 
		SEL_MenuHandler(&StartLayer::SoundItemPressed), 
		soundOnItem, 
		soundOffItem, 
		NULL);

	soundItem->setPosition(ccp(30, _winHeight - 30));
	soundItem->setSelectedIndex(IsSoundOff ? 1 : 0);
	soundItem->setScale(2*0.5);
    

	CCMenu *menu = CCMenu::create(_startItem, soundItem,_comItem,_recordItem,_userItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu,1,MENU);
	_menu=menu;
	LoadResources();

	return true;
}

void StartLayer::StartItemPressed(CCObject *sender)
{
	clickSound();
	MonsterBeansLayer::mode = MODE_TRADITION;
	CCDirector::sharedDirector()->pushScene(CCTransitionFlipX::create(0.6f, MonsterBeansLayer::scene()));
}

void StartLayer::SoundItemPressed(CCObject *sender)
{
	IsSoundOff = ((CCMenuItemToggle *)sender)->getSelectedIndex() == 1;
	CCUserDefault::sharedUserDefault()->setBoolForKey("is_sound_off", IsSoundOff);
	CCUserDefault::sharedUserDefault()->flush();
}

void StartLayer::LoadResources()
{
	CCTextureCache::sharedTextureCache()->addImageAsync("startButtonNormal.png", this, callfuncO_selector(StartLayer::LoadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("startButtonSelected.png", this, callfuncO_selector(StartLayer::LoadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("mainBg.png", this, callfuncO_selector(StartLayer::LoadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("alpha.png", this, callfuncO_selector(StartLayer::LoadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("clouds.png", this, callfuncO_selector(StartLayer::LoadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("pauseNormal.png", this, callfuncO_selector(StartLayer::LoadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync("pausePressed.png", this, callfuncO_selector(StartLayer::LoadingCallBack));
    
}

void StartLayer::LoadingCallBack(CCObject *sender)
{
	int from = 100 - _loadedResources / TOTALRESOURCENUM * 100;

	_loadedResources++;

	int to = 100 - _loadedResources / TOTALRESOURCENUM * 100;

	CCProgressFromTo *progressAction = CCProgressFromTo::create(0.5f, from, to);

	if(to != 0)
	{
		_progressBar->runAction(CCRepeat::create(progressAction, 1));
	}
	else
	{	
		_progressBar->runAction(CCSequence::create(
			progressAction,
			CCCallFuncND::create(this, callfuncND_selector(StartLayer::ResourcesLoaded), (void *)1),
			NULL
			));
	}
}

void StartLayer::ResourcesLoaded(CCNode *target, void *data)
{
	CCActionInterval *fadeIn = CCFadeIn::create(1.0f);
	_startItem->runAction(fadeIn);
	_startItem->setEnabled(true);
	CCActionInterval *fadeIn2 = CCFadeIn::create(1.0f);
	_comItem->runAction((CCActionInterval*)fadeIn->copy());
	_comItem->setEnabled(true);
	CCActionInterval *fadeIn3 = CCFadeIn::create(1.0f);
	_recordItem->runAction((CCActionInterval*)fadeIn->copy());
	_recordItem->setEnabled(true);
	CCActionInterval *fadeIn4 = CCFadeIn::create(1.0f);
	_userItem->runAction((CCActionInterval*)fadeIn->copy());
	_userItem->setEnabled(true);
	CCActionInterval *fadeIn5 = CCFadeIn::create(1.0f);
	_nameLabel->runAction((CCActionInterval*)fadeIn->copy());
	_menu->setEnabled(true);

	CCParticleExplosion *explosion = CCParticleExplosion::create();
	explosion->setAutoRemoveOnFinish(true);
	explosion->setTotalParticles(85);
	explosion->setLife(1.2f);
	explosion->setLifeVar(0.6f);
	explosion->setTexture(CCTextureCache::sharedTextureCache()->addImage("star.png"));
	explosion->setPosition(ccp(_cloud->getPositionX(), _cloud->getPositionY() ));
	//explosion->setStartColorVar(ccc4f(0, 0, 0, 0));
	explosion->setEndColorVar(ccc4f(0, 0, 0, 0));
	//explosion->setStartColor(ccc4f(0/255.f, 195/255.f, 0/255.f, 1));
	//explosion->setEndColor(ccc4f(0/255.f, 195/255.f, 0/255.f, 0));
	this->addChild(explosion,20);
	_cloud->setVisible(false);
}


void StartLayer::ComItemPressed(CCObject *sender)
{
	clickSound();
	MonsterBeansLayer::mode = MODE_STEP;
	CCDirector::sharedDirector()->pushScene(CCTransitionFlipX::create(0.6f, MonsterBeansLayer::scene()));
}
void StartLayer::RecordItemPressed(CCObject *sender)
{
    clickSound();
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.6f, RecordLayer::scene()));
}
void StartLayer::UserItemPressed(CCObject *sender)
{
	clickSound();
	_menu->setEnabled(false);
	CCLayer *layer=UserChange::create();
	layer->setPosition(ccp(0, 0));
	this->addChild(layer,10);
}



void StartLayer::FirstPlayFun()
{
	WriteGameData("user_number",0);
	addUser("default");
	setMofaNumber(2,MODE_TRADITION);
	setMofaNumber(2,MODE_STEP);
}