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

#include "BottomPileView.h"
#include "../utils/CardUtils.h"

USING_NS_CC;

BottomPileView::BottomPileView()
    : _gameModel(nullptr)
    , _topCardView(nullptr)
    , _cardClickCallback(nullptr)
    , _cardSize(CardUtils::getCardSize())
{
}

BottomPileView::~BottomPileView()
{
    for (auto* cardView : _cardViews) {
        if (cardView) {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();
}

BottomPileView* BottomPileView::create(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
    BottomPileView* view = new (std::nothrow) BottomPileView();
    if (view && view->init(gameModel, cardClickCallback)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool BottomPileView::init(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
    if (!Layer::init()) {
        return false;
    }
    
    _gameModel = gameModel;
    _cardClickCallback = cardClickCallback;
    
    // 初始化显示
    updateDisplay();
    
    return true;
}

void BottomPileView::updateDisplay()
{
    // 清除现有卡牌视图
    if (_topCardView) {
        _topCardView->removeFromParent();
        _topCardView = nullptr;
    }
    
    if (!_gameModel) {
        return;
    }
    
    // 只显示顶部卡牌（底牌区只有一张牌）
    CardModel* topCard = _gameModel->getBottomPileTopCard();
    if (topCard) {
        cocos2d::log("BottomPileView::updateDisplay - Creating card view for card %d, face: %d, suit: %d, pos: (%.1f, %.1f)", 
                     topCard->getCardId(), topCard->getFace(), topCard->getSuit(), 
                     topCard->getPosition().x, topCard->getPosition().y);
        _topCardView = createCardView(topCard);
        if (_topCardView) {
            addChild(_topCardView);
            _topCardView->setPosition(topCard->getPosition());
            cocos2d::log("BottomPileView::updateDisplay - Card view created and positioned");
        } else {
            cocos2d::log("BottomPileView::updateDisplay - Failed to create card view");
        }
    } else {
        cocos2d::log("BottomPileView::updateDisplay - No top card found");
    }
}

CardView* BottomPileView::createCardView(CardModel* card)
{
    if (!card) {
        return nullptr;
    }
    
    CardView* cardView = CardView::create(card, [this](int cardId) {
        // 底牌堆卡牌点击回调
        if (_cardClickCallback) {
            _cardClickCallback(cardId);
        }
    });
    if (cardView) {
        cardView->setClickable(card->isClickable());
        if (card->isRevealed()) {
            cardView->setRevealed(true);
        } else {
            cardView->setRevealed(false);
        }
    }
    
    return cardView;
}

void BottomPileView::playCardMoveAnimation(int cardId, const Vec2& targetPosition, float duration, std::function<void()> callback)
{
    if (_topCardView && _topCardView->getCardId() == cardId) {
        auto moveAction = MoveTo::create(duration, targetPosition);
        if (callback) {
            auto sequence = Sequence::create(moveAction, CallFunc::create(callback), nullptr);
            _topCardView->runAction(sequence);
        } else {
            _topCardView->runAction(moveAction);
        }
    }
}

void BottomPileView::addCard(CardModel* card)
{
    if (card) {
        // 移除现有顶部卡牌
        if (_topCardView) {
            _topCardView->removeFromParent();
        }
        
        // 添加新卡牌
        _topCardView = createCardView(card);
        if (_topCardView) {
            addChild(_topCardView);
            _topCardView->setPosition(card->getPosition());
        }
    }
}

int BottomPileView::removeTopCard()
{
    if (_topCardView) {
        int cardId = _topCardView->getCardId();
        _topCardView->removeFromParent();
        _topCardView = nullptr;
        return cardId;
    }
    return -1;
}
