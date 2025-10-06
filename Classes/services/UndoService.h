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

#ifndef __UNDO_SERVICE_H__
#define __UNDO_SERVICE_H__

#include "../models/UndoModel.h"
#include "../models/GameModel.h"

/**
 * 撤销服务
 * 职责：提供撤销相关的业务逻辑服务，不管理数据生命周期
 * 使用场景：处理撤销操作的业务逻辑，验证撤销条件等
 */
class UndoService
{
public:
    /**
     * 创建手牌交换撤销记录
     * @param gameModel 游戏模型
     * @param fromCardId 源卡牌ID
     * @param toCardId 目标卡牌ID
     * @return 撤销记录
     */
    static UndoModel::UndoRecord createHandSwapRecord(const GameModel* gameModel, 
                                                      int fromCardId, int toCardId);
    
    /**
     * 创建桌面到手牌撤销记录
     * @param gameModel 游戏模型
     * @param playfieldCardId 桌面卡牌ID
     * @param stackCardId 手牌卡牌ID
     * @return 撤销记录
     */
    static UndoModel::UndoRecord createPlayfieldToHandRecord(const GameModel* gameModel,
                                                             int playfieldCardId, int stackCardId);
    
    /**
     * 验证撤销操作是否有效
     * @param gameModel 游戏模型
     * @param record 撤销记录
     * @return 是否有效
     */
    static bool validateUndoRecord(const GameModel* gameModel, const UndoModel::UndoRecord& record);
    
    /**
     * 执行撤销操作
     * @param gameModel 游戏模型
     * @param record 撤销记录
     * @return 是否成功
     */
    static bool executeUndo(GameModel* gameModel, const UndoModel::UndoRecord& record);

private:
    /**
     * 查找卡牌在桌面牌区中的索引
     * @param gameModel 游戏模型
     * @param cardId 卡牌ID
     * @return 索引，未找到返回-1
     */
    static int findPlayfieldCardIndex(const GameModel* gameModel, int cardId);
    
    /**
     * 查找卡牌在手牌区中的索引
     * @param gameModel 游戏模型
     * @param cardId 卡牌ID
     * @return 索引，未找到返回-1
     */
    static int findStackCardIndex(const GameModel* gameModel, int cardId);
};

#endif // __UNDO_SERVICE_H__
