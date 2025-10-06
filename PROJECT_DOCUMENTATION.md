# 扑克游戏项目文档

## 项目概述

这是一个基于 Cocos2d-x 框架开发的扑克游戏项目，采用 MVC（Model-View-Controller）架构模式，实现了完整的游戏逻辑、用户界面和状态管理功能。

## 技术栈

- **游戏引擎**: Cocos2d-x 3.x
- **编程语言**: C++11
- **架构模式**: MVC (Model-View-Controller)
- **JSON解析**: RapidJSON
- **内存管理**: Cocos2d-x 自动引用计数

## 项目结构

```
PokerGame/
├── Classes/                          # 主要源代码目录
│   ├── AppDelegate.h/.cpp            # 应用程序委托
│   ├── TestScene.h/.cpp              # 主测试场景
│   ├── HelloWorldScene.h/.cpp        # Hello World 场景
│   │
│   ├── models/                       # 数据模型层 (Model)
│   │   ├── GameModel.h/.cpp          # 游戏主数据模型
│   │   ├── CardModel.h/.cpp          # 卡牌数据模型
│   │   └── UndoModel.h/.cpp          # 撤销操作数据模型
│   │
│   ├── views/                        # 视图层 (View)
│   │   ├── GameView.h/.cpp           # 游戏主视图
│   │   ├── CardView.h/.cpp           # 单张卡牌视图
│   │   ├── MainPileView.h/.cpp       # 主牌堆视图
│   │   ├── BottomPileView.h/.cpp     # 底牌堆视图
│   │   ├── ReservePileView.h/.cpp    # 备用牌堆视图
│   │   ├── StackView.h/.cpp          # 牌堆视图
│   │   └── PlayFieldView.h/.cpp      # 游戏场地视图
│   │
│   ├── controllers/                  # 控制器层 (Controller)
│   │   ├── GameController.h/.cpp     # 游戏主控制器
│   │   ├── PlayFieldController.h/.cpp # 游戏场地控制器
│   │   └── StackController.h/.cpp    # 牌堆控制器
│   │
│   ├── managers/                     # 管理器层
│   │   ├── GameStateManager.h/.cpp   # 游戏状态管理器
│   │   └── UndoManager.h/.cpp        # 撤销管理器
│   │
│   ├── services/                     # 服务层
│   │   ├── GameModelFromLevelGenerator.h/.cpp # 关卡生成服务
│   │   └── UndoService.h/.cpp        # 撤销服务
│   │
│   ├── configs/                      # 配置层
│   │   ├── models/
│   │   │   ├── LevelConfig.h/.cpp    # 关卡配置模型
│   │   └── loaders/
│   │       ├── LevelConfigLoader.h/.cpp # 关卡配置加载器
│   │
│   └── utils/                        # 工具类
│       └── CardUtils.h/.cpp          # 卡牌工具类
│
├── proj.win32/                       # Windows 项目文件
├── Resources/                        # 游戏资源文件
└── cocos2d/                          # Cocos2d-x 引擎源码
```

## 核心架构

### MVC 架构模式

#### Model 层 (数据模型)
- **GameModel**: 游戏核心数据模型，管理所有卡牌和游戏状态
- **CardModel**: 单张卡牌的数据模型，包含卡牌属性和状态
- **UndoModel**: 撤销操作的数据模型

#### View 层 (视图)
- **GameView**: 游戏主视图，协调所有子视图
- **CardView**: 单张卡牌的显示和交互
- **各种PileView**: 不同牌堆的显示视图

#### Controller 层 (控制器)
- **GameController**: 游戏主控制器，处理游戏逻辑
- **PlayFieldController**: 游戏场地控制器
- **StackController**: 牌堆控制器

### 设计模式

1. **观察者模式**: 通过回调函数实现视图与模型的解耦
2. **工厂模式**: CardView 的创建使用工厂方法
3. **状态模式**: 游戏状态管理
4. **命令模式**: 撤销/重做功能

## 核心功能模块

### 1. 卡牌系统

#### CardModel (卡牌模型)
```cpp
class CardModel {
    // 卡牌属性
    int _cardId;                    // 卡牌唯一ID
    CardFaceType _face;             // 面值 (A, 2-10, J, Q, K)
    CardSuitType _suit;             // 花色 (♠, ♥, ♦, ♣)
    cocos2d::Vec2 _position;        // 位置
    bool _isRevealed;               // 是否翻开
    bool _isClickable;              // 是否可点击
    
    // 核心方法
    std::string getCardText() const;    // 获取卡牌文本
    bool isRed() const;                 // 是否为红色
    void setPosition(const Vec2& pos);  // 设置位置
};
```

#### CardView (卡牌视图)
```cpp
class CardView : public cocos2d::Node {
    // 显示和交互
    void createCardSprite(const CardModel* cardModel);
    void playMoveAnimation(const Vec2& targetPosition, float duration);
    void playFlipAnimation(std::function<void()> callback);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    // 状态管理
    void setClickable(bool clickable);
    void setRevealed(bool revealed);
};
```

