#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include <functional>

// Forward declaration to avoid include issues
class CardModel;

class CardView : public cocos2d::Node
{
public:
    typedef std::function<void(int cardId)> CardClickCallback;
    
    CardView();
    virtual ~CardView();
    
    static CardView* create(const CardModel* cardModel);
    static CardView* create(const CardModel* cardModel, CardClickCallback clickCallback);
    bool init(const CardModel* cardModel, CardClickCallback clickCallback = nullptr);
    
    int getCardId() const;
    void updateCard(const CardModel* cardModel);
    void playMoveAnimation(const cocos2d::Vec2& targetPosition, float duration, std::function<void()> callback = nullptr);
    void playMatchAnimation(std::function<void()> callback = nullptr);
    void playUndoAnimation(const cocos2d::Vec2& targetPosition, float duration, std::function<void()> callback = nullptr);
    void setCardVisible(bool visible);
    void playFlipAnimation(std::function<void()> callback = nullptr);
    void setClickable(bool clickable);
    void setRevealed(bool revealed);
    void createCardSprite(const CardModel* cardModel);
    void createCardLabel(const CardModel* cardModel);
    void setupTouchListener();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void createNumberSprite(const CardModel* cardModel);
    void createSuitSprite(const CardModel* cardModel);
    std::string getNumberImageFile(const CardModel* cardModel);
    std::string getSuitImageFile(const CardModel* cardModel);

private:
    const CardModel* _cardModel;
    CardClickCallback _clickCallback;
    cocos2d::Sprite* _cardSprite;
    cocos2d::Label* _cardLabel;
    bool _isClickable;
    bool _isRevealed;
};

#endif // __CARD_VIEW_H__