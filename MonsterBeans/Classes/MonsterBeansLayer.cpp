#include "MonsterBeansLayer.h"
#define BEAN_TEXTURE_RECT(KIND) CCRectMake(30.5 * KIND-30, 0, 30.5, 30.5)
#define PEN_TEXTURE_RECT(KIND) CCRectMake(25.5 * KIND-25.5, 0, 25.5, 50)
#define BEAN_RADIUS 15.25
#define BEAN_WIDTH 30.5
#define BEAN_HEIGHT 30.5
#define BEGIN_POS_X 6+alpha->getPositionX() - alpha->getContentSize().width/2
#define BEGIN_POS_Y 20+alpha->getPositionY() - alpha->getContentSize().height/2
#define BEGIN_POS_Y2 5+alpha->getPositionY() - alpha->getContentSize().height/2
#define CELL_TO_POSITION(COLUMN,ROW)  ccp(BEGIN_POS_X + BEAN_WIDTH * COLUMN +BEAN_RADIUS ,BEGIN_POS_Y+BEAN_HEIGHT*ROW)
#define POSITION_TO_CELL(PT)  ccp((PT.x-(BEGIN_POS_X))/BEAN_WIDTH ,(PT.y-(BEGIN_POS_Y2))/BEAN_HEIGHT)
#define MIN_BEANS 2
#define SCORE_BASE 5
#define TSCORE_BASE 1000
#define PEN_NULL -11
#define PEN_Y alpha->getPositionY() + alpha->getContentSize().height / 2 -18
#define PEN_X(index) BEGIN_POS_X+12+(index-1)*40
#define INT2CHAR 22
#define TAG_PASS 24
#define COM true
#define FONT "Arial"
#define STEPCOUNT 27


using namespace cocos2d;
int MonsterBeansLayer::mode = MODE_TRADITION;

MonsterBeansLayer::MonsterBeansLayer()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_winWidth = winSize.width;
	_winHeight = winSize.height;
}

MonsterBeansLayer::~MonsterBeansLayer()
{
}

CCScene* MonsterBeansLayer::scene()
{
	CCScene *scene = CCScene::create();

	MonsterBeansLayer *layer = MonsterBeansLayer::create();
	scene->addChild(layer);

	return scene;
}


bool MonsterBeansLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	
	CCPoint touchpoint = pTouch->getLocationInView();
	touchpoint=CCDirector::sharedDirector()->convertToGL(touchpoint);
	if(touchpoint.x<BEGIN_POS_X||touchpoint.x>BEGIN_POS_X+BEAN_WIDTH*10||touchpoint.y<BEGIN_POS_Y2
		||touchpoint.y>BEGIN_POS_Y2+BEAN_HEIGHT*10)
		return false;
	touchpoint=POSITION_TO_CELL(touchpoint);
	if(_map[(int)touchpoint.x][(int)touchpoint.y]==NULL)
		return true;
	if(_pen!=PEN_NULL&&haveMofa())//是否使用了画笔道具
	{
		CCSprite *curBean=_map[(int)touchpoint.x][(int)touchpoint.y];
		BeanInfo *beanData= (BeanInfo*)(curBean->getUserData());
		beanData->kind=_pen;
		switch (_pen)
		{
		case BEAN_BLUE:
			curBean->setTextureRect((BEAN_TEXTURE_RECT(BEAN_BLUE)));
			break;
		case BEAN_RED:
			curBean->setTextureRect((BEAN_TEXTURE_RECT(BEAN_RED)));
			break;
		case BEAN_YELLOW:
			curBean->setTextureRect((BEAN_TEXTURE_RECT(BEAN_YELLOW)));
			break;
		case BEAN_GREY:
			curBean->setTextureRect((BEAN_TEXTURE_RECT(BEAN_GREY)));
			break;
		case BEAN_GREEN:
			curBean->setTextureRect((BEAN_TEXTURE_RECT(BEAN_GREEN)));
			break;	
		}
		curBean->setScale(2*0.01);
		curBean->runAction(
				CCSequence::create(
						CCDelayTime::create(0.1),
						CCScaleTo::create(0.2f, 1.0),
						NULL
					)
				);
		_pen=PEN_NULL;
		_selectPen->setPosition(ccp(PEN_X(PEN_NULL),PEN_Y));
		return true;
	}
	if(_pickedBeans->count()<MIN_BEANS)	//是否第一次点击豆豆
	{
		CCSprite *curBean=_map[(int)touchpoint.x][(int)touchpoint.y];
		touchOnce(curBean);
	}
	else
	{
		
		CCSprite *curBean=_map[(int)touchpoint.x][(int)touchpoint.y];
		if(_pickedBeans->containsObject(curBean))		//两次单击的位置都是同一区域的豆豆，消除它
		{
			createAndAddScore();	//计算得分
			removeBeans();      //删除连在一片的豆豆
			combo();
			moveBeans();		//调整怪物豆的位置
			if(MonsterBeansLayer::mode==MODE_STEP)//限步模式减少步数
			{
				--_stepCount;
				char tmp_step[10];
				sprintf(tmp_step,"%d",_stepCount);
                this->formatString(tmp_step);
				this->_step_label->setString(tmp_step);
			}
			if(isGameEnd())
			{
				int leftCount=0;
				for(int i=0;i<BEAN_ROW_NUM;i++)
				{
					for(int j=0;j<BEAN_COL_NUM;j++)
					{
						if(_map[i][j]!=NULL)
							++leftCount;
					}
				}
				int leftScore=leftCount>10?0:(10-leftCount)*200;
			
				_level_complete=CCLayer::create();
				CCSprite *bg=CCSprite::create("clearbg.png");
				bg->setScale(2*0.5);
				bg->setPosition(ccp(0, 0));
				_level_complete->addChild(bg);

				char tmp_str[100];
				sprintf(tmp_str, "%d", leftCount);
				formatString(tmp_str);
				CCLabelAtlas *levelTTF = CCLabelAtlas::create(tmp_str, "numbers.png", 15, 25, '1');
				levelTTF->setPosition(ccp(45,15));
				levelTTF->setScale(2*0.5);
				levelTTF->setColor(ccYELLOW);
				_level_complete->addChild(levelTTF,2);

				char tmp_str2[100];
				sprintf(tmp_str2, "%d", leftScore);
				formatString(tmp_str2);
				CCLabelAtlas *scoreTTF = CCLabelAtlas::create(tmp_str2, "numbers.png", 15, 25, '1');
				scoreTTF->setPosition(ccp(45, -20));
				scoreTTF->setScale(2*0.5);
				scoreTTF->setColor(ccRED);
				_level_complete->addChild(scoreTTF,2);
				_level_complete->setPosition(ccp(_winWidth+30, _winHeight/2+55));
				this->addChild(_level_complete,70);

				if(MonsterBeansLayer::mode==MODE_STEP)
				{
					CCSprite *stepbg=CCSprite::create("clearbg_mode2.png");
					stepbg->setScale(2*0.5);
					stepbg->setPosition(ccp(0, -90));
					_level_complete->addChild(stepbg);

					char tmp_step[100];
					sprintf(tmp_step, "%d", this->_stepCount);
					formatString(tmp_step);
					CCLabelAtlas *stepLabel = CCLabelAtlas::create(tmp_step, "numbers.png", 15, 25, '1');
					stepLabel->setPosition(ccp(45,15-100));
					stepLabel->setScale(2*0.5);
					stepLabel->setColor(ccYELLOW);
					_level_complete->addChild(stepLabel,2);

					int stepSorce=this->_stepCount*100;
					char tmp_Sorce[100];
					sprintf(tmp_Sorce, "%d", stepSorce);
					formatString(tmp_Sorce);
					CCLabelAtlas *scoreTTF = CCLabelAtlas::create(tmp_Sorce, "numbers.png", 15, 25, '1');
					scoreTTF->setPosition(ccp(45, -110));
					scoreTTF->setScale(2*0.5);
					scoreTTF->setColor(ccRED);
					_level_complete->addChild(scoreTTF,2);
					leftScore+=stepSorce;
				}

				_level_complete->runAction(CCSequence::create(
					//CCDelayTime::create(0.1f),
					CCEaseIn::create(CCMoveTo::create(0.5, ccp(_winWidth/2-10, _winHeight/2+55)), 1.5),
					CCDelayTime::create(4.0f),
					CCEaseOut::create(CCMoveTo::create(0.5, ccp(- 1000, _winHeight / 2+55)), 4),
					CCCallFuncN::create(this, SEL_CallFuncN(&MonsterBeansLayer::RemoveTheNode)),
					NULL));
						
				if(leftScore>0)
				{
					_totalScore+=leftScore;
					char totalScoreStr[20];
					sprintf(totalScoreStr, "%d",_totalScore);
					formatString(totalScoreStr);
					_totalScore_label->setString(totalScoreStr);
				}
				
				for(int i=0;i<BEAN_ROW_NUM;i++)
				{
					for(int j=0;j<BEAN_COL_NUM;j++)
					{
						if(_map[i][j]!=NULL)
						{
							_map[i][j]->runAction(CCSequence::create(
								CCDelayTime::create(0.2f),
								CCBlink::create(2.0f,8),
								CCDelayTime::create(0.2f),
								CCCallFuncN::create(this, SEL_CallFuncN(&MonsterBeansLayer::RemoveLeaveNode)),
								NULL
								));
						}
					}
				}
				scheduleOnce(schedule_selector(MonsterBeansLayer::nextLevel),4); 
				return true;
			}
			saveGame();
			this->_pickedBeans->removeAllObjects();
		}
		else
		{

			for(unsigned int i=0;i<_pickedBeans->count();i++)
			{
				CCSprite *item=(CCSprite *)_pickedBeans->objectAtIndex(i);
				item->setOpacity(255);
			}
			_pickedBeans->removeAllObjects();
			CCSprite *curBean=_map[(int)touchpoint.x][(int)touchpoint.y];
			touchOnce(curBean);
		}
	}

