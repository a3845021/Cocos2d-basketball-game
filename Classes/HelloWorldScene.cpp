#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace cocostudio::timeline;
using namespace CocosDenshion;
void HelloWorld::setPhysicsWorld(PhysicsWorld* world) {
	m_world = world;
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Point(0, 0));
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init(PhysicsWorld* world)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	Isthrow = false;
	IsthrowOver = false;

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
	addbgm();

    return true;
}

HelloWorld* HelloWorld::create(PhysicsWorld* world) {
	HelloWorld* pRet = new(std::nothrow) HelloWorld();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}
//音乐
void HelloWorld::addbgm()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("StartScene/bgm.mp3", true);
}

void HelloWorld::preloadmusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("StartScene/bgm.mp3");
	audio->preloadEffect("Sounds/打板.mp3");
	audio->preloadEffect("Sounds/打地.mp3");
	audio->preloadEffect("Sounds/打框.mp3");
}
void HelloWorld::addEdge() {
	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize);
	boundBody->setDynamic(false);
	boundBody->setGroup(1);
	boundBody->setTag(0);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 15));
	edgeSp->setPhysicsBody(boundBody);
	this->addChild(edgeSp);
}

void HelloWorld::addBall() {
	Basketball = Sprite::create("Basketball.png");
	Basketball->setAnchorPoint(Vec2(0.5, 0.5));
	Basketball->setPosition(visibleSize / 2);
	Basketball->setPhysicsBody(PhysicsBody::createCircle(Basketball->getContentSize().height / 2));
	Basketball->getPhysicsBody()->setTag(1);
	Basketball->getPhysicsBody()->setAngularVelocityLimit(0);
	Basketball->getPhysicsBody()->setCategoryBitmask(0x03);
	Basketball->getPhysicsBody()->setCollisionBitmask(0x03);
	Basketball->getPhysicsBody()->setGravityEnable(false);
	Basketball->getPhysicsBody()->setContactTestBitmask(0x01);

	addChild(Basketball, 3);
}

void HelloWorld::addBackground() {
	auto bgsprite = Sprite::create("Background1.png");
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setAnchorPoint(Vec2(0.5, 0.5));
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);
}

void HelloWorld::addArrow() {
	Arrow = Sprite::create("Throw.png");
	Arrow->setPosition(visibleSize.width, visibleSize.height + 100);
	addChild(Arrow, 1);
}

void HelloWorld::addPlayground() {
	Support = Sprite::create("Support.png");
	Support->setPosition(visibleSize.width * 5 / 6 - 40, visibleSize.height / 4 - 5);
	Support->setPhysicsBody(PhysicsBody::createBox(Size(3, 200)));
	Support->getPhysicsBody()->setPositionOffset(Vec2(80, 15));
	//Support->setPhysicsBody(PhysicsBody::createCircle(Support->getContentSize().height / 2));
	Support->getPhysicsBody()->setGroup(1);
    Support->getPhysicsBody()->setDynamic(false);
	Support->getPhysicsBody()->setCategoryBitmask(0x00);
	Support->getPhysicsBody()->setCollisionBitmask(0x00);

	Basketball->getPhysicsBody()->setTag(2);
	

	Board = Sprite::create("Board1.png");
	Board->setPhysicsBody(PhysicsBody::createBox(Size(1, 70)));
	Board->getPhysicsBody()->setPositionOffset(Vec2(-10, 0));
	Board->setPosition(visibleSize.width / 2 + 260, visibleSize.height / 2 - 35);
	Board->getPhysicsBody()->setAngularDamping(2000);
	Board->getPhysicsBody()->setGravityEnable(false);
	Board->getPhysicsBody()->setVelocityLimit(0.0001f);

	Board->getPhysicsBody()->setCategoryBitmask(0x01);
	Board->getPhysicsBody()->setCollisionBitmask(0x02);
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

void HelloWorld::addShadow() {
	Shadow = Sprite::create("Shadow.png");
	
	float x = Basketball->getPositionX();
	float y = 10;
	Shadow->setPosition(x, 15);
	addChild(Shadow, 2);
}

void HelloWorld::addTouchListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void HelloWorld::addMouseListener() {
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void HelloWorld::addContactListener() {
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onConcactBegan, this);
	/*contactListener->onContactPreSolve = CC_CALLBACK_1(HelloWorld::onContactPreSolve, this);*/
	/*contactListener->onContactSeparate = CC_CALLBACK_1(HelloWorld::onContactSeprate, this);*/
	_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
}


void HelloWorld::onMouseMove(Event* event) {
	if (Basketball->getPhysicsBody()->getVelocity() == Vec2(0, 0) && IsthrowOver == true) {
		//m_world->setGravity(Vec2(0, 0));
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

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event) {
	if (Basketball->getPhysicsBody()->isGravityEnabled() == false) {
		Isthrow = true;
		StartPos = touch->getLocation();
		MovingPos - StartPos;
		Judge->setPosition(Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
		return true;
	}
	return false;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event) {
	MovingPos = touch->getLocation();
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event) {
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

bool HelloWorld::onConcactBegan(PhysicsContact& contact) {
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB) {
		if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 30) {
			nodeB->setPosition(Vec2(visibleSize.width, visibleSize.height + 100));
		}
		else if (nodeA->getPhysicsBody()->getTag() == 30 && nodeB->getPhysicsBody()->getTag() == 2) {
		    nodeA->setPosition(Vec2(visibleSize.width, visibleSize.height + 100));
		}
	}
	return true;
}

//bool HelloWorld::onContactPreSolve(PhysicsContact& contact) {
//
//}

//bool HelloWorld::onContactSeprate(PhysicsContact& contact) {
//	auto nodeA = contact.getShapeA()->getBody()->getNode();
//	auto nodeB = contact.getShapeB()->getBody()->getNode();
//
//	if (nodeA && nodeB) {
//		int a = nodeA->getPhysicsBody()->getTag();
//		int b = nodeB->getPhysicsBody()->getTag();
//		if (nodeA->getPhysicsBody()->getTag() == 2 && nodeB->getPhysicsBody()->getTag() == 30) {
//			nodeB->setPosition(Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
//		}
//		else if (nodeA->getPhysicsBody()->getTag() == 30 && nodeB->getPhysicsBody()->getTag() == 2) {
//			nodeA->setPosition(Vec2(visibleSize.width / 2 + 215, visibleSize.height / 2 - 63));
//		}
//	}
//	return true;
//}

void HelloWorld::update(float dt) {
	if (visibleSize.width != Director::getInstance()->getVisibleSize().width ||
		visibleSize.height != Director::getInstance()->getVisibleSize().height)
	{
		visibleSize = Director::getInstance()->getVisibleSize();
	}
	// use fixed time and calculate 3 times per frame makes physics simulate more precisely.
	if (Basketball->getPhysicsBody()->isGravityEnabled() == true) {
		m_world->setAutoStep(false);
		for (int i = 0; i < 3; ++i)
		{
			m_world->step(1 / 180.0f);
		}
	}


	Shadow->setPosition(Basketball->getPositionX(), 15);
	if (Basketball->getPhysicsBody()->getVelocity() != Vec2(0, 0)) {
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
