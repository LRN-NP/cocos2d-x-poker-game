# 扑克游戏项目类关系图

## 整体架构图

```
┌─────────────────────────────────────────────────────────────────┐
│                        Cocos2d-x 框架层                        │
├─────────────────────────────────────────────────────────────────┤
│  AppDelegate  │  TestScene  │  HelloWorldScene  │  Director     │
└─────────────────────────────────────────────────────────────────┘
                                │
┌─────────────────────────────────────────────────────────────────┐
│                        视图层 (View)                           │
├─────────────────────────────────────────────────────────────────┤
│  GameView  │  CardView  │  MainPileView  │  BottomPileView     │
│            │            │                │                     │
│  ReservePileView  │  StackView  │  PlayFieldView              │
└─────────────────────────────────────────────────────────────────┘
                                │
┌─────────────────────────────────────────────────────────────────┐
│                      控制器层 (Controller)                      │
├─────────────────────────────────────────────────────────────────┤
│  GameController  │  PlayFieldController  │  StackController    │
└─────────────────────────────────────────────────────────────────┘
                                │
┌─────────────────────────────────────────────────────────────────┐
│                        模型层 (Model)                          │
├─────────────────────────────────────────────────────────────────┤
│  GameModel  │  CardModel  │  UndoModel  │  LevelConfig         │
└─────────────────────────────────────────────────────────────────┘
                                │
┌─────────────────────────────────────────────────────────────────┐
│                      服务层 (Service)                          │
├─────────────────────────────────────────────────────────────────┤
│  GameStateManager  │  UndoManager  │  GameModelFromLevelGenerator │
│                    │               │                            │
│  UndoService       │  LevelConfigLoader                        │
└─────────────────────────────────────────────────────────────────┘
                                │
┌─────────────────────────────────────────────────────────────────┐
│                        工具层 (Utils)                          │
├─────────────────────────────────────────────────────────────────┤
│  CardUtils  │  Cocos2d-x Utils  │  RapidJSON                   │
└─────────────────────────────────────────────────────────────────┘
```

## 核心类详细关系

### 1. 模型层 (Model Layer)

#### CardModel - 卡牌模型
```
CardModel
├── 属性
│   ├── int _cardId              // 卡牌唯一标识
│   ├── CardFaceType _face       // 面值 (A, 2-10, J, Q, K)
│   ├── CardSuitType _suit       // 花色 (♠, ♥, ♦, ♣)
│   ├── Vec2 _position           // 位置坐标
│   ├── bool _isRevealed         // 是否翻开
│   └── bool _isClickable        // 是否可点击
├── 方法
│   ├── getCardText()            // 获取卡牌文本表示
│   ├── isRed()                  // 判断是否为红色
│   ├── setPosition()            // 设置位置
│   └── serialize()              // 序列化
└── 关系
    └── 被 GameModel 包含
```

#### GameModel - 游戏模型
```
GameModel
├── 属性
│   ├── vector<CardModel*> _mainPileCards      // 主牌堆
│   ├── vector<CardModel*> _bottomPileCards    // 底牌堆
│   ├── vector<CardModel*> _reservePileCards   // 备用牌堆
│   ├── vector<vector<CardModel*>> _stacks     // 桌面牌堆
│   ├── int _score                             // 分数
│   ├── int _moves                             // 移动次数
│   └── bool _isGameOver                       // 游戏结束标志
├── 方法
│   ├── canMoveCard()                          // 检查是否可以移动卡牌
│   ├── moveCard()                             // 移动卡牌
│   ├── checkWinCondition()                    // 检查胜利条件
│   ├── getMainPileCards()                     // 获取主牌堆
│   ├── getBottomPileCards()                   // 获取底牌堆
│   └── getReservePileCards()                  // 获取备用牌堆
└── 关系
    ├── 包含多个 CardModel
    ├── 被 GameController 使用
    └── 被 GameView 观察
```

### 2. 视图层 (View Layer)

#### CardView - 卡牌视图
```
CardView : public cocos2d::Node
├── 属性
│   ├── const CardModel* _cardModel            // 关联的卡牌模型
│   ├── CardClickCallback _clickCallback       // 点击回调
│   ├── Sprite* _cardSprite                    // 卡牌精灵
│   ├── Label* _cardLabel                      // 卡牌标签
│   ├── bool _isClickable                      // 是否可点击
│   └── bool _isRevealed                       // 是否翻开
├── 方法
│   ├── create()                               // 静态创建方法
│   ├── init()                                 // 初始化
│   ├── playMoveAnimation()                    // 播放移动动画
│   ├── playFlipAnimation()                    // 播放翻转动画
│   ├── onTouchBegan()                         // 触摸事件处理
│   ├── setClickable()                         // 设置可点击状态
│   └── setRevealed()                          // 设置翻开状态
└── 关系
    ├── 观察 CardModel
    ├── 被各种 PileView 包含
    └── 继承自 cocos2d::Node
```

#### GameView - 游戏主视图
```
GameView : public cocos2d::Layer
├── 属性
│   ├── const GameModel* _gameModel            // 游戏模型
│   ├── MainPileView* _mainPileView            // 主牌堆视图
│   ├── BottomPileView* _bottomPileView        // 底牌堆视图
│   ├── ReservePileView* _reservePileView      // 备用牌堆视图
│   ├── ui::Button* _undoButton                // 撤销按钮
│   ├── UndoButtonCallback _undoCallback       // 撤销回调
│   ├── CardClickCallback _cardClickCallback   // 卡牌点击回调
│   └── DrawCardCallback _drawCardCallback     // 抽卡回调
├── 方法
│   ├── create()                               // 静态创建方法
│   ├── init()                                 // 初始化
│   ├── updateDisplay()                        // 更新显示
│   ├── setupLayout()                          // 设置布局
│   ├── onCardClicked()                        // 卡牌点击处理
│   ├── onUndoButtonClicked()                  // 撤销按钮点击
│   └── playCardMoveAnimation()                // 播放卡牌移动动画
└── 关系
    ├── 观察 GameModel
    ├── 包含多个子视图
    ├── 被 GameController 控制
    └── 继承自 cocos2d::Layer
```

