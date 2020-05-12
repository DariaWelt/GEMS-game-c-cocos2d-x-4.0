#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
/**
@brief    The start game screen with Play, Info and Exit buttons
*/
class MainMenuScene : public cocos2d::Scene {
  bool isInfoScreen;
  float scale;

  /**
    @brief  Add the game info text as a child to parent node
    @param  the Layer to wich text will be attached
    */
  void SetInfoText(Node* parent);
  /**
   @brief  Called when user click a info button
   */
  void GameInfoCallback(cocos2d::Ref* pSender);
  /**
   @brief  Called when user click a exit button on the info table
   */
  void InfoCloseCallback();
public:
  static cocos2d::Scene* createScene();
  /**
   @brief  Initialization of the main menu scene with background and main combonents (buttons, lables)
   */
  virtual bool init();

  void menuCloseCallback(cocos2d::Ref* pSender);
  CREATE_FUNC(MainMenuScene);
};