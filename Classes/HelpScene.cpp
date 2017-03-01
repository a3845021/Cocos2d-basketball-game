#include "HelpScene.h"
#include "StartScene.h"
#include "ui\CocosGUI.h"

using namespace cocos2d::ui;

Scene * HelpScene::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}

bool HelpScene::init()
{
	if (!Layer::init()) return false;

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	hitNum = 1;
	addBackground();
	addHitPic();
	addQuitButton();
	addRightButton();
	return true;
}

void HelpScene::addBackground()
{
	auto bgColor = Sprite::create("HelpScene/bg_color.png");
	bgColor->setScaleX(visibleSize.width / bgColor->getContentSize().width);
	bgColor->setScaleY(visibleSize.height / bgColor->getContentSize().height);
	bgColor->setPosition(visibleSize / 2);
	this->addChild(bgColor, 0);
}

void HelpScene::addQuitButton()
{
	auto button = Button::create("HelpScene/quit_normal.png", "HelpScene/quit_click.png");
	button->addClickEventListener(CC_CALLBACK_1(HelpScene::BackToStart, this));
	button->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + button->getContentSize().height));
	this->addChild(button);
}

void HelpScene::addLeftButton()
{
	auto leftButton = Button::create("HelpScene/left_normal.png", "HelpScene/left_click.png");
	leftButton->setName("leftButton");
	leftButton->setScale(100 / leftButton->getContentSize().width);
	leftButton->addClickEventListener(CC_CALLBACK_1(HelpScene::leftClick, this));
	leftButton->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2));
	this->addChild(leftButton, 1);
}

void HelpScene::addRightButton()
{
	auto rightButton = Button::create("HelpScene/right_normal.png", "HelpScene/right_click.png");
	rightButton->setName("rightButton");
	rightButton->setScale(100 / rightButton->getContentSize().width);
	rightButton->addClickEventListener(CC_CALLBACK_1(HelpScene::rightClick, this));
	rightButton->setPosition(Vec2(origin.x + visibleSize.width / 4 * 3, origin.y + visibleSize.height / 2));
	this->addChild(rightButton, 1);
}

void HelpScene::addHitPic()
{
	auto picture = Sprite::create("HelpScene/hit1.png");
	picture->setName("picture");
	picture->setScale(visibleSize.width / 3 / picture->getContentSize().width);
	picture->setPosition(visibleSize / 2);
	this->addChild(picture, 1);
}

void HelpScene::setPicture()
{
	auto picture = (Sprite *)(this->getChildByName("picture"));
	switch (hitNum) {
	case 1: picture->initWithFile("HelpScene/hit1.png"); break;
	case 2: picture->initWithFile("HelpScene/hit2.png"); break;
	case 3: picture->initWithFile("HelpScene/hit3.png"); break;
	default: log("hitNum error");
	}
}

void HelpScene::leftClick(Ref * ref)
{
	hitNum--;
	setPicture();
	switch (hitNum) {
	case 2:
		addRightButton();
		break;
	case 1:
		this->removeChildByName("leftButton", false);
		break;
	default:
		log("HelpScene::leftClick has a problem");
	}
}

void HelpScene::rightClick(Ref * ref)
{
	hitNum++;
	setPicture();
	switch (hitNum) {
	case 2:
		addLeftButton();
		break;
	case 3:
		this->removeChildByName("rightButton", false);
		break;
	default:
		log("HelpScene::rightClick has a problem");
	}
}

void HelpScene::BackToStart(Ref * ref)
{
	Director::getInstance()->replaceScene(TransitionFade::create(2.0f, StartScene::createScene()));
}
