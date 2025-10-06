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

#ifndef __PLAYFIELD_VIEW_H__
#define __PLAYFIELD_VIEW_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "CardView.h"
#include <vector>
#include <functional>

/**
 * 桌面牌区视图
 * 职责：负责桌面牌区的UI显示和用户交互，不包含业务逻辑
 * 使用场景：显示桌面上的卡牌，处理桌面卡牌点击事件
 */
class PlayFieldView : public cocos2d::Node
{
public:
    /**
     * 桌面卡牌点击回调函数类型
     * @param cardId 卡牌ID
     */
    typedef std::function<void(int cardId)> PlayFieldCardClickCallback;
    
    /**
     * 创建桌面牌区视图
     * @return 桌面牌区视图实例
     */
    static PlayFieldView* create();
    
    /**
     * 创建桌面牌区视图
     * @param clickCallback 卡牌点击回调函数
     * @return 桌面牌区视图实例
     */
    static PlayFieldView* create(PlayFieldCardClickCallback clickCallback);
    
    /**
     * 初始化桌面牌区视图
     * @param clickCallback 卡牌点击回调函数
     * @return 是否初始化成功
     */
    bool init(PlayFieldCardClickCallback clickCallback = nullptr);
    
    /**
     * 设置卡牌点击回调函数
     * @param callback 卡牌点击回调函数
     */
    void setCardClickCallback(PlayFieldCardClickCallback callback) { _cardClickCallback = callback; }
    
    /**
     * 更新桌面牌区显示
     * @param gameModel 游戏数据模型
     */
    void updatePlayField(const GameModel* gameModel);
    
    /**
     * 播放匹配动画
     * @param cardId 卡牌ID
     * @param callback 动画完成回调
     */
    void playMatchAnimation(int cardId, std::function<void()> callback = nullptr);
    
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

private:
    std::vector<CardView*> _cardViews;              ///< 卡牌视图列表
    PlayFieldCardClickCallback _cardClickCallback;  ///< 卡牌点击回调函数
    
    /**
     * 创建卡牌视图
     * @param cardModel 卡牌数据模型
     * @return 卡牌视图
     */
    CardView* createCardView(const CardModel* cardModel);
    
    /**
     * 清理所有卡牌视图
     */
    void clearAllCardViews();
    
    /**
     * 卡牌点击事件处理
     * @param cardId 卡牌ID
     */
    void onCardClicked(int cardId);
};

#endif // __PLAYFIELD_VIEW_H__