### 2. 游戏状态管理

#### GameModel (游戏模型)
```cpp
class GameModel {
    // 牌堆管理
    std::vector<CardModel*> _mainPileCards;      // 主牌堆
    std::vector<CardModel*> _bottomPileCards;    // 底牌堆
    std::vector<CardModel*> _reservePileCards;   // 备用牌堆
    std::vector<std::vector<CardModel*>> _stacks; // 桌面牌堆
    
    // 游戏状态
    int _score;                                  // 分数
    int _moves;                                  // 移动次数
    bool _isGameOver;                            // 游戏是否结束
    
    // 核心方法
    bool canMoveCard(int cardId, int targetStackId);
    void moveCard(int cardId, int targetStackId);
    bool checkWinCondition();
};
```

### 3. 撤销/重做系统

#### GameStateManager (游戏状态管理器)
```cpp
class GameStateManager {
    std::vector<std::shared_ptr<GameState>> _undoStack;  // 撤销栈
    std::vector<std::shared_ptr<GameState>> _redoStack;  // 重做栈
    
    // 状态管理
    void saveState(const GameModel* gameModel);
    bool undo(GameModel* gameModel);
    bool redo(GameModel* gameModel);
    void clearHistory();
};
```

### 4. 关卡系统

#### LevelConfig (关卡配置)
```cpp
class LevelConfig {
    std::string _levelName;                      // 关卡名称
    int _levelId;                                // 关卡ID
    std::vector<CardPosition> _mainPileCards;    // 主牌堆配置
    std::vector<CardPosition> _bottomPileCards;  // 底牌堆配置
    std::vector<CardPosition> _reservePileCards; // 备用牌堆配置
    std::vector<StackConfig> _stacks;            // 桌面牌堆配置
};
```

## 关键设计决策

### 1. 内存管理
- 使用 Cocos2d-x 的自动引用计数系统
- 智能指针管理复杂对象生命周期
- 避免内存泄漏和野指针

### 2. 事件处理
- 使用回调函数实现松耦合
- 触摸事件通过 Cocos2d-x 事件系统处理
- 支持事件冒泡和捕获

### 3. 动画系统
- 基于 Cocos2d-x 的 Action 系统
- 支持动画链和回调
- 流畅的用户体验

### 4. 数据持久化
- JSON 格式存储关卡配置
- 支持游戏状态保存和恢复
- 跨平台兼容性

## 编译和构建

### 环境要求
- Visual Studio 2015 或更高版本
- Windows 10 SDK
- Cocos2d-x 3.x

### 构建步骤
1. 打开 `proj.win32/PokerGame.sln`
2. 选择 Debug 或 Release 配置
3. 编译解决方案
4. 运行项目

### 依赖库
- Cocos2d-x 引擎
- RapidJSON (JSON 解析)
- Windows API

## 扩展性设计

### 1. 新卡牌类型
- 继承 CardModel 基类
- 实现特定的卡牌逻辑
- 添加对应的 CardView 显示

### 2. 新游戏模式
- 扩展 GameModel 接口
- 实现新的游戏规则
- 添加对应的控制器

### 3. 新关卡类型
- 扩展 LevelConfig 结构
- 实现新的关卡加载器
- 添加关卡验证逻辑

## 性能优化

### 1. 渲染优化
- 使用对象池减少内存分配
- 批量渲染减少绘制调用
- 纹理图集优化

### 2. 内存优化
- 及时释放不需要的资源
- 使用智能指针管理内存
- 避免循环引用

### 3. 逻辑优化
- 缓存频繁计算的结果
- 使用事件驱动减少轮询
- 异步加载资源

## 测试策略

### 1. 单元测试
- 模型层逻辑测试
- 工具类功能测试
- 边界条件测试

### 2. 集成测试
- 视图与模型交互测试
- 控制器逻辑测试
- 完整游戏流程测试

### 3. 性能测试
- 内存使用监控
- 帧率性能测试
- 长时间运行稳定性测试

## 已知问题和限制

### 1. 当前限制
- 仅支持 Windows 平台
- 关卡数量有限
- 部分动画效果待优化

### 2. 待改进项
- 添加音效和背景音乐
- 实现更多游戏模式
- 优化移动端适配
- 添加网络对战功能

## 贡献指南

### 1. 代码规范
- 遵循 C++11 标准
- 使用统一的命名约定
- 添加必要的注释和文档

### 2. 提交规范
- 使用清晰的提交信息
- 一个提交只做一件事
- 确保代码可以编译通过

### 3. 测试要求
- 新功能必须包含测试
- 修复 bug 需要回归测试
- 性能敏感代码需要性能测试

---

**项目维护者**: [LI RUNLONG]  
**最后更新**: 2025年10月  
**版本**: 1.0.0
