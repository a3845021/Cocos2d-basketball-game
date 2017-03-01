#ifndef _GAME1_H_
#define _GAME1_H_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class Game1 : public cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld* world);

	static cocos2d::Scene* createScene();

	virtual bool init(PhysicsWorld* world);

	static Game1* create(PhysicsWorld* world);

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
	bool Isphysic;
	Vec2 StartPos;
	Vec2 FinalPos;
	Vec2 MovingPos;
	int time1;
	Label* time_str,*score_str,*alert,*next,*adds;
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
	//调用时间调度器
	void schedule_use();
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
	void addvoice(string name);
	//时间调度器
	void update_time(float t);
	//label变大
	void label_scale(Label* s);
	//淡入淡出
	void fade(float fadein, float fadeout, Label* s);
	bool istimeover,isbegin;

	void turn_yellow(Label *y);
	void turn_white(Label *w);
};

#endif // __HELLOWORLD_SCENE_H__
