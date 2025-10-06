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

#include "CardModel.h"
#include <sstream>
#include <algorithm>

int CardModel::_nextCardId = 1;

CardModel::CardModel()
    : _cardId(_nextCardId++)
    , _face(CFT_NONE)
    , _suit(CST_NONE)
    , _position(cocos2d::Vec2::ZERO)
    , _isRevealed(false)
    , _isClickable(false)
{
}

CardModel::CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position)
    : _cardId(_nextCardId++)
    , _face(face)
    , _suit(suit)
    , _position(position)
    , _isRevealed(false)
    , _isClickable(false)
{
}

CardModel::~CardModel()
{
}

bool CardModel::isValid() const
{
    return _face != CFT_NONE && _suit != CST_NONE;
}

bool CardModel::isAdjacentTo(const CardModel& other) const
{
    if (!isValid() || !other.isValid()) {
        return false;
    }
    
    int faceDiff = std::abs((int)_face - (int)other._face);
    return faceDiff == 1;
}

std::string CardModel::getCardText() const
{
    static const char* faces[] = {"?", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    static const char* suits[] = {"C", "D", "H", "S"};
    
    std::string result;
    if (_face >= 0 && _face < CFT_NUM_CARD_FACE_TYPES) {
        result += faces[_face];
    }
    if (_suit >= 0 && _suit < CST_NUM_CARD_SUIT_TYPES) {
        result += suits[_suit];
    }
    return result;
}

std::string CardModel::serialize() const
{
    std::ostringstream oss;
    oss << _cardId << "," << (int)_face << "," << (int)_suit << "," 
        << _position.x << "," << _position.y << "," 
        << (_isRevealed ? 1 : 0) << "," << (_isClickable ? 1 : 0);
    return oss.str();
}

bool CardModel::deserialize(const std::string& data)
{
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() != 7) {
        return false;
    }
    
    try {
        _cardId = std::stoi(tokens[0]);
        _face = (CardFaceType)std::stoi(tokens[1]);
        _suit = (CardSuitType)std::stoi(tokens[2]);
        _position.x = std::stof(tokens[3]);
        _position.y = std::stof(tokens[4]);
        _isRevealed = std::stoi(tokens[5]) != 0;
        _isClickable = std::stoi(tokens[6]) != 0;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}
