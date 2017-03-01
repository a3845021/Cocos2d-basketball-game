#include "Game1.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Global.h"
#include "Game2.h"
#include "StartScene.h"
#include "LoginScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
using namespace std;
using namespace cocostudio::timeline;
using namespace CocosDenshion;
void Game1::setPhysicsWorld(PhysicsWorld* world) {
	m_world = world;
}

Scene* Game1::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Point(0, 0));
	// 'layer' is an autorelease object
	auto layer = Game1::create(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Game1::init(PhysicsWorld* world)
{
	if (!Layer::init())
	{
		return false;
	}

	Isthrow = false;
	IsthrowOver = false;
	Isphysic = false;

	this->setPhysicsWorld(world);
	visibleSize = Director::getInstance()->getVisibleSize();
	addEdge();
	addBall();
	addArrow();
	addBackground();
	addPlayground();
	addShadow();
	addTouchListener();
	addMouseListener();
	addContactListener();

	m_world->setAutoStep(true);

	m_world->setGravity(Vec2(0, -500));
	//m_world->step(1.0 / 180);
	this->scheduleUpdate();

	//音乐
	preloadmusic();
	addvoice("Smack That.mp3");
	//分数初始化
	Global::score1 = 0;
	//时间初始化
	time1 = 30;
	time_str = Label::createWithTTF("30", "fonts/Marker Felt.ttf", 36);
	time_str->setPosition(Vec2(visibleSize.width/2, visibleSize.height - time_str->getContentSize().height));
	addChild(time_str,3);
	
	//提示
	auto visibleWidth = Director::getInstance()->getVisibleSize().width;
	auto label = Label::create("在30秒内进行投篮，\n每投进一球加一分并续一秒，\n结束时分数大于三分可进入下一关！", "StartScene/font.ttf", 36);
	label->setPosition(Vec2(visibleSize.width - visibleSize.width / 3-80, visibleSize.height-label->getContentSize().height));
	label->enableGlow(Color4B::BLUE);     //设置荧光效果，仅限ttf文字
	label->setOpacity(0);
	label->setScale(visibleWidth / label->getContentSize().width);
	this->addChild(label,3);
	auto fadein = FadeIn::create(2);
	auto delay = DelayTime::create(2);
	auto fadeout = FadeOut::create(2);
	auto func = CallFunc::create(CC_CALLBACK_0(Game1::schedule_use, this));
	auto seq = Sequence::create(fadein,delay,fadeout,func,nullptr);
	label->runAction(seq);
	//分数
	score_str = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 36);
	score_str->setPosition(Vec2(visibleSize.width - score_str->getContentSize().width, visibleSize.height - score_str->getContentSize().height));
	addChild(score_str, 3);
	auto score_label = Label::createWithTTF("Score:", "fonts/Marker Felt.ttf", 36);
	score_label ->setPosition(Vec2(visibleSize.width - score_label->getContentSize().width, visibleSize.height - score_str->getContentSize().height));
	addChild(score_label, 3);
	//进球无效
	alert = Label::createWithTTF("进球无效！", "StartScene/font.ttf", 36);
	alert->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - alert->getContentSize().height*2));
	alert->setOpacity(0);
	alert->setColor(Color3B::RED);
	alert->enableGlow(Color4B::WHITE);
	addChild(alert, 3);
	//判断时间结束
	istimeover = false;
	isbegin = false;
	//过关
	next = Label::createWithTTF("恭喜过关！", "StartScene/font.ttf", 56);
	next->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - visibleSize.height / 3));
	next->setOpacity(0);
	next->enableGlow(Color4B::YELLOW);
	addChild(next, 3);
	//+1s
	adds = Label::createWithTTF("+1s", "fonts/Marker Felt.ttf", 20);
	adds->setPosition(Vec2(visibleSize.width / 2 + adds->getContentSize().width*2, visibleSize.height - score_str->getContentSize().height));
	adds->setOpacity(0);
	adds->setColor(Color3B::RED);
	addChild(adds, 3);
	return true;
}
//调用调度器
void Game1::schedule_use() {
	//时间调度器
	isbegin = true;
	this->schedule(schedule_selector(Game1::update_time), 1);
}

