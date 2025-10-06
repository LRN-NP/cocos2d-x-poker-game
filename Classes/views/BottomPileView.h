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

#ifndef __BOTTOM_PILE_VIEW_H__
#define __BOTTOM_PILE_VIEW_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "CardView.h"
#include <functional>

/**
 * 卡牌点击回调函数类型
 */
using CardClickCallback = std::function<void(int cardId)>;

/**
 * 底牌堆视图
 * 职责：显示底牌堆中的卡牌，用于接收匹配的卡牌
 * 使用场景：游戏主界面，显示当前可匹配的底牌
 */
class BottomPileView : public cocos2d::Layer
{
public:
    /**
     * 创建底牌堆视图
     * @param gameModel 游戏模型
     * @param cardClickCallback 卡牌点击回调
     * @return 底牌堆视图实例
     */
    static BottomPileView* create(const GameModel* gameModel, CardClickCallback cardClickCallback = nullptr);
    
    /**
     * 初始化
     * @param gameModel 游戏模型
     * @param cardClickCallback 卡牌点击回调
     * @return 是否初始化成功
     */
    bool init(const GameModel* gameModel, CardClickCallback cardClickCallback = nullptr);
    
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
     * 添加卡牌
     * @param card 卡牌模型
     */
    void addCard(CardModel* card);
    
    /**
     * 移除顶部卡牌
     * @return 被移除的卡牌ID，-1表示无卡牌
     */
    int removeTopCard();

private:
    /**
     * 构造函数
     */
    BottomPileView();
    
    /**
     * 析构函数
     */
    ~BottomPileView();
    
    /**
     * 创建卡牌视图
     * @param card 卡牌模型
     * @return 卡牌视图
     */
    CardView* createCardView(CardModel* card);

private:
    const GameModel* _gameModel;                    ///< 游戏模型（只读）
    CardView* _topCardView;                         ///< 顶部卡牌视图
    std::vector<CardView*> _cardViews;              ///< 卡牌视图列表
    CardClickCallback _cardClickCallback;           ///< 卡牌点击回调
    cocos2d::Size _cardSize;                        ///< 卡牌尺寸
};

#endif // __BOTTOM_PILE_VIEW_H__
