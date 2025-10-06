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

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <string>

/**
 * 游戏数据模型
 * 职责：存储游戏运行时的核心数据，包括桌面牌区、手牌区、游戏状态等
 * 使用场景：游戏运行时管理所有卡牌数据，支持序列化用于存档
 */
class GameModel
{
public:
    /**
     * 构造函数
     */
    GameModel();
    
    /**
     * 析构函数
     */
    ~GameModel();
    
    /**
     * 获取主牌堆卡牌列表
     * @return 主牌堆卡牌列表
     */
    const std::vector<CardModel*>& getMainPileCards() const { return _mainPileCards; }
    
    /**
     * 获取主牌堆卡牌列表（可修改版本）
     * @return 主牌堆卡牌列表
     */
    std::vector<CardModel*>& getMainPileCards() { return _mainPileCards; }
    
    /**
     * 获取底牌堆卡牌列表
     * @return 底牌堆卡牌列表
     */
    const std::vector<CardModel*>& getBottomPileCards() const { return _bottomPileCards; }
    
    /**
     * 获取底牌堆卡牌列表（可修改版本）
     * @return 底牌堆卡牌列表
     */
    std::vector<CardModel*>& getBottomPileCards() { return _bottomPileCards; }
    
    /**
     * 获取备用牌堆卡牌列表
     * @return 备用牌堆卡牌列表
     */
    const std::vector<CardModel*>& getReservePileCards() const { return _reservePileCards; }
    
    /**
     * 获取备用牌堆卡牌列表（可修改版本）
     * @return 备用牌堆卡牌列表
     */
    std::vector<CardModel*>& getReservePileCards() { return _reservePileCards; }
    
    /**
     * 获取当前底牌堆顶部卡牌索引
     * @return 顶部卡牌索引，-1表示无顶部卡牌
     */
    int getBottomPileTopIndex() const { return _bottomPileTopIndex; }
    
    /**
     * 获取当前备用牌堆顶部卡牌索引
     * @return 顶部卡牌索引，-1表示无顶部卡牌
     */
    int getReservePileTopIndex() const { return _reservePileTopIndex; }
    
    /**
     * 获取游戏状态
     * @return 游戏状态字符串
     */
    const std::string& getGameState() const { return _gameState; }
    
    /**
     * 设置主牌堆卡牌列表
     * @param cards 主牌堆卡牌列表
     */
    void setMainPileCards(const std::vector<CardModel*>& cards);
    
    /**
     * 设置底牌堆卡牌列表
     * @param cards 底牌堆卡牌列表
     */
    void setBottomPileCards(const std::vector<CardModel*>& cards);
    
    /**
     * 设置备用牌堆卡牌列表
     * @param cards 备用牌堆卡牌列表
     */
    void setReservePileCards(const std::vector<CardModel*>& cards);
    
    /**
     * 设置底牌堆顶部卡牌索引
     * @param index 顶部卡牌索引
     */
    void setBottomPileTopIndex(int index) { _bottomPileTopIndex = index; }
    
    /**
     * 设置备用牌堆顶部卡牌索引
     * @param index 顶部卡牌索引
     */
    void setReservePileTopIndex(int index) { _reservePileTopIndex = index; }
    
    /**
     * 设置游戏状态
     * @param state 游戏状态
     */
    void setGameState(const std::string& state) { _gameState = state; }
    
    /**
     * 添加主牌堆卡牌
     * @param card 卡牌模型
     */
    void addMainPileCard(CardModel* card);
    
    /**
     * 添加底牌堆卡牌
     * @param card 卡牌模型
     */
    void addBottomPileCard(CardModel* card);
    
    /**
     * 添加备用牌堆卡牌
     * @param card 卡牌模型
     */
    void addReservePileCard(CardModel* card);
    
    /**
     * 移除主牌堆卡牌
     * @param cardId 卡牌ID
     * @return 是否成功移除
     */
    bool removeMainPileCard(int cardId);
    
    /**
     * 移除底牌堆卡牌
     * @param cardId 卡牌ID
     * @return 是否成功移除
     */
    bool removeBottomPileCard(int cardId);
    
    /**
     * 移除备用牌堆卡牌
     * @param cardId 卡牌ID
     * @return 是否成功移除
     */
    bool removeReservePileCard(int cardId);
    
    /**
     * 根据ID查找主牌堆卡牌
     * @param cardId 卡牌ID
     * @return 卡牌模型，未找到返回nullptr
     */
    CardModel* findMainPileCard(int cardId) const;
    
    /**
     * 根据ID查找底牌堆卡牌
     * @param cardId 卡牌ID
     * @return 卡牌模型，未找到返回nullptr
     */
    CardModel* findBottomPileCard(int cardId) const;
    
    /**
     * 根据ID查找备用牌堆卡牌
     * @param cardId 卡牌ID
     * @return 卡牌模型，未找到返回nullptr
     */
    CardModel* findReservePileCard(int cardId) const;
    
    /**
     * 获取当前底牌堆顶部卡牌
     * @return 顶部卡牌模型，无顶部卡牌返回nullptr
     */
    CardModel* getBottomPileTopCard() const;
    
    /**
     * 获取当前备用牌堆顶部卡牌
     * @return 顶部卡牌模型，无顶部卡牌返回nullptr
     */
    CardModel* getReservePileTopCard() const;
    
    /**
     * 清空所有卡牌
     */
    void clearAllCards();
    
    /**
     * 检查游戏是否结束
     * @return 游戏是否结束
     */
    bool isGameOver() const;
    
    /**
     * 序列化游戏数据
     * @return 序列化后的数据
     */
    std::string serialize() const;
    
    /**
     * 反序列化游戏数据
     * @param data 序列化数据
     * @return 是否成功
     */
    bool deserialize(const std::string& data);

private:
    std::vector<CardModel*> _mainPileCards;      ///< 主牌堆卡牌列表
    std::vector<CardModel*> _bottomPileCards;    ///< 底牌堆卡牌列表
    std::vector<CardModel*> _reservePileCards;   ///< 备用牌堆卡牌列表
    int _bottomPileTopIndex;                     ///< 底牌堆顶部卡牌索引
    int _reservePileTopIndex;                    ///< 备用牌堆顶部卡牌索引
    std::string _gameState;                      ///< 游戏状态
};

#endif // __GAME_MODEL_H__
