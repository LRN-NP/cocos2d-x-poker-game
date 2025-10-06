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

#ifndef __CARD_UTILS_H__
#define __CARD_UTILS_H__

#include "../models/CardModel.h"
#include "cocos2d.h"

/**
 * 卡牌工具类
 * 职责：提供卡牌相关的通用功能，不涉及业务逻辑
 * 使用场景：卡牌显示、文本转换、位置计算等通用功能
 */
class CardUtils
{
public:
    /**
     * 获取卡牌面值文本
     * @param face 卡牌面值
     * @return 面值文本
     */
    static std::string getFaceText(CardFaceType face);
    
    /**
     * 获取卡牌花色文本
     * @param suit 卡牌花色
     * @return 花色文本
     */
    static std::string getSuitText(CardSuitType suit);
    
    /**
     * 获取卡牌完整文本
     * @param face 卡牌面值
     * @param suit 卡牌花色
     * @return 完整文本
     */
    static std::string getCardText(CardFaceType face, CardSuitType suit);
    
    /**
     * 获取卡牌颜色
     * @param suit 卡牌花色
     * @return 卡牌颜色
     */
    static cocos2d::Color3B getCardColor(CardSuitType suit);
    
    /**
     * 检查两张卡牌是否相邻
     * @param face1 第一张卡牌面值
     * @param face2 第二张卡牌面值
     * @return 是否相邻
     */
    static bool areFacesAdjacent(CardFaceType face1, CardFaceType face2);
    
    /**
     * 获取卡牌尺寸
     * @return 卡牌尺寸
     */
    static cocos2d::Size getCardSize();
    
    /**
     * 计算扇形位置
     * @param center 中心位置
     * @param index 索引
     * @param total 总数
     * @param radius 半径
     * @return 计算后的位置
     */
    static cocos2d::Vec2 calculateFanPosition(const cocos2d::Vec2& center, int index, int total, float radius);
    
    /**
     * 验证卡牌数据是否有效
     * @param face 卡牌面值
     * @param suit 卡牌花色
     * @return 是否有效
     */
    static bool isValidCard(CardFaceType face, CardSuitType suit);
    
    /**
     * 检查两张卡牌是否可以匹配（数字差1）
     * @param card1 第一张卡牌
     * @param card2 第二张卡牌
     * @return 是否可以匹配
     */
    static bool canMatch(const CardModel* card1, const CardModel* card2);
    
    /**
     * 检查卡牌是否可以与底牌堆顶部卡牌匹配
     * @param card 要检查的卡牌
     * @param bottomPileTopCard 底牌堆顶部卡牌
     * @return 是否可以匹配
     */
    static bool canMatchWithBottomPile(const CardModel* card, const CardModel* bottomPileTopCard);

private:
    // 禁止实例化
    CardUtils() = delete;
    ~CardUtils() = delete;
    CardUtils(const CardUtils&) = delete;
    CardUtils& operator=(const CardUtils&) = delete;
};

#endif // __CARD_UTILS_H__
