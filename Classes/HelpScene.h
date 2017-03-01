#ifndef _HELOSCENE_H_
#define _HELOSCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class HelpScene : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelpScene);

private:
	void addBackground();

	void addQuitButton();
	void addLeftButton();
	void addRightButton();
	void addHitPic();
	void setPicture();
	void leftClick(Ref * ref);
	void rightClick(Ref * ref);
	void BackToStart(Ref* ref);


	// 常用变量
	Size visibleSize;
	Vec2 origin;
	int hitNum;
};

#endif // _HELOSCENE_H_