//	printMap();


	return true;
}

bool MonsterBeansLayer::init()
{
	CCLayer::init();

    CCSprite *background =NULL;
    iphone5_pos=0;
    if(IS_IPHONE5)
    {
        background=CCSprite::create("mainBg2.png");
        iphone5_pos=35;
    }
    else{
        background=CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("mainBg.png"));
    }
	background->setScale(2*0.5f);
	background->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(background, 1);

	alpha = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("alpha.png"));
	alpha->setScale(2*0.5f);
	alpha->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(alpha, 3);

	CCSprite *clouds = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("clouds.png"));
	clouds->setScale(2*0.5f);
	clouds->setPosition(ccp(_winWidth / 2, 45));
	this->addChild(clouds, 2);

	CCSprite *level_text = CCSprite::create("level_text.png");
	level_text->setScale(2*0.5);
	level_text->setPosition(ccp(60, _winHeight - 38-iphone5_pos));
	this->addChild(level_text, 5);

	_levelLabel = CCLabelAtlas::create(":", "numbers.png", 15, 25, '1');
	_levelLabel->setScale(2*0.45);
	_levelLabel->setPosition(ccp(85,_winHeight - 48-iphone5_pos));
	_levelLabel->setColor(ccYELLOW);
	this->addChild(_levelLabel,5);


	CCSprite *totalScore_Text = CCSprite::create("score_text.png");
	totalScore_Text->setScale(2*0.5f);
	totalScore_Text->setPosition(ccp(_winWidth / 2-20,  _winHeight - 25-iphone5_pos));
	this->addChild(totalScore_Text, 2);

	_totalScore_label = CCLabelAtlas::create(":", "numbers.png", 15, 25, '1');
	_totalScore_label->setScale(2*0.4f);
	_totalScore_label->setColor(ccRED);
	_totalScore_label->setPosition(ccp(_winWidth / 2, _winHeight - 35-iphone5_pos));
	this->addChild(_totalScore_label, 6);


	CCSprite *goalScore_Text = CCSprite::create("goal.png");
	goalScore_Text->setScale(2*0.5f);
	goalScore_Text->setPosition(ccp(_winWidth -80,  _winHeight - 25-iphone5_pos));
	this->addChild(goalScore_Text, 2);

	_goalScore_label = CCLabelAtlas::create("", "numbers2.png", 15, 33.5, '1');
	_goalScore_label->setScale(2*0.4f);
	//_goalScore_label->setColor(ccYELLOW);
	_goalScore_label->setPosition(ccp(_winWidth -65,_winHeight - 40-iphone5_pos));
	this->addChild(_goalScore_label, 7);

	_selectPen=CCSprite::create("select.png");
	_selectPen->setScale(2*0.35f);
	_selectPen->setPosition(ccp(PEN_X(-11),PEN_Y));
	this->addChild(_selectPen, 8);

	CCMenuItemImage *pauseNormalItem = CCMenuItemImage::create("pauseNormal.png", NULL);
	CCMenuItemImage *pausePressedItem = CCMenuItemImage::create("pausePressed.png", NULL);

	CCMenuItemToggle *pauseItem = CCMenuItemToggle::createWithTarget(
		this, 
		SEL_MenuHandler(&MonsterBeansLayer::PausePressed), 
		pauseNormalItem, 
		pausePressedItem, 
		NULL);

	pauseItem->setPosition(ccp(20, _winHeight - 25-iphone5_pos));
	pauseItem->setSelectedIndex(0);
	pauseItem->setScale(2*0.5);

	CCMenuItemImage *penGreen = CCMenuItemImage::create("pen1.png","pen1.png",this, SEL_MenuHandler(&MonsterBeansLayer::penGreenTouch));
	penGreen->setPosition(ccp(PEN_X(4),PEN_Y));
	penGreen->setScale(2*0.35);
	CCMenuItemImage *penYellow = CCMenuItemImage::create("pen3.png","pen3.png",this, SEL_MenuHandler(&MonsterBeansLayer::penYellowTouch));
	penYellow->setPosition(ccp(PEN_X(6),PEN_Y));
	penYellow->setScale(2*0.35);
	CCMenuItemImage *penRed = CCMenuItemImage::create("pen2.png","pen2.png",this, SEL_MenuHandler(&MonsterBeansLayer::penRedTouch));
	penRed->setPosition(ccp(PEN_X(5),PEN_Y));
	penRed->setScale(2*0.35);
	CCMenuItemImage *penBlue = CCMenuItemImage::create("pen4.png","pen4.png",this, SEL_MenuHandler(&MonsterBeansLayer::penBlueTouch));
	penBlue->setPosition(ccp(PEN_X(7),PEN_Y));
	penBlue->setScale(2*0.35);
	CCMenuItemImage *penGrey = CCMenuItemImage::create("pen5.png","pen5.png",this, SEL_MenuHandler(&MonsterBeansLayer::penGreyTouch));
	penGrey->setPosition(ccp(PEN_X(8),PEN_Y));
	penGrey->setScale(2*0.35);

	CCMenuItemImage *reflash = CCMenuItemImage::create("reflash.png","reflash.png",this, SEL_MenuHandler(&MonsterBeansLayer::reflashBeans));
	reflash->setPosition(ccp(PEN_X(3),PEN_Y));
	reflash->setScale(2*0.3);

	CCSprite *smofa = CCSprite::create("mofa.png");
	smofa->setPosition(ccp(PEN_X(1)+20,PEN_Y));
	smofa->setScale(2*0.4);
	this->addChild(smofa,5);

	_mofa = CCLabelAtlas::create(":", "numbers.png", 15, 25, '1');
	_mofa->setScale(2*0.32);
	_mofa->setPosition(ccp(PEN_X(1)+32,PEN_Y-10));
	_mofa->setColor(ccBLUE);
	this->addChild(_mofa,5);

	CCMenu *menu = CCMenu::create(pauseItem,penGreen,penYellow,penRed,penBlue,penGrey,reflash, NULL);
	_menu=menu;
	menu->setPosition(CCPointZero);
	this->addChild(menu, 4);

	if(MonsterBeansLayer::mode==MODE_TRADITION)
	{
		CCSprite *mod1_text = CCSprite::create("mode1.png");
		mod1_text->setScale(2*0.5);
		mod1_text->setPosition(ccp(68, _winHeight - 18-iphone5_pos));
		this->addChild(mod1_text, 5);
	}
	else if(MonsterBeansLayer::mode==MODE_STEP)
	{
		CCSprite *mod2_text = CCSprite::create("mode2.png");
		mod2_text->setScale(2*0.5);
		mod2_text->setPosition(ccp(70, _winHeight - 18-iphone5_pos));
		this->addChild(mod2_text, 5);

		goalScore_Text->setPosition(ccp(_winWidth -80,  _winHeight - 18-iphone5_pos));
		_goalScore_label->setPosition(ccp(_winWidth -65,_winHeight - 32-iphone5_pos));

		CCSprite *step_text = CCSprite::create("step_txt.png");
		step_text->setScale(2*0.45);
		step_text->setPosition(ccp(_winWidth -80, _winHeight - 37-iphone5_pos));
		this->addChild(step_text, 5);

		_stepCount=STEPCOUNT;
		char tmp_stepc[3];
		sprintf(tmp_stepc,"%d",_stepCount);
		_step_label = CCLabelAtlas::create(":", "numbers.png", 15, 25, '1');
		_step_label->setScale(2*0.45);
		_step_label->setColor(ccRED);
		_step_label->setPosition(ccp(_winWidth -45, _winHeight - 48-iphone5_pos));
		this->addChild(_step_label, 5);
	}

	initData(-1);
	return true;
}

