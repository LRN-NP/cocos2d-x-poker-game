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

#include "MainPileView.h"
#include "../utils/CardUtils.h"

USING_NS_CC;

MainPileView::MainPileView()
    : _gameModel(nullptr)
    , _cardClickCallback(nullptr)
    , _cardSize(CardUtils::getCardSize())
    , _cardSpacing(10.0f)
{
}

MainPileView::~MainPileView()
{
}

MainPileView* MainPileView::create(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
    MainPileView* view = new (std::nothrow) MainPileView();
    if (view && view->init(gameModel, cardClickCallback)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool MainPileView::init(const GameModel* gameModel, CardClickCallback cardClickCallback)
{
    if (!Layer::init()) {
        return false;
    }
    
    _gameModel = gameModel;
    _cardClickCallback = cardClickCallback;
    
    // 不需要在这里设置触摸事件，CardView会处理自己的触摸事件
    
    // 初始化显示
    updateDisplay();
    
    return true;
}

void MainPileView::updateDisplay()
{
    // 清除现有卡牌视图
    for (auto* cardView : _cardViews) {
        if (cardView) {
            cardView->removeFromParent();
        }
    }
    _cardViews.clear();
    
    if (!_gameModel) {
        return;
    }
    
    // 创建新的卡牌视图
    const auto& cards = _gameModel->getMainPileCards();
    for (auto* card : cards) {
        if (card) {
            CardView* cardView = createCardView(card);
            if (cardView) {
                addChild(cardView);
                _cardViews.push_back(cardView);
            }
        }
    }
    
    // 更新卡牌位置
    updateCardPositions();
}

CardView* MainPileView::createCardView(CardModel* card)
{
    if (!card) {
        return nullptr;
    }
    
    CardView* cardView = CardView::create(card, [this](int cardId) {
        onCardClicked(cardId);
    });
    if (cardView) {
        // 设置卡牌状态
        if (card->isClickable()) {
            cardView->setClickable(true);
        } else {
            cardView->setClickable(false);
        }
        if (card->isRevealed()) {
            cardView->setRevealed(true);
        } else {
            cardView->setRevealed(false);
        }
    }
    
    return cardView;
}

void MainPileView::onCardClicked(int cardId)
{
    if (_cardClickCallback) {
        _cardClickCallback(cardId);
    }
}

void MainPileView::updateCardPositions()
{
    const auto& cards = _gameModel->getMainPileCards();
    for (size_t i = 0; i < cards.size() && i < _cardViews.size(); ++i) {
        if (cards[i] && _cardViews[i]) {
            _cardViews[i]->setPosition(cards[i]->getPosition());
        }
    }
}

void MainPileView::playCardMoveAnimation(int cardId, const Vec2& targetPosition, float duration, std::function<void()> callback)
{
    cocos2d::log("MainPileView::playCardMoveAnimation - CardId: %d, target: (%.1f, %.1f)", 
                 cardId, targetPosition.x, targetPosition.y);
    
    for (auto* cardView : _cardViews) {
        if (cardView && cardView->getCardId() == cardId) {
            cocos2d::log("MainPileView::playCardMoveAnimation - Found card view for %d", cardId);
            auto moveAction = MoveTo::create(duration, targetPosition);
            if (callback) {
                auto sequence = Sequence::create(moveAction, CallFunc::create(callback), nullptr);
                cardView->runAction(sequence);
            } else {
                cardView->runAction(moveAction);
            }
            return;
        }
    }
    
    cocos2d::log("MainPileView::playCardMoveAnimation - Card view not found for %d", cardId);
    // 如果找不到卡牌视图，直接调用回调
    if (callback) {
        callback();
    }
}

void MainPileView::playCardFlipAnimation(int cardId, std::function<void()> callback)
{
    for (auto* cardView : _cardViews) {
        if (cardView && cardView->getCardId() == cardId) {
            cardView->playFlipAnimation(callback);
            break;
        }
    }
}

void MainPileView::removeCard(int cardId)
{
    for (auto it = _cardViews.begin(); it != _cardViews.end(); ++it) {
        if (*it && (*it)->getCardId() == cardId) {
            (*it)->removeFromParent();
            _cardViews.erase(it);
            break;
        }
    }
}

void MainPileView::addCard(CardModel* card)
{
    if (card) {
        CardView* cardView = createCardView(card);
        if (cardView) {
            addChild(cardView);
            _cardViews.push_back(cardView);
            updateCardPositions();
        }
    }
}
