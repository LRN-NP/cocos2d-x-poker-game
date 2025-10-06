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

#ifndef __MAIN_PILE_VIEW_H__
#define __MAIN_PILE_VIEW_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "CardView.h"
#include <functional>
#include <vector>

/**
 * 主牌堆视图
 * 职责：显示主牌堆中的卡牌，处理卡牌点击事件
 * 使用场景：游戏主界面，显示待消除的卡牌
 */
class MainPileView : public cocos2d::Layer
{
public:
    /**
     * 卡牌点击回调函数类型
     * @param cardId 被点击的卡牌ID
     */
    using CardClickCallback = std::function<void(int cardId)>;
    
    /**
     * 创建主牌堆视图
     * @param gameModel 游戏模型
     * @param cardClickCallback 卡牌点击回调
     * @return 主牌堆视图实例
     */
    static MainPileView* create(const GameModel* gameModel, CardClickCallback cardClickCallback);
    
    /**
     * 初始化
     * @param gameModel 游戏模型
     * @param cardClickCallback 卡牌点击回调
     * @return 是否初始化成功
     */
    bool init(const GameModel* gameModel, CardClickCallback cardClickCallback);
    
    /**
     * 更新显示
     */
    void updateDisplay();
    
    /**
     * 播放卡牌移动动画
     * @param cardId 卡牌ID
     * @param targetPosition 目标位置
     * @param duration 动画持续时间
     * @param callback 动画完成回调
     */
    void playCardMoveAnimation(int cardId, const cocos2d::Vec2& targetPosition, float duration, std::function<void()> callback = nullptr);
    
    /**
     * 播放卡牌翻转动画
     * @param cardId 卡牌ID
     * @param callback 动画完成回调
     */
    void playCardFlipAnimation(int cardId, std::function<void()> callback = nullptr);
    
    /**
     * 移除卡牌
     * @param cardId 卡牌ID
     */
    void removeCard(int cardId);
    
    /**
     * 添加卡牌
     * @param card 卡牌模型
     */
    void addCard(CardModel* card);

private:
    /**
     * 构造函数
     */
    MainPileView();
    
    /**
     * 析构函数
     */
    ~MainPileView();
    
    /**
     * 创建卡牌视图
     * @param card 卡牌模型
     * @return 卡牌视图
     */
    CardView* createCardView(CardModel* card);
    
    /**
     * 处理卡牌点击事件
     * @param cardId 卡牌ID
     */
    void onCardClicked(int cardId);
    
    /**
     * 更新卡牌位置
     */
    void updateCardPositions();

private:
    const GameModel* _gameModel;                    ///< 游戏模型（只读）
    CardClickCallback _cardClickCallback;           ///< 卡牌点击回调
    std::vector<CardView*> _cardViews;              ///< 卡牌视图列表
    cocos2d::Size _cardSize;                        ///< 卡牌尺寸
    float _cardSpacing;                             ///< 卡牌间距
};

#endif // __MAIN_PILE_VIEW_H__
