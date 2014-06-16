#ifndef __RECORDLAYER_H__
#define __RECORDLAYER_H__
#include "Common.h"
using namespace cocos2d;
class RecordLayer: public CCLayer
{
public:
    static CCScene *scene();
    CREATE_FUNC(RecordLayer);
    RecordLayer();
    virtual bool init();
    
private:
    void BackItemPressed(CCObject *item);
	float _winWidth;
	float _winHeight;
    int iphone5_pos;
};

#endif