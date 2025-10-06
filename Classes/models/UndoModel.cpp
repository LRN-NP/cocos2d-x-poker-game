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

#include "UndoModel.h"
#include <sstream>

UndoModel::UndoModel()
{
}

UndoModel::~UndoModel()
{
}

void UndoModel::addUndoRecord(const UndoRecord& record)
{
    _undoRecords.push_back(record);
}

UndoModel::UndoRecord UndoModel::getLastUndoRecord()
{
    if (_undoRecords.empty()) {
        return UndoRecord();
    }
    return _undoRecords.back();
}

void UndoModel::removeLastUndoRecord()
{
    if (!_undoRecords.empty()) {
        _undoRecords.pop_back();
    }
}

bool UndoModel::hasUndoableAction() const
{
    return !_undoRecords.empty();
}

void UndoModel::clearAllRecords()
{
    _undoRecords.clear();
}

std::string UndoModel::serialize() const
{
    std::ostringstream oss;
    oss << "recordCount:" << _undoRecords.size() << ";";
    
    for (const auto& record : _undoRecords) {
        oss << "actionType:" << (int)record.actionType << ";";
        oss << "sourceCardId:" << record.sourceCardId << ";";
        oss << "targetCardId:" << record.targetCardId << ";";
        oss << "sourcePosition:" << record.sourcePosition.x << "," << record.sourcePosition.y << ";";
        oss << "targetPosition:" << record.targetPosition.x << "," << record.targetPosition.y << ";";
        oss << "handTopIndex:" << record.handTopIndex << ";";
        oss << "playfieldIndex:" << record.playfieldIndex << ";";
        oss << "stackIndex:" << record.stackIndex << ";";
        oss << "---"; // 记录分隔符
    }
    
    return oss.str();
}

bool UndoModel::deserialize(const std::string& data)
{
    clearAllRecords();
    
    std::istringstream iss(data);
    std::string line;
    
    // 读取记录数量
    if (std::getline(iss, line, ';') && line.find("recordCount:") == 0) {
        int recordCount = std::stoi(line.substr(12));
        
        for (int i = 0; i < recordCount; ++i) {
            UndoRecord record;
            std::string recordData;
            
            // 读取一条记录
            while (std::getline(iss, line, ';') && line != "---") {
                size_t colonPos = line.find(':');
                if (colonPos == std::string::npos) continue;
                
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                
                if (key == "actionType") {
                    record.actionType = (UndoActionType)std::stoi(value);
                } else if (key == "sourceCardId") {
                    record.sourceCardId = std::stoi(value);
                } else if (key == "targetCardId") {
                    record.targetCardId = std::stoi(value);
                } else if (key == "sourcePosition") {
                    size_t commaPos = value.find(',');
                    if (commaPos != std::string::npos) {
                        record.sourcePosition.x = std::stof(value.substr(0, commaPos));
                        record.sourcePosition.y = std::stof(value.substr(commaPos + 1));
                    }
                } else if (key == "targetPosition") {
                    size_t commaPos = value.find(',');
                    if (commaPos != std::string::npos) {
                        record.targetPosition.x = std::stof(value.substr(0, commaPos));
                        record.targetPosition.y = std::stof(value.substr(commaPos + 1));
                    }
                } else if (key == "handTopIndex") {
                    record.handTopIndex = std::stoi(value);
                } else if (key == "playfieldIndex") {
                    record.playfieldIndex = std::stoi(value);
                } else if (key == "stackIndex") {
                    record.stackIndex = std::stoi(value);
                }
            }
            
            _undoRecords.push_back(record);
        }
    }
    
    return true;
}
