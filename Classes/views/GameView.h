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

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../models/GameModel.h"
#include "MainPileView.h"
#include "BottomPileView.h"
#include "ReservePileView.h"
#include <functional>

/**
 * 游戏主视图
 * 职责：管理整个游戏的UI显示，包括桌面牌区、手牌区、控制按钮等
 * 使用场景：游戏主界面，协调各个子视图的显示
 */
class GameView : public cocos2d::Node
{
public:
    /**
     * 撤销按钮点击回调函数类型
     */
    typedef std::function<void()> UndoButtonCallback;
    
    /**
     * 卡牌点击回调函数类型
     * @param cardId 被点击的卡牌ID
     */
    using CardClickCallback = std::function<void(int cardId)>;
    
    /**
     * 抽取卡牌回调函数类型
     */
    using DrawCardCallback = std::function<void()>;
    
    /**
     * 创建游戏视图
     * @param gameModel 游戏数据模型
     * @return 游戏视图实例
     */
    static GameView* create(const GameModel* gameModel);
    
    /**
     * 创建游戏视图
     * @param gameModel 游戏数据模型
     * @param undoCallback 撤销按钮回调函数
     * @return 游戏视图实例
     */
    static GameView* create(const GameModel* gameModel, UndoButtonCallback undoCallback);
    
    /**
     * 初始化游戏视图
     * @param gameModel 游戏数据模型
     * @param undoCallback 撤销按钮回调函数
     * @return 是否初始化成功
     */
    bool init(const GameModel* gameModel, UndoButtonCallback undoCallback = nullptr);
    
    /**
     * 设置撤销按钮回调函数
     * @param callback 撤销按钮回调函数
     */
    void setUndoButtonCallback(UndoButtonCallback callback);
    
    /**
     * 设置卡牌点击回调函数
     * @param callback 卡牌点击回调函数
     */
    void setCardClickCallback(CardClickCallback callback);
    
    /**
     * 设置抽取卡牌回调函数
     * @param callback 抽取卡牌回调函数
     */
    void setDrawCardCallback(DrawCardCallback callback);
    
    /**
     * 更新显示
     */
    void updateDisplay();
    
    /**
     * 播放匹配动画
     * @param cardId 卡牌ID
     * @param targetPosition 目标位置
     * @param duration 动画持续时间
     * @param callback 动画完成回调
     */
    void playMatchAnimation(int cardId, const cocos2d::Vec2& targetPosition, float duration, std::function<void()> callback = nullptr);
    
    /**
     * 抽取顶部卡牌
     * @return 被抽取的卡牌模型，nullptr表示无卡牌
     */
    CardModel* drawTopCard();
    
    /**
     * 获取主牌堆视图
     * @return 主牌堆视图
     */
    MainPileView* getMainPileView() const { return _mainPileView; }
    
    /**
     * 获取底牌堆视图
     * @return 底牌堆视图
     */
    BottomPileView* getBottomPileView() const { return _bottomPileView; }
    
    /**
     * 获取备用牌堆视图
     * @return 备用牌堆视图
     */
    ReservePileView* getReservePileView() const { return _reservePileView; }
    
    /**
     * 更新游戏显示
     * @param gameModel 游戏数据模型
     */
    void updateGame(const GameModel* gameModel);
    
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
     * 设置撤销按钮是否可用
     * @param enabled 是否可用
     */
    void setUndoButtonEnabled(bool enabled);
    
    /**
     * 播放入场动画
     * @param callback 动画完成回调
     */
    void playEnterAnimation(std::function<void()> callback = nullptr);
    
    /**
     * 播放退场动画
     * @param callback 动画完成回调
     */
    void playExitAnimation(std::function<void()> callback = nullptr);

private:
    const GameModel* _gameModel;        ///< 游戏数据模型（只读）
    MainPileView* _mainPileView;        ///< 主牌堆视图
    BottomPileView* _bottomPileView;    ///< 底牌堆视图
    ReservePileView* _reservePileView;  ///< 备用牌堆视图
    cocos2d::ui::Button* _undoButton;   ///< 撤销按钮
    UndoButtonCallback _undoCallback;   ///< 撤销按钮回调函数
    CardClickCallback _cardClickCallback; ///< 卡牌点击回调函数
    DrawCardCallback _drawCardCallback;   ///< 抽取卡牌回调函数
    
    /**
     * 创建撤销按钮
     */
    void createUndoButton();
    
    /**
     * 设置布局
     */
    void setupLayout();
    
    /**
     * 撤销按钮点击事件处理
     * @param sender 发送者
     * @param eventType 事件类型
     */
    void onUndoButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
};

#endif // __GAME_VIEW_H__
