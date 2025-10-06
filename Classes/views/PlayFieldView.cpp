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

#include "PlayFieldView.h"

PlayFieldView* PlayFieldView::create()
{
    return create(nullptr);
}

PlayFieldView* PlayFieldView::create(PlayFieldCardClickCallback clickCallback)
{
    PlayFieldView* ret = new PlayFieldView();
    if (ret && ret->init(clickCallback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PlayFieldView::init(PlayFieldCardClickCallback clickCallback)
{
    if (!Node::init()) {
        return false;
    }
    
    _cardClickCallback = clickCallback;
    return true;
}

void PlayFieldView::updatePlayField(const GameModel* gameModel)
{
    if (!gameModel) return;
    
    // 清理现有卡牌视图
    clearAllCardViews();
    
    // 创建新的卡牌视图
    const auto& playfieldCards = gameModel->getMainPileCards();
    for (const auto* cardModel : playfieldCards) {
        if (cardModel) {
            CardView* cardView = createCardView(cardModel);
            if (cardView) {
                _cardViews.push_back(cardView);
                addChild(cardView);
            }
        }
    }
}

void PlayFieldView::playMatchAnimation(int cardId, std::function<void()> callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView) {
        cardView->playMatchAnimation(callback);
    }
}

void PlayFieldView::playUndoAnimation(int cardId, const cocos2d::Vec2& targetPosition, 
                                     std::function<void()> callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView) {
        cardView->playUndoAnimation(targetPosition, 0.25f, callback);
    }
}

CardView* PlayFieldView::getCardView(int cardId) const
{
    for (CardView* cardView : _cardViews) {
        if (cardView && cardView->getCardId() == cardId) {
            return cardView;
        }
    }
    return nullptr;
}

CardView* PlayFieldView::createCardView(const CardModel* cardModel)
{
    if (!cardModel) return nullptr;
    
    auto clickCallback = [this](int cardId) {
        onCardClicked(cardId);
    };
    
    return CardView::create(cardModel, clickCallback);
}

void PlayFieldView::clearAllCardViews()
{
    for (CardView* cardView : _cardViews) {
        if (cardView) {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();
}

void PlayFieldView::onCardClicked(int cardId)
{
    if (_cardClickCallback) {
        _cardClickCallback(cardId);
    }
}
