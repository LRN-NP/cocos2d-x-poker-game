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

#include "GameController.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../services/GameModelFromLevelGenerator.h"

GameController::GameController()
    : _parent(nullptr)
    , _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr)
    , _playFieldController(nullptr)
    , _stackController(nullptr)
{
}

GameController::~GameController()
{
    if (_gameModel) {
        delete _gameModel;
        _gameModel = nullptr;
    }
    
    if (_undoManager) {
        delete _undoManager;
        _undoManager = nullptr;
    }
    
    if (_playFieldController) {
        delete _playFieldController;
        _playFieldController = nullptr;
    }
    
    if (_stackController) {
        delete _stackController;
        _stackController = nullptr;
    }
}

bool GameController::init(cocos2d::Node* parent)
{
    if (!parent) {
        return false;
    }
    
    _parent = parent;
    
    // 初始化撤销管理器
    _undoManager = new UndoManager();
    
    // 初始化子控制器
    if (!initSubControllers()) {
        return false;
    }
    
    // 初始化游戏视图
    if (!initGameView()) {
        return false;
    }
    
    return true;
}

bool GameController::startGame(const std::string& levelId)
{
    // 加载关卡配置
    LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (!levelConfig) {
        cocos2d::log("GameController: Failed to load level config for level %s", levelId.c_str());
        return false;
    }
    
    // 生成游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    if (!_gameModel) {
        cocos2d::log("GameController: Failed to generate game model");
        delete levelConfig;
        return false;
    }
    
    // 初始化撤销管理器
    _undoManager->init(_gameModel, [this](bool success) {
        onUndoComplete(success);
    });
    
    // 初始化子控制器
    _playFieldController->init(_gameModel, _undoManager);
    _stackController->init(_gameModel, _undoManager);
    
    // 更新游戏视图
    updateGameView();
    
    // 播放入场动画
    if (_gameView) {
        _gameView->playEnterAnimation();
    }
    
    delete levelConfig;
    return true;
}

bool GameController::handleCardClick(int cardId)
{
    if (!_gameModel) {
        return false;
    }
    
    // 检查是否在桌面牌区
    CardModel* playfieldCard = _gameModel->findMainPileCard(cardId);
    if (playfieldCard) {
        onPlayFieldCardClicked(cardId);
        return true;
    }
    
    // 检查是否在手牌区
    CardModel* stackCard = _gameModel->findBottomPileCard(cardId);
    if (stackCard) {
        onStackCardClicked(cardId);
        return true;
    }
    
    return false;
}

bool GameController::handleUndo()
{
    if (!_undoManager) {
        return false;
    }
    
    return _undoManager->executeUndo();
}

bool GameController::isGameOver() const
{
    return _gameModel ? _gameModel->isGameOver() : false;
}

bool GameController::initSubControllers()
{
    _playFieldController = new PlayFieldController();
    _stackController = new StackController();
    
    return _playFieldController != nullptr && _stackController != nullptr;
}

bool GameController::initGameView()
{
    _gameView = GameView::create(nullptr, [this]() {
        handleUndo();
    });
    
    if (!_gameView) {
        return false;
    }
    
    _parent->addChild(_gameView);
    return true;
}

void GameController::onPlayFieldCardClicked(int cardId)
{
    if (_playFieldController) {
        _playFieldController->handleCardClick(cardId);
        updateGameView();
    }
}

void GameController::onStackCardClicked(int cardId)
{
    if (_stackController) {
        _stackController->handleCardClick(cardId);
        updateGameView();
    }
}

void GameController::onUndoComplete(bool success)
{
    if (success) {
        updateGameView();
    }
}

void GameController::updateGameView()
{
    if (_gameView && _gameModel) {
        _gameView->updateGame(_gameModel);
        
        // 更新撤销按钮状态
        bool hasUndoableAction = _undoManager ? _undoManager->hasUndoableAction() : false;
        _gameView->setUndoButtonEnabled(hasUndoableAction);
    }
}
