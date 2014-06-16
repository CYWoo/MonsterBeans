#include "UserChange.h"

#define ITEM_HEIGHT 41
#define POSX background->getPositionX()
#define POSY background->getPositionY()
bool UserChange::init()
{
	CCLayer::init();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_winWidth = winSize.width;
	_winHeight = winSize.height;
	initView();
	return true;
}

void UserChange::editUser(CCObject *node)
{
	this->clickSound();
	CCMenuItemImage *menuItem=(CCMenuItemImage*)node;
	int index=(int)menuItem->getUserData();
	_oldname=_user_items[index]->username;
	this->removeAllChildrenWithCleanup(true);
	editView(_oldname);
}

void UserChange::selectUser(CCObject *node)
{
	this->clickSound();
	CCMenuItemImage *menuItem=(CCMenuItemImage*)node;
	int index=(int)menuItem->getUserData();
	//_select->setPosition(ccp(160,350-ITEM_HEIGHT*(index+1)));
	_select->runAction(CCMoveTo::create(0.2,ccp(POSX,POSY+111-ITEM_HEIGHT*(index+1))));
	WriteGameDataString("user_selected",this->_user_items[index]->username);
	changeUserLabel(this->_user_items[index]->username);
}

void UserChange::changeUserLabel(const char *name)
{
	CCLabelTTF *ptext=(CCLabelTTF*)(this->getParent()->getChildByTag(MENU_USER));
	ptext->setString(name);
}


void UserChange::clickAddButton(CCObject *node)
{
	this->clickSound();
	this->removeAllChildrenWithCleanup(true);
	insertView();

}

void UserChange::close(CCObject *node)
{
	this->clickSound();
	CCMenu *pmenu=(CCMenu*)(this->getParent()->getChildByTag(MENU));
	pmenu->setEnabled(true);
	this->removeFromParentAndCleanup(true);
}

void UserChange::initView()
{
	
	background = CCSprite::create("user_bg.png");
	background->setScale(2*0.5);
	background->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(background);
    
	CCSprite * title = CCSprite::create("user_title.png");
	title->setScale(2*0.5);
	title->setPosition(ccp(POSX,POSY+112));
	this->addChild(title);

	CCMenuItemImage * but_add = CCMenuItemImage::create("user_add.png","user_add.png",this,SEL_MenuHandler(&UserChange::clickAddButton));
	but_add->setScale(2*0.5);
	but_add->setPosition(ccp(POSX,POSY-112));

	CCMenuItemImage* close_bt = CCMenuItemImage::create("exit.png","exit.png",this,SEL_MenuHandler(&UserChange::close) );  
	close_bt->setPosition(ccp(POSX+125, POSY+124));
	close_bt->setScale(2*0.5);

	_menu = CCMenu::create(but_add,close_bt,NULL);
    _menu->setPosition(ccp(0,0));
	this->addChild(_menu);

	for(int i=0;i<USER_COUNT;i++)
	{
		_user_items[i]=NULL;
	}
	int userNum= ReadGameData("user_number");
	int pos=0;
	for(int i=0;i<userNum;i++)
	{
		UserItem *uitem=new UserItem();
		char namestr[20];
		sprintf(namestr,"user_name_%d",i+1);
		ReadGameDataString(namestr,uitem->username);
		char selectedUser[30];
		getSelectedUser(selectedUser);
		if(strcmp(uitem->username,selectedUser)==0)
		{
			pos=i+1;
		}
		CCMenuItemImage* item_bg = CCMenuItemImage::create("user_item.png","user_item.png",this,SEL_MenuHandler(&UserChange::selectUser) );  
		item_bg->setPosition(ccp(POSX-23, ITEM_HEIGHT*(1-i)+POSY+28));
		item_bg->setScale(2*0.5);
		item_bg->setUserData((void*)i);
		_menu->addChild(item_bg);
		uitem->user_bg=item_bg;

		CCMenuItemImage* btn_edit = CCMenuItemImage::create("edit.png","edit.png",this,SEL_MenuHandler(&UserChange::editUser));  
		btn_edit->setPosition(ccp(POSX+90, ITEM_HEIGHT*(1-i)+POSY+28));
		btn_edit->setScale(2*0.5);
		btn_edit->setUserData((void*)i);
		_menu->addChild(btn_edit);
		uitem->user_edit=btn_edit;

		CCLabelTTF *nameLabel = CCLabelTTF::create(uitem->username, "Arial", 25);
		nameLabel->setColor(ccRED);
		nameLabel->setScale(2*0.5);
		nameLabel->setPosition(ccp(POSX,POSY+72-ITEM_HEIGHT*i));
		this->addChild(nameLabel,11);
		uitem->uname=nameLabel;
		_user_items[i]=uitem;
	}
	_select = CCSprite::create("user_select.png");
	_select->setScale(2*0.5);
	_select->setPosition(ccp(POSX,POSY+111-ITEM_HEIGHT*pos));
	if(userNum>4)
	{
		but_add->setVisible(false);
	}
	else
	{
		but_add->setVisible(true);
	}
	this->addChild(_select,20);
}

