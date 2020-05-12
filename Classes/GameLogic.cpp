#include "GameScene.h"
#include "Constants.h"
#include <random>
#include <ctime>
#include <iostream>
#include <cmath>
#include "MainMenuScene.h"


void GameScene::MenuCloseCallback(Ref* pSender) {
  //Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();
}

bool GameScene::OnClickBegan(EventMouse* event) {
  return true;
}

void GameScene::OnClickMoved(EventMouse* event) {

}

void GameScene::OnClickEnded(EventMouse* event) {
  if (gameState == GAME_STATE_PLAYING) {
    std::pair<int, int> elemIndexes = GetFieldElemIndex(event->getLocationInView());
    if (elemIndexes.first != -1) {
      // 2 cases: choise first block, choise second block
      if (currentSelectedBlock.first == -1) {
        currentSelectedBlock = elemIndexes;
        FILE* fout;
        fout = fopen("test.txt", "a");
        fprintf(fout, "current Selected Block setted: %i %i\n", elemIndexes.first, elemIndexes.second);
        fclose(fout);
      }
      else {
        TryToMoveBlocks(currentSelectedBlock, elemIndexes);
        currentSelectedBlock = { -1,-1 };
        FILE* fout;
        fout = fopen("test.txt", "a");
        fprintf(fout, "current Selected Block deleted\n");
        fclose(fout);
      }
    }
  }
}

void GameScene::SwapBlocks(int firstRow, int firstCol, int secRow, int secCol) {
  // change the blocks positions
  gameField[firstRow][firstCol]->setPosition(getFieldElemPos(secRow, secCol));
  gameField[secRow][secCol]->setPosition(getFieldElemPos(firstRow, firstCol));

  // change places of blocks on the game grid
  Block* tmp = gameField[firstRow][firstCol];
  gameField[firstRow][firstCol] = gameField[secRow][secCol];
  gameField[secRow][secCol] = tmp;
}

void GameScene::TryToMoveBlocks(std::pair<int, int> block1, std::pair<int, int> block2) {

    // if the blocks are not on the one line
  if ((block1.first != block2.first && block1.second != block2.second) ||

    // if they are not neighbours
    -abs(block1.first - block2.first) < -1 || -abs(block1.second - block2.second) < -1 ||

    // if there is blocked block
    gameField[block1.first][block1.second]->getBahaviorType() == BLOCK_TYPE_BLOCKED ||
    gameField[block2.first][block2.second]->getBahaviorType() == BLOCK_TYPE_BLOCKED) {
    return;
  }
  
  SwapBlocks(block1.first, block1.second, block2.first, block2.second);
  if (ActivateField()) {
    FallGemses();
    gameState = GAME_STATE_FALLING_BLOCK;
  }
  else {
    SwapBlocks(block1.first, block1.second, block2.first, block2.second);
  }
}

// return screen position of block on gameField[i][j] place
cocos2d::Point GameScene::getFieldElemPos(int i, int j) {
  if (i >= GAME_FIELD_ROWS || j >= GAME_FIELD_COLS) {
    // throw ...  
    return Point(-1,-1);
  }
  return Point(GAME_FIELD_SIZE_W / GAME_FIELD_COLS * j + GAME_FIELD_POS_X,
   GAME_FIELD_SIZE_H / GAME_FIELD_ROWS * (GAME_FIELD_ROWS - i - 1) + GAME_FIELD_POS_Y) * scale;
}

// return indexes {i, j} of grid with blocks or {-1, -1}
std::pair<int, int> GameScene::GetFieldElemIndex(cocos2d::Point point) {
  float x = point.x - GAME_FIELD_POS_X * scale, y = point.y - GAME_FIELD_POS_Y * scale;
  if (x > GAME_FIELD_SIZE_W * scale || y > GAME_FIELD_SIZE_H * scale || x < 0 || y < 0) {
    return { -1,-1 };
  }
  else {
    int j = (int)floor(x / (GAME_FIELD_SIZE_W  * scale) * GAME_FIELD_COLS) , i = GAME_FIELD_ROWS - 1 - (int)floor(y / (GAME_FIELD_SIZE_H * scale) * GAME_FIELD_ROWS );
    return { i, j };
  }
}

