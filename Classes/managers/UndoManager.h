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

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "../models/UndoModel.h"
#include "../models/GameModel.h"
#include <functional>

/**
 * 撤销管理器
 * 职责：管理撤销功能，持有撤销数据并对撤销数据进行加工
 * 使用场景：作为控制器的成员变量，提供撤销功能的管理
 */
class UndoManager
{
public:
    /**
     * 撤销完成回调函数类型
     * @param success 是否成功
     */
    typedef std::function<void(bool success)> UndoCompleteCallback;
    
    /**
     * 构造函数
     */
    UndoManager();
    
    /**
     * 析构函数
     */
    ~UndoManager();
    
    /**
     * 初始化撤销管理器
     * @param gameModel 游戏模型
     * @param callback 撤销完成回调函数
     */
    void init(GameModel* gameModel, UndoCompleteCallback callback = nullptr);
    
    /**
     * 添加撤销记录
     * @param record 撤销记录
     */
    void addUndoRecord(const UndoModel::UndoRecord& record);
    
    /**
     * 执行撤销操作
     * @return 是否成功
     */
    bool executeUndo();
    
    /**
     * 检查是否有可撤销的操作
     * @return 是否有可撤销的操作
     */
    bool hasUndoableAction() const;
    
    /**
     * 清空所有撤销记录
     */
    void clearAllRecords();
    
    /**
     * 获取撤销记录数量
     * @return 撤销记录数量
     */
    int getRecordCount() const;
    
    /**
     * 设置撤销完成回调函数
     * @param callback 撤销完成回调函数
     */
    void setUndoCompleteCallback(UndoCompleteCallback callback) { _undoCompleteCallback = callback; }

private:
    UndoModel* _undoModel;              ///< 撤销数据模型
    GameModel* _gameModel;              ///< 游戏数据模型
    UndoCompleteCallback _undoCompleteCallback; ///< 撤销完成回调函数
    
    /**
     * 通知撤销完成
     * @param success 是否成功
     */
    void notifyUndoComplete(bool success);
};

#endif // __UNDO_MANAGER_H__
