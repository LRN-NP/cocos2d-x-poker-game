#include "CardView.h"
#include "cocos2d.h"
#include "../models/CardModel.h"

USING_NS_CC;

CardView::CardView()
    : _cardModel(nullptr)
    , _clickCallback(nullptr)
    , _cardSprite(nullptr)
    , _cardLabel(nullptr)
    , _isClickable(true)
    , _isRevealed(false)
{
}

CardView::~CardView()
{
}

CardView* CardView::create(const CardModel* cardModel)
{
    return create(cardModel, nullptr);
}

CardView* CardView::create(const CardModel* cardModel, CardClickCallback clickCallback)
{
    CardView* ret = new CardView();
    if (ret && ret->init(cardModel, clickCallback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CardView::init(const CardModel* cardModel, CardClickCallback clickCallback)
{
    if (!Node::init()) {
        return false;
    }

    _cardModel = cardModel;
    _clickCallback = clickCallback;
    _isClickable = true;

    // 设置CardView的锚点为(0,0)，左下角为原点，与碰撞盒原点对齐
    setAnchorPoint(cocos2d::Vec2(0, 0));

    if (_cardModel) {
        createCardSprite(_cardModel);
        setPosition(_cardModel->getPosition());
    }

    setupTouchListener();
    return true;
}

int CardView::getCardId() const
{
    return _cardModel ? _cardModel->getCardId() : -1;
}

void CardView::updateCard(const CardModel* cardModel)
{
    _cardModel = cardModel;
    if (_cardModel) {
        setPosition(_cardModel->getPosition());
    }
}

void CardView::playMoveAnimation(const cocos2d::Vec2& targetPosition, float duration,
    std::function<void()> callback)
{
    auto moveAction = cocos2d::MoveTo::create(duration, targetPosition);
    if (callback) {
        auto sequence = cocos2d::Sequence::create(moveAction,
            cocos2d::CallFunc::create(callback),
            nullptr);
        runAction(sequence);
    }
    else {
        runAction(moveAction);
    }
}

void CardView::playMatchAnimation(std::function<void()> callback)
{
    // 播放缩放动画表示匹配
    auto scaleUp = cocos2d::ScaleTo::create(0.1f, 1.2f);
    auto scaleDown = cocos2d::ScaleTo::create(0.1f, 1.0f);
    auto sequence = cocos2d::Sequence::create(scaleUp, scaleDown, nullptr);

    if (callback) {
        auto fullSequence = cocos2d::Sequence::create(sequence,
            cocos2d::CallFunc::create(callback),
            nullptr);
        runAction(fullSequence);
    }
    else {
        runAction(sequence);
    }
}

void CardView::playUndoAnimation(const cocos2d::Vec2& targetPosition, float duration,
    std::function<void()> callback)
{
    playMoveAnimation(targetPosition, duration, callback);
}

void CardView::setCardVisible(bool visible)
{
    setVisible(visible);
}

void CardView::createCardSprite(const CardModel* cardModel)
{
    if (!cardModel) return;

    // 使用卡牌背景
    _cardSprite = cocos2d::Sprite::create("card_general.png");
    if (_cardSprite) {
        _cardSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
        _cardSprite->setPosition(cocos2d::Vec2(91, 141)); // 卡牌中心位置
        addChild(_cardSprite);

        // 添加数字图片
        createNumberSprite(cardModel);
        // 添加花色图片
        createSuitSprite(cardModel);
    }
}

void CardView::createCardLabel(const CardModel* cardModel)
{
    if (!cardModel) return;

    if (_cardLabel) {
        _cardLabel->removeFromParent();
    }

    _cardLabel = cocos2d::Label::createWithTTF(cardModel->getCardText(),
        "fonts/Marker Felt.ttf", 28);
    _cardLabel->setColor(cocos2d::Color3B::BLACK);
    _cardLabel->setPosition(cocos2d::Vec2(60, 80));
    addChild(_cardLabel);
}

void CardView::setupTouchListener()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!_isClickable || !_cardModel || !_cardSprite) {
        return false;
    }

    // 1. 获取触摸坐标
    cocos2d::Vec2 touchWorldPos = touch->getLocation();
    cocos2d::Vec2 localPos = this->convertToNodeSpaceAR(touchWorldPos);

    // 2. 定义 cardSize
    cocos2d::Size cardSize = _cardSprite->getContentSize();

    // 3. 计算边界框
    cocos2d::Vec2 anchor = this->getAnchorPoint();
    float bboxX = -cardSize.width * anchor.x;
    float bboxY = -cardSize.height * anchor.y;
    cocos2d::Rect actualBbox(cocos2d::Vec2(bboxX, bboxY), cardSize);

    // 调试日志：显示锚点和边界框信息
    cocos2d::log("CardView::onTouchBegan - Card %d: anchor(%.1f,%.1f) bbox(%.1f,%.1f,%.1f,%.1f) local(%.1f,%.1f)",
        _cardModel->getCardId(), anchor.x, anchor.y,
        actualBbox.origin.x, actualBbox.origin.y, actualBbox.size.width, actualBbox.size.height,
        localPos.x, localPos.y);

    // 4. 判定触摸是否在边界框内
    if (actualBbox.containsPoint(localPos)) {
        cocos2d::log("CardView::onTouchBegan - Touch hit card %d", _cardModel->getCardId());
        if (_clickCallback) {
            _clickCallback(_cardModel->getCardId());
        }
        return true;
    }
    else {
        cocos2d::log("CardView::onTouchBegan - Touch missed card %d: local(%.1f,%.1f) not in bbox(%.1f,%.1f,%.1f,%.1f)",
            _cardModel->getCardId(), localPos.x, localPos.y,
            actualBbox.origin.x, actualBbox.origin.y,
            actualBbox.size.width, actualBbox.size.height);
    }

    return false;
}

void CardView::playFlipAnimation(std::function<void()> callback)
{
    // 简单的翻转动画 - 使用缩放动画模拟翻转效果
    auto scaleDown = cocos2d::ScaleTo::create(0.15f, 0.0f, 1.0f);
    auto scaleUp = cocos2d::ScaleTo::create(0.15f, 1.0f, 1.0f);
    auto flip = cocos2d::Sequence::create(scaleDown, scaleUp, nullptr);

    if (callback) {
        auto callFunc = cocos2d::CallFunc::create(callback);
        auto sequence = cocos2d::Sequence::create(flip, callFunc, nullptr);
        runAction(sequence);
    }
    else {
        runAction(flip);
    }
}

void CardView::setClickable(bool clickable)
{
    _isClickable = clickable;
    // 可以在这里添加视觉反馈，比如改变透明度
    if (clickable) {
        setOpacity(255);
    }
    else {
        setOpacity(128);
    }
}

void CardView::setRevealed(bool revealed)
{
    _isRevealed = revealed;
    // 可以在这里添加视觉反馈，比如显示/隐藏卡牌内容
    // 这里简化处理，实际应该显示卡牌的正反面
}

void CardView::createNumberSprite(const CardModel* cardModel)
{
    if (!cardModel) return;

    // 根据卡牌面值选择数字图片
    std::string numberFile = getNumberImageFile(cardModel);

    if (!numberFile.empty()) {
        // 小的数字在左上角
        auto smallNumberSprite = cocos2d::Sprite::create(numberFile);
        if (smallNumberSprite) {
            smallNumberSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
            smallNumberSprite->setPosition(cocos2d::Vec2(30, 221)); // 左上角，稍微右移
            smallNumberSprite->setScale(0.4f);
            addChild(smallNumberSprite, 1);
        }

        // 大的数字在中间偏下
        auto bigNumberSprite = cocos2d::Sprite::create(numberFile);
        if (bigNumberSprite) {
            bigNumberSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
            bigNumberSprite->setPosition(cocos2d::Vec2(91, 100)); // 中间偏下，稍微上移
            bigNumberSprite->setScale(1.0f);
            addChild(bigNumberSprite, 1);
        }
    }
}

void CardView::createSuitSprite(const CardModel* cardModel)
{
    if (!cardModel) return;

    // 根据卡牌花色选择花色图片
    std::string suitFile = getSuitImageFile(cardModel);
    if (!suitFile.empty()) {
        // 花色在右上角
        auto suitSprite = cocos2d::Sprite::create(suitFile);
        if (suitSprite) {
            suitSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
            suitSprite->setPosition(cocos2d::Vec2(152, 221)); // 右上角，稍微左移
            suitSprite->setScale(0.6f);
            addChild(suitSprite, 1);
        }
    }
}

std::string CardView::getNumberImageFile(const CardModel* cardModel)
{
    if (!cardModel) return "";

    CardFaceType face = cardModel->getFace();
    CardSuitType suit = cardModel->getSuit();

    // 判断是红色还是黑色
    bool isRed = (suit == CST_HEARTS || suit == CST_DIAMONDS);
    std::string colorPrefix = isRed ? "big_red_" : "big_black_";

    // 根据面值选择图片
    switch (face) {
    case CFT_ACE: return "number/" + colorPrefix + "A.png";
    case CFT_TWO: return "number/" + colorPrefix + "2.png";
    case CFT_THREE: return "number/" + colorPrefix + "3.png";
    case CFT_FOUR: return "number/" + colorPrefix + "4.png";
    case CFT_FIVE: return "number/" + colorPrefix + "5.png";
    case CFT_SIX: return "number/" + colorPrefix + "6.png";
    case CFT_SEVEN: return "number/" + colorPrefix + "7.png";
    case CFT_EIGHT: return "number/" + colorPrefix + "8.png";
    case CFT_NINE: return "number/" + colorPrefix + "9.png";
    case CFT_TEN: return "number/" + colorPrefix + "10.png";
    case CFT_JACK: return "number/" + colorPrefix + "J.png";
    case CFT_QUEEN: return "number/" + colorPrefix + "Q.png";
    case CFT_KING: return "number/" + colorPrefix + "K.png";
    default: return "";
    }
}

std::string CardView::getSuitImageFile(const CardModel* cardModel)
{
    if (!cardModel) return "";

    CardSuitType suit = cardModel->getSuit();

    switch (suit) {
    case CST_CLUBS: return "suits/club.png";
    case CST_DIAMONDS: return "suits/diamond.png";
    case CST_HEARTS: return "suits/heart.png";
    case CST_SPADES: return "suits/spade.png";
    default: return "";
    }
}
