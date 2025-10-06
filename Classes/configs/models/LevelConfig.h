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

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include <vector>

/**
 * 关卡配置类
 * 职责：存储关卡静态配置数据，包括桌面牌区和手牌区的卡牌配置
 * 使用场景：关卡加载时使用，提供关卡的基础数据
 */
class LevelConfig
{
public:
    /**
     * 卡牌配置结构
     * 存储单张卡牌的配置信息
     */
    struct CardConfig
    {
        int cardFace;           ///< 卡牌面值 (0-12: A,2,3,4,5,6,7,8,9,10,J,Q,K)
        int cardSuit;           ///< 卡牌花色 (0-3: 梅花,方块,红桃,黑桃)
        cocos2d::Vec2 position; ///< 卡牌位置
        
        CardConfig() : cardFace(0), cardSuit(0) {}
        CardConfig(int face, int suit, const cocos2d::Vec2& pos) 
            : cardFace(face), cardSuit(suit), position(pos) {}
    };
    
    /**
     * 构造函数
     */
    LevelConfig();
    
    /**
     * 析构函数
     */
    ~LevelConfig();
    
    /**
     * 获取主牌堆卡牌配置
     * @return 主牌堆卡牌配置列表
     */
    const std::vector<CardConfig>& getMainPileCards() const { return _mainPileCards; }
    
    /**
     * 获取底牌堆卡牌配置
     * @return 底牌堆卡牌配置列表
     */
    const std::vector<CardConfig>& getBottomPileCards() const { return _bottomPileCards; }
    
    /**
     * 获取备用牌堆卡牌配置
     * @return 备用牌堆卡牌配置列表
     */
    const std::vector<CardConfig>& getReservePileCards() const { return _reservePileCards; }
    
    /**
     * 设置主牌堆卡牌配置
     * @param cards 主牌堆卡牌配置列表
     */
    void setMainPileCards(const std::vector<CardConfig>& cards) { _mainPileCards = cards; }
    
    /**
     * 设置底牌堆卡牌配置
     * @param cards 底牌堆卡牌配置列表
     */
    void setBottomPileCards(const std::vector<CardConfig>& cards) { _bottomPileCards = cards; }
    
    /**
     * 设置备用牌堆卡牌配置
     * @param cards 备用牌堆卡牌配置列表
     */
    void setReservePileCards(const std::vector<CardConfig>& cards) { _reservePileCards = cards; }
    
    /**
     * 添加主牌堆卡牌配置
     * @param card 卡牌配置
     */
    void addMainPileCard(const CardConfig& card) { _mainPileCards.push_back(card); }
    
    /**
     * 添加底牌堆卡牌配置
     * @param card 卡牌配置
     */
    void addBottomPileCard(const CardConfig& card) { _bottomPileCards.push_back(card); }
    
    /**
     * 添加备用牌堆卡牌配置
     * @param card 卡牌配置
     */
    void addReservePileCard(const CardConfig& card) { _reservePileCards.push_back(card); }
    
    /**
     * 清空所有配置
     */
    void clear();
    
    /**
     * 检查配置是否有效
     * @return 配置是否有效
     */
    bool isValid() const;

private:
    std::vector<CardConfig> _mainPileCards;      ///< 主牌堆卡牌配置
    std::vector<CardConfig> _bottomPileCards;    ///< 底牌堆卡牌配置
    std::vector<CardConfig> _reservePileCards;   ///< 备用牌堆卡牌配置
};

#endif // __LEVEL_CONFIG_H__
