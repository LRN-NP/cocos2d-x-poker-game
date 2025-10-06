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

#include "UndoManager.h"
#include "../services/UndoService.h"

UndoManager::UndoManager()
    : _undoModel(nullptr)
    , _gameModel(nullptr)
    , _undoCompleteCallback(nullptr)
{
}

UndoManager::~UndoManager()
{
    if (_undoModel) {
        delete _undoModel;
        _undoModel = nullptr;
    }
}

void UndoManager::init(GameModel* gameModel, UndoCompleteCallback callback)
{
    _gameModel = gameModel;
    _undoCompleteCallback = callback;
    
    if (!_undoModel) {
        _undoModel = new UndoModel();
    }
}

void UndoManager::addUndoRecord(const UndoModel::UndoRecord& record)
{
    if (_undoModel) {
        _undoModel->addUndoRecord(record);
    }
}

bool UndoManager::executeUndo()
{
    if (!_undoModel || !_gameModel) {
        notifyUndoComplete(false);
        return false;
    }
    
    if (!_undoModel->hasUndoableAction()) {
        notifyUndoComplete(false);
        return false;
    }
    
    UndoModel::UndoRecord record = _undoModel->getLastUndoRecord();
    _undoModel->removeLastUndoRecord();
    
    bool success = UndoService::executeUndo(_gameModel, record);
    notifyUndoComplete(success);
    return success;
}

bool UndoManager::hasUndoableAction() const
{
    return _undoModel ? _undoModel->hasUndoableAction() : false;
}

void UndoManager::clearAllRecords()
{
    if (_undoModel) {
        _undoModel->clearAllRecords();
    }
}

int UndoManager::getRecordCount() const
{
    return _undoModel ? _undoModel->getRecordCount() : 0;
}

void UndoManager::notifyUndoComplete(bool success)
{
    if (_undoCompleteCallback) {
        _undoCompleteCallback(success);
    }
}
