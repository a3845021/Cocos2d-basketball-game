#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld* world);

    static cocos2d::Scene* createScene();

    virtual bool init(PhysicsWorld* world);

	static HelloWorld* create(PhysicsWorld* world);

private:
	Sprite* Basketball;
	Sprite* Arrow;
	Sprite* Support;
	Sprite* Board;
	Sprite* Hoop;
	Sprite* Hoop1;
	Sprite* Hoop2;
	Sprite* Judge;
	Sprite* Shadow;
	PhysicsWorld* m_world;
	Size visibleSize;
	bool Isthrow;
	bool IsthrowOver;
	bool Isrotate;
	Vec2 StartPos;
	Vec2 FinalPos;
	Vec2 MovingPos;
	//加边框
	void addEdge();
	//创建篮球
	void addBall();
	//创建箭头
	void addArrow();
	//创建球场
	void addPlayground();
	//创建阴影
	void addShadow();

	//监听器
	void addTouchListener();
	void addMouseListener();
	void addContactListener();
	//背景
	void addBackground();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	bool onConcactBegan(PhysicsContact& contact);
	bool onContactPreSolve(PhysicsContact& contact);
	bool onContactSeprate(PhysicsContact& contact);
	
	void onMouseMove(Event* event);

	void update(float f);
	//音乐
	void preloadmusic();
	void addbgm();
};

#endif // __HELLOWORLD_SCENE_H__