void MonsterBeansLayer::PausePressed(CCObject *sender)
{
	clickSound();
	this->setTouchEnabled(false);
	_menu->setTouchEnabled(false);

	CCLayer *pauseLayer = CCLayer::create();
	_pauseLayer = pauseLayer;
	
	CCSprite *panelSprite = CCSprite::create("menu.png");
	panelSprite->setPosition(ccp(_winWidth/ 2, _winHeight / 2));
	pauseLayer->addChild(panelSprite);

	CCSprite *titleSprite = CCSprite::create("stop.png");
	titleSprite->setPosition(ccp(_winWidth  / 2, _winHeight / 2 + 40));
	pauseLayer->addChild(titleSprite);
	titleSprite->setScale(2*0.25);

	CCMenuItemImage *resumeItem = CCMenuItemImage::create("play.png", "play2.png", this, SEL_MenuHandler(&MonsterBeansLayer::ResumeItemPressed));
	resumeItem->setPosition(ccp(_winWidth / 2 - 75, _winHeight / 2 - 13));
	resumeItem->setScale(2*0.4);

	CCMenuItemImage *restartItem = CCMenuItemImage::create("again.png", "again2.png", this, SEL_MenuHandler(&MonsterBeansLayer::RestartItemPressed));
	restartItem->setPosition(ccp(_winWidth/ 2, _winHeight / 2 - 20));
	restartItem->setScale(2*0.4);

	CCMenuItemImage *menuItem = CCMenuItemImage::create("home.png", "home2.png", this, SEL_MenuHandler(&MonsterBeansLayer::MenuItemPressed));
	menuItem->setPosition(ccp(_winWidth/ 2 + 75, _winHeight / 2 - 13));
	menuItem->setScale(2*0.4);

	CCMenu *menu = CCMenu::create(resumeItem, restartItem, menuItem, NULL);
	menu->setPosition(ccp(0, 0));
	pauseLayer->addChild(menu);
	pauseLayer->setScale(2*0.5);
	pauseLayer->setPosition(-_winWidth,0);
	this->addChild(pauseLayer,100);
	pauseLayer->runAction(
				CCSequence::create(
					//CCDelayTime::create(0.1f),
					 CCMoveTo::create(0.4,ccp(0,0) ),
					NULL
				)
			);
}