### 3. 控制器层 (Controller Layer)

#### GameController - 游戏主控制器
```
GameController
├── 属性
│   ├── GameModel* _gameModel                  // 游戏模型
│   ├── GameView* _gameView                    // 游戏视图
│   ├── GameStateManager* _stateManager        // 状态管理器
│   └── UndoManager* _undoManager              // 撤销管理器
├── 方法
│   ├── init()                                 // 初始化
│   ├── startGame()                            // 开始游戏
│   ├── endGame()                              // 结束游戏
│   ├── onCardClicked()                        // 卡牌点击处理
│   ├── onUndoButtonClicked()                  // 撤销按钮处理
│   ├── onDrawCardButtonClicked()              // 抽卡按钮处理
│   ├── checkGameOver()                        // 检查游戏结束
│   └── updateScore()                          // 更新分数
└── 关系
    ├── 控制 GameModel
    ├── 控制 GameView
    ├── 使用 GameStateManager
    └── 使用 UndoManager
```

### 4. 管理器层 (Manager Layer)

#### GameStateManager - 游戏状态管理器
```
GameStateManager
├── 属性
│   ├── vector<shared_ptr<GameState>> _undoStack  // 撤销栈
│   └── vector<shared_ptr<GameState>> _redoStack  // 重做栈
├── 方法
│   ├── saveState()                             // 保存状态
│   ├── undo()                                  // 撤销
│   ├── redo()                                  // 重做
│   ├── canUndo()                               // 是否可以撤销
│   ├── canRedo()                               // 是否可以重做
│   └── clearHistory()                          // 清除历史
└── 关系
    ├── 管理 GameState
    ├── 被 GameController 使用
    └── 与 UndoService 协作
```

### 5. 服务层 (Service Layer)

#### GameModelFromLevelGenerator - 关卡生成服务
```
GameModelFromLevelGenerator
├── 方法
│   ├── generateFromLevel()                     // 从关卡生成游戏模型
│   ├── randomizeCardPositions()                // 随机化卡牌位置
│   ├── createCardFromConfig()                  // 从配置创建卡牌
│   └── validateLevelConfig()                   // 验证关卡配置
└── 关系
    ├── 使用 LevelConfig
    ├── 创建 GameModel
    └── 被 TestScene 使用
```

## 数据流图

### 1. 游戏初始化流程
```
TestScene
    ↓
GameController::init()
    ↓
GameModelFromLevelGenerator::generateFromLevel()
    ↓
GameModel (创建)
    ↓
GameView::create()
    ↓
各种 PileView::create()
    ↓
CardView::create() (为每张卡牌)
```

### 2. 卡牌点击处理流程
```
CardView::onTouchBegan()
    ↓
CardClickCallback
    ↓
GameView::onCardClicked()
    ↓
GameController::onCardClicked()
    ↓
GameModel::canMoveCard() + moveCard()
    ↓
GameView::updateDisplay()
    ↓
CardView::playMoveAnimation()
```

### 3. 撤销操作流程
```
UndoButton::onClick()
    ↓
GameView::onUndoButtonClicked()
    ↓
GameController::onUndoButtonClicked()
    ↓
GameStateManager::undo()
    ↓
GameModel (恢复状态)
    ↓
GameView::updateDisplay()
```

## 设计模式应用

### 1. MVC 模式
- **Model**: GameModel, CardModel, UndoModel
- **View**: GameView, CardView, 各种 PileView
- **Controller**: GameController, PlayFieldController, StackController

### 2. 观察者模式
- CardView 观察 CardModel 的变化
- GameView 观察 GameModel 的变化
- 通过回调函数实现松耦合

### 3. 工厂模式
- CardView::create() 静态工厂方法
- GameView::create() 静态工厂方法
- 各种 PileView 的创建

### 4. 命令模式
- 撤销/重做操作
- 卡牌移动操作
- 游戏状态变更操作

### 5. 状态模式
- 游戏状态管理
- 卡牌状态管理
- 视图状态管理

## 内存管理策略

### 1. Cocos2d-x 自动引用计数
- 所有继承自 cocos2d::Ref 的对象
- 使用 autorelease() 自动管理
- 避免手动 delete

### 2. 智能指针
- std::shared_ptr 管理复杂对象
- std::unique_ptr 管理独占资源
- 避免内存泄漏

### 3. 对象池模式
- 卡牌视图对象池
- 动画对象池
- 减少频繁创建销毁

## 扩展点设计

### 1. 新卡牌类型
- 继承 CardModel 基类
- 实现特定的卡牌逻辑
- 添加对应的 CardView

### 2. 新游戏模式
- 扩展 GameModel 接口
- 实现新的游戏规则
- 添加对应的控制器

### 3. 新关卡类型
- 扩展 LevelConfig 结构
- 实现新的关卡加载器
- 添加关卡验证逻辑

---

这个类关系图展示了项目的整体架构和各个类之间的关系，有助于理解代码结构和设计思路。
