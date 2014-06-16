//
//  MonsterBeanAppDelegate.cpp
//  MonsterBean
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CCEGLView.h"
#include "StartLayer.h"


USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCDirector *director = CCDirector::sharedDirector();
    CCEGLView *view = CCEGLView::sharedOpenGLView();
    
    director->setOpenGLView(view);
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0 / 60);
    
    CCSize winSize = director->getWinSize();
    std::vector<std::string> order;

        order.push_back("640p");
        director->setContentScaleFactor(2);
        view->setDesignResolutionSize(320, winSize.height == 1136 ? 568 : 480, kResolutionShowAll);
    
    order.push_back("sounds");
    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(order);
    
    SimpleAudioEngine::sharedEngine()->preloadEffect("button_click.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("cheer.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("clear.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("loveFire.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("game_over.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("ReadyGo.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("select.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sparkCome.wav");
   // SimpleAudioEngine::sharedEngine()->preloadEffect("game_over.mp3");
    director->runWithScene(StartLayer::scene());
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
