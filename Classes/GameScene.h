#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Block.h"
#include <utility>
#include "WonLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class GameScene : public cocos2d::Scene {
public:
  static cocos2d::Scene* createScene();

  virtual bool init() override;
  void update(float dt) override;

  // a selector callback
  void MenuCloseCallback(cocos2d::Ref* pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(GameScene);


private:
  void ToMainMenuCallback();

  bool OnClickBegan(EventMouse* event);
  void OnClickMoved(EventMouse* event);
  void OnClickEnded(EventMouse* event);

  void InitLeftBar(cocos2d::Vec2 position, cocos2d::Size size);
  void InitEventListener();
  void InitGameField();
  cocos2d::Point getFieldElemPos(int i, int j);

  std::pair<int, int> GetFieldElemIndex(cocos2d::Point point);

  void UpdateTextFields();
  void MoveBlocks();
  void RandomBonusCreate(int i, int j);
  void SwapBlocks(int firstRow, int firstCol, int secRow, int secCol);
  void TryToMoveBlocks(std::pair<int, int> first, std::pair<int, int> second);
  bool CheckWin();
  void UpdateFallingState();
  bool ActivateField();
  void DeleteGemses(std::vector<cocos2d::Point>& forDelit);
  void FallGemses();
  void FieldCrawl(int i, int j, std::vector<cocos2d::Point>& forDelit, std::vector<std::vector<bool>>& isChecked);
  void EndGame();

  //cocos2d::Node gameField[GAME_FIELD_ROWS][GAME_FIELD_ROWS];
  std::vector<std::vector<Block*>> gameField;
  std::pair<int, int> currentSelectedBlock;
  float scale;
  int startGame;
  int score;
  int bonusCount;
  GameState gameState;
  WonLayer* wonLayer;

};