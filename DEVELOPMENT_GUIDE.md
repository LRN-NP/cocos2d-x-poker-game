# 扑克游戏项目开发指南

## 开发环境设置

### 1. 系统要求
- **操作系统**: Windows 10 或更高版本
- **IDE**: Visual Studio 2015 或更高版本
- **SDK**: Windows 10 SDK
- **C++标准**: C++11 或更高版本

### 2. 依赖库
- **Cocos2d-x**: 3.x 版本
- **RapidJSON**: JSON 解析库
- **Visual Studio**: 用于 Windows 平台编译

### 3. 环境配置步骤

#### 步骤 1: 安装 Visual Studio
1. 下载并安装 Visual Studio 2015 或更高版本
2. 确保安装 C++ 开发工具
3. 安装 Windows 10 SDK

#### 步骤 2: 配置 Cocos2d-x
1. 下载 Cocos2d-x 3.x 源码
2. 设置环境变量 `COCOS2DX_ROOT`
3. 运行 `setup.py` 配置环境

#### 步骤 3: 编译项目
1. 打开 `proj.win32/PokerGame.sln`
2. 选择 Debug 或 Release 配置
3. 编译解决方案

## 代码规范

### 1. 命名规范

#### 类名
- 使用 PascalCase (首字母大写)
- 名称应该清晰表达类的用途
```cpp
class CardView;           // 正确
class GameController;     // 正确
class card_view;          // 错误
```

#### 方法名
- 使用 camelCase (首字母小写)
- 动词开头，表达操作含义
```cpp
void playMoveAnimation();     // 正确
void setClickable();          // 正确
void PlayMoveAnimation();     // 错误
```

#### 变量名
- 成员变量使用下划线前缀
- 局部变量使用 camelCase
```cpp
class CardView {
private:
    bool _isClickable;        // 成员变量
    int _cardId;              // 成员变量
};

void someFunction() {
    int cardCount = 0;        // 局部变量
    bool isVisible = true;    // 局部变量
}
```

#### 常量名
- 使用全大写，下划线分隔
```cpp
const int MAX_CARDS = 52;
const std::string CARD_BACK_IMAGE = "card_back.png";
```

### 2. 文件组织

#### 头文件 (.h)
- 包含类声明、接口定义
- 使用头文件保护
- 最小化包含依赖
```cpp
#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
// 其他必要的包含

class CardView : public cocos2d::Node {
    // 类声明
};

#endif // __CARD_VIEW_H__
```

#### 源文件 (.cpp)
- 包含具体实现
- 包含必要的头文件
- 使用命名空间
```cpp
#include "CardView.h"
#include "cocos2d.h"

USING_NS_CC;

// 实现代码
```

### 3. 注释规范

#### 类注释
```cpp
/**
 * 卡牌视图类
 * 职责：负责卡牌的UI显示和用户交互
 * 使用场景：显示单张卡牌，处理卡牌点击事件
 */
class CardView : public cocos2d::Node {
    // ...
};
```

#### 方法注释
```cpp
/**
 * 播放移动动画
 * @param targetPosition 目标位置
 * @param duration 动画持续时间
 * @param callback 动画完成回调
 */
void playMoveAnimation(const Vec2& targetPosition, float duration,
                      std::function<void()> callback = nullptr);
```

#### 成员变量注释
```cpp
private:
    const CardModel* _cardModel;        ///< 卡牌数据模型（只读）
    CardClickCallback _clickCallback;   ///< 点击回调函数
    bool _isClickable;                  ///< 是否可点击
```

### 4. 内存管理

#### Cocos2d-x 对象
- 使用 `create()` 方法创建
- 使用 `autorelease()` 自动管理
- 避免手动 `delete`
```cpp
// 正确
CardView* cardView = CardView::create(cardModel);
cardView->autorelease();

// 错误
CardView* cardView = new CardView();
delete cardView;
```

#### 智能指针
- 使用 `std::shared_ptr` 管理复杂对象
- 使用 `std::unique_ptr` 管理独占资源
```cpp
std::shared_ptr<GameState> state = std::make_shared<GameState>();
std::unique_ptr<LevelConfig> config = std::make_unique<LevelConfig>();
```

### 5. 错误处理

#### 返回值检查
```cpp
CardView* cardView = CardView::create(cardModel);
if (!cardView) {
    CCLOG("Failed to create CardView");
    return false;
}
```

#### 异常安全
```cpp
try {
    // 可能抛出异常的代码
    auto result = someRiskyOperation();
} catch (const std::exception& e) {
    CCLOG("Exception: %s", e.what());
    return false;
}
```

## 开发流程

### 1. 新功能开发

#### 步骤 1: 设计
1. 分析需求
2. 设计接口
3. 确定数据结构
4. 考虑扩展性

#### 步骤 2: 实现
1. 创建头文件
2. 实现基本功能
3. 添加错误处理
4. 编写单元测试

#### 步骤 3: 测试
1. 单元测试
2. 集成测试
3. 性能测试
4. 用户测试

#### 步骤 4: 文档
1. 更新API文档
2. 添加使用示例
3. 更新设计文档

### 2. Bug 修复

#### 步骤 1: 复现
1. 重现问题
2. 确定影响范围
3. 分析根本原因

