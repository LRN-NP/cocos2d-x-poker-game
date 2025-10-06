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

#include "ReservePileView.h"
#include "../utils/CardUtils.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

ReservePileView::ReservePileView()
    : _gameModel(nullptr)
    , _drawCardCallback(nullptr)
    , _cardClickCallback(nullptr)
    , _drawCardButton(nullptr)
    , _cardSize(cocos2d::Size(182, 282))  // 使用卡牌精灵的实际尺寸
    , _cardSpacing(5.0f)
{
}

ReservePileView::~ReservePileView()
{
}

ReservePileView* ReservePileView::create(const GameModel* gameModel, DrawCardCallback drawCardCallback, CardClickCallback cardClickCallback)
{
    ReservePileView* view = new (std::nothrow) ReservePileView();
    if (view && view->init(gameModel, drawCardCallback, cardClickCallback)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool ReservePileView::init(const GameModel* gameModel, DrawCardCallback drawCardCallback, CardClickCallback cardClickCallback)
{
    if (!Layer::init()) {
        return false;
    }
    
    _gameModel = gameModel;
    _drawCardCallback = drawCardCallback;
    _cardClickCallback = cardClickCallback;
    
    // 创建抽取卡牌按钮
    _drawCardButton = ui::Button::create("CloseNormal.png", "CloseSelected.png");
    if (_drawCardButton) {
        _drawCardButton->setTitleText("抽取");
        _drawCardButton->setTitleFontSize(24);
        _drawCardButton->setPosition(Vec2(100, 100));
        _drawCardButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType eventType) {
            if (eventType == ui::Widget::TouchEventType::ENDED) {
                onDrawCardButtonClicked();
            }
        });
        addChild(_drawCardButton);
    }
    
    // 初始化显示
    updateDisplay();
    
    return true;
}

void ReservePileView::updateDisplay()
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
    const auto& cards = _gameModel->getReservePileCards();
    for (auto* card : cards) {
        if (card) {
            CardView* cardView = createCardView(card);
            if (cardView) {
                addChild(cardView);
                _cardViews.push_back(cardView);
            }
        }
    }
    
    // 更新卡牌位置（水平排列显示）
    for (size_t i = 0; i < _cardViews.size(); ++i) {
        if (_cardViews[i]) {
            Vec2 position = Vec2(100 + i * (_cardSize.width + _cardSpacing), 200);
            _cardViews[i]->setPosition(position);
            // 同时更新模型中的位置
            if (i < cards.size() && cards[i]) {
                cards[i]->setPosition(position);
                cocos2d::log("ReservePileView::updateDisplay - Card %d at position (%.1f, %.1f) [index %zu]", 
                             cards[i]->getCardId(), position.x, position.y, i);
            }
        }
    }
    
    // 更新按钮状态
    if (_drawCardButton) 
    {
        _drawCardButton->setEnabled(!cards.empty());
    }
}

void ReservePileView::setupTouchListener()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) -> bool {
        cocos2d::Vec2 touchPos = this->convertToNodeSpace(touch->getLocation());
        
        // 遍历子节点（卡牌），判断哪个被点击
        for (auto* cardView : _cardViews) {
            if (cardView && cardView->isVisible()) {
                cocos2d::Rect cardBbox = cardView->getBoundingBox();
                if (cardBbox.containsPoint(touchPos)) {
                    // 处理卡牌点击逻辑
                    if (_cardClickCallback) {
                        _cardClickCallback(cardView->getCardId());
                    }
                    return true; // 吞没事件，避免其他节点处理
                }
            }
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

CardView* ReservePileView::createCardView(CardModel* card)
{
    if (!card) {
        return nullptr;
    }
    
    CardView* cardView = CardView::create(card, [this](int cardId) {
        // 备用牌堆卡牌点击回调
        cocos2d::log("ReservePileView::createCardView - Card %d clicked, callback: %s", 
                     cardId, _cardClickCallback ? "SET" : "NULL");
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

void ReservePileView::onDrawCardButtonClicked()
{
    if (_drawCardCallback) {
        _drawCardCallback();
    }
}

void ReservePileView::playCardMoveAnimation(int cardId, const Vec2& targetPosition, float duration, std::function<void()> callback)
{
    cocos2d::log("ReservePileView::playCardMoveAnimation - CardId: %d, target: (%.1f, %.1f)", 
                 cardId, targetPosition.x, targetPosition.y);
    
    for (auto* cardView : _cardViews) {
        if (cardView && cardView->getCardId() == cardId) {
            cocos2d::log("ReservePileView::playCardMoveAnimation - Found card view for %d", cardId);
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
    
    cocos2d::log("ReservePileView::playCardMoveAnimation - Card view not found for %d", cardId);
    // 如果找不到卡牌视图，直接调用回调
    if (callback) {
        callback();
    }
}

CardModel* ReservePileView::drawTopCard()
{
    if (!_gameModel) {
        return nullptr;
    }
    
    CardModel* topCard = _gameModel->getReservePileTopCard();
    if (topCard) {
        // 从视图中移除顶部卡牌
        if (!_cardViews.empty()) {
            auto* cardView = _cardViews.back();
            if (cardView) {
                cardView->removeFromParent();
            }
            _cardViews.pop_back();
        }
        
        // 更新显示
        updateDisplay();
    }
    
    return topCard;
}