bool GameScene::ActivateField() {
  std::vector<Point> forDelit;
  std::vector<std::vector<bool>> isChecked(GAME_FIELD_ROWS, std::vector<bool>(GAME_FIELD_COLS, false));
  for (int i = 0; i < GAME_FIELD_ROWS; ++i) {
    for (int j = 0; j < GAME_FIELD_COLS; ++j) {
      int startSize = forDelit.size();
      FieldCrawl(i, j, forDelit, isChecked);
      int combinationSize = forDelit.size() - startSize;
      if (combinationSize < 3) {
        forDelit.erase(forDelit.begin() + startSize, forDelit.end());
      }
    }
  }
  if (forDelit.empty()) {
    return false;
  }
  else {
    score += GAME_SCORE_KOEF * forDelit.size();
    bonusCount = (rand() % 2) * (int)floor(forDelit.size() / 5);
    DeleteGemses(forDelit);
    return true;
  }
}

void GameScene::DeleteGemses(std::vector<cocos2d::Point>& forDelit) {
  int toBonusShift = forDelit.size() / (bonusCount + 1);
  for (int i = 0; i < (int)forDelit.size(); ++i) {
    if (gameField[(int)forDelit[i].y][(int)forDelit[i].x] != nullptr) {
      gameField[(int)forDelit[i].y][(int)forDelit[i].x]->Activate((int)forDelit[i].y, (int)forDelit[i].x, gameField);
    }
    delete(gameField[(int)forDelit[i].y][(int)forDelit[i].x]);
    gameField[(int)forDelit[i].y][(int)forDelit[i].x] = nullptr;
    if (i > 0 && i % toBonusShift == 0)
      RandomBonusCreate((int)forDelit[i].y, (int)forDelit[i].x);
  }
  bonusCount = 0;
}

void GameScene::RandomBonusCreate(int i, int j) {
  float curScale = scale * GAME_FIELD_SIZE_W / GAME_FIELD_COLS / GEMS_ATLAS_ELEMENT_SIZE;
  int effectType = rand() % GEMS_BONUS_NUMBER + (int)BLOCK_TYPE_BONUS_BOMB;
  int modelType = rand() % GEMS_COLOUR_NUMBER;
  switch (effectType) {
  case BLOCK_TYPE_BONUS_BOMB:
    gameField[i][j] = (Block*) new BombBlock(modelType, getFieldElemPos(i, j), this, curScale);
    break;
  case BLOCK_TYPE_BONUS_HORIZ_DEL:
    gameField[i][j] = (Block*) new HorizontalBlock(modelType, getFieldElemPos(i, j), this, curScale);
    break;
  case BLOCK_TYPE_BONUS_REPAINT:
    gameField[i][j] = (Block*) new RepaintBlock(modelType, getFieldElemPos(i, j), this, curScale);
    break;
  case BLOCK_TYPE_BONUS_VERTIC_DEL:
    gameField[i][j] = (Block*) new VerticalBlock(modelType, getFieldElemPos(i, j), this, curScale);
    break;
  }
}

void GameScene::MoveBlocks() {
  // set move action to blocks
  for (int i = GAME_FIELD_ROWS - 1; i >= 0; --i) {
    for (int j = 0; j < GAME_FIELD_COLS; ++j) {
      if (gameField[i][j]->getPosition() != getFieldElemPos(i, j)) {
        gameField[i][j]->Move(getFieldElemPos(i, j));
      }
    }
  }
}

void GameScene::FallGemses() {
  // NULL cells "rise to surface"
  for (int i = GAME_FIELD_ROWS - 1; i > 0; --i) {
    for (int j = 0; j < GAME_FIELD_COLS; ++j) {
      if (gameField[i][j] == nullptr) {
        int k = i;
        while (k > 0 && gameField[k - 1][j] == nullptr) {
          --k;
        }
        if (k != 0) {
          --k;
        }
        std::swap(gameField[i][j], gameField[k][j]);
      }
    }
  }

  // fill the NULL cells in game field
  float curScale = scale * GAME_FIELD_SIZE_W / GAME_FIELD_COLS / GEMS_ATLAS_ELEMENT_SIZE;
  for (int j = 0; j < GAME_FIELD_COLS; ++j) {
    if (gameField[0][j] == nullptr) {
      int count = 0;
      for (count; (count + 1) < GAME_FIELD_ROWS && gameField[count + 1][j] == nullptr; ++count);
      ++count;
      // create "count" amount blocks above game field
      for (int i = 0; i < count; ++i) {
        Point pos = Point(GAME_FIELD_POS_X + j * GAME_FIELD_SIZE_W / GAME_FIELD_COLS, GAME_FIELD_POS_Y + GAME_FIELD_SIZE_H + i * GAME_FIELD_SIZE_H / GAME_FIELD_COLS) * scale;
        gameField[count - 1 - i][j] = new Block(rand() % GEMS_COLOUR_NUMBER, BLOCK_TYPE_SIMPLE, pos, this, curScale);
      }
    }
  }

  MoveBlocks();
}

