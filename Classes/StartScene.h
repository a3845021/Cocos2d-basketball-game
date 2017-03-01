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
	// ����
	void addBackground();
	void addbgm();
	//Ԥ��������
	void preloadmusic();
	// ����
	void addPlayButton();
	void clickPlay(Ref* ref);
	void removePlayButton();
	void addMenuButton();
	void MenuFun(int menu);
	void quitMenuButton();
	void addAbout();
	void quitAboutClick(Ref* ref);
	void quitAbout();

	// ���ñ���
	Size visibleSize;
	Vec2 origin;
};

#endif // !_STARTSCENE_H_