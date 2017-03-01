#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;
#pragma execution_character_set("utf-8")  // 为了正常显示
using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;
#pragma execution_character_set("utf-8");
Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}





// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

	addbackground();

    textField = TextField::create("点击此处输入名字", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 3, visibleHeight / 4 * 3));
    this->addChild(textField, 2);

    auto button = Button::create("login_normal.png", "login_click.png");
    button->setPosition(Size(visibleWidth / 3, visibleHeight / 2));
	button->addClickEventListener(CC_CALLBACK_1(LoginScene::startMenuCallback, this));
    this->addChild(button, 2);
	
    return true;
}

void LoginScene::addbackground()
{
	auto bgColor = Sprite::create("login_bgColor.png");
	bgColor->setScaleX(visibleWidth / bgColor->getContentSize().width);
	bgColor->setScaleY(visibleHeight / bgColor->getContentSize().height);
	bgColor->setPosition(visibleWidth / 2, visibleHeight / 2);
	this->addChild(bgColor, 0);

	auto bgMan = Sprite::create("login_bgman.png");
	float bgMan_scale = visibleHeight * 2 / 3 / bgMan->getContentSize().height;
	bgMan->setScale(bgMan_scale);
	auto origin = Director::getInstance()->getVisibleOrigin();
	bgMan->setPosition(Vec2(origin.x + visibleWidth - bgMan->getContentSize().width * bgMan_scale / 2,
							origin.y + bgMan->getContentSize().height * bgMan_scale / 2));
	this->addChild(bgMan, 1);
}


void LoginScene::startMenuCallback(cocos2d::Ref* pSender) {
	//转到下一个页面
	if (textField->getString() != "") {
		httpclient();
	//	Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameScene::createScene(), Color3B(0, 255, 255)));
	}
	else
	textField->setPlaceHolder("请输入用户名！");

}

void LoginScene::httpclient() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpRequestCompleted, this));
	//write post
	string postData_str = "username=" + textField->getString();
	const char* postData = postData_str.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("Post test");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void LoginScene::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response) {
	if (!response)
		return;
	if (!response->isSucceed()) {
		log("Response Failed!");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	
	std::vector<char> *buffer;
	Global::gameSessionId = Global::getSessionIdFromHeader(Global::toString(response->getResponseHeader()));
	buffer = response->getResponseData();
	string buf = Global::toString(buffer);
	CCLOG("%s\n", buf.c_str());
	//Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameScene::createScene(), Color3B(0, 255, 255)));
	Director::getInstance()->replaceScene(GameScene::createScene());
}
