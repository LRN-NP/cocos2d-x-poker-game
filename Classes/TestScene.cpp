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

#include "TestScene.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "utils/CardUtils.h"
#include "cocos2d.h"

USING_NS_CC;

TestScene::~TestScene()
{
    if (_stateManager) {
        delete _stateManager;
        _stateManager = nullptr;
    }
}

Scene* TestScene::createScene()
{
    return TestScene::create();
}

bool TestScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    // 初始化状态管理器
    _stateManager = new GameStateManager();
    
    // 初始化游戏模型
    if (!initGameModel()) {
        return false;
    }
    
    // 初始化游戏视图
    if (!initGameView()) {
        return false;
    }
    
    // 保存初始状态
    _stateManager->saveState(_gameModel, "init", -1, -1);
    
    return true;
}

bool TestScene::initGameModel()
{
    // 加载关卡配置
    LevelConfigLoader loader;
    LevelConfig* levelConfig = loader.loadLevelConfig("1");
    if (!levelConfig) {
        cocos2d::log("Failed to load level config");
        return false;
    }
    
    // 生成游戏模型
    GameModelFromLevelGenerator generator;
    _gameModel = generator.generateGameModel(levelConfig);
    if (!_gameModel) {
        cocos2d::log("Failed to generate game model");
        delete levelConfig;
        return false;
    }
    
    delete levelConfig;
    return true;
}

bool TestScene::initGameView()
{
    // 创建游戏视图
    _gameView = GameView::create(_gameModel, [this]() {
        onUndo();
    });
    
    if (!_gameView) {
        cocos2d::log("Failed to create game view");
        return false;
    }
    
    // 设置回调
    _gameView->setCardClickCallback([this](int cardId) {
        onCardClicked(cardId);
    });
    
    _gameView->setDrawCardCallback([this]() {
        onDrawCard();
    });
    
    addChild(_gameView);
    
    return true;
}

