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

#include "LevelConfigLoader.h"
#include "external/json/document.h"
#include "external/json/filereadstream.h"
#include <cstdio>

LevelConfig* LevelConfigLoader::loadLevelConfig(const std::string& levelId)
{
    std::string filePath = "level" + levelId + ".json";
    return loadLevelConfigFromFile(filePath);
}

LevelConfig* LevelConfigLoader::loadLevelConfigFromFile(const std::string& filePath)
{
    // 获取文件完整路径
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    
    // 读取文件内容
    std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
    if (content.empty()) {
        cocos2d::log("LevelConfigLoader: Failed to read file %s", filePath.c_str());
        return nullptr;
    }
    
    return parseJsonToLevelConfig(content);
}

LevelConfig* LevelConfigLoader::parseJsonToLevelConfig(const std::string& jsonData)
{
    using namespace rapidjson;
    
    Document document;
    document.Parse(jsonData.c_str());
    
    if (document.HasParseError() || !document.IsObject()) {
        cocos2d::log("LevelConfigLoader: Failed to parse JSON data");
        return nullptr;
    }
    
    LevelConfig* config = new LevelConfig();
    
    // 解析主牌堆配置
    if (document.HasMember("MainPile") && document["MainPile"].IsArray()) {
        const Value& mainPileArray = document["MainPile"];
        for (auto it = mainPileArray.Begin(); it != mainPileArray.End(); ++it) {
            if (it->IsObject() && 
                it->HasMember("CardFace") && it->HasMember("CardSuit") && it->HasMember("Position")) {
                
                int cardFace = (*it)["CardFace"].GetInt();
                int cardSuit = (*it)["CardSuit"].GetInt();
                
                const Value& pos = (*it)["Position"];
                if (pos.IsObject() && pos.HasMember("x") && pos.HasMember("y")) {
                    float x = pos["x"].GetFloat();
                    float y = pos["y"].GetFloat();
                    
                    LevelConfig::CardConfig cardConfig(cardFace, cardSuit, cocos2d::Vec2(x, y));
                    config->addMainPileCard(cardConfig);
                }
            }
        }
    }
    
    // 解析底牌堆配置
    if (document.HasMember("BottomPile") && document["BottomPile"].IsArray()) {
        const Value& bottomPileArray = document["BottomPile"];
        for (auto it = bottomPileArray.Begin(); it != bottomPileArray.End(); ++it) {
            if (it->IsObject() && 
                it->HasMember("CardFace") && it->HasMember("CardSuit") && it->HasMember("Position")) {
                
                int cardFace = (*it)["CardFace"].GetInt();
                int cardSuit = (*it)["CardSuit"].GetInt();
                
                const Value& pos = (*it)["Position"];
                if (pos.IsObject() && pos.HasMember("x") && pos.HasMember("y")) {
                    float x = pos["x"].GetFloat();
                    float y = pos["y"].GetFloat();
                    
                    LevelConfig::CardConfig cardConfig(cardFace, cardSuit, cocos2d::Vec2(x, y));
                    config->addBottomPileCard(cardConfig);
                }
            }
        }
    }
    
    // 解析备用牌堆配置
    if (document.HasMember("ReservePile") && document["ReservePile"].IsArray()) {
        const Value& reservePileArray = document["ReservePile"];
        for (auto it = reservePileArray.Begin(); it != reservePileArray.End(); ++it) {
            if (it->IsObject() && 
                it->HasMember("CardFace") && it->HasMember("CardSuit") && it->HasMember("Position")) {
                
                int cardFace = (*it)["CardFace"].GetInt();
                int cardSuit = (*it)["CardSuit"].GetInt();
                
                const Value& pos = (*it)["Position"];
                if (pos.IsObject() && pos.HasMember("x") && pos.HasMember("y")) {
                    float x = pos["x"].GetFloat();
                    float y = pos["y"].GetFloat();
                    
                    LevelConfig::CardConfig cardConfig(cardFace, cardSuit, cocos2d::Vec2(x, y));
                    config->addReservePileCard(cardConfig);
                }
            }
        }
    }
    
    // 验证配置有效性
    if (!config->isValid()) {
        delete config;
        cocos2d::log("LevelConfigLoader: Invalid level configuration");
        return nullptr;
    }
    
    return config;
}
