# 扑克游戏项目 API 文档

## 概述

本文档详细描述了扑克游戏项目中各个类的公共接口、方法签名、参数说明和使用示例。

## 核心类 API

### 1. CardModel - 卡牌模型

#### 构造函数
```cpp
CardModel();
CardModel(int cardId, CardFaceType face, CardSuitType suit, const Vec2& position);
```

#### 公共方法

##### 基本信息获取
```cpp
// 获取卡牌ID
int getCardId() const;

// 获取面值
CardFaceType getFace() const;

// 获取花色
CardSuitType getSuit() const;

// 获取位置
Vec2 getPosition() const;

// 获取卡牌文本表示 (如 "A♠", "10♥")
std::string getCardText() const;
```

##### 状态查询
```cpp
// 是否翻开
bool isRevealed() const;

// 是否可点击
bool isClickable() const;

// 是否为红色 (♥ 或 ♦)
bool isRed() const;
```

##### 状态设置
```cpp
// 设置位置
void setPosition(const Vec2& position);

// 设置是否翻开
void setRevealed(bool revealed);

// 设置是否可点击
void setClickable(bool clickable);
```

##### 序列化
```cpp
// 序列化为字符串 (用于存档)
std::string serialize() const;

// 从字符串反序列化
void deserialize(const std::string& data);
```

#### 使用示例
```cpp
// 创建一张红桃A
CardModel* card = new CardModel(1, CFT_ACE, CST_HEARTS, Vec2(100, 200));
card->setRevealed(true);
card->setClickable(true);

// 检查是否为红色
if (card->isRed()) {
    CCLOG("This is a red card");
}

// 获取卡牌文本
std::string text = card->getCardText(); // "A♥"
```

### 2. GameModel - 游戏模型

#### 构造函数
```cpp
GameModel();
```

#### 公共方法

##### 牌堆管理
```cpp
// 获取主牌堆
const std::vector<CardModel*>& getMainPileCards() const;
std::vector<CardModel*>& getMainPileCards();

// 获取底牌堆
const std::vector<CardModel*>& getBottomPileCards() const;
std::vector<CardModel*>& getBottomPileCards();

// 获取备用牌堆
const std::vector<CardModel*>& getReservePileCards() const;
std::vector<CardModel*>& getReservePileCards();

// 获取桌面牌堆
const std::vector<std::vector<CardModel*>>& getStacks() const;
std::vector<std::vector<CardModel*>>& getStacks();

// 获取指定牌堆的顶部卡牌
CardModel* getReservePileTopCard() const;
```

##### 游戏逻辑
```cpp
// 检查是否可以移动卡牌
bool canMoveCard(int cardId, int targetStackId) const;

// 移动卡牌
void moveCard(int cardId, int targetStackId);

// 检查胜利条件
bool checkWinCondition() const;

// 检查游戏是否结束
bool isGameOver() const;
```

##### 游戏状态
```cpp
// 获取分数
int getScore() const;

// 设置分数
void setScore(int score);

// 获取移动次数
int getMoves() const;

// 增加移动次数
void incrementMoves();
```

##### 卡牌查找
```cpp
// 根据ID查找卡牌
CardModel* findCardById(int cardId) const;

// 查找卡牌所在的牌堆
int findCardStack(int cardId) const;
```

#### 使用示例
```cpp
// 创建游戏模型
GameModel* gameModel = new GameModel();

// 检查是否可以移动卡牌
if (gameModel->canMoveCard(1, 2)) {
    gameModel->moveCard(1, 2);
    gameModel->incrementMoves();
}

// 检查胜利条件
if (gameModel->checkWinCondition()) {
    CCLOG("Game won!");
}
```

### 3. CardView - 卡牌视图

#### 静态创建方法
```cpp
// 创建卡牌视图
static CardView* create(const CardModel* cardModel);
static CardView* create(const CardModel* cardModel, CardClickCallback clickCallback);
```

#### 公共方法

##### 初始化和更新
```cpp
// 初始化
bool init(const CardModel* cardModel, CardClickCallback clickCallback = nullptr);

// 更新卡牌数据
void updateCard(const CardModel* cardModel);
```

##### 信息获取
```cpp
// 获取卡牌ID
int getCardId() const;
```

##### 动画控制
```cpp
// 播放移动动画
void playMoveAnimation(const Vec2& targetPosition, float duration, 
                      std::function<void()> callback = nullptr);

// 播放匹配动画
void playMatchAnimation(std::function<void()> callback = nullptr);

// 播放撤销动画
void playUndoAnimation(const Vec2& targetPosition, float duration,
                      std::function<void()> callback = nullptr);

// 播放翻转动画
void playFlipAnimation(std::function<void()> callback = nullptr);
```