//初始化矩阵和豆豆精灵
void MonsterBeansLayer::initBeans()
{
    
   // int move_time[BEAN_ROW_NUM]={0};
	srand(time(0));
	for(int i=0;i<BEAN_ROW_NUM;i++)
	{
		for(int j=0;j<BEAN_COL_NUM;j++)
		{
			int kind=rand() % KIND_NUM+1;
            //CCLog("kind:%d",kind);
			BeanInfo *beaninfo= new BeanInfo();
			beaninfo->column=i;
			beaninfo->row=j;
			beaninfo->kind=kind;
			CCSprite *bean = CCSprite::create("bean.png", BEAN_TEXTURE_RECT(kind));
			bean->setScale(2*0.5f);
			bean->setUserData(beaninfo);
			this->addChild(bean,10);
			_map[i][j]=bean;

			bean->retain();
			//让豆豆从天而降
			CCPoint end=CELL_TO_POSITION(i,j);
			bean->setPosition(ccp(end.x,end.y+_winHeight-BEGIN_POS_Y-BEAN_RADIUS));
			bean->runAction(
				CCSequence::create(
					CCDelayTime::create(0.1f),
					CCEaseBounceOut ::create(
                        CCMoveTo::create(0.8+0.2*i+0.1*j, end)
                    ),
					NULL
				)
                            );
		}
	}

	CCSprite *level_text = CCSprite::create("text_guanka.png");
	level_text->setScale(2*0.5f);
	level_text->setPosition(ccp(_winWidth/2,_winHeight/2));
	level_text->setOpacity(0);
	this->addChild(level_text,11);
	level_text->runAction(
		CCSequence::create(
			CCDelayTime::create(2.3f),
			CCFadeIn::create(0.5),
			CCCallFunc::create(this, SEL_CallFunc(&MonsterBeansLayer::RedyGo)),
			CCDelayTime::create(1.5f),
			CCFadeOut::create(0.5),
			NULL
		)
	);
	char level[4];
	sprintf(level,"%d",_level);
	formatString(level);
	//CCLog("level:%s",level);
	CCLabelAtlas *levelLB = CCLabelAtlas::create(level, "numbers.png", 15, 25, '1');
	levelLB->setOpacity(0);
	levelLB->setScale(2*1.0);
	levelLB->setPosition(ccp(115,3));
	levelLB->setColor(ccYELLOW);
	levelLB->runAction(
		CCSequence::create(
			CCDelayTime::create(2.3f),
			CCFadeIn::create(0.5),
			CCDelayTime::create(1.5f),
			CCFadeOut::create(0.5),
			NULL
		)
	);
	level_text->addChild(levelLB,5);
}

void MonsterBeansLayer::onEnterTransitionDidFinish()
{
	if(isSaveGame(MonsterBeansLayer::mode))
	{
		this->recoverGame();
	}
	else
	{
		this->initBeans();
	}
}


void MonsterBeansLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, false);
}


void MonsterBeansLayer::printMap()
{

	for(int i=0;i<BEAN_COL_NUM;i++)
	{
		for(int j=0;j<BEAN_ROW_NUM;j++)
		{
			if(_map[i][j]!=NULL)
			{
				BeanInfo *bi=(BeanInfo*)_map[i][j]->getUserData();
				CCLog("(%d,%d) :  (%d,%d)",i,j,bi->column,bi->row);
			}
			else
				CCLog("(%d,%d) :  (-1,-1)",i,j);
		}
	}
}

void MonsterBeansLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void MonsterBeansLayer::initData(int level)
{
	_menu->setTouchEnabled(false);
	this->setTouchEnabled(false);
	if(level==-1)
	{
		_pickedBeans=new CCArray();

		_totalScore=0;
		char str1[20];
		sprintf(str1, "%d",_totalScore);
		formatString(str1);
		_totalScore_label->setString(str1);
		_level=1;

		this->_mofa_count=getMofaNumber(MonsterBeansLayer::mode);
	}
	else
	{
		this->removeChildByTag(TAG_PASS);
		setMofaNumber(2,MonsterBeansLayer::mode);
		_mofa_count=_mofa_count+2;
	}
	char str3[20];
	sprintf(str3, "%d",_level);
    this->formatString(str3);
	_levelLabel->setString(str3);

	//_goalScore=(_level+1)*_level*TSCORE_BASE/2;
	_goalScore=countTargetSorce(_level);
	char str2[20];
	sprintf(str2, "%d",_goalScore);
	formatString(str2);
	_goalScore_label->setString(str2);

	char tmp4[10];
	sprintf(tmp4,"%d",this->_mofa_count);
    this->formatString(tmp4);
	_mofa->setString(tmp4);

	_is_show_complete=false;
	_pen=PEN_NULL;
	_selectPen->setPosition(ccp(PEN_X(PEN_NULL),PEN_Y));
	_overLayer=NULL;
	_pauseLayer=NULL;

	if(MonsterBeansLayer::mode==MODE_STEP)
	{
		_stepCount=STEPCOUNT;
		char tmp_step[3];
		sprintf(tmp_step,"%d",_stepCount);
        this->formatString(tmp_step);
		this->_step_label->setString(tmp_step);
	}
}

bool MonsterBeansLayer::findLinkBeans(CCSprite *root_bean)
{
	if(_pickedBeans==NULL) 
		_pickedBeans=new CCArray();
	_pickedBeans->addObject(root_bean);

	unsigned int currentIndex = 0;
	while(currentIndex < _pickedBeans->count())
	{
		CCSprite *bean = (CCSprite *)_pickedBeans->objectAtIndex(currentIndex);
		BeanInfo *beanInfo = (BeanInfo *)bean->getUserData();
		for(int i = 0; i < 4; ++i)
		{
			int trow=beanInfo->row;
			int tcolumn=beanInfo->column;
			switch (i)
			{
			case 0:
				++tcolumn;
				break;
			case 1:
				--trow;
				break;
			case 2:
				--tcolumn;
				break;
			case 3:
				++trow;
				break;
			}
			

			if(trow >= 0 && trow < BEAN_ROW_NUM && tcolumn >= 0 && tcolumn < BEAN_ROW_NUM)
			{
				CCSprite *tbean = _map[tcolumn][trow];
				if(tbean)
				{
					BeanInfo *tbeanInfo = (BeanInfo *)tbean->getUserData();
					if(tbeanInfo->kind == beanInfo->kind && !_pickedBeans->containsObject(tbean))
						_pickedBeans->addObject(tbean);
				}
			}
		}
		++currentIndex;
	}
	if(_pickedBeans->count()<MIN_BEANS)
	{
		_pickedBeans->removeAllObjects();
		return false;
	}
	return true;
}

void MonsterBeansLayer::touchOnce(CCSprite *curBean)
{
	if(findLinkBeans(curBean))		//找到与单击的豆豆相连接的豆豆
	{
		if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("select.wav");
		for(unsigned int i=0;i<_pickedBeans->count();i++)
		{
			CCSprite *item=(CCSprite *)_pickedBeans->objectAtIndex(i);
			CCActionInterval * moveBy = CCMoveBy::create(0.1f,ccp(0, 10));
			 CCActionInterval * moveback= moveBy->reverse();
			item->runAction(
				CCSequence::create(moveBy,moveback,
				CCCallFuncN::create(this, SEL_CallFuncN(&MonsterBeansLayer::setBeanOpacity)),
				NULL)
			);
		}
	}
}