//变大函数
void Game1::label_scale(Label* s) {
	auto bigger = ScaleTo::create(0.5, 1.5);
	auto smaller = ScaleTo::create(0.5, 1);
	auto yellow1 = CallFunc::create(CC_CALLBACK_0(Game1::turn_yellow, this,s));
	auto white1 = CallFunc::create(CC_CALLBACK_0(Game1::turn_white, this,s));
	auto seq2 = Sequence::create(yellow1,bigger, white1,smaller,nullptr);
	s->runAction(seq2);
}

void Game1::turn_yellow(Label *y) {
	y->setColor(Color3B(235,212,61));
	y->enableGlow(Color4B(235,212,61,0));
}
void Game1::turn_white(Label *w) {
	w->setColor(Color3B::WHITE);
	w->enableGlow(Color4B(185,189,172,0));
}

//淡入淡出函数
void Game1::fade(float fadein,float fadeout,Label* a) {
	auto fadein1 = FadeIn::create(fadein);
	auto fadeout1 = FadeOut::create(fadeout);
	auto seq1 = Sequence::create(fadein1, fadeout1, nullptr);
	a->runAction(seq1);
}

//时间
void Game1::update_time(float t) {
	char d[10];
	if (time1 != 0) {
		time1--;
		itoa(time1, d, 10);
		time_str->setString(d);
	}
	else {
		istimeover = true;
		if (Global::score1 < 3) {
			next->setString("闯关失败！");
			next->setColor(Color3B::BLACK);
			fade(2, 2, next);
			auto delay_next = DelayTime::create(2);
			auto func = CallFunc::create([]() {
				SimpleAudioEngine::sharedEngine()->end();
				Director::getInstance()->replaceScene(TransitionFade::create(2.0f, LoginScene::createScene()));
			});
			auto seq = Sequence::create(delay_next, func, nullptr);
			this->runAction(seq);
		}
		else {
			next->setString("恭喜过关！");
			fade(2, 2, next);
			auto delay_next = DelayTime::create(2);
			auto func = CallFunc::create([]() {
				Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Game2::createScene()));
			});
			auto seq = Sequence::create(delay_next, func, nullptr);
			this->runAction(seq);
		}
	}
}

Game1* Game1::create(PhysicsWorld* world) {
	Game1* pRet = new(std::nothrow) Game1();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

void Game1::addEdge() {
	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.1f, 1.0f, 0.5f));
	boundBody->setDynamic(false);
	boundBody->setGroup(1);
	boundBody->setTag(0);
	boundBody->setContactTestBitmask(0x11);
	boundBody->setCategoryBitmask(0x03);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 15));
	edgeSp->setPhysicsBody(boundBody);
	this->addChild(edgeSp);
}
//音乐
void Game1::addvoice(string name)
{
	string where = "Sounds/" + name;
	auto audio = SimpleAudioEngine::getInstance();
	if (name == "Smack That.mp3")
		audio->playBackgroundMusic(where.c_str(), true);
	else
		audio->playEffect(where.c_str());
}

void Game1::preloadmusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("Sounds/Smack That.mp3");
	audio->preloadEffect("Sounds/打板.mp3");
	audio->preloadEffect("Sounds/打地.mp3");
	audio->preloadEffect("Sounds/打框.mp3");
}
void Game1::addBall() {
	Basketball = Sprite::create("Basketball.png");
	Basketball->setAnchorPoint(Vec2(0.5, 0.5));
	Basketball->setPosition(visibleSize / 2);


	addChild(Basketball, 3);
}

void Game1::addBackground() {
	auto bgsprite = Sprite::create("Background1.png");
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setAnchorPoint(Vec2(0.5, 0.5));
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);
}

void Game1::addArrow() {
	Arrow = Sprite::create("Throw.png");
	Arrow->setPosition(visibleSize.width, visibleSize.height + 100);
	addChild(Arrow, 1);
}

