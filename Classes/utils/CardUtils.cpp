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

#include "CardUtils.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::string CardUtils::getFaceText(CardFaceType face)
{
    static const char* faces[] = {"?", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    
    if (face >= 0 && face < CFT_NUM_CARD_FACE_TYPES) {
        return faces[face];
    }
    return "?";
}

std::string CardUtils::getSuitText(CardSuitType suit)
{
    static const char* suits[] = {"C", "D", "H", "S"};
    
    if (suit >= 0 && suit < CST_NUM_CARD_SUIT_TYPES) {
        return suits[suit];
    }
    return "?";
}

std::string CardUtils::getCardText(CardFaceType face, CardSuitType suit)
{
    return getFaceText(face) + getSuitText(suit);
}

cocos2d::Color3B CardUtils::getCardColor(CardSuitType suit)
{
    switch (suit) {
        case CST_HEARTS:
        case CST_DIAMONDS:
            return cocos2d::Color3B::RED;
        case CST_CLUBS:
        case CST_SPADES:
            return cocos2d::Color3B::BLACK;
        default:
            return cocos2d::Color3B::BLACK;
    }
}

bool CardUtils::areFacesAdjacent(CardFaceType face1, CardFaceType face2)
{
    if (face1 == CFT_NONE || face2 == CFT_NONE) {
        return false;
    }
    
    int diff = std::abs((int)face1 - (int)face2);
    return diff == 1;
}

cocos2d::Size CardUtils::getCardSize()
{
    return cocos2d::Size(120, 160);
}

cocos2d::Vec2 CardUtils::calculateFanPosition(const cocos2d::Vec2& center, int index, int total, float radius)
{
    if (total <= 1) {
        return center;
    }
    
    float angle = (float)index / (total - 1) * M_PI - M_PI / 2;
    float x = center.x + radius * std::cos(angle);
    float y = center.y + radius * std::sin(angle);
    
    return cocos2d::Vec2(x, y);
}

bool CardUtils::isValidCard(CardFaceType face, CardSuitType suit)
{
    return face >= 0 && face < CFT_NUM_CARD_FACE_TYPES &&
           suit >= 0 && suit < CST_NUM_CARD_SUIT_TYPES;
}

bool CardUtils::canMatch(const CardModel* card1, const CardModel* card2)
{
    if (!card1 || !card2) {
        return false;
    }
    
    // 检查两张卡牌是否都翻开且可点击
    if (!card1->isRevealed() || !card1->isClickable() ||
        !card2->isRevealed() || !card2->isClickable()) {
        return false;
    }
    
    // 检查面值是否相邻（差1）
    return areFacesAdjacent(card1->getFace(), card2->getFace());
}

bool CardUtils::canMatchWithBottomPile(const CardModel* card, const CardModel* bottomPileTopCard)
{
    if (!card || !bottomPileTopCard) {
        return false;
    }
    
    // 检查卡牌是否翻开且可点击
    if (!card->isRevealed() || !card->isClickable()) {
        return false;
    }
    
    // 检查面值是否相邻（差1）
    return areFacesAdjacent(card->getFace(), bottomPileTopCard->getFace());
}
