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
	//�ӱ߿�
	void addEdge();
	//��������
	void addBall();
	//������ͷ
	void addArrow();
	//������
	void addPlayground();
	//������Ӱ
	void addShadow();

	//������
	void addTouchListener();
	void addMouseListener();
	void addContactListener();
	//����
	void addBackground();
	//����ʱ�������
	void schedule_use();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	bool onConcactBegan(PhysicsContact& contact);
	bool onContactPreSolve(PhysicsContact& contact);
	bool onContactSeprate(PhysicsContact& contact);

	void onMouseMove(Event* event);

	void update(float f);
	//����
	void preloadmusic();
	void addvoice(string name);
	//ʱ�������
	void update_time(float t);
	//label���
	void label_scale(Label* s);
	//���뵭��
	void fade(float fadein, float fadeout, Label* s);
	bool istimeover,isbegin;

	void turn_yellow(Label *y);
	void turn_white(Label *w);
};

#endif // __HELLOWORLD_SCENE_H__
