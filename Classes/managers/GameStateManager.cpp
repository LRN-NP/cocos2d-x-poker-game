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

#include "GameStateManager.h"
#include "../models/CardModel.h"
#include <algorithm>

USING_NS_CC;

GameStateManager::GameStateManager()
    : _currentStateIndex(0)
{
}

GameStateManager::~GameStateManager()
{
    clear();
}

void GameStateManager::saveState(GameModel* gameModel, const std::string& actionType, int sourceCardId, int targetCardId)
{
    if (!gameModel) return;
    
    GameStateSnapshot snapshot;
    
    // 深拷贝所有卡牌
    snapshot.mainPileCards = cloneCardList(gameModel->getMainPileCards());
    snapshot.bottomPileCards = cloneCardList(gameModel->getBottomPileCards());
    snapshot.reservePileCards = cloneCardList(gameModel->getReservePileCards());
    
    // 保存索引
    snapshot.bottomPileTopIndex = gameModel->getBottomPileTopIndex();
    snapshot.reservePileTopIndex = gameModel->getReservePileTopIndex();
    
    // 保存操作信息
    snapshot.actionType = actionType;
    snapshot.sourceCardId = sourceCardId;
    snapshot.targetCardId = targetCardId;
    
    // 如果当前不在历史末尾，删除后面的状态
    if (_currentStateIndex + 1 < _stateHistory.size()) {
        _stateHistory.erase(_stateHistory.begin() + _currentStateIndex + 1, _stateHistory.end());
    }
    
    // 添加新状态
    _stateHistory.push_back(snapshot);
    _currentStateIndex = _stateHistory.size() - 1;
    
    // 限制状态数量
    while (_stateHistory.size() > MAX_STATES) {
        _stateHistory.erase(_stateHistory.begin());
        if (_currentStateIndex > 0) {
            _currentStateIndex--;
        }
    }
    
    cocos2d::log("Saved state: %s, source: %d, target: %d, current index: %zu, total states: %zu", 
                 actionType.c_str(), sourceCardId, targetCardId, _currentStateIndex, _stateHistory.size());
}

bool GameStateManager::undo(GameModel* gameModel)
{
    cocos2d::log("Undo requested - canUndo: %s, current index: %zu, total states: %zu", 
                 canUndo() ? "YES" : "NO", _currentStateIndex, _stateHistory.size());
    
    if (!canUndo() || !gameModel) {
        cocos2d::log("Undo failed - canUndo: %s, gameModel: %s", 
                     canUndo() ? "YES" : "NO", gameModel ? "OK" : "NULL");
        return false;
    }
    
    // 移动到上一个状态
    _currentStateIndex--;
    
    // 检查索引是否有效
    if (_currentStateIndex >= _stateHistory.size()) {
        cocos2d::log("Invalid state index: %zu, size: %zu", _currentStateIndex, _stateHistory.size());
        return false;
    }
    
    // 恢复状态
    const GameStateSnapshot& snapshot = _stateHistory[_currentStateIndex];
    
    // 清空当前卡牌
    gameModel->clearAllCards();
    
    // 恢复卡牌列表
    restoreCardList(gameModel->getMainPileCards(), snapshot.mainPileCards);
    restoreCardList(gameModel->getBottomPileCards(), snapshot.bottomPileCards);
    restoreCardList(gameModel->getReservePileCards(), snapshot.reservePileCards);
    
    // 恢复索引
    gameModel->setBottomPileTopIndex(snapshot.bottomPileTopIndex);
    gameModel->setReservePileTopIndex(snapshot.reservePileTopIndex);
    
    cocos2d::log("Undo to state: %s, source: %d, target: %d", 
                 snapshot.actionType.c_str(), snapshot.sourceCardId, snapshot.targetCardId);
    
    return true;
}

bool GameStateManager::redo(GameModel* gameModel)
{
    if (!canRedo() || !gameModel) {
        return false;
    }
    
    // 移动到下一个状态
    _currentStateIndex++;
    
    // 检查索引是否有效
    if (_currentStateIndex >= _stateHistory.size()) {
        cocos2d::log("Invalid state index in redo: %zu, size: %zu", _currentStateIndex, _stateHistory.size());
        return false;
    }
    
    // 恢复状态
    const GameStateSnapshot& snapshot = _stateHistory[_currentStateIndex];
    
    // 清空当前卡牌
    gameModel->clearAllCards();
    
    // 恢复卡牌列表
    restoreCardList(gameModel->getMainPileCards(), snapshot.mainPileCards);
    restoreCardList(gameModel->getBottomPileCards(), snapshot.bottomPileCards);
    restoreCardList(gameModel->getReservePileCards(), snapshot.reservePileCards);
    
    // 恢复索引
    gameModel->setBottomPileTopIndex(snapshot.bottomPileTopIndex);
    gameModel->setReservePileTopIndex(snapshot.reservePileTopIndex);
    
    cocos2d::log("Redo to state: %s, source: %d, target: %d", 
                 snapshot.actionType.c_str(), snapshot.sourceCardId, snapshot.targetCardId);
    
    return true;
}

bool GameStateManager::canUndo() const
{
    return _currentStateIndex > 0;
}

bool GameStateManager::canRedo() const
{
    return _currentStateIndex < _stateHistory.size() - 1;
}

void GameStateManager::clear()
{
    _stateHistory.clear();
    _currentStateIndex = 0;
}

size_t GameStateManager::getStateCount() const
{
    return _stateHistory.size();
}

std::shared_ptr<CardModel> GameStateManager::cloneCard(const CardModel* card)
{
    if (!card) return nullptr;
    
    auto clonedCard = std::make_shared<CardModel>(card->getFace(), card->getSuit(), card->getPosition());
    clonedCard->setRevealed(card->isRevealed());
    clonedCard->setClickable(card->isClickable());
    
    return clonedCard;
}

std::vector<std::shared_ptr<CardModel>> GameStateManager::cloneCardList(const std::vector<CardModel*>& cards)
{
    std::vector<std::shared_ptr<CardModel>> clonedCards;
    clonedCards.reserve(cards.size());
    
    for (auto* card : cards) {
        if (card) {
            clonedCards.push_back(cloneCard(card));
        }
    }
    
    return clonedCards;
}

void GameStateManager::restoreCardList(std::vector<CardModel*>& targetCards, const std::vector<std::shared_ptr<CardModel>>& sourceCards)
{
    targetCards.clear();
    targetCards.reserve(sourceCards.size());
    
    for (const auto& card : sourceCards) {
        if (card) {
            // 创建新的CardModel对象
            CardModel* newCard = new CardModel(card->getFace(), card->getSuit(), card->getPosition());
            newCard->setCardId(card->getCardId());  // 设置CardId
            newCard->setRevealed(card->isRevealed());
            newCard->setClickable(card->isClickable());
            targetCards.push_back(newCard);
        }
    }
}
