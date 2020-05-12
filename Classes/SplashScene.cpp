/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "SplashScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* SplashScene::createScene() {
  return SplashScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SplashSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SplashScene::init() {
  // super init first
  if ( !Scene::init() ) {
    return false;
  }
  auto director = Director::getInstance();
  auto visibleSize = director->getVisibleSize();
  Vec2 origin = director->getVisibleOrigin();

  // add "SplashScene" splash screen
  auto sprite = Sprite::create(SPLASH_SCENE_BACKGROUND_PATH);
  if (sprite == nullptr) {
    problemLoading(SPLASH_SCENE_BACKGROUND_PATH);
    return false;
  }
  // position the sprite on the center of the screen
  sprite->setScale(visibleSize.width/ sprite->getContentSize().width);
  sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

  // add the sprite as a child to this layer
  this->addChild(sprite, 0);
  // one-time tansition from the splash screen to the menu screen
  scheduleOnce([](float dt) {
      auto scene = MainMenuScene::createScene();
      // transition stage between scenes
      auto transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene, Color3B::WHITE);
      Director::getInstance()->replaceScene(transition);
    },
    SPLASH_SCENE_SHOW_TIME, "SplashToMenu");
  return true;
}


void SplashScene::menuCloseCallback(Ref* pSender) {
  //Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

  /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

  //EventCustom customEndEvent("game_scene_close_event");
  //_eventDispatcher->dispatchEvent(&customEndEvent);

}