void MonsterBeansLayer::setBeanOpacity(CCNode *obj)
{
	CCSprite *bean=(CCSprite*)obj;
	bean->setOpacity(100);
}


void MonsterBeansLayer::removeBeans()
{
	for(unsigned int i=0;i<_pickedBeans->count();i++)
	{	
		BeanInfo *info=(BeanInfo*)((CCSprite *)_pickedBeans->objectAtIndex(i))->getUserData();
		explode(_map[info->column][info->row]);
		_map[info->column][info->row]->removeFromParentAndCleanup(true);
		_map[info->column][info->row]=NULL;
	}
}

void MonsterBeansLayer::moveBeans()
{
	bool emptyColumn[BEAN_COL_NUM]={false};
	for(int i=0;i<BEAN_COL_NUM;i++)
	{
		int count=0;
		for(int j=0;j<BEAN_ROW_NUM;j++)
		{
			//CCLog("out i:%d  out j:%d  count:%d",i,j,count);
			if(!_map[i][j])
				count++;
			if(_map[i][j]&&count>0)		//被消除的豆豆，上面的豆豆掉下来，取代原来的位置
			{
				//CCLog("i:%d j:%d",i,j);
				_map[i][j]->runAction(
					CCSequence::create(
					CCDelayTime::create(0.2f),
					CCEaseBounceOut ::create(CCMoveBy::create(0.2*count,ccp(0,-BEAN_HEIGHT*count))),
					NULL));
				_map[i][j-count]=_map[i][j];
				_map[i][j]=NULL;
				BeanInfo *bif=(BeanInfo*)_map[i][j-count]->getUserData();
				bif->row=j-count;
			}
			if(count==BEAN_ROW_NUM)//如果整列都空了
			{
				emptyColumn[i]=true;
			}
		}
	}


	//对于整列为空的豆豆，右边的列先左边移动一列
	for(int i=BEAN_COL_NUM-2;i>-1;i--)
	{
		if(emptyColumn[i]&&!emptyColumn[i+1])
		{
			for(int t=i;t<BEAN_COL_NUM-1&&!emptyColumn[t+1];t++)
			{
				for(int j=0;j<BEAN_ROW_NUM;j++)
				{
					if(_map[t+1][j])
					{
						_map[t+1][j]->runAction(
							CCSequence::create(
							CCDelayTime::create(0.2f),
							CCEaseBounceOut ::create(CCMoveBy::create(0.2f,ccp(-BEAN_HEIGHT,0))),
							NULL
							));
						_map[t][j]=_map[t+1][j];
						_map[t+1][j]=NULL;
						BeanInfo *bif=(BeanInfo*)_map[t][j]->getUserData();
						bif->column--;
					}
				}
			}
			emptyColumn[i]=false;
		}
	}
}

void MonsterBeansLayer::explode(CCNode *bean)
{
	if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("clear.wav");
	BeanInfo *beanInfo = (BeanInfo *)bean->getUserData();
	CCParticleExplosion *explosion = CCParticleExplosion::create();
	explosion->setAutoRemoveOnFinish(true);
	explosion->setTotalParticles(15);
	explosion->setLife(1.2f);
	explosion->setLifeVar(0.6f);
	explosion->setTexture(CCTextureCache::sharedTextureCache()->addImage("star.png"));
	explosion->setPosition(ccp(bean->getPositionX(), bean->getPositionY() +BEAN_RADIUS));
	explosion->setStartColorVar(ccc4f(0, 0, 0, 0));
	explosion->setEndColorVar(ccc4f(0, 0, 0, 0));
	switch(beanInfo->kind)
	{
	case BEAN_RED:
		explosion->setStartColor(ccc4f(255/255.f, 30/255.f, 30/255.f, 1));

		explosion->setEndColor(ccc4f(255/255.f, 30/255.f, 30/255.f, 0));
		break;
	case BEAN_BLUE:
		explosion->setStartColor(ccc4f(28/255.f, 119/255.f, 255/255.f, 1));
		explosion->setEndColor(ccc4f(28/255.f, 119/255.f, 255/255.f, 0));
		break;
	case BEAN_GREEN:
		explosion->setStartColor(ccc4f(0/255.f, 195/255.f, 0/255.f, 1));
		explosion->setEndColor(ccc4f(0/255.f, 195/255.f, 0/255.f, 0));
		break;
	case BEAN_YELLOW:
		explosion->setStartColor(ccc4f(255/255.f, 226/255.f, 33/255.f, 1));
		explosion->setEndColor(ccc4f(255/255.f, 226/255.f, 33/255.f, 0));
		break;
	case BEAN_GREY:
		explosion->setStartColor(ccc4f(0/255.f, 10/255.f, 10/255.f, 1));
		explosion->setEndColor(ccc4f(0/255.f, 10/255.f, 10/255.f, 0));
		break;
	default:
		break;
	}
	this->addChild(explosion,20);
}

void MonsterBeansLayer::ShowScoreLabel(cocos2d::CCSprite *bean, int index,bool isTotal)
{
	BeanInfo *beanInfo=(BeanInfo *)bean->getUserData();
	ccColor3B color;
	switch(beanInfo->kind)
	{
	case BEAN_RED:
		color=ccc3(255 , 30 , 30 );
		break;
	case BEAN_BLUE:
		color=ccc3(28 , 119 , 255 );
		break;
	case BEAN_GREEN:
		color=ccc3(0 , 195 , 0);
		break;
	case BEAN_YELLOW:
		color=ccc3(255 , 226 , 33);
		break;
	case BEAN_GREY:
		color=ccc3(0 , 10 , 10 );
		break;
	}
	color=ccc3(255 , 255 , 255 );//还是白色好
	char scoreString[32];
	if(isTotal)
	{
		int score=index*index*SCORE_BASE;
		sprintf(scoreString, "%d",score);
		formatString(scoreString);
		CCLog("score:%d    %s:", index*index*SCORE_BASE,scoreString);
		CCLabelAtlas *scoreLabel = CCLabelAtlas::create(scoreString, "numbers2.png",15,33.5,'1');
		scoreLabel->setColor(color);
		float pos_x=bean->getPositionX();
		if(bean->getPositionX()>_winWidth-50)
			pos_x=_winWidth-50;
		scoreLabel->setPosition(ccp(pos_x, bean->getPositionY()));
		scoreLabel->setScale(2*0);
		this->addChild(scoreLabel,50);
		scoreLabel->runAction(CCSequence::create(
			CCDelayTime::create(0.1f * index),
			CCEaseOut::create(CCScaleTo::create(0.3f, 2), 5),
			CCDelayTime::create(0.2f),
			CCFadeOut::create(0.5f),
			CCCallFuncND::create(this, SEL_CallFuncND(&MonsterBeansLayer::addScore),(void*)0),
			NULL));
	}
	else
	{
		int score=(2*index-1)*SCORE_BASE;
		sprintf(scoreString, "%d", score);
		formatString(scoreString);
		CCLabelAtlas *scoreLabel = CCLabelAtlas::create(scoreString, "numbers.png",15,25,'1');
		scoreLabel->setAnchorPoint(ccp(0.5f, 0.5f));
		scoreLabel->setPosition(ccp(bean->getPositionX(), bean->getPositionY()));
		scoreLabel->setColor(color);
		this->addChild(scoreLabel,50);
		scoreLabel->runAction(CCSequence::create(
			CCDelayTime::create(0.1f * (index-1)),
			CCSpawn::create(CCMoveTo::create(0.8,ccp(_winWidth/2+5,_winHeight-25-iphone5_pos)),CCScaleTo::create(0.8, 0.2),NULL),
			//CCDelayTime::create(0.1f),
			CCFadeOut::create(0.01f),
			CCCallFuncND::create(this, SEL_CallFuncND(&MonsterBeansLayer::addScore),(void*)score),
			NULL));
	}

}

