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

#ifndef __STACK_VIEW_H__
#define __STACK_VIEW_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "CardView.h"
#include <vector>
#include <functional>

/**
 * 手牌区视图
 * 职责：负责手牌区的UI显示和用户交互，不包含业务逻辑
 * 使用场景：显示手牌区的卡牌，处理手牌卡牌点击事件
 */
class StackView : public cocos2d::Node
{
public:
    /**
     * 手牌卡牌点击回调函数类型
     * @param cardId 卡牌ID
     */
    typedef std::function<void(int cardId)> StackCardClickCallback;
    
    /**
     * 创建手牌区视图
     * @return 手牌区视图实例
     */
    static StackView* create();
    
    /**
     * 创建手牌区视图
     * @param clickCallback 卡牌点击回调函数
     * @return 手牌区视图实例
     */
    static StackView* create(StackCardClickCallback clickCallback);
    
    /**
     * 初始化手牌区视图
     * @param clickCallback 卡牌点击回调函数
     * @return 是否初始化成功
     */
    bool init(StackCardClickCallback clickCallback = nullptr);
    
    /**
     * 设置卡牌点击回调函数
     * @param callback 卡牌点击回调函数
     */
    void setCardClickCallback(StackCardClickCallback callback) { _cardClickCallback = callback; }
    
    /**
     * 更新手牌区显示
     * @param gameModel 游戏数据模型
     */
    void updateStack(const GameModel* gameModel);
    
    /**
     * 播放手牌交换动画
     * @param fromCardId 源卡牌ID
     * @param toCardId 目标卡牌ID
     * @param callback 动画完成回调
     */
    void playSwapAnimation(int fromCardId, int toCardId, std::function<void()> callback = nullptr);
    
    /**
     * 播放撤销动画
     * @param cardId 卡牌ID
     * @param targetPosition 目标位置
     * @param callback 动画完成回调
     */
    void playUndoAnimation(int cardId, const cocos2d::Vec2& targetPosition, 
                          std::function<void()> callback = nullptr);
    
    /**
     * 根据卡牌ID获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图，未找到返回nullptr
     */
    CardView* getCardView(int cardId) const;
    
    /**
     * 获取手牌区顶部位置
     * @return 顶部位置
     */
    cocos2d::Vec2 getHandTopPosition() const { return _handTopPosition; }

private:
    std::vector<CardView*> _cardViews;              ///< 卡牌视图列表
    StackCardClickCallback _cardClickCallback;  ///< 卡牌点击回调函数
    cocos2d::Vec2 _handTopPosition;                 ///< 手牌区顶部位置
    std::vector<cocos2d::Vec2> _fanPositions;       ///< 扇形位置列表
    
    /**
     * 创建卡牌视图
     * @param cardModel 卡牌数据模型
     * @param position 卡牌位置
     * @return 卡牌视图
     */
    CardView* createCardView(const CardModel* cardModel, const cocos2d::Vec2& position);
    
    /**
     * 清理所有卡牌视图
     */
    void clearAllCardViews();
    
    /**
     * 设置布局
     */
    void setupLayout();
    
    /**
     * 卡牌点击事件处理
     * @param cardId 卡牌ID
     */
    void onCardClicked(int cardId);
};

#endif // __STACK_VIEW_H__
