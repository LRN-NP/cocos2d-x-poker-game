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

#include "GameModelFromLevelGenerator.h"
#include <random>
#include <algorithm>

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig)
{
    return generateGameModel(levelConfig, false);
}

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig, bool randomize)
{
    if (!levelConfig || !levelConfig->isValid()) {
        return nullptr;
    }
    
    GameModel* gameModel = new GameModel();
    
    // 生成主牌堆卡牌
    const auto& mainPileCards = levelConfig->getMainPileCards();
    for (const auto& cardConfig : mainPileCards) {
        CardModel* cardModel = createCardModel(cardConfig, "MainPile");
        if (cardModel) {
            gameModel->addMainPileCard(cardModel);
        }
    }
    
    // 生成底牌堆卡牌
    const auto& bottomPileCards = levelConfig->getBottomPileCards();
    for (const auto& cardConfig : bottomPileCards) {
        CardModel* cardModel = createCardModel(cardConfig, "BottomPile");
        if (cardModel) {
            gameModel->addBottomPileCard(cardModel);
        }
    }
    
    // 生成备用牌堆卡牌
    const auto& reservePileCards = levelConfig->getReservePileCards();
    for (const auto& cardConfig : reservePileCards) {
        CardModel* cardModel = createCardModel(cardConfig, "ReservePile");
        if (cardModel) {
            gameModel->addReservePileCard(cardModel);
        }
    }
    
    // 随机化位置（如果需要）
    if (randomize) {
        randomizeCardPositions(gameModel, levelConfig);
    }
    
    return gameModel;
}

CardModel* GameModelFromLevelGenerator::createCardModel(const LevelConfig::CardConfig& cardConfig, const std::string& pileType)
{
    CardFaceType face = (CardFaceType)cardConfig.cardFace;
    CardSuitType suit = (CardSuitType)cardConfig.cardSuit;
    cocos2d::Vec2 position = cardConfig.position;
    
    CardModel* card = new CardModel(face, suit, position);
    
    // 根据牌区类型设置卡牌状态
    if (pileType == "MainPile") {
        // 主牌区：所有牌都翻开且可点击
        card->setRevealed(true);
        card->setClickable(true);
    } else if (pileType == "BottomPile") {
        // 底牌区：翻开但不可点击（作为匹配目标）
        card->setRevealed(true);
        card->setClickable(false);
    } else if (pileType == "ReservePile") {
        // 备用牌区：所有牌都翻开且可点击
        card->setRevealed(true);
        card->setClickable(true);
    }
    
    return card;
}

void GameModelFromLevelGenerator::randomizeCardPositions(GameModel* gameModel, const LevelConfig* levelConfig)
{
    if (!gameModel || !levelConfig) return;
    
    // 随机化主牌堆位置
    auto& mainPileCards = gameModel->getMainPileCards();
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(mainPileCards.begin(), mainPileCards.end(), g);
    
    // 重新分配位置
    const auto& originalMainPileCards = levelConfig->getMainPileCards();
    for (size_t i = 0; i < mainPileCards.size() && i < originalMainPileCards.size(); ++i) {
        if (mainPileCards[i]) {
            mainPileCards[i]->setPosition(originalMainPileCards[i].position);
        }
    }
}
