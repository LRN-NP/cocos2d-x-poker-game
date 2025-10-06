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

#include "StackController.h"
USING_NS_CC;
StackController::StackController()
    : _gameModel(nullptr)
    , _undoManager(nullptr)
{
}

StackController::~StackController()
{
}

bool StackController::init(GameModel* gameModel, UndoManager* undoManager)
{
    if (!gameModel || !undoManager) {
        return false;
    }
    
    _gameModel = gameModel;
    _undoManager = undoManager;
    return true;
}

bool StackController::handleCardClick(int cardId)
{
    if (!_gameModel || !_undoManager) {
        return false;
    }
    
    // 检查是否可以交换
    if (!canSwapHandCard(cardId)) {
        return false;
    }
    
    // 执行交换操作
    return executeHandSwap(cardId);
}

bool StackController::swapHandCards(int fromCardId, int toCardId)
{
    return handleCardClick(fromCardId);
}

bool StackController::canSwapHandCard(int cardId)
{
    if (!_gameModel) {
        return false;
    }
    
    // 获取手牌区顶部卡牌索引
    int handTopIndex = _gameModel->getBottomPileTopIndex();
    if (handTopIndex < 0) {
        return false;
    }
    
    // 检查点击的卡牌是否是顶部卡牌
    if (findStackCardIndex(cardId) == handTopIndex) {
        return false; // 不能点击顶部卡牌
    }
    
    // 检查卡牌是否在手牌区中
    CardModel* card = _gameModel->findBottomPileCard(cardId);
    return card != nullptr;
}

bool StackController::executeHandSwap(int fromCardId)
{
    if (!_gameModel || !_undoManager) {
        return false;
    }
    
    // 获取源卡牌和当前顶部卡牌
    CardModel* fromCard = _gameModel->findBottomPileCard(fromCardId);
    CardModel* topCard = _gameModel->getBottomPileTopCard();
    
    if (!fromCard || !topCard) {
        return false;
    }
    
    // 创建撤销记录
    UndoModel::UndoRecord undoRecord = UndoService::createHandSwapRecord(
        _gameModel, fromCardId, topCard->getCardId());
    
    // 添加撤销记录
    _undoManager->addUndoRecord(undoRecord);
    
    // 执行交换：更新手牌区顶部索引
    int fromIndex = findStackCardIndex(fromCardId);
    if (fromIndex >= 0) {
        _gameModel->setBottomPileTopIndex(fromIndex);
    }
    
    return true;
}

int StackController::findStackCardIndex(int cardId)
{
    if (!_gameModel) {
        return -1;
    }
    
    const auto& stackCards = _gameModel->getBottomPileCards();
    for (size_t i = 0; i < stackCards.size(); ++i) {
        if (stackCards[i] && stackCards[i]->getCardId() == cardId) {
            return (int)i;
        }
    }
    return -1;
}
