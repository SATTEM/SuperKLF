#ifndef EVENTLEVEL_H
#define EVENTLEVEL_H
#include "Level.h"
#include "Effect/InstantEffect.h"
class EventLevel:public Level{
public:
    class Option{
    public:
        std::string text,explain;
        std::vector<std::shared_ptr<InstantEffect>> effects;
        std::vector<std::string> NeedRelics,ExcludeRelics;
        Option(const nlohmann::json& json);
        ~Option()=default;
        const bool isAvailable()const;
        void apply(Player& player)const;
    };
private:
    std::string background,text,title;
    std::vector<Option> options;
    void setupUI()const;
public:
    EventLevel(const nlohmann::json& json);
    ~EventLevel()=default;
    void update()override final;
    void onActivate()override final;
    void onDeactivate()override final;

    friend class EventUI;
};
#endif