void TestScene::onCardClicked(int cardId)
{
    cocos2d::log("Card clicked: %d", cardId);
    
    // 调试：检查卡牌在哪个牌堆中
    CardModel* mainCard = _gameModel->findMainPileCard(cardId);
    CardModel* bottomCard = _gameModel->findBottomPileCard(cardId);
    CardModel* reserveCard = _gameModel->findReservePileCard(cardId);
    
    cocos2d::log("Card %d found in - Main: %s, Bottom: %s, Reserve: %s", 
                 cardId, 
                 mainCard ? "YES" : "NO",
                 bottomCard ? "YES" : "NO", 
                 reserveCard ? "YES" : "NO");
    
    // 检查是否为主牌堆的卡牌
    CardModel* mainPileCard = _gameModel->findMainPileCard(cardId);
    if (mainPileCard) {
        // 主牌和底牌匹配：主牌替换底牌
        CardModel* bottomCard = _gameModel->getBottomPileTopCard();
        if (bottomCard && CardUtils::canMatchWithBottomPile(mainPileCard, bottomCard)) {
            cocos2d::log("Main pile card %d can match with bottom pile card %d", cardId, bottomCard->getCardId());
            
            // 播放匹配动画
            // 在动画开始前保存底部卡牌ID
            int bottomCardId = bottomCard->getCardId();
            
            _gameView->playMatchAnimation(cardId, bottomCard->getPosition(), 0.5f, [this, cardId, mainPileCard, bottomCard, bottomCardId]() {
                // 动画完成后更新模型
                // 先保存底部卡牌的位置
                cocos2d::Vec2 bottomCardPos = bottomCard->getPosition();
                
                // 移除主牌堆的卡牌
                _gameModel->removeMainPileCard(cardId);
                
                // 替换底牌堆的顶部卡牌
                auto& bottomPileCards = _gameModel->getBottomPileCards();
                int topIndex = _gameModel->getBottomPileTopIndex();
                if (topIndex >= 0 && topIndex < bottomPileCards.size()) {
                    // 删除原来的顶部卡牌
                    delete bottomPileCards[topIndex];
                    // 设置新的顶部卡牌
                    bottomPileCards[topIndex] = mainPileCard;
                    mainPileCard->setPosition(bottomCardPos);
                }
                
                // 在操作完成后保存状态
                _stateManager->saveState(_gameModel, "main_to_bottom", cardId, bottomCardId);
                
                _gameView->updateDisplay();
            });
        } else {
            cocos2d::log("Main pile card %d cannot match with bottom pile", cardId);
        }
        return;
    }
    
    // 检查是否为底牌堆的卡牌
    CardModel* bottomPileCard = _gameModel->findBottomPileCard(cardId);
    if (bottomPileCard) {
        cocos2d::log("Bottom pile card %d clicked - but bottom pile cards should not be clickable", cardId);
        // 底牌不应该被点击，直接返回
        return;
    }
    
    // 检查是否为备用牌堆的卡牌
    CardModel* reservePileCard = _gameModel->findReservePileCard(cardId);
    if (reservePileCard) {
        cocos2d::log("Reserve pile card %d clicked", cardId);
        // 备用牌点击：可以与底牌交换
        CardModel* bottomCard = _gameModel->getBottomPileTopCard();
        if (bottomCard) {
            cocos2d::log("Reserve pile card %d swap with bottom pile card %d", cardId, bottomCard->getCardId());
            
            // 在动画开始前保存底部卡牌ID
            int bottomCardId = bottomCard->getCardId();
            
            // 播放交换动画
            _gameView->playMatchAnimation(cardId, bottomCard->getPosition(), 0.5f, [this, cardId, reservePileCard, bottomCard, bottomCardId]() {
                cocos2d::log("Reserve to bottom animation completed for card %d", cardId);
                
                // 动画完成后更新模型
                cocos2d::Vec2 bottomCardPos = bottomCard->getPosition();
                cocos2d::Vec2 reserveCardPos = reservePileCard->getPosition();
                
                cocos2d::log("Before swap - Reserve cards: %zu, Bottom cards: %zu", 
                             _gameModel->getReservePileCards().size(), 
                             _gameModel->getBottomPileCards().size());
                
                // 找到被点击的备用牌在备用牌堆中的位置
                auto& reservePileCards = _gameModel->getReservePileCards();
                int reserveCardIndex = -1;
                for (size_t i = 0; i < reservePileCards.size(); ++i) {
                    if (reservePileCards[i] && reservePileCards[i]->getCardId() == cardId) {
                        reserveCardIndex = (int)i;
                        break;
                    }
                }
                
                if (reserveCardIndex >= 0) {
                    // 将原来的底部卡牌替换到被点击的备用牌位置
                    bottomCard->setPosition(reserveCardPos);
                    bottomCard->setClickable(true); // 设置为可点击
                    reservePileCards[reserveCardIndex] = bottomCard;
                    cocos2d::log("Replaced reserve card at index %d with bottom card %d", reserveCardIndex, bottomCard->getCardId());
                } else {
                    // 如果找不到位置，添加到末尾（备用方案）
                    bottomCard->setPosition(reserveCardPos);
                    bottomCard->setClickable(true); // 设置为可点击
                    _gameModel->addReservePileCard(bottomCard);
                    cocos2d::log("Added bottom card %d to reserve pile (fallback)", bottomCard->getCardId());
                }
                
                // 替换底牌堆的顶部卡牌
                auto& bottomPileCards = _gameModel->getBottomPileCards();
                int topIndex = _gameModel->getBottomPileTopIndex();
                if (topIndex >= 0 && topIndex < bottomPileCards.size()) {
                    // 设置新的顶部卡牌
                    bottomPileCards[topIndex] = reservePileCard;
                    reservePileCard->setPosition(bottomCardPos);
                    reservePileCard->setClickable(false); // 设置为不可点击
                    cocos2d::log("Set reserve card %d as new bottom card", reservePileCard->getCardId());
                } else {
                    cocos2d::log("Invalid bottom pile top index: %d, size: %zu", topIndex, bottomPileCards.size());
                }
                
                cocos2d::log("After swap - Reserve cards: %zu, Bottom cards: %zu", 
                             _gameModel->getReservePileCards().size(), 
                             _gameModel->getBottomPileCards().size());
                
                // 在操作完成后保存状态
                _stateManager->saveState(_gameModel, "reserve_to_bottom", cardId, bottomCardId);
                
                _gameView->updateDisplay();
            });
        } else {
            cocos2d::log("No bottom card available for reserve card %d", cardId);
        }
        return;
    }
    
    // 如果卡牌不在任何牌堆中，记录日志
    cocos2d::log("Card %d not found in any pile", cardId);
}

