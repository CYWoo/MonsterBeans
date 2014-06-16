#include "RecordLayer.h"
#define FONTA "Arial"
#define LINE_HEIGHT 30
#define LINE_FIRST_Y _winHeight - 80-iphone5_pos
#define LINE_FIRST_X _winWidth / 2 - 115
RecordLayer::RecordLayer()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_winWidth = winSize.width;
	_winHeight = winSize.height;
}

CCScene * RecordLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    RecordLayer *layer = RecordLayer::create();
    scene->addChild(layer);
    
    return scene;
}

bool RecordLayer::init()
{
    CCLayer::init();
    
    CCSprite *background =NULL;
    iphone5_pos=0;
    if(IS_IPHONE5)
    {
        background=CCSprite::create("mainBg2.png");
        iphone5_pos=45;
    }
    else{
        background=CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("mainBg.png"));
    }

	background->setScale(2*0.5f);
	background->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(background, 1);

	CCSprite *alpha = CCSprite::create("score_frame.png");
	alpha->setScale(2*0.5f);
	alpha->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(alpha, 3);

	CCSprite *clouds = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("clouds.png"));
	clouds->setScale(2*0.5f);
	clouds->setPosition(ccp(_winWidth / 2, 45));
	this->addChild(clouds, 2);
    
    CCMenuItemImage *backItem = CCMenuItemImage::create("back_button_h.png", "back_button_h.png", this, SEL_MenuHandler(&RecordLayer::BackItemPressed));
    backItem->setPosition(ccp(30, _winHeight - 25-iphone5_pos));
	backItem->setScale(2*0.5f);

    CCMenu *mainMenu = CCMenu::create(backItem, NULL);
    mainMenu->setPosition(ccp(0, 0));
    this->addChild(mainMenu,10);
    

	CCSprite *title = CCSprite::create("title_record.png");
	title->setScale(2*0.5);
    title->setPosition(ccp(115, _winHeight - 25-iphone5_pos));
	 this->addChild(title,10);


	CCSprite *sprite = CCSprite::create("mode1_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y));
	 this->addChild(sprite,10);

	sprite = CCSprite::create("player_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT));
	 this->addChild(sprite,10);

	 sprite = CCSprite::create("score_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT));
	 this->addChild(sprite,10);

	  sprite = CCSprite::create("level_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT));
	 this->addChild(sprite,10);



	CCLabelTTF *label = CCLabelTTF::create("1", FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X, LINE_FIRST_Y-LINE_HEIGHT*2));
    this->addChild(label,10);

	char sorceStr[40];
	char uname[40];
	char level[40];
	int sorce;
	
	sorce=ReadGameData("mode1_hightSorce_1");
	if(sorce>0)
	{
		sprintf(sorceStr,"%d",sorce);
		ReadGameDataString("mode1_username_1",uname);
		sprintf(level,"%d", ReadGameData("mode1_hightLevel_1"));
	}
	else{
		sprintf(sorceStr,"--");
		sprintf(uname,"--");
		sprintf(level,"--");
	}

	label = CCLabelTTF::create(uname, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*2));
	 this->addChild(label,10);

	 label = CCLabelTTF::create(sorceStr, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccRED);
    label->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT*2));
	 this->addChild(label,10);

	  label = CCLabelTTF::create(level, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT*2));
	 this->addChild(label,10);

	label = CCLabelTTF::create("2", FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X, LINE_FIRST_Y-LINE_HEIGHT*3));
    this->addChild(label,10);

	sorce=ReadGameData("mode1_hightSorce_2");
	if(sorce>0)
	{
		sprintf(sorceStr,"%d",sorce);
		ReadGameDataString("mode1_username_2",uname);
		sprintf(level,"%d", ReadGameData("mode1_hightLevel_2"));
	}
	else{
		sprintf(sorceStr,"--");
		sprintf(uname,"--");
		sprintf(level,"--");
	}

	label = CCLabelTTF::create(uname, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*3));
	 this->addChild(label,10);

	 label = CCLabelTTF::create(sorceStr, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccRED);
    label->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT*3));
	 this->addChild(label,10);

	  label = CCLabelTTF::create(level, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT*3));
	 this->addChild(label,10);


	 sorce=ReadGameData("mode1_hightSorce_3");
	if(sorce>0)
	{
		sprintf(sorceStr,"%d",sorce);
		ReadGameDataString("mode1_username_3",uname);
		sprintf(level,"%d", ReadGameData("mode1_hightLevel_3"));
	}
	else{
		sprintf(sorceStr,"--");
		sprintf(uname,"--");
		sprintf(level,"--");
	}

	label = CCLabelTTF::create("3", FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X, LINE_FIRST_Y-LINE_HEIGHT*4));
    this->addChild(label,10);

	label = CCLabelTTF::create(uname, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*4));
	 this->addChild(label,10);

	 label = CCLabelTTF::create(sorceStr, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccRED);
    label->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT*4));
	 this->addChild(label,10);

	  label = CCLabelTTF::create(level, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT*4));
	 this->addChild(label,10);




	sprite = CCSprite::create("mode2_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*6));
	 this->addChild(sprite,10);

	sprite = CCSprite::create("player_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*7));
	 this->addChild(sprite,10);

	 sprite = CCSprite::create("score_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT*7));
	 this->addChild(sprite,10);

	  sprite = CCSprite::create("level_text.png");
	sprite->setScale(2*0.5);
    sprite->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT*7));
	 this->addChild(sprite,10);

	  sorce=ReadGameData("mode2_hightSorce_1");
	if(sorce>0)
	{
		sprintf(sorceStr,"%d",sorce);
		ReadGameDataString("mode2_username_1",uname);
		sprintf(level,"%d", ReadGameData("mode2_hightLevel_1"));
	}
	else{
		sprintf(sorceStr,"--");
		sprintf(uname,"--");
		sprintf(level,"--");
	}

	label = CCLabelTTF::create("1", FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X, LINE_FIRST_Y-LINE_HEIGHT*8));
    this->addChild(label,10);

	label = CCLabelTTF::create(uname, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*8));
	 this->addChild(label,10);

	 label = CCLabelTTF::create(sorceStr, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccRED);
    label->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT*8));
	 this->addChild(label,10);

	  label = CCLabelTTF::create(level, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT*8));
	 this->addChild(label,10);


	  sorce=ReadGameData("mode2_hightSorce_2");
	if(sorce>0)
	{
		sprintf(sorceStr,"%d",sorce);
		ReadGameDataString("mode2_username_2",uname);
		sprintf(level,"%d", ReadGameData("mode2_hightLevel_2"));
	}
	else{
		sprintf(sorceStr,"--");
		sprintf(uname,"--");
		sprintf(level,"--");
	}

	label = CCLabelTTF::create("2", FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X, LINE_FIRST_Y-LINE_HEIGHT*9));
    this->addChild(label,10);

	label = CCLabelTTF::create(uname, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*9));
	 this->addChild(label,10);

	 label = CCLabelTTF::create(sorceStr, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccRED);
    label->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT*9));
	 this->addChild(label,10);

	  label = CCLabelTTF::create(level, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT*9));
	 this->addChild(label,10);

	  sorce=ReadGameData("mode2_hightSorce_3");
	if(sorce>0)
	{
		sprintf(sorceStr,"%d",sorce);
		ReadGameDataString("mode2_username_3",uname);
		sprintf(level,"%d", ReadGameData("mode2_hightLevel_3"));
	}
	else{
		sprintf(sorceStr,"--");
		sprintf(uname,"--");
		sprintf(level,"--");
	}

	label = CCLabelTTF::create("3", FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X, LINE_FIRST_Y-LINE_HEIGHT*10));
    this->addChild(label,10);

	label = CCLabelTTF::create(uname, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+40, LINE_FIRST_Y-LINE_HEIGHT*10));
	 this->addChild(label,10);

	 label = CCLabelTTF::create(sorceStr, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccRED);
    label->setPosition(ccp(LINE_FIRST_X+120, LINE_FIRST_Y-LINE_HEIGHT*10));
	 this->addChild(label,10);

	  label = CCLabelTTF::create(level, FONTA, 15);
	label->setScale(2*0.5);
    label->setColor(ccc3(8, 104, 179));
    label->setPosition(ccp(LINE_FIRST_X+200, LINE_FIRST_Y-LINE_HEIGHT*10));
	 this->addChild(label,10);

    return true;
}

void RecordLayer::BackItemPressed(CCObject *item)
{
    clickSound();
    CCDirector::sharedDirector()->popScene();
}
