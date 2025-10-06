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

#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "../models/GameModel.h"
#include "../managers/UndoManager.h"
#include "../models/UndoModel.h"
#include "../services/UndoService.h"

/**
 * 手牌区控制器
 * 职责：处理手牌区卡牌相关的业务逻辑
 * 使用场景：处理手牌卡牌点击事件，管理手牌区内部交换
 */
class StackController
{
public:
    /**
     * 构造函数
     */
    StackController();
    
    /**
     * 析构函数
     */
    ~StackController();
    
    /**
     * 初始化手牌区控制器
     * @param gameModel 游戏数据模型
     * @param undoManager 撤销管理器
     * @return 是否初始化成功
     */
    bool init(GameModel* gameModel, UndoManager* undoManager);
    
    /**
     * 处理手牌卡牌点击事件
     * @param cardId 卡牌ID
     * @return 是否处理成功
     */
    bool handleCardClick(int cardId);
    
    /**
     * 交换手牌区卡牌
     * @param fromCardId 源卡牌ID
     * @param toCardId 目标卡牌ID
     * @return 是否处理成功
     */
    bool swapHandCards(int fromCardId, int toCardId);

private:
    GameModel* _gameModel;      ///< 游戏数据模型
    UndoManager* _undoManager;  ///< 撤销管理器
    
    /**
     * 检查手牌卡牌是否可以交换
     * @param cardId 卡牌ID
     * @return 是否可以交换
     */
    bool canSwapHandCard(int cardId);
    
    /**
     * 执行手牌区内部交换
     * @param fromCardId 源卡牌ID
     * @return 是否处理成功
     */
    bool executeHandSwap(int fromCardId);
    
    /**
     * 查找卡牌在手牌区中的索引
     * @param cardId 卡牌ID
     * @return 索引，未找到返回-1
     */
    int findStackCardIndex(int cardId);
};

#endif // __STACK_CONTROLLER_H__
