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

#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"

/**
 * 游戏模型生成服务
 * 职责：将静态配置（LevelConfig）转换为动态运行时数据（GameModel）
 * 使用场景：游戏初始化时，根据关卡配置生成游戏数据模型
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * 从关卡配置生成游戏模型
     * @param levelConfig 关卡配置
     * @return 游戏模型，生成失败返回nullptr
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig);
    
    /**
     * 从关卡配置生成游戏模型（带随机化）
     * @param levelConfig 关卡配置
     * @param randomize 是否随机化卡牌位置
     * @return 游戏模型，生成失败返回nullptr
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig, bool randomize);

private:
    /**
     * 创建卡牌模型
     * @param cardConfig 卡牌配置
     * @return 卡牌模型
     */
    static CardModel* createCardModel(const LevelConfig::CardConfig& cardConfig, const std::string& pileType);
    
    /**
     * 随机化卡牌位置
     * @param gameModel 游戏模型
     * @param levelConfig 关卡配置
     */
    static void randomizeCardPositions(GameModel* gameModel, const LevelConfig* levelConfig);
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
