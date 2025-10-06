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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "controllers/GameController.h"

/**
 * 重构后的HelloWorld场景
 * 职责：作为游戏入口场景，使用新的架构模式
 * 使用场景：游戏启动时的主场景
 */
class HelloWorld : public cocos2d::Scene
{
public:
    /**
     * 创建场景
     * @return 场景实例
     */
    static cocos2d::Scene* createScene();

    /**
     * 初始化场景
     * @return 是否初始化成功
     */
    virtual bool init();
    
    /**
     * 关闭按钮回调
     * @param pSender 发送者
     */
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    /**
     * 实现静态create()方法
     */
    CREATE_FUNC(HelloWorld);

private:
    GameController* _gameController; ///< 游戏控制器
    
    /**
     * 初始化游戏控制器
     * @return 是否初始化成功
     */
    bool initGameController();
};

#endif // __HELLOWORLD_SCENE_H__
