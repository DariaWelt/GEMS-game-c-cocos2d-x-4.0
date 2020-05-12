#include "Block.h"

USING_NS_CC;

void Block::loadIcon() {
	if (icon != nullptr) {
		icon->removeFromParentAndCleanup(true);
		icon = nullptr;
		effect = nullptr;
	}
	switch (iconType) {
	case 0:
		icon = Sprite::create(GEMS_ATLAS_PATH, Rect(0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case 1:
		icon = Sprite::create(GEMS_ATLAS_PATH, Rect(0, 0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case 2:
		icon = Sprite::create(GEMS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case 3:
		icon = Sprite::create(GEMS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE, 0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case 4:
		icon = Sprite::create(GEMS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE * 2, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case 5:
		icon = Sprite::create(GEMS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE * 2, 0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case 6:
		icon = Sprite::create(GEMS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE * 3, 0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	//default:
		// create an empty sprite
		icon = Sprite::create();
		// throw ...
		break;
	}
}

void Block::loadEffect() {
	if (effect != nullptr) {
		effect->removeFromParentAndCleanup(true);
		effect = nullptr;
	}
	switch (effectType)	{
	case BLOCK_TYPE_SIMPLE:
		effect = nullptr;
		break;
	case BLOCK_TYPE_BLOCKED:
		effect = Sprite::create(GEMS_EFFECTS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case BLOCK_TYPE_BONUS_BOMB:
		effect = Sprite::create(GEMS_EFFECTS_ATLAS_PATH, Rect(0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case BLOCK_TYPE_BONUS_REPAINT:
		effect = Sprite::create(GEMS_EFFECTS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE, 0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case BLOCK_TYPE_BONUS_HORIZ_DEL:
		effect = Sprite::create(GEMS_EFFECTS_ATLAS_PATH, Rect(GEMS_ATLAS_ELEMENT_SIZE * 2, 0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	case BLOCK_TYPE_BONUS_VERTIC_DEL:
		effect = Sprite::create(GEMS_EFFECTS_ATLAS_PATH, Rect(0, 0, GEMS_ATLAS_ELEMENT_SIZE, GEMS_ATLAS_ELEMENT_SIZE));
		break;
	default:
		break;
	}
}
void Block::UpdateFields(int modelKey, int typeKey, cocos2d::Point pos, float scale) {
	iconType = modelKey;
	effectType = typeKey;
	this->loadIcon();
	loadEffect();
	position = pos;
	icon->setPosition(pos);
	icon->setAnchorPoint(Vec2(0, 0));
	icon->setScale(scale);
	if (effect != nullptr) {
		effect->setPosition(icon->getContentSize().width * 0.5f, icon->getContentSize().height * 0.5f);
		icon->addChild(effect);
	}
}
Block::Block(int modelKey, int typeKey, cocos2d::Point pos, cocos2d::Node* parent, float scale): icon(nullptr), effect(nullptr), scale(scale) {
	UpdateFields(modelKey, typeKey, pos, scale);
	isActivated = false;
	parent->addChild(icon);
}

Block::Block(const Block& block): scale(block.getScale()) {
	auto parent = icon->getParent();
	UpdateFields(block.getModelType(), block.getBahaviorType(), block.getPosition(),scale);
	isActivated = block.IsActivated();
	parent->addChild(icon,1);
}

Block& Block::operator=(const Block& block) {
	auto parent = icon->getParent();
	UpdateFields(block.getModelType(), block.getBahaviorType(), block.getPosition(),scale);
	parent->addChild(icon,1);
	isActivated = block.IsActivated();
	return *this;
}

bool Block::IsActivated() const {
	return isActivated;
}

float Block::getScale() const {
	return scale;
}

void Block::setIcon(int modelKey) {
	auto parent = icon->getParent();
	UpdateFields(modelKey, this->effectType, this->position, this->scale);
	parent->addChild(icon);
}

void Block::setEffect(int typeKey) {
	effectType = typeKey;
	loadEffect();
	if (effect != nullptr) {
		effect->setPosition(icon->getContentSize().width * 0.5f, icon->getContentSize().height * 0.5f);
		icon->addChild(effect);
	}
}
void Block::setPosition(cocos2d::Point newPosition) {
	position = newPosition;
	icon->setPosition(position);
}
cocos2d::Point Block::getPosition() const {
	return position;
}

int Block::getModelType() const {
	return iconType;
}
int Block::getBahaviorType() const {
	return effectType;
}

Block::~Block() {
	if (icon != nullptr) {
		icon->removeFromParentAndCleanup(true);
		icon = nullptr;
		effect = nullptr;
	}
}

bool Block::MovingIsDone() {
	auto action = icon->getActionByTag(99);
	if (action == nullptr) {
		return true;
	}
	else if (action->isDone()) {
		return true;
	}
	return false;
}

void Block::Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) { isActivated = true; }

void Block::Move(cocos2d::Point pos) {
	if (pos.x != position.x)
		return;
	float time = abs(pos.y - position.y) / BLOCK_VELOCITY;
	auto moveTo = MoveTo::create(time, pos);
	position.y = pos.y;
	moveTo->setTag(99);

	icon->runAction(moveTo);
	//if (effect != nullptr) {
	//	effect->runAction(moveTo->clone());
	//}
}


void BombBlock::Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) {
	if (isActivated) {
		return;
	}
	isActivated = true;
	for (int k = 0; k < bonusPower; ++k) {
		while (true) {
			int pos_i = rand() % GAME_FIELD_ROWS, pos_j = rand() % GAME_FIELD_COLS;
			if (gameField[pos_i][pos_j] != nullptr && pos_i != i && pos_j != j) {
				gameField[pos_i][pos_j]->Activate(pos_i, pos_j, gameField);
				delete(gameField[pos_i][pos_j]);
				gameField[pos_i][pos_j] = nullptr;
				break;
			}
		}
	}
}
BombBlock::BombBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale): Block(modelKey, BLOCK_TYPE_BONUS_BOMB, pos, parent, scale) {}

void RepaintBlock::Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) {
	if (isActivated) {
		return;
	}
	isActivated = true;
	std::vector<Vec2> avaliable;
	if (i < GAME_FIELD_ROWS - 1 && j < GAME_FIELD_COLS - 1 && gameField[i + 1][j + 1] != nullptr)
		avaliable.push_back(Vec2((float)(i + 1), (float)(j + 1)));
	if (i < GAME_FIELD_ROWS - 1 && j > 0 && gameField[i + 1][j - 1] != nullptr)
		avaliable.push_back(Vec2((float)(i + 1), (float)(j - 1)));
	if (i > 0 && j > 0 && gameField[i - 1][j - 1] != nullptr)
		avaliable.push_back(Vec2((float)(i - 1), (float)(j - 1)));
	if (i > 0 && j < GAME_FIELD_COLS - 1 && gameField[i - 1][j + 1] != nullptr)
		avaliable.push_back(Vec2((float)(i - 1), (float)(j + 1)));

	if (avaliable.size() == 0)
		return;

	for (int k = 0; k < 2; ++k) {
		auto pos = avaliable[rand() % avaliable.size()];
		gameField[(int)pos.x][(int)pos.y]->setIcon(this->getModelType());
	}

}
RepaintBlock::RepaintBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale) : Block(modelKey, BLOCK_TYPE_BONUS_REPAINT, pos, parent, scale) {}

void HorizontalBlock::Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) {
	if (isActivated) {
		return;
	}
	isActivated = true;
	for (int k = 0; k < GAME_FIELD_COLS; ++k) {
		if (gameField[i][k] != nullptr && k != j) {
			gameField[i][k]->Activate(i, k, gameField);
			delete(gameField[i][k]);
			gameField[i][k] = nullptr;
		}
	}
}
HorizontalBlock::HorizontalBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale) : Block(modelKey, BLOCK_TYPE_BONUS_HORIZ_DEL, pos, parent, scale) {}

void VerticalBlock::Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) {
	if (isActivated) {
		return;
	}
	isActivated = true;
	for (int k = 0; k < GAME_FIELD_ROWS; ++k) {
		if (gameField[k][j] != nullptr && k != i) {
			gameField[k][j]->Activate(k, j, gameField);
			delete(gameField[k][j]);
			gameField[k][j] = nullptr;
		}
	}
	
}
VerticalBlock::VerticalBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale) : Block(modelKey, BLOCK_TYPE_BONUS_VERTIC_DEL, pos, parent, scale) {}