##### 状态控制
```cpp
// 设置卡牌可见性
void setCardVisible(bool visible);

// 设置是否可点击
void setClickable(bool clickable);

// 设置是否翻开
void setRevealed(bool revealed);
```

#### 使用示例
```cpp
// 创建卡牌视图
CardView* cardView = CardView::create(cardModel, [](int cardId) {
    CCLOG("Card %d clicked", cardId);
});

// 播放移动动画
cardView->playMoveAnimation(Vec2(200, 300), 1.0f, []() {
    CCLOG("Move animation completed");
});

// 设置状态
cardView->setClickable(true);
cardView->setRevealed(true);
```

### 4. GameView - 游戏主视图

#### 静态创建方法
```cpp
// 创建游戏视图
static GameView* create(const GameModel* gameModel, UndoButtonCallback undoCallback);
```

#### 公共方法

##### 初始化和更新
```cpp
// 初始化
bool init(const GameModel* gameModel, UndoButtonCallback undoCallback);

// 更新显示
void updateDisplay();
```

##### 事件处理
```cpp
// 设置卡牌点击回调
void setCardClickCallback(CardClickCallback callback);

// 设置抽卡回调
void setDrawCardCallback(DrawCardCallback callback);
```

##### 动画控制
```cpp
// 播放卡牌移动动画
void playCardMoveAnimation(int cardId, const Vec2& targetPosition, 
                          float duration, std::function<void()> callback = nullptr);

// 播放进入动画
void playEnterAnimation(std::function<void()> callback = nullptr);

// 播放退出动画
void playExitAnimation(std::function<void()> callback = nullptr);
```

#### 使用示例
```cpp
// 创建游戏视图
GameView* gameView = GameView::create(gameModel, [](int cardId) {
    CCLOG("Undo button clicked");
});

// 设置回调
gameView->setCardClickCallback([](int cardId) {
    CCLOG("Card %d clicked", cardId);
});

// 播放动画
gameView->playCardMoveAnimation(1, Vec2(200, 300), 1.0f);
```

### 5. GameController - 游戏主控制器

#### 公共方法

##### 初始化和控制
```cpp
// 初始化
bool init();

// 开始游戏
void startGame();

// 结束游戏
void endGame();
```

##### 事件处理
```cpp
// 卡牌点击处理
void onCardClicked(int cardId);

// 撤销按钮点击处理
void onUndoButtonClicked();

// 抽卡按钮点击处理
void onDrawCardButtonClicked();
```

##### 游戏逻辑
```cpp
// 检查游戏结束
void checkGameOver();

// 更新分数
void updateScore(int points);
```

#### 使用示例
```cpp
// 创建游戏控制器
GameController* controller = new GameController();
controller->init();

// 开始游戏
controller->startGame();

// 处理卡牌点击
controller->onCardClicked(1);
```

### 6. GameStateManager - 游戏状态管理器

#### 公共方法

##### 状态管理
```cpp
// 保存当前状态
void saveState(const GameModel* gameModel);

// 撤销操作
bool undo(GameModel* gameModel);

// 重做操作
bool redo(GameModel* gameModel);
```

##### 状态查询
```cpp
// 是否可以撤销
bool canUndo() const;

// 是否可以重做
bool canRedo() const;

// 获取撤销栈大小
size_t getUndoStackSize() const;

// 获取重做栈大小
size_t getRedoStackSize() const;
```

##### 历史管理
```cpp
// 清除历史记录
void clearHistory();

// 清除重做历史
void clearRedoHistory();
```

#### 使用示例
```cpp
// 创建状态管理器
GameStateManager* stateManager = new GameStateManager();

// 保存状态
stateManager->saveState(gameModel);

// 执行撤销
if (stateManager->canUndo()) {
    stateManager->undo(gameModel);
}

// 清除历史
stateManager->clearHistory();
```

## 回调函数类型定义

### 1. 卡牌点击回调
```cpp
typedef std::function<void(int cardId)> CardClickCallback;
```

### 2. 撤销按钮回调
```cpp
typedef std::function<void()> UndoButtonCallback;
```

### 3. 抽卡回调
```cpp
typedef std::function<void()> DrawCardCallback;
```

## 枚举类型定义

