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

#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include <vector>

/**
 * 撤销操作类型枚举
 */
enum UndoActionType
{
    UAT_NONE = 0,
    UAT_HAND_SWAP,          ///< 手牌区内部交换
    UAT_PLAYFIELD_TO_HAND   ///< 桌面牌区到手牌区
};

/**
 * 撤销数据模型
 * 职责：存储撤销操作所需的数据，支持撤销功能的实现
 * 使用场景：每次操作前记录状态，撤销时恢复状态
 */
class UndoModel
{
public:
    /**
     * 撤销记录结构
     */
    struct UndoRecord
    {
        UndoActionType actionType;           ///< 操作类型
        int sourceCardId;                    ///< 源卡牌ID
        int targetCardId;                    ///< 目标卡牌ID
        cocos2d::Vec2 sourcePosition;        ///< 源位置
        cocos2d::Vec2 targetPosition;        ///< 目标位置
        int handTopIndex;                    ///< 手牌区顶部索引
        int playfieldIndex;                  ///< 桌面牌区索引
        int stackIndex;                      ///< 手牌区索引
        
        UndoRecord()
            : actionType(UAT_NONE)
            , sourceCardId(-1)
            , targetCardId(-1)
            , sourcePosition(cocos2d::Vec2::ZERO)
            , targetPosition(cocos2d::Vec2::ZERO)
            , handTopIndex(-1)
            , playfieldIndex(-1)
            , stackIndex(-1)
        {}
    };
    
    /**
     * 构造函数
     */
    UndoModel();
    
    /**
     * 析构函数
     */
    ~UndoModel();
    
    /**
     * 添加撤销记录
     * @param record 撤销记录
     */
    void addUndoRecord(const UndoRecord& record);
    
    /**
     * 获取最后一个撤销记录
     * @return 撤销记录，无记录返回空记录
     */
    UndoRecord getLastUndoRecord();
    
    /**
     * 移除最后一个撤销记录
     */
    void removeLastUndoRecord();
    
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
    int getRecordCount() const { return (int)_undoRecords.size(); }
    
    /**
     * 序列化撤销数据
     * @return 序列化后的数据
     */
    std::string serialize() const;
    
    /**
     * 反序列化撤销数据
     * @param data 序列化数据
     * @return 是否成功
     */
    bool deserialize(const std::string& data);

private:
    std::vector<UndoRecord> _undoRecords; ///< 撤销记录列表
};

#endif // __UNDO_MODEL_H__
