#include "GameScene.h"
#include "Constants.h"
#include <random>
#include <ctime>
#include <iostream>
#include <ctime>

Scene* GameScene::createScene() {
  return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SplashSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init() {
  // super init first
  if (!Scene::init()) {
     return false;
  }
  startGame = clock();
  score = 0;
  bonusCount = 0;
  wonLayer = new WonLayer(this);
  auto director = Director::getInstance();
  auto visibleSize = director->getVisibleSize();
  Vec2 origin = director->getVisibleOrigin();

  // add "GameScene" background 
  auto background = Sprite::create(GAME_BACKGROUND_PATH);
  if ( background == nullptr) {
    problemLoading("game components");
    return false;
  }
  // position the sprite on the center of the screen
  scale = visibleSize.width / background->getContentSize().width;
  background->setScale(scale);
  background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
  // add the sprite as a child to this layer
  this->addChild(background);

  auto toMenuItem = Button::create(GAME_BUTTON_EXIT_REST, GAME_BUTTON_EXIT_PRESSED, GAME_BUTTON_EXIT_REST);
  toMenuItem->addClickEventListener(CC_CALLBACK_0(GameScene::ToMainMenuCallback,this));
  toMenuItem->setAnchorPoint(Vec2(1.0f, 1.0f));
  background->addChild(toMenuItem);
  toMenuItem->setPosition(background->getContentSize());

  InitLeftBar(Vec2(origin.x, origin.y), Size(visibleSize.width * (float)(7.0 / 33.0), visibleSize.height));
  InitGameField();
  InitEventListener();

  // now State is play
  gameState = GameState::GAME_STATE_PLAYING;
  currentSelectedBlock = { -1,-1 };

  if (ActivateField()) {
    FallGemses();
    gameState = GameState::GAME_STATE_FALLING_BLOCK;
  }
  this->scheduleUpdate();
  return true;
}

void GameScene::InitLeftBar(Vec2 position, Size size) {
  // create a left bar of scene (frame of window)
  auto leftBar = Node::create();
  leftBar->setPosition(position);
  // 7.0/33.0 - part used for free space at the edges of the playing field
  leftBar->setContentSize(size);
  this->addChild(leftBar);

  // create a label config and add labels to the left bar
  TTFConfig labelConfig;
  labelConfig.fontFilePath = FONT_PATH;
  labelConfig.fontSize = 100 * scale;

  auto labelTime = Label::createWithTTF(labelConfig, "Time:");
  labelTime->setColor(Color3B(78, 104, 191));
  auto labelScore = Label::createWithTTF(labelConfig, "Score:");
  labelScore->setColor(Color3B(78, 104, 191));
  auto timeVal = Label::createWithTTF(labelConfig, "k");
  timeVal->setColor(Color3B(78, 104, 191));
  timeVal->setName("Time");
  auto scoreVal = Label::createWithTTF(labelConfig, "k");
  scoreVal->setColor(Color3B(78, 104, 191));
  scoreVal->setName("Score");

  leftBar->addChild(timeVal);
  leftBar->addChild(scoreVal);
  leftBar->addChild(labelTime);
  leftBar->addChild(labelScore);
  leftBar->setName("Left bar");

  labelTime->setPosition(Vec2(leftBar->getContentSize().width * 0.5f, leftBar->getContentSize().height * 0.8f));
  labelScore->setPosition(Vec2(leftBar->getContentSize().width * 0.5f, leftBar->getContentSize().height * 0.55f));
  timeVal->setPosition(Vec2(leftBar->getContentSize().width * 0.5f, leftBar->getContentSize().height * 0.7f));
  scoreVal->setPosition(Vec2(leftBar->getContentSize().width * 0.5f, leftBar->getContentSize().height * 0.45f));
}

void GameScene::InitEventListener() {
  auto evListener = EventListenerMouse::create();
  evListener->setEnabled(true);
  evListener->onMouseDown = CC_CALLBACK_1(GameScene::OnClickBegan, this);
  evListener->onMouseMove = CC_CALLBACK_1(GameScene::OnClickMoved, this);
  evListener->onMouseUp = CC_CALLBACK_1(GameScene::OnClickEnded, this);
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(evListener, this);
}

void GameScene::InitGameField() {
  gameField = std::vector<std::vector<Block*>>(GAME_FIELD_ROWS, std::vector<Block*>(GAME_FIELD_COLS, nullptr));
  srand((unsigned int)time(nullptr));
  float curScale = scale * GAME_FIELD_SIZE_W / GAME_FIELD_COLS / GEMS_ATLAS_ELEMENT_SIZE;
  // set blocked blocks on the field
  int numOfBlocked = (rand() % (int)(GAME_FIELD_ROWS * GAME_FIELD_COLS * GAME_BLOCKED_PROBABILITY)) + 1;
  for (int k = 0; k < numOfBlocked; ++k) {
    while (true) {
      int i = rand() % GAME_FIELD_ROWS, j = rand() % GAME_FIELD_COLS;
        if (gameField[i][j] == nullptr) {
          int model = rand() % GEMS_COLOUR_NUMBER;
          gameField[i][j] = new Block(model, BLOCK_TYPE_BLOCKED, getFieldElemPos(i, j), this, curScale);
          break;
      }
    }
  }
  // set simple blocks on the field
  for (int i = 0; i < GAME_FIELD_ROWS; ++i) {
    for (int j = 0; j < GAME_FIELD_COLS; ++j) {
      if (gameField[i][j] == nullptr) {
        int model = rand() % GEMS_COLOUR_NUMBER;
        gameField[i][j] = new Block(model, BLOCK_TYPE_SIMPLE, getFieldElemPos(i, j), this, curScale);
      }
    }
  }
}