void Game1::addPlayground() {
	Support = Sprite::create("Support.png");
	Support->setPosition(visibleSize.width * 5 / 6 - 40, visibleSize.height / 4 - 5);
	Support->setPhysicsBody(PhysicsBody::createBox(Size(3, 200)));
	Support->getPhysicsBody()->setPositionOffset(Vec2(80, 15));
	//Support->setPhysicsBody(PhysicsBody::createCircle(Support->getContentSize().height / 2));
	Support->getPhysicsBody()->setGroup(1);
	Support->getPhysicsBody()->setDynamic(false);
	Support->getPhysicsBody()->setCategoryBitmask(0x00);
	Support->getPhysicsBody()->setCollisionBitmask(0x00);


	Board = Sprite::create("Board1.png");
	Board->setPhysicsBody(PhysicsBody::createBox(Size(1, 70)));
	Board->getPhysicsBody()->setPositionOffset(Vec2(-10, 0));
	Board->setPosition(visibleSize.width / 2 + 260, visibleSize.height / 2 - 35);
	Board->getPhysicsBody()->setAngularDamping(2000);
	Board->getPhysicsBody()->setGravityEnable(false);
	Board->getPhysicsBody()->setVelocityLimit(0.0001f);
	Board->getPhysicsBody()->setTag(8);//篮板的标签
	Board->getPhysicsBody()->setCategoryBitmask(0x01);
	Board->getPhysicsBody()->setCollisionBitmask(0x02);
	Board->getPhysicsBody()->setContactTestBitmask(0x11);
	//Board->getPhysicsBody()->setDynamic(false);

	PhysicsJointFixed* fixedJoint = PhysicsJointFixed::construct(Support->getPhysicsBody(), Board->getPhysicsBody(), Vec2(visibleSize.width * 5 / 6 - 40, visibleSize.height / 4 - 5));

	addChild(Support, 2);
	addChild(Board, 2);

	m_world->addJoint(fixedJoint);


	Hoop1 = Sprite::create();
	Hoop1->setPhysicsBody(PhysicsBody::createBox(Size(8, 3)));
	Hoop1->setPosition(Vec2(visibleSize.width / 2 + 195, visibleSize.height / 2 - 60));
	Hoop1->getPhysicsBody()->setAngularDamping(2000);
	Hoop1->getPhysicsBody()->setGravityEnable(false);
	Hoop1->getPhysicsBody()->setVelocityLimit(0.0001f);

	Hoop1->getPhysicsBody()->setCategoryBitmask(0x01);
	Hoop1->getPhysicsBody()->setCollisionBitmask(0x02);
	Hoop1->getPhysicsBody()->setContactTestBitmask(0x11);
	Hoop1->getPhysicsBody()->setTag(5);//篮筐的标签
	PhysicsJointFixed* fixedJoint2 = PhysicsJointFixed::construct(Hoop1->getPhysicsBody(), Board->getPhysicsBody(), Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
	addChild(Hoop1, 2);

	m_world->addJoint(fixedJoint2);

	Hoop2 = Sprite::create();
	Hoop2->setPhysicsBody(PhysicsBody::createBox(Size(8, 3)));
	Hoop2->setPosition(Vec2(visibleSize.width / 2 + 235, visibleSize.height / 2 - 60));
	Hoop2->getPhysicsBody()->setAngularDamping(2000);
	Hoop2->getPhysicsBody()->setGravityEnable(false);
	Hoop2->getPhysicsBody()->setVelocityLimit(0.0001f);
	Hoop2->getPhysicsBody()->setCategoryBitmask(0x01);
	Hoop2->getPhysicsBody()->setCollisionBitmask(0x02);
	Hoop2->getPhysicsBody()->setContactTestBitmask(0x11);
	Hoop2->getPhysicsBody()->setTag(5);//篮筐的标签
	PhysicsJointFixed* fixedJoint3 = PhysicsJointFixed::construct(Hoop2->getPhysicsBody(), Board->getPhysicsBody(), Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
	addChild(Hoop2, 2);

	m_world->addJoint(fixedJoint3);


	Hoop = Sprite::create("Hoop1.png");
	Hoop->setPosition(Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
	Hoop->setPhysicsBody(PhysicsBody::createBox(Size(20, 3)));
	Hoop->getPhysicsBody()->setCategoryBitmask(0x00);
	Hoop->getPhysicsBody()->setCollisionBitmask(0x00);
	Hoop->getPhysicsBody()->setAngularDamping(2000);
	Hoop->getPhysicsBody()->setGravityEnable(false);
	Hoop->getPhysicsBody()->setVelocityLimit(0.0001f);

	Hoop->getPhysicsBody()->setPositionOffset(Vec2(0, -5));
	PhysicsJointFixed* fixedJoint4 = PhysicsJointFixed::construct(Hoop->getPhysicsBody(), Hoop1->getPhysicsBody(), Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));

	addChild(Hoop, 4);

	Judge = Sprite::create();
	Judge->setPosition(Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
	Judge->setPhysicsBody(PhysicsBody::createBox(Size(5, 3)));
	Judge->getPhysicsBody()->setCategoryBitmask(0x01);
	Judge->getPhysicsBody()->setCollisionBitmask(0x02);
	Judge->getPhysicsBody()->setAngularDamping(100);
	Judge->getPhysicsBody()->setGravityEnable(false);
	Judge->getPhysicsBody()->setVelocityLimit(0.001f);
	Judge->getPhysicsBody()->setTag(30);
	Judge->getPhysicsBody()->setContactTestBitmask(0x01);
	addChild(Judge);
}

void Game1::addShadow() {
	Shadow = Sprite::create("Shadow.png");

	float x = Basketball->getPositionX();
	float y = 10;
	Shadow->setPosition(x, 15);
	addChild(Shadow, 2);
}

void Game1::addTouchListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Game1::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Game1::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Game1::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void Game1::addMouseListener() {
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(Game1::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Game1::addContactListener() {
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Game1::onConcactBegan, this);
	/*contactListener->onContactPreSolve = CC_CALLBACK_1(HelloWorld::onContactPreSolve, this);*/
	/*contactListener->onContactSeparate = CC_CALLBACK_1(HelloWorld::onContactSeprate, this);*/
	_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
}


void Game1::onMouseMove(Event* event) {
	//if (Isphysic == true && Basketball->getPhysicsBody()->getVelocity() == Vec2(0, 0) && IsthrowOver == true) {
	if (Isphysic == true
		&& Basketball->getPhysicsBody()->getVelocity().length() < 0.5 && Basketball->getPositionY() < Hoop->getPositionY()
		&& IsthrowOver == true) {
		Basketball->getPhysicsBody()->setGravityEnable(false);
		Isthrow = false;
		IsthrowOver = false;
	}
	if (!Isthrow) {
		EventMouse* e = (EventMouse*)event;
		Vec2 pos = e->getLocation();
		float x = pos.x;
		float y = visibleSize.height - pos.y;
		Basketball->setPosition(Vec2(x, y));
	}
}

bool Game1::onTouchBegan(Touch *touch, Event *unused_event) {
	if (Isphysic == false) {
		Basketball->setPhysicsBody(PhysicsBody::createCircle(Basketball->getContentSize().height / 2));
		Basketball->getPhysicsBody()->setTag(2);
		Basketball->getPhysicsBody()->setAngularVelocityLimit(0);
		Basketball->getPhysicsBody()->setCategoryBitmask(0x03);
		Basketball->getPhysicsBody()->setCollisionBitmask(0x03);
		Basketball->getPhysicsBody()->setGravityEnable(false);
		Basketball->getPhysicsBody()->setContactTestBitmask(0x01);

		Isphysic = true;
	}

	if (Basketball->getPhysicsBody()->isGravityEnabled() == false) {
		Isthrow = true;
		StartPos = touch->getLocation();
		MovingPos - StartPos;
		Judge->setPosition(Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
		return true;
	}
	return false;
}

void Game1::onTouchMoved(Touch *touch, Event *unused_event) {
	MovingPos = touch->getLocation();
}

void Game1::onTouchEnded(Touch *touch, Event *unused_event) {
	FinalPos = touch->getLocation();
	//m_world->setGravity(Vec2(0, -98));
	Basketball->getPhysicsBody()->setGravityEnable(true);
	Vec2 ShootPos;
	ShootPos.x = StartPos.x - FinalPos.x;
	ShootPos.y = StartPos.y - FinalPos.y;

	Basketball->getPhysicsBody()->setVelocity(ShootPos * 5);
	int t = 10;


	IsthrowOver = true;
}

bool Game1::onConcactBegan(PhysicsContact& contact) {
	if (!istimeover && isbegin) {
		auto nodeA = contact.getShapeA()->getBody()->getNode();
		auto nodeB = contact.getShapeB()->getBody()->getNode();
		char d[10], t[10];
		if (nodeA && nodeB) {
			if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 0)
				addvoice("打地.mp3");
			else if (nodeB->getPhysicsBody()->getTag() == 2 && nodeA->getPhysicsBody()->getTag() == 0)
				addvoice("打地.mp3");
			if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 8)
				addvoice("打板.mp3");
			else if (nodeB->getPhysicsBody()->getTag() == 2 && nodeA->getPhysicsBody()->getTag() == 8)
				addvoice("打板.mp3");
			if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 5)
				addvoice("打框.mp3");
			else if (nodeB->getPhysicsBody()->getTag() == 2 && nodeA->getPhysicsBody()->getTag() == 5)
				addvoice("打框.mp3");
			if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 30) {
				if (nodeA->getPosition().y > nodeB->getPosition().y) {
					Global::score1++;
					itoa(Global::score1, d, 10);
					score_str->setString(d);
					label_scale(score_str);
					time1++;
					itoa(time1, t, 10);
					time_str->setString(t);
					label_scale(time_str);
					fade(0.5, 0.5, adds);
				}
				else {
					fade(1,1,alert);
				}
				nodeB->setPosition(Vec2(visibleSize.width, visibleSize.height + 100));
			}
			else if (nodeA->getPhysicsBody()->getTag() == 30 && nodeB->getPhysicsBody()->getTag() == 2) {
				if (nodeA->getPosition().y < nodeB->getPosition().y) {
					Global::score1++;
					itoa(Global::score1, d, 10);
					score_str->setString(d);
					label_scale(score_str);
					time1++;
					itoa(time1, t, 10);
					time_str->setString(t);
					label_scale(time_str);
					fade(0.5, 0.5, adds);
				}
				else {
					fade(1,1,alert);
				}

				nodeA->setPosition(Vec2(visibleSize.width, visibleSize.height + 100));
			}
		}
	}
	return true;
}

void Game1::update(float dt) {
	if (visibleSize.width != Director::getInstance()->getVisibleSize().width ||
		visibleSize.height != Director::getInstance()->getVisibleSize().height)
	{
		visibleSize = Director::getInstance()->getVisibleSize();
	}
	// use fixed time and calculate 3 times per frame makes physics simulate more precisely.
	if (Isphysic == true && Basketball->getPhysicsBody()->isGravityEnabled() == true) {
		m_world->setAutoStep(false);
		for (int i = 0; i < 3; ++i)
		{
			m_world->step(1 / 180.0f);
		}
	}


	Shadow->setPosition(Basketball->getPositionX(), 15);
	if (Isphysic == true && Basketball->getPhysicsBody()->getVelocity() != Vec2(0, 0)) {
		float x = Basketball->getPhysicsBody()->getVelocity().x;
		float y = Basketball->getPhysicsBody()->getVelocity().y;

		Basketball->setRotation(Basketball->getRotation() + sqrt(x * x + y * y) / 50);
	}

	if (Isthrow == true) {
		Vec2 TouchPos = MovingPos - StartPos;
		Vec2 normalized = ccpNormalize(TouchPos);
		float radians = atan2(normalized.y, -1 * normalized.x);
		float degree = CC_RADIANS_TO_DEGREES(radians);

		Arrow->setRotation(degree);
		//auto rotateto = RotateTo::create(0, degree);
		//Arrow->runAction(rotateto);

		float x = TouchPos.x;
		float y = TouchPos.y;
		float lenth = sqrt(x * x + y * y);
		if (MovingPos.x != 0 && MovingPos.y != 0) {
			Arrow->setScale(lenth / 40, lenth / 180);
			/*auto scaleto = ScaleTo::create(0, lenth / 40, lenth / 180);
			Arrow->runAction(scaleto);*/

			Arrow->setPosition(Vec2(TouchPos.x / 2 + StartPos.x, TouchPos.y / 2 + StartPos.y));
		}

	}
	if (IsthrowOver == true) {
		MovingPos = Vec2(0, 0);
		Arrow->setScale(0, 0);
		Arrow->setPosition(visibleSize.width, visibleSize.height + 100);
	}
}