void MonsterBeansLayer::createAndAddScore()
{
	for(unsigned int i=0;i<_pickedBeans->count();i++)
	{	
		CCSprite *item=(CCSprite *)_pickedBeans->objectAtIndex(i);
		//CCLog("x:%d  y:%d",info->column,info->row);
		ShowScoreLabel(item,i+1,false);
	}
	CCSprite* last=(CCSprite *)_pickedBeans->objectAtIndex(_pickedBeans->count()-1);
	ShowScoreLabel(last,_pickedBeans->count(),true);
}

void MonsterBeansLayer::addScore(CCNode *target, void *data)
{
	int score=(int )data;
	if(score>0)
	{
		_totalScore+=score;
		char totalScoreStr[20];
		sprintf(totalScoreStr, "%d",_totalScore);
		formatString(totalScoreStr);
		_totalScore_label->setString(totalScoreStr);
	}
	else
	{
		if(!_is_show_complete&&_totalScore>=_goalScore)	//判断是否过关
		{
			if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("cheer.wav");
			CCSprite *levelUp_Label = CCSprite::create("guoguan.png");
			levelUp_Label->setScale(2*0.1);
			//levelUp_Label->setPosition(ccp(_winWidth + 100, _winHeight/ 2));
			levelUp_Label->setPosition(ccp(_winWidth / 2, _winHeight / 2));
			this->addChild(levelUp_Label,51,TAG_PASS);

			levelUp_Label->runAction(CCSequence::create(
				CCDelayTime::create(0.1f),
				CCSpawn::create(CCEaseIn::create(CCScaleTo::create(0.35, 1.0),0.7),NULL),
				//CCEaseIn::create(CCMoveTo::create(0.5, ccp(_winWidth / 2, _winHeight / 2)), 3),
				//CCEaseIn::create(CCMoveTo::create(0.5, ccp(_winWidth / 2, _winHeight / 2)), 3),
				CCDelayTime::create(0.5f),
				CCSpawn::create(CCMoveTo::create(0.5,ccp(_winWidth/2+5,_winHeight-48-iphone5_pos)),
                CCScaleTo::create(0.4, 0.56),NULL),
				NULL));
			_is_show_complete=true;
		}
	}

	target->removeFromParentAndCleanup(true);

	
}

void MonsterBeansLayer::formatString(char *str)
{
	while(*str)
	{
		if(*str=='0')
			*str=':';
		str++;
	}
}

void MonsterBeansLayer::penGreenTouch(CCObject*o)
{
	if(_pen==BEAN_GREEN)
		_pen=PEN_NULL;
	else
		_pen=BEAN_GREEN;
	_selectPen->setPosition(ccp(PEN_X(_pen+3),PEN_Y));
}

void MonsterBeansLayer::penYellowTouch(CCObject*o)
{
	if(_pen==BEAN_YELLOW)
		_pen=PEN_NULL;
	else
		_pen=BEAN_YELLOW;
	_selectPen->setPosition(ccp(PEN_X(_pen+3),PEN_Y));
}

void MonsterBeansLayer::penBlueTouch(CCObject*o)
{
	if(_pen==BEAN_BLUE)
		_pen=PEN_NULL;
	else
		_pen=BEAN_BLUE;
	_selectPen->setPosition(ccp(PEN_X(_pen+3),PEN_Y));
}
void MonsterBeansLayer::penRedTouch(CCObject*o)
{
	if(_pen==BEAN_RED)
		_pen=PEN_NULL;
	else
		_pen=BEAN_RED;
	_selectPen->setPosition(ccp(PEN_X(_pen+3),PEN_Y));
}
void MonsterBeansLayer::penGreyTouch(CCObject*o)
{
	if(_pen==BEAN_GREY)
		_pen=PEN_NULL;
	else
		_pen=BEAN_GREY;
	_selectPen->setPosition(ccp(PEN_X(_pen+3),PEN_Y));
}

bool MonsterBeansLayer::isGameEnd()
{
	if(MonsterBeansLayer::mode==MODE_STEP&&_stepCount==0)
		return true;
	for(int i=0;i<BEAN_ROW_NUM;i++)
	{
		for(int j=0;j<BEAN_COL_NUM;j++)
		{
			if(_map[i][j]!=NULL&&findLinkBeans(_map[i][j]))
				return false;
		}
	}
	return true;
}

void MonsterBeansLayer::nextLevel(float time)
{
	if(_totalScore>=_goalScore) //下一关
	{
		initData(++_level);
		initBeans();
		saveGame();
	}
	else	//gameover
	{
		GameOver();
	}
}

void MonsterBeansLayer::RemoveLeaveNode(CCNode *node)
{
	explode(node);
	BeanInfo *bubbleInfo = (BeanInfo *)node->getUserData();
	if(bubbleInfo) delete bubbleInfo;
	node->removeFromParentAndCleanup(true);
}

