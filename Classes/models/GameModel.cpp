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

#include "GameModel.h"
#include <sstream>
#include <algorithm>

GameModel::GameModel()
    : _bottomPileTopIndex(-1)
    , _reservePileTopIndex(-1)
    , _gameState("playing")
{
}

GameModel::~GameModel()
{
    clearAllCards();
}

void GameModel::setMainPileCards(const std::vector<CardModel*>& cards)
{
    // 清理原有卡牌
    for (auto* card : _mainPileCards) {
        delete card;
    }
    _mainPileCards.clear();
    
    // 设置新卡牌
    _mainPileCards = cards;
}

void GameModel::setBottomPileCards(const std::vector<CardModel*>& cards)
{
    // 清理原有卡牌
    for (auto* card : _bottomPileCards) {
        delete card;
    }
    _bottomPileCards.clear();
    
    // 设置新卡牌
    _bottomPileCards = cards;
    
    // 设置顶部卡牌索引
    if (!_bottomPileCards.empty()) {
        _bottomPileTopIndex = (int)_bottomPileCards.size() - 1;
    } else {
        _bottomPileTopIndex = -1;
    }
}

void GameModel::setReservePileCards(const std::vector<CardModel*>& cards)
{
    // 清理原有卡牌
    for (auto* card : _reservePileCards) {
        delete card;
    }
    _reservePileCards.clear();
    
    // 设置新卡牌
    _reservePileCards = cards;
    
    // 设置顶部卡牌索引
    if (!_reservePileCards.empty()) {
        _reservePileTopIndex = (int)_reservePileCards.size() - 1;
    } else {
        _reservePileTopIndex = -1;
    }
}

void GameModel::addMainPileCard(CardModel* card)
{
    if (card) {
        _mainPileCards.push_back(card);
    }
}

void GameModel::addBottomPileCard(CardModel* card)
{
    if (card) {
        _bottomPileCards.push_back(card);
        _bottomPileTopIndex = (int)_bottomPileCards.size() - 1;
    }
}

void GameModel::addReservePileCard(CardModel* card)
{
    if (card) {
        _reservePileCards.push_back(card);
        _reservePileTopIndex = (int)_reservePileCards.size() - 1;
    }
}

bool GameModel::removeMainPileCard(int cardId)
{
    auto it = std::find_if(_mainPileCards.begin(), _mainPileCards.end(),
        [cardId](CardModel* card) { return card && card->getCardId() == cardId; });
    
    if (it != _mainPileCards.end()) {
        // 不要删除卡牌对象，只是从列表中移除
        // 卡牌对象会被移动到其他牌堆中
        _mainPileCards.erase(it);
        return true;
    }
    return false;
}

bool GameModel::removeBottomPileCard(int cardId)
{
    auto it = std::find_if(_bottomPileCards.begin(), _bottomPileCards.end(),
        [cardId](CardModel* card) { return card && card->getCardId() == cardId; });
    
    if (it != _bottomPileCards.end()) {
        delete *it;
        _bottomPileCards.erase(it);
        
        // 更新顶部卡牌索引
        if (_bottomPileTopIndex >= (int)_bottomPileCards.size()) {
            _bottomPileTopIndex = (int)_bottomPileCards.size() - 1;
        }
        return true;
    }
    return false;
}

bool GameModel::removeReservePileCard(int cardId)
{
    auto it = std::find_if(_reservePileCards.begin(), _reservePileCards.end(),
        [cardId](CardModel* card) { return card && card->getCardId() == cardId; });
    
    if (it != _reservePileCards.end()) {
        // 不要删除卡牌对象，只是从列表中移除
        // 卡牌对象会被移动到其他牌堆中
        _reservePileCards.erase(it);
        
        // 更新顶部卡牌索引
        if (_reservePileTopIndex >= (int)_reservePileCards.size()) {
            _reservePileTopIndex = (int)_reservePileCards.size() - 1;
        }
        return true;
    }
    return false;
}

CardModel* GameModel::findMainPileCard(int cardId) const
{
    auto it = std::find_if(_mainPileCards.begin(), _mainPileCards.end(),
        [cardId](CardModel* card) { return card && card->getCardId() == cardId; });
    
    return (it != _mainPileCards.end()) ? *it : nullptr;
}

