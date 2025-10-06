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

#include "GameView.h"
#include "ui/CocosGUI.h"
#include "MainPileView.h"
#include "BottomPileView.h"
#include "ReservePileView.h"

GameView* GameView::create(const GameModel* gameModel)
{
    return create(gameModel, nullptr);
}

GameView* GameView::create(const GameModel* gameModel, UndoButtonCallback undoCallback)
{
    GameView* ret = new GameView();
    if (ret && ret->init(gameModel, undoCallback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GameView::init(const GameModel* gameModel, UndoButtonCallback undoCallback)
{
    if (!Node::init()) {
        return false;
    }
    
    _gameModel = gameModel;
    _undoCallback = undoCallback;
    _mainPileView = nullptr;
    _bottomPileView = nullptr;
    _reservePileView = nullptr;
    _undoButton = nullptr;
    _cardClickCallback = nullptr;
    _drawCardCallback = nullptr;
    
    setupLayout();
    createUndoButton();
    
    if (_gameModel) {
        updateGame(_gameModel);
    }
    
    return true;
}

void GameView::updateGame(const GameModel* gameModel)
{
    _gameModel = gameModel;
    updateDisplay();
}

void GameView::setUndoButtonCallback(UndoButtonCallback callback)
{
    _undoCallback = callback;
}

void GameView::setCardClickCallback(CardClickCallback callback)
{
    _cardClickCallback = callback;
    
    // 重新设置各个视图的回调
    if (_mainPileView) {
        // MainPileView的回调在创建时已经设置，不需要重新设置
    }
    if (_bottomPileView) {
        // BottomPileView的回调在创建时已经设置，不需要重新设置
    }
    if (_reservePileView) {
        // 重新创建ReservePileView以设置正确的回调
        _reservePileView->removeFromParent();
        _reservePileView = ReservePileView::create(_gameModel, [this]() {
            if (_drawCardCallback) {
                _drawCardCallback();
            }
        }, _cardClickCallback);
        if (_reservePileView) {
            auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
            _reservePileView->setPosition(origin);
            addChild(_reservePileView, 3);
        }
    }
}

void GameView::setDrawCardCallback(DrawCardCallback callback)
{
    _drawCardCallback = callback;
}

void GameView::updateDisplay()
{
    if (_mainPileView) {
        _mainPileView->updateDisplay();
    }
    if (_bottomPileView) {
        _bottomPileView->updateDisplay();
    }
    if (_reservePileView) {
        _reservePileView->updateDisplay();
    }
}

void GameView::playMatchAnimation(int cardId, const cocos2d::Vec2& targetPosition, float duration, std::function<void()> callback)
{
    cocos2d::log("GameView::playMatchAnimation - CardId: %d, target: (%.1f, %.1f)", 
                 cardId, targetPosition.x, targetPosition.y);
    
    // 先尝试在备用牌堆中播放动画
    if (_reservePileView) {
        cocos2d::log("GameView::playMatchAnimation - Trying ReservePileView first");
        _reservePileView->playCardMoveAnimation(cardId, targetPosition, duration, callback);
        return;
    }
    
    // 再尝试在主牌堆中播放动画
    if (_mainPileView) {
        cocos2d::log("GameView::playMatchAnimation - Trying MainPileView");
        _mainPileView->playCardMoveAnimation(cardId, targetPosition, duration, callback);
        return;
    }
    
    // 如果都没有找到，直接调用回调
    cocos2d::log("GameView::playMatchAnimation - No view found, calling callback directly");
    if (callback) {
        callback();
    }
}

CardModel* GameView::drawTopCard()
{
    if (_reservePileView) {
        return _reservePileView->drawTopCard();
    }
    return nullptr;
}

void GameView::setUndoButtonEnabled(bool enabled)
{
    if (_undoButton) {
        _undoButton->setEnabled(enabled);
    }
}

void GameView::playEnterAnimation(std::function<void()> callback)
{
    setScale(0.0f);
    auto scaleAction = cocos2d::ScaleTo::create(0.3f, 1.0f);
    auto easeAction = cocos2d::EaseBackOut::create(scaleAction);
    
    if (callback) {
        auto sequence = cocos2d::Sequence::create(easeAction, 
                                                 cocos2d::CallFunc::create(callback), 
                                                 nullptr);
        runAction(sequence);
    } else {
        runAction(easeAction);
    }
}

void GameView::playExitAnimation(std::function<void()> callback)
{
    auto scaleAction = cocos2d::ScaleTo::create(0.3f, 0.0f);
    auto easeAction = cocos2d::EaseBackIn::create(scaleAction);
    
    if (callback) {
        auto sequence = cocos2d::Sequence::create(easeAction, 
                                                 cocos2d::CallFunc::create(callback), 
                                                 nullptr);
        runAction(sequence);
    } else {
        runAction(easeAction);
    }
}

void GameView::createUndoButton()
{
    _undoButton = cocos2d::ui::Button::create();
    _undoButton->setTitleText("Undo");
    _undoButton->setTitleFontSize(36);
    _undoButton->setTitleColor(cocos2d::Color3B::WHITE);
    _undoButton->setColor(cocos2d::Color3B(100, 100, 100));
    _undoButton->setPosition(cocos2d::Vec2(100, 1600));
    _undoButton->addTouchEventListener(CC_CALLBACK_2(GameView::onUndoButtonClicked, this));
    addChild(_undoButton, 10);
}

void GameView::setupLayout()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    // 创建主牌堆视图
    _mainPileView = MainPileView::create(_gameModel, [this](int cardId) {
        // 处理主牌堆卡牌点击
        if (_cardClickCallback) {
            _cardClickCallback(cardId);
        }
    });
    if (_mainPileView) {
        _mainPileView->setPosition(origin);
        addChild(_mainPileView, 1);
    }
    
    // 创建底牌堆视图
    _bottomPileView = BottomPileView::create(_gameModel, _cardClickCallback);
    if (_bottomPileView) {
        _bottomPileView->setPosition(origin);
        addChild(_bottomPileView, 2);
    }
    
    // 创建备用牌堆视图
    _reservePileView = ReservePileView::create(_gameModel, [this]() {
        // 处理抽取卡牌
        if (_drawCardCallback) {
            _drawCardCallback();
        }
    }, _cardClickCallback);
    if (_reservePileView) {
        _reservePileView->setPosition(origin);
        addChild(_reservePileView, 3);
    }
    
    // 添加分隔线
    auto drawNode = cocos2d::DrawNode::create();
    cocos2d::Vec2 start(origin.x, origin.y + visibleSize.height - 1500);
    cocos2d::Vec2 end(origin.x + visibleSize.width, origin.y + visibleSize.height - 1500);
    drawNode->drawLine(start, end, cocos2d::Color4F::GRAY);
    addChild(drawNode, 0);
}

void GameView::onUndoButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
    if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
        if (_undoCallback) {
            _undoCallback();
        }
    }
}