void UserChange::editView(char *str)
{
	CCSprite *background = CCSprite::create("msg_bg.png");
	background->setScale(2*0.5);
	background->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(background);

	CCSprite * title = CCSprite::create("edit_title.png");
	title->setScale(2*0.5);
	title->setPosition(ccp(POSX,POSY+52));
	this->addChild(title);

	CCMenuItemImage * but_update = CCMenuItemImage::create("bnt_update.png","bnt_update.png",this,SEL_MenuHandler(&UserChange::UpdateUser) );
	but_update->setScale(2*0.5);
	but_update->setPosition(ccp(POSX-50,POSY-60));

	CCMenuItemImage * bnt_delete = CCMenuItemImage::create("bnt_delete.png","bnt_delete.png",this,SEL_MenuHandler(&UserChange::DeleteUser));
	bnt_delete->setScale(2*0.5);
	bnt_delete->setPosition(ccp(POSX+50,POSY-60));

	CCMenuItemImage* close_bt = CCMenuItemImage::create("exit.png","exit.png",this,SEL_MenuHandler(&UserChange::close) );  
	close_bt->setPosition(ccp(POSX+123, POSY+91));
	close_bt->setScale(2*0.5);

	CCMenuItemImage* return_bt = CCMenuItemImage::create("return.png","return.png",this,SEL_MenuHandler(&UserChange::preLayer) );  
	return_bt->setPosition(ccp(POSX-75, POSY+56));
	return_bt->setScale(2*0.5);

	CCSize editBoxSize = CCSizeMake(_winWidth/2+30, 40);
    m_pEditName = CCEditBox::create(editBoxSize, CCScale9Sprite::create("white_edit.png"));
	m_pEditName->setScale(2*0.5);
    m_pEditName->setText(str);
    m_pEditName->setPosition(ccp(POSX, POSY));
	m_pEditName->setFontName("Arial");
    m_pEditName->setFontSize(20);
    m_pEditName->setFontColor(ccRED);
   // m_pEditName->setPlaceHolder("Name:");
    m_pEditName->setPlaceholderFontColor(ccWHITE);
    m_pEditName->setMaxLength(8);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    addChild(m_pEditName);
	
	_menu = CCMenu::create(but_update,bnt_delete,return_bt,close_bt,NULL);
    _menu->setPosition(ccp(0,0));
	this->addChild(_menu);
}


void UserChange::preLayer(CCObject *node)
{
	this->clickSound();
	this->removeAllChildrenWithCleanup(true);
	this->initView();
}

void UserChange::insertView()
{
	CCSprite *background = CCSprite::create("msg_bg.png");
	background->setScale(2*0.5);
	background->setPosition(ccp(_winWidth / 2, _winHeight / 2));
	this->addChild(background);
    
	CCSprite * title = CCSprite::create("edit_title.png");
	title->setScale(2*0.5);
	title->setPosition(ccp(POSX,POSY+52));
	this->addChild(title);

	CCMenuItemImage * but_add = CCMenuItemImage::create("user_add.png","user_add.png",this,SEL_MenuHandler(&UserChange::AddUser));
	but_add->setScale(2*0.5);
	but_add->setPosition(ccp(POSX,POSY-60));

	CCMenuItemImage* close_bt = CCMenuItemImage::create("exit.png","exit.png",this,SEL_MenuHandler(&UserChange::close) );
	close_bt->setPosition(ccp(POSX+123, POSY+91));
	close_bt->setScale(2*0.5);
    
	CCMenuItemImage* return_bt = CCMenuItemImage::create("return.png","return.png",this,SEL_MenuHandler(&UserChange::preLayer) );
	return_bt->setPosition(ccp(POSX-75, POSY+56));
	return_bt->setScale(2*0.5);
    
	CCSize editBoxSize = CCSizeMake(_winWidth/2+30, 40);
    m_pEditName = CCEditBox::create(editBoxSize, CCScale9Sprite::create("white_edit.png"));
	m_pEditName->setScale(2*0.5);
    m_pEditName->setPosition(ccp(POSX, POSY));
	m_pEditName->setFontName("Thonburi");

    m_pEditName->setFontSize(20);
    m_pEditName->setFontColor(ccRED);
    // m_pEditName->setPlaceHolder("Name:");
    m_pEditName->setPlaceholderFontColor(ccWHITE);
    m_pEditName->setMaxLength(8);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    addChild(m_pEditName);
	
	_menu = CCMenu::create(but_add,return_bt,close_bt,NULL);
    _menu->setPosition(ccp(0,0));
	this->addChild(_menu);
}

void UserChange::DeleteUser(CCObject *node)
{
	this->clickSound();
	if(this->m_pEditName->getText()=="")
		return;
	if(deleteUser(this->m_pEditName->getText()))
	{
		char name[20];
		ReadGameDataString("user_selected",name);
		this->changeUserLabel(name);
		this->preLayer(NULL);
	}
}
void UserChange::AddUser(CCObject *node)
{
	this->clickSound();
	if(this->m_pEditName->getText()=="")
		return;
	if(addUser(this->m_pEditName->getText()))
	{
		this->changeUserLabel(this->m_pEditName->getText());
		this->preLayer(NULL);
	}
}
void UserChange::UpdateUser(CCObject *node)
{
	this->clickSound();
	if(this->m_pEditName->getText()=="")
		return;
	if(updateUser(_oldname,this->m_pEditName->getText()))
	{
		this->changeUserLabel(this->m_pEditName->getText());
		this->preLayer(NULL);
	}
}

void UserChange::clickSound()
{
	if(!IsSoundOff) SimpleAudioEngine::sharedEngine()->playEffect("select.wav");
}