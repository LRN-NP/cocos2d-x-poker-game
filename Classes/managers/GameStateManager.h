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

#ifndef __GAME_STATE_MANAGER_H__
#define __GAME_STATE_MANAGER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include <vector>
#include <memory>

/**
 * 游戏状态管理器
 * 职责：管理游戏状态的保存和恢复，实现撤销/重做功能
 * 使用场景：游戏过程中保存每一步的状态，支持撤销操作
 */
class GameStateManager
{
public:
    /**
     * 游戏状态快照
     * 保存某一时刻的完整游戏状态
     */
    struct GameStateSnapshot
    {
        std::vector<std::shared_ptr<CardModel>> mainPileCards;     ///< 主牌堆卡牌
        std::vector<std::shared_ptr<CardModel>> bottomPileCards;   ///< 底牌堆卡牌
        std::vector<std::shared_ptr<CardModel>> reservePileCards;  ///< 备用牌堆卡牌
        int bottomPileTopIndex;                                    ///< 底牌堆顶部索引
        int reservePileTopIndex;                                   ///< 备用牌堆顶部索引
        std::string actionType;                                    ///< 操作类型
        int sourceCardId;                                          ///< 源卡牌ID
        int targetCardId;                                          ///< 目标卡牌ID
        
        GameStateSnapshot() : bottomPileTopIndex(-1), reservePileTopIndex(-1), sourceCardId(-1), targetCardId(-1) {}
    };
    
    /**
     * 构造函数
     */
    GameStateManager();
    
    /**
     * 析构函数
     */
    ~GameStateManager();
    
    /**
     * 保存当前游戏状态
     * @param gameModel 游戏模型
     * @param actionType 操作类型
     * @param sourceCardId 源卡牌ID
     * @param targetCardId 目标卡牌ID
     */
    void saveState(GameModel* gameModel, const std::string& actionType, int sourceCardId, int targetCardId);
    
    /**
     * 撤销到上一个状态
     * @param gameModel 游戏模型
     * @return 是否成功撤销
     */
    bool undo(GameModel* gameModel);
    
    /**
     * 重做到下一个状态
     * @param gameModel 游戏模型
     * @return 是否成功重做
     */
    bool redo(GameModel* gameModel);
    
    /**
     * 检查是否可以撤销
     * @return 是否可以撤销
     */
    bool canUndo() const;
    
    /**
     * 检查是否可以重做
     * @return 是否可以重做
     */
    bool canRedo() const;
    
    /**
     * 清空所有状态
     */
    void clear();
    
    /**
     * 获取状态数量
     * @return 状态数量
     */
    size_t getStateCount() const;

private:
    /**
     * 深拷贝卡牌模型
     * @param card 源卡牌
     * @return 拷贝的卡牌
     */
    std::shared_ptr<CardModel> cloneCard(const CardModel* card);
    
    /**
     * 深拷贝卡牌列表
     * @param cards 源卡牌列表
     * @return 拷贝的卡牌列表
     */
    std::vector<std::shared_ptr<CardModel>> cloneCardList(const std::vector<CardModel*>& cards);
    
    /**
     * 恢复卡牌列表到游戏模型
     * @param gameModel 游戏模型
     * @param targetCards 目标卡牌列表
     * @param sourceCards 源卡牌列表
     */
    void restoreCardList(std::vector<CardModel*>& targetCards, const std::vector<std::shared_ptr<CardModel>>& sourceCards);

private:
    std::vector<GameStateSnapshot> _stateHistory;  ///< 状态历史
    size_t _currentStateIndex;                     ///< 当前状态索引
    static const size_t MAX_STATES = 100;          ///< 最大状态数量
};

#endif // __GAME_STATE_MANAGER_H__