### 1. 卡牌面值
```cpp
enum CardFaceType {
    CFT_UNKNOWN = 0,
    CFT_ACE = 1,
    CFT_TWO = 2,
    CFT_THREE = 3,
    CFT_FOUR = 4,
    CFT_FIVE = 5,
    CFT_SIX = 6,
    CFT_SEVEN = 7,
    CFT_EIGHT = 8,
    CFT_NINE = 9,
    CFT_TEN = 10,
    CFT_JACK = 11,
    CFT_QUEEN = 12,
    CFT_KING = 13
};
```

### 2. 卡牌花色
```cpp
enum CardSuitType {
    CST_UNKNOWN = 0,
    CST_CLUBS = 1,      // ♣
    CST_DIAMONDS = 2,   // ♦
    CST_HEARTS = 3,     // ♥
    CST_SPADES = 4      // ♠
};
```

## 工具类 API

### CardUtils - 卡牌工具类

#### 静态方法
```cpp
// 获取面值文本
static std::string getFaceText(CardFaceType face);

// 获取花色文本
static std::string getSuitText(CardSuitType suit);

// 获取花色符号
static std::string getSuitSymbol(CardSuitType suit);

// 检查是否为红色
static bool isRedSuit(CardSuitType suit);

// 获取卡牌值 (用于比较)
static int getCardValue(CardFaceType face);

// 检查是否可以移动 (游戏规则)
static bool canMoveToStack(CardModel* card, const std::vector<CardModel*>& stack);
```

#### 使用示例
```cpp
// 获取面值文本
std::string faceText = CardUtils::getFaceText(CFT_ACE); // "A"

// 获取花色符号
std::string suitSymbol = CardUtils::getSuitSymbol(CST_HEARTS); // "♥"

// 检查是否为红色
bool isRed = CardUtils::isRedSuit(CST_DIAMONDS); // true

// 检查是否可以移动
bool canMove = CardUtils::canMoveToStack(card, targetStack);
```

## 配置类 API

### LevelConfig - 关卡配置

#### 公共方法
```cpp
// 获取关卡名称
std::string getLevelName() const;

// 获取关卡ID
int getLevelId() const;

// 获取主牌堆配置
const std::vector<CardPosition>& getMainPileCards() const;

// 获取底牌堆配置
const std::vector<CardPosition>& getBottomPileCards() const;

// 获取备用牌堆配置
const std::vector<CardPosition>& getReservePileCards() const;

// 获取桌面牌堆配置
const std::vector<StackConfig>& getStacks() const;
```

### LevelConfigLoader - 关卡配置加载器

#### 静态方法
```cpp
// 从文件加载关卡配置
static LevelConfig* loadFromFile(const std::string& filename);

// 从JSON字符串加载
static LevelConfig* loadFromJson(const std::string& jsonString);

// 保存关卡配置到文件
static bool saveToFile(const LevelConfig* config, const std::string& filename);
```

#### 使用示例
```cpp
// 加载关卡配置
LevelConfig* config = LevelConfigLoader::loadFromFile("levels/level1.json");

if (config) {
    CCLOG("Level loaded: %s", config->getLevelName().c_str());
}

// 保存关卡配置
bool success = LevelConfigLoader::saveToFile(config, "levels/level1.json");
```

## 错误处理

### 常见错误码
```cpp
enum ErrorCode {
    ERROR_NONE = 0,
    ERROR_INVALID_CARD_ID = 1,
    ERROR_INVALID_MOVE = 2,
    ERROR_FILE_NOT_FOUND = 3,
    ERROR_INVALID_JSON = 4,
    ERROR_MEMORY_ALLOCATION = 5
};
```

### 异常处理
```cpp
// 检查返回值
CardView* cardView = CardView::create(cardModel);
if (!cardView) {
    CCLOG("Failed to create CardView");
    return;
}

// 检查指针有效性
if (cardView && cardView->getCardId() > 0) {
    // 安全使用
}
```

## 性能优化建议

### 1. 对象创建
- 使用对象池减少频繁创建销毁
- 预分配容器大小避免重新分配
- 使用智能指针管理内存

### 2. 动画优化
- 避免同时播放过多动画
- 使用动画链减少回调嵌套
- 及时停止不需要的动画

### 3. 渲染优化
- 使用纹理图集减少绘制调用
- 避免每帧更新不变的内容
- 合理使用批处理渲染

---

这个API文档提供了项目中所有公共接口的详细说明，包括方法签名、参数说明、返回值和使用示例，有助于开发者理解和使用项目代码。
