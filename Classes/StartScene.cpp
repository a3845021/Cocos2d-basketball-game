#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "HelpScene.h"
#include "HelloWorldScene.h"
#include "Game1.h"
#include "Game3.h"
#pragma execution_character_set("utf-8")  // 为了正常显示

using namespace CocosDenshion;
using namespace ui;

Scene * StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init()
{
	if (!Layer::init()) return false;

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	/*
	 * 图层设置
	 * 0.背景色
	 * 1.左边的人
	 * 2.右边的按键
	 */

	addBackground();
	preloadmusic();
	addbgm();
	addPlayButton();

	return true;
}

void StartScene::addBackground()
{
	auto bgColor = Sprite::create("StartScene/bg_color.png");
	bgColor->setScaleX(visibleSize.width / bgColor->getContentSize().width);
	bgColor->setScaleY(visibleSize.height / bgColor->getContentSize().height);
	bgColor->setPosition(visibleSize / 2);
	this->addChild(bgColor, 0);

	auto bgSprite = Sprite::create("StartScene/bg_sp.png");
	bgSprite->setScale(visibleSize.height / bgSprite->getContentSize().height);
	bgSprite->setAnchorPoint(Vec2(1, 1));
	bgSprite->setPosition(Vec2(origin.x + visibleSize.width,
							   origin.y + visibleSize.height));
	this->addChild(bgSprite, 1);
}

void StartScene::addbgm()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("StartScene/bgm.mp3", true);
}

void StartScene::preloadmusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("StartScene/bgm.mp3");
}

void StartScene::addPlayButton()
{
	auto gameName = Label::create("樱木特训", "StartScene/晨光大字.ttf", 100);
	gameName->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height - gameName->getContentSize().height * 2));
	gameName->setColor(Color3B(0, 0, 0));
	gameName->setName("gameName");
	this->addChild(gameName, 2);
	auto playButton = Button::create("StartScene/play_normal.png",
									 "StartScene/play_Clicked.png");
	playButton->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + playButton->getContentSize().height * 3));
	playButton->addClickEventListener(CC_CALLBACK_1(StartScene::clickPlay, this));
	playButton->setName("playButton");
	this->addChild(playButton, 2);
}

void StartScene::clickPlay(Ref* ref)
{
	this->getChildByName("gameName")->runAction(FadeOut::create(0.5f));
	Vector<FiniteTimeAction*> ve;
	ve.pushBack(FadeOut::create(0.5f));
	ve.pushBack(CallFunc::create(CC_CALLBACK_0(StartScene::removePlayButton, this)));
	ve.pushBack(CallFunc::create(CC_CALLBACK_0(StartScene::addMenuButton, this)));
	auto action = Sequence::create(ve);
	this->getChildByName("playButton")->runAction(action);
}

void StartScene::removePlayButton()
{
	this->removeChildByName("playButton");
}

void StartScene::addMenuButton()
{
	// 1.人机pk模式
	// 2.闯关模式
	// 3.帮助
	// 4.关于
	// 5.退出

	Vector<MenuItem*> items;
	char normalPath[50];
	char clickPath[50];
	for (int i = 1; i <= 5; i++) {
		sprintf(normalPath, "StartScene/menu%d_normal.png", i);
		sprintf(clickPath, "StartScene/menu%d_click.png", i);
		items.pushBack(MenuItemImage::create(normalPath, clickPath, CC_CALLBACK_0(StartScene::MenuFun, this, i)));
	}
	auto menu = Menu::createWithArray(items);
	menu->setName("menu");
	menu->alignItemsVertically();
	menu->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 2));
	menu->setOpacity(0);
	menu->runAction(FadeIn::create(0.5f));
	this->addChild(menu, 2);
}

void StartScene::MenuFun(int menu)
{
	switch (menu) {
	case 1:
		// 跳到人机PK模式
		SimpleAudioEngine::sharedEngine()->end();
		Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Game3::createScene()));
		break;
	case 2:
		// 跳到闯关模式
		SimpleAudioEngine::sharedEngine()->end();
		 Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Game1::createScene()));
		break;
	case 3:
		Director::getInstance()->replaceScene(TransitionFade::create(2.0f, HelpScene::createScene()));
		break;
	case 4:
		this->getChildByName("menu")->runAction(Sequence::create(FadeOut::create(0.5f),
												CallFunc::create(CC_CALLBACK_0(StartScene::quitMenuButton, this)),
												CallFunc::create(CC_CALLBACK_0(StartScene::addAbout, this)),
												NULL));
		break;
	case 5:
		exit(0);
		break;
	default:
		break;
	}
}

void StartScene::quitMenuButton()
{
	this->removeChild(this->getChildByName("menu"));
}

void StartScene::addAbout()
{
	auto aboutLayer = Layer::create();
	aboutLayer->setName("aboutLayer");

	auto nameList = Label::create("徐嘉辰 许佳灿 吴佳雨", "StartScene/晨光大字.ttf", 30.0f);
	nameList->setColor(Color3B(0, 0, 0));
	nameList->setPosition(Vec2(origin.x + visibleSize.width / 3,
							   origin.y + visibleSize.height / 2 + nameList->getContentSize().height));
	aboutLayer->addChild(nameList);

	auto quitButton = Button::create("StartScene/quit_normal.png", "StartScene/quit_click.png");
	quitButton->addClickEventListener(CC_CALLBACK_1(StartScene::quitAboutClick, this));
	quitButton->setPosition(Vec2(nameList->getPositionX(),
								 nameList->getPositionY() - nameList->getContentSize().height / 2 - quitButton->getContentSize().height));
	aboutLayer->addChild(quitButton);

	this->addChild(aboutLayer, 2);
	aboutLayer->setOpacity(0);
	aboutLayer->runAction(FadeIn::create(0.5f));
}

void StartScene::quitAboutClick(Ref* ref)
{
	this->getChildByName("aboutLayer")->runAction(Sequence::create(
		FadeOut::create(0.5f),
		CallFunc::create(CC_CALLBACK_0(StartScene::quitAbout, this)),
		CallFunc::create(CC_CALLBACK_0(StartScene::addMenuButton, this)),
		NULL
	));
}

void StartScene::quitAbout()
{
	this->removeChildByName("aboutLayer", false);
}