#### 步骤 2: 修复
1. 编写测试用例
2. 实现修复
3. 验证修复效果

#### 步骤 3: 验证
1. 运行所有测试
2. 回归测试
3. 性能验证

### 3. 代码审查

#### 审查要点
1. **功能正确性**: 代码是否实现了预期功能
2. **性能**: 是否有性能问题
3. **可读性**: 代码是否清晰易懂
4. **可维护性**: 是否易于修改和扩展
5. **安全性**: 是否有安全漏洞

#### 审查清单
- [ ] 代码符合命名规范
- [ ] 注释完整清晰
- [ ] 错误处理完善
- [ ] 内存管理正确
- [ ] 性能考虑充分
- [ ] 测试覆盖完整

## 调试技巧

### 1. 日志调试
```cpp
// 使用 CCLOG 输出调试信息
CCLOG("CardView created with ID: %d", cardId);
CCLOG("Position: (%.1f, %.1f)", position.x, position.y);

// 使用条件日志
#ifdef DEBUG
    CCLOG("Debug info: %s", debugInfo.c_str());
#endif
```

### 2. 断点调试
- 在关键位置设置断点
- 使用条件断点
- 观察变量值变化

### 3. 性能分析
```cpp
// 测量执行时间
auto start = std::chrono::high_resolution_clock::now();
// 执行代码
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
CCLOG("Execution time: %lld ms", duration.count());
```

### 4. 内存调试
- 使用 Visual Studio 诊断工具
- 检查内存泄漏
- 监控内存使用情况

## 测试策略

### 1. 单元测试
```cpp
// 测试卡牌模型
TEST(CardModelTest, TestCardCreation) {
    CardModel* card = new CardModel(1, CFT_ACE, CST_HEARTS, Vec2(100, 200));
    EXPECT_EQ(card->getCardId(), 1);
    EXPECT_EQ(card->getFace(), CFT_ACE);
    EXPECT_EQ(card->getSuit(), CST_HEARTS);
    EXPECT_TRUE(card->isRed());
    delete card;
}
```

### 2. 集成测试
```cpp
// 测试游戏流程
TEST(GameFlowTest, TestCardMove) {
    GameModel* gameModel = new GameModel();
    GameController* controller = new GameController();
    
    controller->init();
    controller->startGame();
    
    // 测试卡牌移动
    EXPECT_TRUE(gameModel->canMoveCard(1, 2));
    gameModel->moveCard(1, 2);
    EXPECT_EQ(gameModel->findCardStack(1), 2);
    
    delete controller;
    delete gameModel;
}
```

### 3. 性能测试
```cpp
// 测试动画性能
TEST(PerformanceTest, TestAnimationPerformance) {
    CardView* cardView = CardView::create(cardModel);
    
    auto start = std::chrono::high_resolution_clock::now();
    cardView->playMoveAnimation(Vec2(200, 300), 1.0f);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 100); // 应该在100ms内完成
}
```

## 性能优化

### 1. 渲染优化
- 使用对象池减少创建销毁
- 批量渲染减少绘制调用
- 使用纹理图集

### 2. 内存优化
- 及时释放不需要的资源
- 使用智能指针
- 避免内存泄漏

### 3. 逻辑优化
- 缓存频繁计算的结果
- 使用事件驱动
- 异步加载资源

## 常见问题解决

### 1. 编译错误
- **C2065**: 未声明的标识符 - 检查头文件包含
- **C2614**: 非法的成员初始化 - 检查构造函数初始化列表
- **LNK2019**: 无法解析的外部符号 - 检查函数实现

### 2. 运行时错误
- **崩溃**: 检查空指针访问
- **内存泄漏**: 检查对象生命周期管理
- **性能问题**: 使用性能分析工具

### 3. 平台兼容性
- **Windows**: 确保使用正确的SDK版本
- **移动端**: 考虑屏幕分辨率和触摸交互
- **跨平台**: 使用Cocos2d-x的跨平台API

## 版本控制

### 1. Git 工作流
```bash
# 创建功能分支
git checkout -b feature/new-card-type

# 提交更改
git add .
git commit -m "Add new card type support"

# 推送分支
git push origin feature/new-card-type

# 创建合并请求
```

### 2. 提交信息规范
```
类型(范围): 简短描述

详细描述（可选）

相关Issue: #123
```

类型包括：
- `feat`: 新功能
- `fix`: Bug修复
- `docs`: 文档更新
- `style`: 代码格式
- `refactor`: 重构
- `test`: 测试
- `chore`: 构建过程或辅助工具的变动

## 部署和发布

### 1. 构建配置
- **Debug**: 开发调试版本
- **Release**: 生产发布版本
- **Profile**: 性能分析版本

### 2. 发布检查清单
- [ ] 所有测试通过
- [ ] 性能满足要求
- [ ] 内存使用正常
- [ ] 文档更新完整
- [ ] 版本号更新

### 3. 发布流程
1. 更新版本号
2. 运行完整测试
3. 构建发布版本
4. 创建发布说明
5. 部署到目标平台

---

这个开发指南提供了完整的开发环境设置、代码规范、开发流程、调试技巧、测试策略和常见问题解决方案，有助于开发者快速上手项目开发。
