#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <string>
#include "StartScene.h"
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using std::to_string;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;
	addbackground();
	addReturn();
	submit_httpclient();//自动提交分数
/*	char d[10];
	itoa(Global::score1 + Global::score2 + Global::score3, d, 10);
    score_field = TextField::create(d, "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);
	score_field->setString(d);
    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
	submit_button->addClickEventListener(CC_CALLBACK_0(GameScene::submit_httpclient, this));
    this->addChild(submit_button, 2);
	*/
    rank_field = TextField::create("", "Arial", 30);
	rank_field->setAnchorPoint(Vec2(0.5, 1));
    rank_field->setPosition(Size(visibleWidth / 4, visibleHeight));
	rank_field->setColor(Color3B::RED);
    this->addChild(rank_field, 2);

  /*  rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
    rank_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
	//rank_button->addClickEventListener(CC_CALLBACK_0(GameScene::rank_httpclient, this));
    this->addChild(rank_button, 2);*/
	rank_httpclient();
    return true;
}

void GameScene::submit_httpclient() {
	char d[10];
	string s;
	itoa(Global::score1 + Global::score2 + Global::score3, d, 10);
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/submit");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequest_submit, this));
	//write post
	s = d;
	string postData_str = "score=" + s;
	const char* postData = postData_str.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("Post test");
	//write request header
	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}
void GameScene::onHttpRequest_submit(HttpClient* sender, HttpResponse* response) {
	if (!response)
		return;
	if (!response->isSucceed()) {
		log("Response Failed!");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	/*std::vector<char> *buffer;
	buffer = response->getResponseData();
	string str = Global::toString(buffer);
	CCLOG("%s\n", str.c_str());
	rapidjson::Document d;
	d.Parse<0>(str.c_str());
	//if (d.IsObject() && d.HasMember("info"))
	//	score_field->setString(d["info"].GetString());*/
}

void GameScene::rank_httpclient() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/rank?top=10");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequest_rank, this));
	request->setTag("Get test");
	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::onHttpRequest_rank(HttpClient* sender, HttpResponse* response) {
	if (!response)
		return;
	if (!response->isSucceed()) {
		log("Response Failed!");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char> *buffer;
	buffer = response->getResponseData();
	string str = Global::toString(buffer);
	CCLOG("%s\n", str.c_str());
	rapidjson::Document d;
	d.Parse<0>(str.c_str());
	string str_rank = "",str_last = "";
	if (d.IsObject() && d.HasMember("info"))
		str_rank = d["info"].GetString();
	for (int i = 0; i < str_rank.length(); i++) {
		if (str_rank[i] == '|')
			str_last += '\n';
		else
			str_last += str_rank[i];
	}
		rank_field->setString(str_last);

}

void GameScene::addbackground()
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

void GameScene::addReturn()
{
	rank_button = Button::create("quit_normal.png", "quit_click.png");
	rank_button->setPosition(Size(visibleWidth / 4, rank_button->getContentSize().height));
	rank_button->addClickEventListener(CC_CALLBACK_0(GameScene::return_button, this));
	this->addChild(rank_button, 2);
}

void GameScene::return_button() {
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, StartScene::createScene(), Color3B(0, 255, 255)));
	Global::score1 = 0;
	Global::score2 = 0;
	Global::score3 = 0;
}