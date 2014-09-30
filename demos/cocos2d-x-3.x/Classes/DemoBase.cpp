//
//  DemoBase.cpp
//  DragonBonesCPPDemos
//
//  Created by zrong on 9/30/14.
//
//

#include "DemoBase.h"

#include "DemoDragonBoy.h"
#include "DemoKnight.h"
//#include "example/DragonDemoEntry.h"
//#include "example/DragonSwitchClothes.h"
//#include "example/DragonChaseStarling.h"

static int sceneIdx = 0;

DemoBase* nextDBDemoAction();
DemoBase* backDBDemoAction();
DemoBase* restartDBDemoAction();

typedef DemoBase* (*NEW_DB_DEMO_FUNC)();
#define DBDEMO_CREATE_FUNC(className) \
static DemoBase* create##className() \
{ return new className(); }

DBDEMO_CREATE_FUNC(DemoDragonBoy)
DBDEMO_CREATE_FUNC(DemoKnight)
//DBDEMO_CREATE_FUNC(DragonDemoEntry);
//DBDEMO_CREATE_FUNC(DragonSwitchClothes);
//DBDEMO_CREATE_FUNC(DragonChaseStarling);

static NEW_DB_DEMO_FUNC createFunctions[] =
{
    createDemoDragonBoy,
    createDemoKnight,
//    createDragonDemoEntry,
//    createDragonSwitchClothes,
//    createDragonChaseStarling,
};

#define MAX_LAYER    (sizeof(createFunctions) / sizeof(createFunctions[0]))

DemoBase* nextDBDemoAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;
    
    DemoBase* pLayer = (createFunctions[sceneIdx])();
    pLayer->autorelease();
    
    return pLayer;
}

DemoBase* backDBDemoAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if (sceneIdx < 0)
        sceneIdx += total;
    
    DemoBase* pLayer = (createFunctions[sceneIdx])();
    pLayer->autorelease();
    
    return pLayer;
}

DemoBase* restartDBDemoAction()
{
    DemoBase* pLayer = (createFunctions[sceneIdx<0 ? 0 : sceneIdx])();
    pLayer->autorelease();
    
    return pLayer;
}
DemoBase* DemoBase::create(int index)
{
    DemoBase* pLayer = (createFunctions[index])();
    pLayer->autorelease();
    sceneIdx = index;
    return pLayer;
}

std::string DemoBase::title()
{
    return "No title";
}

std::string DemoBase::subtitle()
{
    return "";
}

bool DemoBase::init()
{
    if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(128, 128, 128, 255)))
    {
        return false;
    }
    
    demoInit();
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(DemoBase::updateHandler), this, 0, false);
    return true;
}

void DemoBase::onEnter()
{
    Layer::onEnter();
    
    LabelTTF* pLabel = LabelTTF::create("DragonBonesCPP for cocos2d-x 2.x", "Arial", 28);
    
    // position the label on the center of the screen
    pLabel->setPosition(VisibleRect::top(0, -20));
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    LabelTTF* label = LabelTTF::create(title().c_str(), "Arial", 24);
    addChild(label, 1);
    label->setPosition(VisibleRect::top(0, -50));
    
    std::string strSubtitle = subtitle();
    if (!strSubtitle.empty())
    {
        LabelTTF* l = LabelTTF::create(strSubtitle.c_str(), "Thonburi", 20);
        addChild(l, 1);
        l->setPosition(VisibleRect::top(0, -80));
    }
    
    MenuItemImage *item1 = MenuItemImage::create("b1.png", "b2.png", this, menu_selector(DemoBase::backCallback));
    MenuItemImage *item2 = MenuItemImage::create("r1.png", "r2.png", this, menu_selector(DemoBase::restartCallback));
    MenuItemImage *item3 = MenuItemImage::create("f1.png", "f2.png", this, menu_selector(DemoBase::nextCallback));
    MenuItemImage *pCloseItem = MenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(DemoBase::menuCloseCallback));
    
    CCMenu *menu = CCMenu::create(item1, item2, item3, pCloseItem, NULL);
    
    menu->setPosition(CCPointZero);
    item1->setPosition(VisibleRect::bottom(-item2->getContentSize().width * 2, item2->getContentSize().height / 2));
    item2->setPosition(VisibleRect::bottom(0, item2->getContentSize().height / 2));
    item3->setPosition(VisibleRect::bottom(item2->getContentSize().width * 2, item2->getContentSize().height / 2));
    pCloseItem->setPosition(VisibleRect::rightBottom(-pCloseItem->getContentSize().width / 2, pCloseItem->getContentSize().height / 2));
    
    addChild(menu, 1);
}

void DemoBase::restartCallback(Ref* pSender)
{
    Scene* s = new Scene();
    s->addChild(restartDBDemoAction());
    
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void DemoBase::nextCallback(Ref* pSender)
{
    Scene* s = new Scene();
    s->addChild(nextDBDemoAction());
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void DemoBase::backCallback(Ref* pSender)
{
    Scene* s = new Scene();
    s->addChild(backDBDemoAction());
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void DemoBase::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
    exit(0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}