void TestScene::onDrawCard()
{
    cocos2d::log("Draw card requested");
    
    // 从备用牌堆抽取卡牌
    CardModel* card = _gameView->drawTopCard();
    if (card) {
        // 添加到底牌堆
        _gameModel->addBottomPileCard(card);
        _gameView->updateDisplay();
        cocos2d::log("Drew card %d from reserve pile", card->getCardId());
    } else {
        cocos2d::log("No cards available in reserve pile");
    }
}

void TestScene::onUndo()
{
    cocos2d::log("Undo requested");
    if (_stateManager && _stateManager->undo(_gameModel)) {
        _gameView->updateDisplay();
        cocos2d::log("Undo successful");
    } else {
        cocos2d::log("No undo actions available or undo failed");
    }
}

int TestScene::findBottomPileCardIndex(int cardId)
{
    const auto& bottomPileCards = _gameModel->getBottomPileCards();
    for (size_t i = 0; i < bottomPileCards.size(); ++i) {
        if (bottomPileCards[i] && bottomPileCards[i]->getCardId() == cardId) {
            return (int)i;
        }
    }
    return -1;
}

void TestScene::recordUndoAction(int sourceCardId, int targetCardId, const std::string& actionType)
{
    // 简单的撤销记录
    UndoAction action;
    action.sourceCardId = sourceCardId;
    action.targetCardId = targetCardId;
    action.actionType = actionType;
    action.sourcePosition = getCardPosition(sourceCardId);
    action.targetPosition = getCardPosition(targetCardId);
    
    _undoActions.push_back(action);
    cocos2d::log("Recorded undo action: %s", actionType.c_str());
}

cocos2d::Vec2 TestScene::getCardPosition(int cardId)
{
    CardModel* card = _gameModel->findMainPileCard(cardId);
    if (card) return card->getPosition();
    
    card = _gameModel->findBottomPileCard(cardId);
    if (card) return card->getPosition();
    
    card = _gameModel->findReservePileCard(cardId);
    if (card) return card->getPosition();
    
    return cocos2d::Vec2::ZERO;
}

void TestScene::playBottomPileSwapAnimation(int fromIndex, int toIndex, std::function<void()> callback)
{
    auto& bottomPileCards = _gameModel->getBottomPileCards();
    if (fromIndex >= 0 && fromIndex < bottomPileCards.size() && 
        toIndex >= 0 && toIndex < bottomPileCards.size()) {
        
        CardModel* fromCard = bottomPileCards[fromIndex];
        CardModel* toCard = bottomPileCards[toIndex];
        
        if (fromCard && toCard) {
            // 交换位置
            cocos2d::Vec2 fromPos = fromCard->getPosition();
            cocos2d::Vec2 toPos = toCard->getPosition();
            
            // 在向量中交换卡牌对象
            std::swap(bottomPileCards[fromIndex], bottomPileCards[toIndex]);
            
            // 更新卡牌位置
            fromCard->setPosition(toPos);
            toCard->setPosition(fromPos);
            
            // 直接调用回调，因为位置已经更新
            if (callback) {
                callback();
            }
        }
    }
}