void MonsterBeansLayer::ResumeItemPressed(CCObject *item)
{
	clickSound();
	if(_pauseLayer)
	{
		_pauseLayer->runAction(
				CCSequence::create(
					//CCDelayTime::create(0.1f),
					 CCMoveTo::create(0.4,ccp(-_winWidth,0) ),
					 CCCallFuncN::create(this, SEL_CallFuncN(&MonsterBeansLayer::RemoveTheNode)),
					NULL
				)
			);
		//_pauseLayer->removeFromParentAndCleanup(true);
		_pauseLayer = NULL;
	}
	if(_overLayer)
	{
		_overLayer->removeFromParentAndCleanup(true);
		_overLayer=NULL;
	}
	this->setTouchEnabled(true);
	_menu->setTouchEnabled(true);
}
void MonsterBeansLayer::RestartItemPressed(CCObject *item)
{
	clickSound();
	if(isSaveGame(MonsterBeansLayer::mode)&&this->_pauseLayer)
	{
		this->_pauseLayer->removeAllChildrenWithCleanup(true);
		CCSprite *panelSprite = CCSprite::create("menu.png");
		panelSprite->setPosition(ccp(_winWidth/ 2, _winHeight / 2));
		_pauseLayer->addChild(panelSprite);

		CCSprite *titleSprite = CCSprite::create("msg.png");
		titleSprite->setPosition(ccp(_winWidth  / 2, _winHeight / 2 + 40));
		_pauseLayer->addChild(titleSprite);
		titleSprite->setScale(2*0.25);

		CCMenuItemImage *sureItem = CCMenuItemImage::create("sure.png", "sure.png", this, SEL_MenuHandler(&MonsterBeansLayer::restartGame));
		sureItem->setPosition(ccp(_winWidth / 2 - 52, _winHeight / 2 - 40));
		sureItem->setScale(2*0.4);

		CCMenuItemImage *cancelItem = CCMenuItemImage::create("cancel.png", "cancel.png", this, SEL_MenuHandler(&MonsterBeansLayer::ResumeItemPressed));
		cancelItem->setPosition(ccp(_winWidth/ 2 + 52, _winHeight / 2 - 40));
		cancelItem->setScale(2*0.4);

		CCMenu *menu = CCMenu::create(sureItem, cancelItem, NULL);
		menu->setPosition(ccp(0, 0));
		_pauseLayer->addChild(menu);
	}
	else
	{
		deleteSave(MonsterBeansLayer::mode);
		this->removeAllChildrenWithCleanup(true);
		this->init();
		this->initBeans();
	}
}
void MonsterBeansLayer::MenuItemPressed(CCObject *item)
{
	clickSound();
	CCDirector::sharedDirector()->popScene();
}

void MonsterBeansLayer::GameOver()
{	
	if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("game_over.mp3");

	this->setTouchEnabled(false);
	_menu->setTouchEnabled(false);

	CCLayer *overLayer = CCLayer::create();
	_overLayer = overLayer;

	CCSprite *panelSprite = CCSprite::create("over.png");
	panelSprite->setPosition(ccp(_winWidth/ 2, _winHeight / 2));
	overLayer->addChild(panelSprite);
	
	CCSprite *title=NULL;
	if(MonsterBeansLayer::mode==MODE_TRADITION)
	{
		title = CCSprite::create("mode1_title.png");
	}
	else if(MonsterBeansLayer::mode==MODE_STEP)
	{
		title = CCSprite::create("mode2_title.png");
	}
	title->setScale(2*0.25);
	title->setPosition(ccp(_winWidth/ 2, _winHeight / 2+65));
	overLayer->addChild(title);

	char totalScoreStr[20];
	sprintf(totalScoreStr, "%d",_totalScore);
	formatString(totalScoreStr);
	CCLabelAtlas *totalScore_label = CCLabelAtlas::create(totalScoreStr, "numbers.png", 15, 25, '1');
	totalScore_label->setScale(2*0.6f);
	totalScore_label->setColor(ccYELLOW);
	totalScore_label->setPosition(ccp(_winWidth / 2-30, _winHeight/2+13));
	overLayer->addChild(totalScore_label);
	char levelStr[20];
	sprintf(levelStr, "%d",_level);
	formatString(levelStr);
	CCLabelAtlas *level_label = CCLabelAtlas::create(levelStr, "numbers.png", 15, 25, '1');
	level_label->setScale(2*0.6f);
	level_label->setColor(ccYELLOW);
	level_label->setPosition(ccp(_winWidth / 2-30, _winHeight/2-30));
	overLayer->addChild(level_label);

	//CCMenuItemImage *resumeItem = CCMenuItemImage::create("play.png", "play2.png", this, SEL_MenuHandler(&MonsterBeansLayer::ResumeItemPressed));
	//resumeItem->setPosition(ccp(_winWidth / 2 - 150, _winHeight / 2 - 135));
	//resumeItem->setScale(2*0.8);

	CCMenuItemImage *restartItem = CCMenuItemImage::create("again.png", "again2.png", this, SEL_MenuHandler(&MonsterBeansLayer::RestartItemPressed));
	restartItem->setPosition(ccp(_winWidth/ 2-52, _winHeight / 2 - 78));
	restartItem->setScale(2*0.5);

	CCMenuItemImage *menuItem = CCMenuItemImage::create("home.png", "home2.png", this, SEL_MenuHandler(&MonsterBeansLayer::MenuItemPressed));
	menuItem->setPosition(ccp(_winWidth/ 2+50, _winHeight / 2 - 78));
	menuItem->setScale(2*0.5);

	CCMenu *menu = CCMenu::create( restartItem, menuItem, NULL);
	menu->setPosition(ccp(0, 0));
	overLayer->addChild(menu);
	overLayer->setScale(2*0.4);
	this->addChild(overLayer,100);
	deleteSave(MonsterBeansLayer::mode);
	int rank= getRanking(this->_totalScore,_level,MonsterBeansLayer::mode);
	CCSprite *minci;
	if(rank==1)
	{
		minci = CCSprite::create("first.png");
	}
	else if(rank==2)
	{
		minci = CCSprite::create("second.png");
	}
	else if(rank==3)
	{
		minci = CCSprite::create("third.png");
		
	}
	else
	{
		minci = CCSprite::create("noname.png");
	}
	minci->setScale(2*0);
	minci->setOpacity(0);
	minci->setPosition(ccp(_winWidth / 2+55, _winHeight/2-5));
	overLayer->addChild(minci);
	if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("sparkCome.wav");
		minci->runAction(
			CCSpawn::create(CCFadeIn::create(0.5f),CCScaleTo::create(0.5f, 0.6f),NULL));
}

void MonsterBeansLayer::RedyGo()
{
	 if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("ReadyGo.wav");
	 this->setTouchEnabled(true);
	 _menu->setTouchEnabled(true);
}

