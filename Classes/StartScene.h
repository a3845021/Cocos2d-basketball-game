#ifndef _STARTSCENE_H_
#define _STARTSCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(StartScene);

private:
	// 背景
	void addBackground();
	void addbgm();
	//预加载音乐
	void preloadmusic();
	// 按键
	void addPlayButton();
	void clickPlay(Ref* ref);
	void removePlayButton();
	void addMenuButton();
	void MenuFun(int menu);
	void quitMenuButton();
	void addAbout();
	void quitAboutClick(Ref* ref);
	void quitAbout();

	// 常用变量
	Size visibleSize;
	Vec2 origin;
};

#endif // !_STARTSCENE_H_