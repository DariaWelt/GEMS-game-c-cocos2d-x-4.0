#include "WonLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

WonLayer::WonLayer(cocos2d::Node* parent) {

}
void WonLayer::ShowWin(cocos2d::Node* parent) {
  auto director = Director::getInstance();
  auto visibleSize = director->getVisibleSize();
  Vec2 origin = director->getVisibleOrigin();

  // add "GameScene" background 
  auto background = Sprite::create(GAME_WON_BACKGROUND_PATH);
  // position the sprite on the center of the screen
  float scale = visibleSize.width / background->getContentSize().width;
  background->setScale(scale);
  background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
  background->setOpacity(200);
  // add the sprite as a child to this layer
  parent->addChild(background);

  auto mainContent = Sprite::create(GAME_WON_MAIN_COMPONENT_PATH);
  mainContent->setPosition(background->getContentSize().width * 0.5f, background->getContentSize().height * 0.5f);

  auto retryItem = MenuItemImage::create(GAME_WON_RETRY_BUTTON_REST_PATH, GAME_WON_RETRY_BUTTON_PRESSED_PATH, CC_CALLBACK_1(WonLayer::Retry, this));
  auto homeItem = MenuItemImage::create(GAME_WON_TOMENU_BUTTON_REST_PATH, GAME_WON_TOMENU_BUTTON_PRESSED_PATH, CC_CALLBACK_1(WonLayer::Home, this));

  retryItem->setPosition(Vec2(mainContent->getContentSize().width * -0.088f, mainContent->getContentSize().height * -0.32f));
  homeItem->setPosition(Vec2(mainContent->getContentSize().width * 0.12f, mainContent->getContentSize().height * -0.32f));
  retryItem->setScale(1.2f);
  homeItem->setScale(1.2f);

  auto menu = Menu::create(retryItem, homeItem, nullptr);
  menu->setPosition(Vec2(background->getContentSize().width * 0.5f, background->getContentSize().height * 0.5f));
  background->addChild(mainContent);
  mainContent->addChild(menu);

  TTFConfig labelConfig;
  labelConfig.fontFilePath = FONT_PATH;
  labelConfig.fontSize = 100.0f;
  auto scoreVal = Label::createWithTTF(labelConfig, "k");
  scoreVal->setName("Final score label");
  mainContent->setName("Won bolder");
  background->setName("Won layer");
  mainContent->addChild(scoreVal);
  scoreVal->setPosition(mainContent->getContentSize().width * 0.51f, mainContent->getContentSize().height * 0.36f);
}

void WonLayer::Retry(cocos2d::Ref* pSender) {
  auto scene = GameScene::createScene();
  auto transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene, Color3B::WHITE);
  Director::getInstance()->replaceScene(transition);
}
void WonLayer::Home(cocos2d::Ref* pSender) {
  auto scene = MainMenuScene::createScene();
  auto transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene, Color3B::WHITE);
  Director::getInstance()->replaceScene(transition);
}