void MonsterBeansLayer::combo()
{
	if(_pickedBeans->count()<8)
	{
		_pickedBeans->removeAllObjects();
		return;
	}
	CCSprite *level_text =NULL;
	if(_pickedBeans->count()>12)
	{
		level_text = CCSprite::create("tiancai.png");

	}
	else if(_pickedBeans->count()>10)
	{
		level_text = CCSprite::create("shuai.png");
	}
	else if(_pickedBeans->count()>7)
	{
		level_text = CCSprite::create("lihai.png");
	}
	level_text->setScale(2*0.5f);
	level_text->setPosition(ccp(_winWidth/2,_winHeight/2+150));
	level_text->setOpacity(0);
	this->addChild(level_text,11);
	level_text->runAction(
		CCSequence::create(
			CCDelayTime::create(0.8f),
			CCFadeIn::create(0.5),
			CCCallFunc::create(this, SEL_CallFunc(& MonsterBeansLayer::SoundChear)),
			CCBlink::create(1.5f,6),
			CCFadeOut::create(0.5),
			CCCallFuncN::create(this, SEL_CallFuncN(& MonsterBeansLayer::RemoveTheNode)),
			NULL
		)
	);
	_pickedBeans->removeAllObjects();
}

void MonsterBeansLayer::RemoveTheNode(CCNode *node)
{
    BeanInfo *bubbleInfo = (BeanInfo *)node->getUserData();
    if(bubbleInfo) delete bubbleInfo;

    node->removeFromParentAndCleanup(true);
}

void MonsterBeansLayer::SoundChear()
{
	 if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("cheer.wav");
}

void MonsterBeansLayer::saveGame()
{
	char gamedata[101];
	for(int i=0;i<BEAN_ROW_NUM;i++)
	{
		for(int j=0;j<BEAN_ROW_NUM;j++)
		{
			if(!_map[i][j])
			{
				gamedata[i*BEAN_ROW_NUM+j]=-1+INT2CHAR;
			}
			else
			{
				int kind=((BeanInfo*) _map[i][j]->getUserData())->kind;
				gamedata[i*BEAN_ROW_NUM+j]=kind+INT2CHAR;
			}
		}
	}
	int step=-1;
	if(MonsterBeansLayer::mode==MODE_STEP)
	{
		step=this->_stepCount;
	}
	SaveGameData(_level,_totalScore,gamedata,step);
}

void MonsterBeansLayer::recoverGame()
{
	char *gamedata=new char[101];
	int magicNum;
	if(MonsterBeansLayer::mode==MODE_TRADITION)
	{
		RecoverGameData(_level,_totalScore,gamedata,magicNum,MODE_TRADITION);
	}
	else if(MonsterBeansLayer::mode==MODE_STEP)
	{
		int step;
		RecoverGameData(_level,_totalScore,gamedata,magicNum,MODE_STEP,&step);
		_stepCount=step;
		char tmp_step[3];
		sprintf(tmp_step,"%d",step);
        this->formatString(tmp_step);
		_step_label->setString(tmp_step);
	}
	char str3[20];
	sprintf(str3, "%d",_level);
    this->formatString(str3);
	_levelLabel->setString(str3);
	_goalScore=countTargetSorce(_level);
	char str2[20];
	sprintf(str2, "%d",_goalScore);
	formatString(str2);
	_goalScore_label->setString(str2);
	char totalScoreStr[20];
	sprintf(totalScoreStr, "%d",_totalScore);
	formatString(totalScoreStr);
	_totalScore_label->setString(totalScoreStr);
	
	for(int i=0;i<BEAN_ROW_NUM;i++)
	{
		for(int j=0;j<BEAN_ROW_NUM;j++)
		{
			int kind=(int)gamedata[i*BEAN_ROW_NUM+j]-INT2CHAR;
			if(kind==-1)
			{
				_map[i][j]=NULL;
			}
			else
			{
				BeanInfo *beaninfo= new BeanInfo();
				beaninfo->column=i;
				beaninfo->row=j;
				beaninfo->kind=kind;
				CCSprite *bean = CCSprite::create("bean.png", BEAN_TEXTURE_RECT(kind));
				bean->setScale(2*0.5f);
				bean->setOpacity(0);
				bean->setUserData(beaninfo);
				this->addChild(bean,10);
				
				_map[i][j]=bean;
				bean->retain();
				bean->setPosition(CELL_TO_POSITION(i,j));
				bean->runAction(
				CCSequence::create(
						CCDelayTime::create(0.1*i+0.05*j),
						CCFadeIn::create(0.05),
						//CCScaleTo::create(0.1f, 0.3),
						//CCScaleTo::create(0.1f, 0.5),
						CCScaleTo::create(0.1f, 0.6),
						CCScaleTo::create(0.1f, 1.0),
						NULL
					)
				);
				
			}
		}
	}
	
	this->setTouchEnabled(true);
	 _menu->setTouchEnabled(true);
}

int MonsterBeansLayer::countTargetSorce(int level) //根据关卡计算目标得分
{
	if(level>0&&level<6)
		return 750*level+250*level*level;
	else
		//return countTargetSorce(level/5*5)+(level-level/5*5)*(2000+level/5*1000);
        return countTargetSorce((level-1)/5*5)+(level-(level-1)/5*5)*(2000+(level-1)/5*1000);
}

void MonsterBeansLayer::reflashBeans(CCObject *node)
{
	if(!haveMofa())
		return;
	while(true)
	{
		srand(time(0));
		for(int i=0;i<BEAN_ROW_NUM;i++)
		{
			for(int j=0;j<BEAN_COL_NUM;j++)
			{
				if(_map[i][j]!=NULL)
				{
					_map[i][j]->setOpacity(0);
					int kind=rand() % KIND_NUM+1;
					BeanInfo *beaninfo=(BeanInfo*)_map[i][j]->getUserData();
					beaninfo->kind=kind;
					_map[i][j]->setTextureRect(BEAN_TEXTURE_RECT(kind));
				}
			}
		}
		if(!isGameEnd())
			break;
	}

	for(int i=0;i<BEAN_ROW_NUM;i++)
	{
		for(int j=0;j<BEAN_ROW_NUM;j++)
		{
			if(_map[i][j]!=NULL)
			{
				_map[i][j]->runAction(
				CCSequence::create(
						CCDelayTime::create(0.1*i+0.05*j),
						CCFadeIn::create(0.05),
						//CCScaleTo::create(0.1f, 0.3),
						//CCScaleTo::create(0.1f, 0.5),
						CCScaleTo::create(0.1f, 0.6),
						CCScaleTo::create(0.1f, 1.0),
						NULL
					)
				);
				
			}
		}
	}
}

bool MonsterBeansLayer::haveMofa()
{
	if(this->_mofa_count==0)
		return  false;
	setMofaNumber(-1,MonsterBeansLayer::mode);
	--this->_mofa_count;
	char tmp4[10];
	sprintf(tmp4,"%d",this->_mofa_count);
    this->formatString(tmp4);
	_mofa->setString(tmp4);
	return true;
}

void MonsterBeansLayer::restartGame(CCObject *item)
{
	clickSound();
	deleteSave(MonsterBeansLayer::mode);
		this->removeAllChildrenWithCleanup(true);
		this->init();
		this->initBeans();
}