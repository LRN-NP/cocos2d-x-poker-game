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

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"

/**
 * 卡牌面值类型枚举
 */
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * 卡牌花色类型枚举
 */
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * 卡牌数据模型
 * 职责：存储卡牌的基础数据，包括面值、花色、位置等
 * 使用场景：游戏运行时存储卡牌状态，支持序列化用于存档
 */
class CardModel
{
public:
    /**
     * 构造函数
     */
    CardModel();
    
    /**
     * 构造函数
     * @param face 卡牌面值
     * @param suit 卡牌花色
     * @param position 卡牌位置
     */
    CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);
    
    /**
     * 析构函数
     */
    ~CardModel();
    
    /**
     * 获取卡牌面值
     * @return 卡牌面值
     */
    CardFaceType getFace() const { return _face; }
    
    /**
     * 获取卡牌花色
     * @return 卡牌花色
     */
    CardSuitType getSuit() const { return _suit; }
    
    /**
     * 获取卡牌位置
     * @return 卡牌位置
     */
    const cocos2d::Vec2& getPosition() const { return _position; }
    
    /**
     * 获取卡牌ID
     * @return 卡牌唯一ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * 设置卡牌ID
     * @param cardId 卡牌ID
     */
    void setCardId(int cardId) { _cardId = cardId; }
    
    /**
     * 获取卡牌是否翻开
     * @return 是否翻开
     */
    bool isRevealed() const { return _isRevealed; }
    
    /**
     * 获取卡牌是否可点击
     * @return 是否可点击
     */
    bool isClickable() const { return _isClickable; }
    
    /**
     * 设置卡牌面值
     * @param face 卡牌面值
     */
    void setFace(CardFaceType face) { _face = face; }
    
    /**
     * 设置卡牌花色
     * @param suit 卡牌花色
     */
    void setSuit(CardSuitType suit) { _suit = suit; }
    
    /**
     * 设置卡牌位置
     * @param position 卡牌位置
     */
    void setPosition(const cocos2d::Vec2& position) { _position = position; }
    
    /**
     * 设置卡牌是否翻开
     * @param revealed 是否翻开
     */
    void setRevealed(bool revealed) { _isRevealed = revealed; }
    
    /**
     * 设置卡牌是否可点击
     * @param clickable 是否可点击
     */
    void setClickable(bool clickable) { _isClickable = clickable; }
    
    /**
     * 检查卡牌是否有效
     * @return 卡牌是否有效
     */
    bool isValid() const;
    
    /**
     * 检查两张卡牌是否相邻（面值差1）
     * @param other 另一张卡牌
     * @return 是否相邻
     */
    bool isAdjacentTo(const CardModel& other) const;
    
    /**
     * 获取卡牌文本表示
     * @return 卡牌文本
     */
    std::string getCardText() const;
    
    /**
     * 序列化卡牌数据
     * @return 序列化后的数据
     */
    std::string serialize() const;
    
    /**
     * 反序列化卡牌数据
     * @param data 序列化数据
     * @return 是否成功
     */
    bool deserialize(const std::string& data);

private:
    static int _nextCardId; ///< 下一个卡牌ID
    
    int _cardId;                    ///< 卡牌唯一ID
    CardFaceType _face;             ///< 卡牌面值
    CardSuitType _suit;             ///< 卡牌花色
    cocos2d::Vec2 _position;        ///< 卡牌位置
    bool _isRevealed;               ///< 是否翻开
    bool _isClickable;              ///< 是否可点击
};

#endif // __CARD_MODEL_H__
