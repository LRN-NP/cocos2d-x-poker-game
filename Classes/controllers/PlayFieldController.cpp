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

#include "PlayFieldController.h"

PlayFieldController::PlayFieldController()
    : _gameModel(nullptr)
    , _undoManager(nullptr)
{
}

PlayFieldController::~PlayFieldController()
{
}

bool PlayFieldController::init(GameModel* gameModel, UndoManager* undoManager)
{
    if (!gameModel || !undoManager) {
        return false;
    }
    
    _gameModel = gameModel;
    _undoManager = undoManager;
    return true;
}

bool PlayFieldController::handleCardClick(int cardId)
{
    if (!_gameModel || !_undoManager) {
        return false;
    }
    
    // 检查是否可以移动到手牌区
    if (!canMoveToHand(cardId)) {
        return false;
    }
    
    // 执行移动操作
    return executeMoveToHand(cardId);
}

bool PlayFieldController::replaceTrayWithPlayFieldCard(int playfieldCardId)
{
    return handleCardClick(playfieldCardId);
}

bool PlayFieldController::canMoveToHand(int playfieldCardId)
{
    if (!_gameModel) {
        return false;
    }
    
    // 获取桌面卡牌
    CardModel* playfieldCard = _gameModel->findMainPileCard(playfieldCardId);
    if (!playfieldCard) {
        return false;
    }
    
    // 获取手牌区顶部卡牌
    CardModel* handTopCard = _gameModel->getBottomPileTopCard();
    if (!handTopCard) {
        return false;
    }
    
    // 检查面值是否相邻（差1）
    return playfieldCard->isAdjacentTo(*handTopCard);
}

bool PlayFieldController::executeMoveToHand(int playfieldCardId)
{
    if (!_gameModel || !_undoManager) {
        return false;
    }
    
    // 获取桌面卡牌和手牌区顶部卡牌
    CardModel* playfieldCard = _gameModel->findMainPileCard(playfieldCardId);
    CardModel* handTopCard = _gameModel->getBottomPileTopCard();
    
    if (!playfieldCard || !handTopCard) {
        return false;
    }
    
    // 创建撤销记录
    UndoModel::UndoRecord undoRecord = UndoService::createPlayfieldToHandRecord(
        _gameModel, playfieldCardId, handTopCard->getCardId());
    
    // 添加撤销记录
    _undoManager->addUndoRecord(undoRecord);
    
    // 执行移动：交换卡牌数据
    CardFaceType tempFace = playfieldCard->getFace();
    CardSuitType tempSuit = playfieldCard->getSuit();
    cocos2d::Vec2 tempPos = playfieldCard->getPosition();
    
    playfieldCard->setFace(handTopCard->getFace());
    playfieldCard->setSuit(handTopCard->getSuit());
    playfieldCard->setPosition(handTopCard->getPosition());
    
    handTopCard->setFace(tempFace);
    handTopCard->setSuit(tempSuit);
    handTopCard->setPosition(tempPos);
    
    return true;
}
