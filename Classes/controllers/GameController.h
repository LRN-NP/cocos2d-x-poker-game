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

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"
#include "../controllers/PlayFieldController.h"
#include "../controllers/StackController.h"
#include <string>

/**
 * 游戏主控制器
 * 职责：管理整个游戏流程，协调各个子控制器和视图
 * 使用场景：游戏的主要控制逻辑，处理游戏状态变化
 */
class GameController
{
public:
    /**
     * 构造函数
     */
    GameController();
    
    /**
     * 析构函数
     */
    ~GameController();
    
    /**
     * 初始化游戏控制器
     * @param parent 父节点
     * @return 是否初始化成功
     */
    bool init(cocos2d::Node* parent);
    
    /**
     * 开始游戏
     * @param levelId 关卡ID
     * @return 是否成功开始游戏
     */
    bool startGame(const std::string& levelId);
    
    /**
     * 处理卡牌点击事件
     * @param cardId 卡牌ID
     * @return 是否处理成功
     */
    bool handleCardClick(int cardId);
    
    /**
     * 处理撤销操作
     * @return 是否处理成功
     */
    bool handleUndo();
    
    /**
     * 获取游戏视图
     * @return 游戏视图
     */
    GameView* getGameView() const { return _gameView; }
    
    /**
     * 获取游戏模型
     * @return 游戏模型
     */
    GameModel* getGameModel() const { return _gameModel; }
    
    /**
     * 检查游戏是否结束
     * @return 游戏是否结束
     */
    bool isGameOver() const;

private:
    cocos2d::Node* _parent;             ///< 父节点
    GameModel* _gameModel;              ///< 游戏数据模型
    GameView* _gameView;                ///< 游戏视图
    UndoManager* _undoManager;          ///< 撤销管理器
    PlayFieldController* _playFieldController; ///< 桌面牌区控制器
    StackController* _stackController;  ///< 手牌区控制器
    
    /**
     * 初始化子控制器
     * @return 是否初始化成功
     */
    bool initSubControllers();
    
    /**
     * 初始化游戏视图
     * @return 是否初始化成功
     */
    bool initGameView();
    
    /**
     * 桌面卡牌点击事件处理
     * @param cardId 卡牌ID
     */
    void onPlayFieldCardClicked(int cardId);
    
    /**
     * 手牌卡牌点击事件处理
     * @param cardId 卡牌ID
     */
    void onStackCardClicked(int cardId);
    
    /**
     * 撤销完成回调处理
     * @param success 是否成功
     */
    void onUndoComplete(bool success);
    
    /**
     * 更新游戏视图
     */
    void updateGameView();
};

#endif // __GAME_CONTROLLER_H__