CardModel* GameModel::findBottomPileCard(int cardId) const
{
    auto it = std::find_if(_bottomPileCards.begin(), _bottomPileCards.end(),
        [cardId](CardModel* card) { return card && card->getCardId() == cardId; });
    
    return (it != _bottomPileCards.end()) ? *it : nullptr;
}

CardModel* GameModel::findReservePileCard(int cardId) const
{
    auto it = std::find_if(_reservePileCards.begin(), _reservePileCards.end(),
        [cardId](CardModel* card) { return card && card->getCardId() == cardId; });
    
    return (it != _reservePileCards.end()) ? *it : nullptr;
}

CardModel* GameModel::getBottomPileTopCard() const
{
    if (_bottomPileTopIndex >= 0 && _bottomPileTopIndex < (int)_bottomPileCards.size()) {
        return _bottomPileCards[_bottomPileTopIndex];
    }
    return nullptr;
}

CardModel* GameModel::getReservePileTopCard() const
{
    if (_reservePileTopIndex >= 0 && _reservePileTopIndex < (int)_reservePileCards.size()) {
        return _reservePileCards[_reservePileTopIndex];
    }
    return nullptr;
}

void GameModel::clearAllCards()
{
    for (auto* card : _mainPileCards) {
        delete card;
    }
    _mainPileCards.clear();
    
    for (auto* card : _bottomPileCards) {
        delete card;
    }
    _bottomPileCards.clear();
    
    for (auto* card : _reservePileCards) {
        delete card;
    }
    _reservePileCards.clear();
    
    _bottomPileTopIndex = -1;
    _reservePileTopIndex = -1;
}

bool GameModel::isGameOver() const
{
    // 简单判断：主牌堆无卡牌或底牌堆无卡牌
    return _mainPileCards.empty() || _bottomPileCards.empty();
}

std::string GameModel::serialize() const
{
    std::ostringstream oss;
    
    // 序列化主牌堆
    oss << "mainPile:" << _mainPileCards.size() << ";";
    for (const auto* card : _mainPileCards) {
        if (card) {
            oss << card->serialize() << ";";
        }
    }
    
    // 序列化底牌堆
    oss << "bottomPile:" << _bottomPileCards.size() << ";";
    for (const auto* card : _bottomPileCards) {
        if (card) {
            oss << card->serialize() << ";";
        }
    }
    
    // 序列化备用牌堆
    oss << "reservePile:" << _reservePileCards.size() << ";";
    for (const auto* card : _reservePileCards) {
        if (card) {
            oss << card->serialize() << ";";
        }
    }
    
    // 序列化其他状态
    oss << "bottomPileTopIndex:" << _bottomPileTopIndex << ";";
    oss << "reservePileTopIndex:" << _reservePileTopIndex << ";";
    oss << "gameState:" << _gameState << ";";
    
    return oss.str();
}

bool GameModel::deserialize(const std::string& data)
{
    clearAllCards();
    
    std::istringstream iss(data);
    std::string line;
    
    while (std::getline(iss, line, ';')) {
        if (line.empty()) continue;
        
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;
        
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        
        if (key == "mainPile") {
            int count = std::stoi(value);
            for (int i = 0; i < count; ++i) {
                if (std::getline(iss, line, ';') && !line.empty()) {
                    CardModel* card = new CardModel();
                    if (card->deserialize(line)) {
                        _mainPileCards.push_back(card);
                    } else {
                        delete card;
                    }
                }
            }
        } else if (key == "bottomPile") {
            int count = std::stoi(value);
            for (int i = 0; i < count; ++i) {
                if (std::getline(iss, line, ';') && !line.empty()) {
                    CardModel* card = new CardModel();
                    if (card->deserialize(line)) {
                        _bottomPileCards.push_back(card);
                    } else {
                        delete card;
                    }
                }
            }
        } else if (key == "reservePile") {
            int count = std::stoi(value);
            for (int i = 0; i < count; ++i) {
                if (std::getline(iss, line, ';') && !line.empty()) {
                    CardModel* card = new CardModel();
                    if (card->deserialize(line)) {
                        _reservePileCards.push_back(card);
                    } else {
                        delete card;
                    }
                }
            }
        } else if (key == "bottomPileTopIndex") {
            _bottomPileTopIndex = std::stoi(value);
        } else if (key == "reservePileTopIndex") {
            _reservePileTopIndex = std::stoi(value);
        } else if (key == "gameState") {
            _gameState = value;
        }
    }
    
    return true;
}

