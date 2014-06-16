#ifndef __MONSTERBEANSLAYER_H__
#define __MONSTERBEANSLAYER_H__


#define BEAN_COL_NUM 10
#define BEAN_ROW_NUM 10

#define NULL_BEAN 0
#define KIND_NUM 5

#define BEAN_GREEN   1
#define	BEAN_RED 2
#define	BEAN_YELLOW 3
#define	BEAN_BLUE 4
#define	BEAN_GREY 5



#include "Common.h"
using namespace cocos2d;

struct BeanInfo
{
	int row;
	int column;
	int kind;
};

class MonsterBeansLayer : public CCLayer
{
private:
	float _winWidth;
	float _winHeight;
	int _totalScore;
	int _level;
	int _goalScore;
	bool _is_show_complete;
	int _pen;
	int _mofa_count;
    int iphone5_pos;
	CCSprite *_tmp_bean;
	CCSprite *_map[BEAN_ROW_NUM][BEAN_COL_NUM];

	CCLabelAtlas *_totalScore_label;
	CCLabelAtlas *_goalScore_label;
	CCLabelAtlas *_mofa;
	CCLabelAtlas *_levelLabel;
	CCLabelAtlas *_step_label;
	CCSprite *alpha ;
	CCSprite *_selectPen;
	CCArray *_pickedBeans;
	CCLayer *_pauseLayer;
	CCLayer *_overLayer;
	CCLayer *_level_complete;
	CCMenu *_menu;
	void PausePressed(CCObject *sender);
	void initBeans();
	void initData(int);
	bool findLinkBeans(CCSprite *);
	void touchOnce(CCSprite *);
	void setBeanOpacity(CCNode *obj);
	void printMap();
	void createAndAddScore();
	void removeBeans();
	void moveBeans();
	void explode(CCNode*);
	void ShowScoreLabel(cocos2d::CCSprite *bubble, int index,bool isTotal);
	void addScore(CCNode *target, void *data);
	void formatString(char*);
	void penGreenTouch(CCObject*);
	void penRedTouch(CCObject*);
	void penYellowTouch(CCObject*);
	void penBlueTouch(CCObject*);
	void penGreyTouch(CCObject*);
	bool isGameEnd();
	void nextLevel(float);
	void RemoveLeaveNode(CCNode*);
	void ResumeItemPressed(CCObject *item);
	void RestartItemPressed(CCObject *item);
	void MenuItemPressed(CCObject *item);
	void GameOver();
	void RedyGo();
	void combo();
	void RemoveTheNode(CCNode *node);
	void SoundChear();
	void saveGame();
	void recoverGame();
	int  countTargetSorce(int level);
	void reflashBeans(CCObject *node);
	bool haveMofa();
	int _stepCount;
	void restartGame(CCObject *item);
public:
	static int mode;
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher(void);
	static CCScene* scene();
	CREATE_FUNC(MonsterBeansLayer);
	MonsterBeansLayer();
	~MonsterBeansLayer();
};

#endif