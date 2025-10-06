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

#include "UndoService.h"
#include <algorithm>

UndoModel::UndoRecord UndoService::createHandSwapRecord(const GameModel* gameModel, 
                                                        int fromCardId, int toCardId)
{
    UndoModel::UndoRecord record;
    record.actionType = UAT_HAND_SWAP;
    record.sourceCardId = fromCardId;
    record.targetCardId = toCardId;
    
    if (gameModel) {
        CardModel* fromCard = gameModel->findBottomPileCard(fromCardId);
        CardModel* toCard = gameModel->findBottomPileCard(toCardId);
        
        if (fromCard) {
            record.sourcePosition = fromCard->getPosition();
        }
        if (toCard) {
            record.targetPosition = toCard->getPosition();
        }
        
        record.handTopIndex = gameModel->getBottomPileTopIndex();
        record.stackIndex = findStackCardIndex(gameModel, fromCardId);
    }
    
    return record;
}

UndoModel::UndoRecord UndoService::createPlayfieldToHandRecord(const GameModel* gameModel,
                                                               int playfieldCardId, int stackCardId)
{
    UndoModel::UndoRecord record;
    record.actionType = UAT_PLAYFIELD_TO_HAND;
    record.sourceCardId = playfieldCardId;
    record.targetCardId = stackCardId;
    
    if (gameModel) {
        CardModel* playfieldCard = gameModel->findMainPileCard(playfieldCardId);
        CardModel* stackCard = gameModel->findBottomPileCard(stackCardId);
        
        if (playfieldCard) {
            record.sourcePosition = playfieldCard->getPosition();
        }
        if (stackCard) {
            record.targetPosition = stackCard->getPosition();
        }
        
        record.handTopIndex = gameModel->getBottomPileTopIndex();
        record.playfieldIndex = findPlayfieldCardIndex(gameModel, playfieldCardId);
        record.stackIndex = findStackCardIndex(gameModel, stackCardId);
    }
    
    return record;
}

bool UndoService::validateUndoRecord(const GameModel* gameModel, const UndoModel::UndoRecord& record)
{
    if (!gameModel) return false;
    
    switch (record.actionType) {
        case UAT_HAND_SWAP:
            return gameModel->findBottomPileCard(record.sourceCardId) != nullptr &&
                   gameModel->findBottomPileCard(record.targetCardId) != nullptr;
                   
        case UAT_PLAYFIELD_TO_HAND:
            return gameModel->findMainPileCard(record.sourceCardId) != nullptr &&
                   gameModel->findBottomPileCard(record.targetCardId) != nullptr;
                   
        default:
            return false;
    }
}

bool UndoService::executeUndo(GameModel* gameModel, const UndoModel::UndoRecord& record)
{
    if (!gameModel || !validateUndoRecord(gameModel, record)) {
        return false;
    }
    
    switch (record.actionType) {
        case UAT_HAND_SWAP: {
            CardModel* sourceCard = gameModel->findBottomPileCard(record.sourceCardId);
            CardModel* targetCard = gameModel->findBottomPileCard(record.targetCardId);
            
            if (sourceCard && targetCard) {
                // 交换位置
                sourceCard->setPosition(record.sourcePosition);
                targetCard->setPosition(record.targetPosition);
                
                // 恢复手牌区顶部索引
                gameModel->setBottomPileTopIndex(record.handTopIndex);
                return true;
            }
            break;
        }
        
        case UAT_PLAYFIELD_TO_HAND: {
            CardModel* playfieldCard = gameModel->findMainPileCard(record.sourceCardId);
            CardModel* stackCard = gameModel->findBottomPileCard(record.targetCardId);
            
            if (playfieldCard && stackCard) {
                // 恢复位置
                playfieldCard->setPosition(record.sourcePosition);
                stackCard->setPosition(record.targetPosition);
                
                // 恢复手牌区顶部索引
                gameModel->setBottomPileTopIndex(record.handTopIndex);
                return true;
            }
            break;
        }
        
        default:
            return false;
    }
    
    return false;
}

int UndoService::findPlayfieldCardIndex(const GameModel* gameModel, int cardId)
{
    if (!gameModel) return -1;
    
    const auto& playfieldCards = gameModel->getMainPileCards();
    for (size_t i = 0; i < playfieldCards.size(); ++i) {
        if (playfieldCards[i] && playfieldCards[i]->getCardId() == cardId) {
            return (int)i;
        }
    }
    return -1;
}

int UndoService::findStackCardIndex(const GameModel* gameModel, int cardId)
{
    if (!gameModel) return -1;
    
    const auto& stackCards = gameModel->getBottomPileCards();
    for (size_t i = 0; i < stackCards.size(); ++i) {
        if (stackCards[i] && stackCards[i]->getCardId() == cardId) {
            return (int)i;
        }
    }
    return -1;
}
