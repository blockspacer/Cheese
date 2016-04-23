#include "scene/IntroScene.h"
#include "scene/MenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "luacppinterface/LuaCppInterface/luacppinterface.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
USING_NS_CC;

using namespace cocostudio::timeline;

void IntroScene::testLua() {
	Lua lua;
	lua.LoadStandardLibraries();
	
	std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("config.lua");
	CCLOG("the path is %s", path);
	std::ifstream file(path);
	if (!file.fail()) {
		std::string script((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		lua.RunScript(script);
		auto global = lua.GetGlobalEnvironment();
		std::string st = global.Get<std::string>("windowTitle");
		CCLOG("copying string");
		std::stringstream ss;
		ss << "the string is " << st << " oo" << std::endl;
		CCLOG("the reading is %s", ss.str().c_str());
		CCLOG("finished");
	}
	else {
		CCLOG("open file failed");
	}
	
}
Scene* IntroScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = IntroScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool IntroScene::init()
{
	/**  you can create scene with following comment code instead of using csb file.
	// 1. super init first
	if ( !Layer::init() )
	{
	return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
	"CloseNormal.png",
	"CloseSelected.png",
	CC_CALLBACK_1(IntroScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
	origin.y + closeItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width/2,
	origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "IntroScene" splash screen"
	auto sprite = Sprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
	**/

	//////////////////////////////
	// 1. super init first
	//test out lua
	testLua();
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("IntroScene.csb");

	addChild(rootNode);
	this->scheduleOnce(schedule_selector(IntroScene::transit), 1.0f);
	
	//
	return true;
}

void IntroScene::transit(float dt) {
	auto menuScene = MenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, (Scene*)menuScene, Color3B::BLACK));
	
}