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

#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/GameStateManager.h"
#include <vector>
#include <string>

/**
 * 撤销操作结构
 */
struct UndoAction
{
    int sourceCardId;                    ///< 源卡牌ID
    int targetCardId;                    ///< 目标卡牌ID
    std::string actionType;              ///< 操作类型
    cocos2d::Vec2 sourcePosition;        ///< 源位置
    cocos2d::Vec2 targetPosition;        ///< 目标位置
};

/**
 * 测试场景
 * 职责：测试新的游戏架构和三个牌堆的显示
 * 使用场景：开发测试阶段
 */
class TestScene : public cocos2d::Scene
{
public:
    /**
     * 创建测试场景
     * @return 测试场景实例
     */
    static cocos2d::Scene* createScene();
    
    /**
     * 析构函数
     */
    ~TestScene();
    
    /**
     * 初始化
     * @return 是否初始化成功
     */
    virtual bool init();
    
    /**
     * 创建函数
     */
    CREATE_FUNC(TestScene);

private:
    /**
     * 初始化游戏模型
     * @return 是否初始化成功
     */
    bool initGameModel();
    
    /**
     * 初始化游戏视图
     * @return 是否初始化成功
     */
    bool initGameView();
    
    /**
     * 处理卡牌点击
     * @param cardId 卡牌ID
     */
    void onCardClicked(int cardId);
    
    /**
     * 处理抽取卡牌
     */
    void onDrawCard();
    
    /**
     * 处理撤销
     */
    void onUndo();
    
    /**
     * 查找底牌堆卡牌索引
     * @param cardId 卡牌ID
     * @return 卡牌索引，-1表示未找到
     */
    int findBottomPileCardIndex(int cardId);
    
    /**
     * 记录撤销操作
     * @param sourceCardId 源卡牌ID
     * @param targetCardId 目标卡牌ID
     * @param actionType 操作类型
     */
    void recordUndoAction(int sourceCardId, int targetCardId, const std::string& actionType);
    
    /**
     * 获取卡牌位置
     * @param cardId 卡牌ID
     * @return 卡牌位置
     */
    cocos2d::Vec2 getCardPosition(int cardId);
    
    /**
     * 播放底牌堆交换动画
     * @param fromIndex 源索引
     * @param toIndex 目标索引
     * @param callback 完成回调
     */
    void playBottomPileSwapAnimation(int fromIndex, int toIndex, std::function<void()> callback);

private:
    GameModel* _gameModel;              ///< 游戏模型
    GameView* _gameView;                ///< 游戏视图
    GameStateManager* _stateManager;    ///< 状态管理器
    std::vector<UndoAction> _undoActions;  ///< 撤销操作列表（保留用于兼容）
};

#endif // __TEST_SCENE_H__
