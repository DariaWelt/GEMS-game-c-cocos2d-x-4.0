#include "MainMenuScene.h"
#include "Constants.h"
#include "GameScene.h"

Scene* MainMenuScene::createScene() {
  return MainMenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SplashSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenuScene::init() {
  // super init first
  isInfoScreen = false;
  if (!Scene::init()) {
    return false;
  }
  auto director = Director::getInstance();
  auto visibleSize = director->getVisibleSize();
  Vec2 origin = director->getVisibleOrigin();

  // add "MainMenuScene" background and title
  auto background = Sprite::create(MAIN_MENU_BACKGROUND_PATH);
  auto title = Sprite::create(MAIN_MENU_TITLE_PATH);
  if ( background == nullptr || title == nullptr) {
    problemLoading("main menu components");
    return false;
  }
  // position the sprite on the center of the screen
  scale = visibleSize.width / background->getContentSize().width;
  background->setScale(scale);
  title->setScale(scale);
  background->setPosition(Vec2(visibleSize.width / 2.0f + origin.x, visibleSize.height / 2.0f + origin.y));
  title->setPosition(Vec2(visibleSize.width / 2.0f + origin.x, visibleSize.height * 0.83f + origin.y));
  // add the sprite as a child to this layer
  this->addChild(background);
  this->addChild(title);

  // add a menu 
  auto playItem = MenuItemImage::create(MAIN_MENU_START_BUTTON_REST_PATH, MAIN_MENU_START_BUTTON_PRESSED_PATH, [&](Ref* sender) {
    if (isInfoScreen) {
      return;
    }
    auto scene = GameScene::createScene();
    // transition stage between scenes
    auto transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene, Color3B::WHITE);
    Director::getInstance()->replaceScene(transition);
    });
  playItem->setScale(scale);

  // add a exit botton and info button
  auto exitItem = MenuItemImage::create(GAME_BUTTON_EXIT_REST, GAME_BUTTON_EXIT_PRESSED, CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
  auto infoItem = MenuItemImage::create(GAME_BUTTON_INFO_REST, GAME_BUTTON_INFO_PRESSED, CC_CALLBACK_1(MainMenuScene::GameInfoCallback, this));
  exitItem->setScale(0.7f);
  infoItem->setScale(0.7f);
  exitItem->setPosition(visibleSize.width * 0.42f, visibleSize.height * - 0.3f);
  infoItem->setPosition(visibleSize.width * -0.42f, visibleSize.height * -0.3f);

  auto menu = Menu::create(playItem, exitItem, infoItem, nullptr);
  menu->setPosition(visibleSize.width / 2.0f + origin.x, visibleSize.height * 0.4f + origin.y);
  this->addChild(menu);

  return true;
}

void MainMenuScene::menuCloseCallback(Ref* pSender) {
  if (!isInfoScreen) {
    Director::getInstance()->end();
  }
}

void MainMenuScene::InfoCloseCallback() {
  isInfoScreen = false;
  Node* infoNode = this->getChildByName("Info");
  if (infoNode != nullptr) {
    infoNode->removeAllChildrenWithCleanup(true);
    this->removeChildByName("Info");
  }
}

void MainMenuScene::SetInfoText(Node* parent) {
  TTFConfig labelConfig;
  labelConfig.fontFilePath = FONT_PATH;
  labelConfig.fontSize = 30.0f;
  std::string text = "In the game you can move two neighboring blocks in order to collect three or more ";
  text.append("identical blocks next to each other for removing them.Blocked blocks with a dark ");
  text.append("background cannot be moved. The goal of the game is to remove all blocked blocks. P.S.");
  text.append("Most of the design resources are taken from the site freepik.com");
  auto infoText = TextField::create(text, FONT_PATH, 25);
  infoText->setEnabled(false);
  infoText->setPosition(parent->getContentSize() * 0.5);
  infoText->ignoreContentAdaptWithSize(false);
  infoText->setTextAreaSize(parent->getContentSize() * 0.7f);
  infoText->setColor(Color3B::WHITE);
  infoText->setPlaceHolderColor(Color4B::WHITE);
  parent->addChild(infoText);

  auto freepikImage = Sprite::create(GAME_INFO_DESIGN_PROVIDER_PATH);
  freepikImage->setPosition(parent->getContentSize().width * 0.7f, parent->getContentSize().height * 0.2f);
  parent->addChild(freepikImage);
}

void MainMenuScene::GameInfoCallback(cocos2d::Ref* pSender) {
  if (!isInfoScreen) {
    isInfoScreen = true;

    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();
    auto infoLayer = Sprite::create(GAME_INFO_BACKGROUND_PATH);
    infoLayer->setName("Info");
    infoLayer->setPosition(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y);  
    SetInfoText(infoLayer);
    auto toMenuItem = Button::create(GAME_BUTTON_EXIT_REST, GAME_BUTTON_EXIT_PRESSED, GAME_BUTTON_EXIT_REST);
    toMenuItem->addClickEventListener(CC_CALLBACK_0(MainMenuScene::InfoCloseCallback, this));
    toMenuItem->setAnchorPoint(Vec2(1.0f, 1.0f));
    toMenuItem->setScale(0.5f);
    infoLayer->addChild(toMenuItem);
    toMenuItem->setPosition(infoLayer->getContentSize());

    this->addChild(infoLayer);
  }
}