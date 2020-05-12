#pragma once

#include "cocos2d.h"
#include "Constants.h"
/**
		@brief  implements a GEMS object
		*/
class Block {
protected:
	// shared ptr!!
	cocos2d::Sprite* effect;
	cocos2d::Sprite* icon;
	int iconType;
	int effectType;
	cocos2d::Point position; // low left point
	const float scale;
	bool isActivated;
	
	/**
		@brief  Updates the field "icon"

		loads the block image depending on the current icon type value
		*/
	void loadIcon();
	/**
		@brief  Updates the field "effect"

		loads the block image depending on the current effect type value
		*/
	void loadEffect();
	/**
		@brief  Updates all field of object
		@param modelKey - int value of model type (color type) (max - 6)
		@param typeKey - int value of behavior type(BlockBehaviorType enum) (max - 5)
		@param pos - position of this block on the screen
		@param scale - size increase indicator
		*/
	void UpdateFields(int modelKey, int typeKey, cocos2d::Point pos, float scale);

public:
	Block(const Block& block);
	Block& operator=(const Block& block);
	/**
		@brief Create Block object (constructor)
		@param modelKey - int value of model type (color type) (max - 6)
		@param typeKey - int value of behavior type(BlockBehaviorType enum) (max - 5)
		@param pos - position of this block on the screen
		@param parent - the node to which this block wil be attached
		@param scale - size increase indicator
		*/
	Block(int modelKey, int typeKey, cocos2d::Point pos, cocos2d::Node* parent, float scale);
	~Block();
	
	/**
		@brief  Update model(color) type and imagination 
		@param modelKey - int value of model type (color type) (max - 6)
		*/
	void setIcon(int modelKey);
	/**
		@brief  Update effect(behaviour) type and imagination 
		@param typeKey - int value of behavior type(BlockBehaviorType enum) (max - 5)
		*/
	void setEffect(int typeKey);
	/**
		@brief Update position of this block
		@param newPosition - new position of this block on the screen
		*/
	void setPosition(cocos2d::Point newPosition);

	/**
		@return position of this block on the screen
		*/
	cocos2d::Point getPosition() const;
	/**
		@brief Check if this block was activated (for example bomb bonus)
		@return true - block was activated
		@return false - block was not activated 
		*/
	bool IsActivated() const;
	/**
		@brief Check if the moving action is done
		@return true - moving action is done
		@return false - moving action now is doing
		*/
	bool MovingIsDone();
	/**
		@brief Set move action on this block
		@param pos - position to which this block should be moved
		*/
	void Move(cocos2d::Point pos);
	/**
		@brief Implement special behavior of this type of block
		@param i - row on the game field
		@param j - column on the game field
		@param gameField - game field with this block and others blocks to which this will be affected
		*/
	virtual void Activate(int i, int j, std::vector<std::vector<Block*>>& gameField);
	/**
		@return int value of model type (color type) (max - 6)
		*/
	int getModelType() const;
	/**
		@return int value of behavior type(BlockBehaviorType enum) (max - 5)
		*/
	int getBahaviorType() const;
	/**
		@return scale factor of this block
		*/
	float getScale() const;
};

/**
		@brief  implements a bomb bonus
		*/
class BombBlock : public Block {
private:
	const static int bonusPower = 5;
public:
	void Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) override;
	BombBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale);
};

/**
		@brief  implements a repaint bonus
		*/
class RepaintBlock : public Block {
public:
	void Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) override;
	RepaintBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale);
};

/**
		@brief  implements a delite horizontal blockes bonus
		*/
class HorizontalBlock : public Block {
public:
	void Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) override;
	HorizontalBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale);
};

/**
		@brief  implements a delite vertical blockes bonus
		*/
class VerticalBlock : public Block {
public:
	void Activate(int i, int j, std::vector<std::vector<Block*>>& gameField) override;
	VerticalBlock(int modelKey, cocos2d::Point pos, cocos2d::Node* parent, float scale);
};