void GameScene::FieldCrawl(int i, int j, std::vector<cocos2d::Point>& forDelit, std::vector<std::vector<bool>>& isChecked) {
  isChecked[i][j] = true;
  forDelit.push_back(Vec2((float)j, (float)i));
  if (i > 0 && isChecked[i - 1][j] == false && gameField[i][j]->getModelType() == gameField[i - 1][j]->getModelType()) {
    FieldCrawl(i - 1, j, forDelit, isChecked);
  }
  if (j > 0 && isChecked[i][j - 1] == false && gameField[i][j]->getModelType() == gameField[i][j - 1]->getModelType()) {
    FieldCrawl(i, j- 1, forDelit, isChecked);
  }
  if (i < GAME_FIELD_ROWS - 1 && isChecked[i + 1][j] == false && gameField[i][j]->getModelType() == gameField[i + 1][j]->getModelType()) {
    FieldCrawl(i + 1, j, forDelit, isChecked);
  }
  if (j < GAME_FIELD_COLS - 1 && isChecked[i][j + 1] == false && gameField[i][j]->getModelType() == gameField[i][j + 1]->getModelType()) {
    FieldCrawl(i, j + 1, forDelit, isChecked);
  }
}

bool GameScene::CheckWin() {
  for (int i = 0; i < GAME_FIELD_ROWS; ++i) {
    for (int j = 0; j < GAME_FIELD_COLS; ++j) {
      if (gameField[i][j]->getBahaviorType() == BLOCK_TYPE_BLOCKED)
        return false;
    }
  }
  gameState = GAME_STATE_WON;
  wonLayer->ShowWin(this);
  if (this->getChildByName("Won layer") != nullptr && 
    this->getChildByName("Won layer")->getChildByName("Won bolder") != nullptr &&
    getChildByName("Won layer")->getChildByName("Won bolder")->getChildByName("Final score label") != nullptr) {
    ((Label*)this->getChildByName("Won layer")->getChildByName("Won bolder")->getChildByName("Final score label"))->setString(std::to_string(score));
  }
  return true;
}

void GameScene::UpdateFallingState() {
  for (int j = 0; j < GAME_FIELD_COLS; ++j) {
    if (!(gameField[0][j]->MovingIsDone()))
      return;
  }

  // if all blocks falled
  if (ActivateField()) {
    FallGemses();
  }
  else if (!CheckWin()) {
    gameState = GAME_STATE_PLAYING;
  }
}

void GameScene::EndGame() {
  for (int i = 0; i < GAME_FIELD_ROWS; ++i) {
    for (int j = 0; j < GAME_FIELD_COLS; ++j) {
      delete(gameField[i][j]);
    }
  }

  if (gameState == GAME_STATE_WON) {
    auto scene = MainMenuScene::createScene();
    // transition stage between scenes
    auto transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene, Color3B::WHITE);
    Director::getInstance()->replaceScene(transition);
  }
}

void GameScene::UpdateTextFields() {
  auto timeVal = (Label*)this->getChildByName("Left bar")->getChildByName("Time");
  int time = (clock() - startGame) / CLOCKS_PER_SEC;
  std::string timeStr = std::to_string(time / 60);
  timeStr.append("m ");
  timeStr.append(std::to_string(time % 60));
  timeStr.append("s");
  timeVal->setString(timeStr);
  auto scoreVal = (Label*)this->getChildByName("Left bar")->getChildByName("Score");
  scoreVal->setString(std::to_string(score));
}

void GameScene::update(float dt) {
  if (gameState == GAME_STATE_PAUSED || gameState == GAME_STATE_WON) {

  }
  else {
    UpdateTextFields();
    if (gameState == GAME_STATE_FALLING_BLOCK) {
      UpdateFallingState();
    }
  }
}

void GameScene::ToMainMenuCallback() {
  if (this->gameState != GAME_STATE_WON) {
    auto transition = TransitionFade::create(SCENE_TRANSITION_TIME, MainMenuScene::createScene(), Color3B::WHITE);
    Director::getInstance()->replaceScene(transition);
  }
}