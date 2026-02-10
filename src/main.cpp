#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>


using namespace geode::prelude;

static const std::vector<std::string> GD_PLAYERS = {
    "Michigun", "Viprin", "Riot", "Juniper", "Wulzy",
    "EVW", "Doggie", "Nexus", "AeonAir", "Tride",
    "Cyclic", "Knobbelboy", "Sunix", "Technical49", "Dorami",
    "SpaceUK", "Diamond", "Trick", "Zoink", "Nswish",
    "Cursed", "BlassCFB", "MiKhaXx", "Mullsy", "Luqualizer",
    "Npesta", "xanii", "BTD6", "GuitarHeroStyles", "Cataclysm",
    "Krazyman50", "Zobros", "Sea1997", "Pennutoh", "FunnyGame",
    "TrusTa", "RicoLP", "ViPriN", "ChaSe", "Lemons"
};

static std::string randomPlayer() {
    return GD_PLAYERS[rand() % GD_PLAYERS.size()];
}

static std::string chat(const std::string& msg) {
    return randomPlayer() + ": " + msg;
}

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        CCLayer* m_chatBox = nullptr;
        CCLabelBMFont* m_chatText = nullptr;
        float m_lastPercent = 0.0f;
        float m_randomChatTimer = 0.0f;
        float m_nextChatDelay = 0.5f;
        float m_deathChatTimer = 0.0f;
        bool m_isDeathSpamming = false;
        float holdPercent = 22;
        float goPercent = 37;
        float superGoPercent = 80;
    };
    
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        
            log::info("Loaded: Hold percent: {}", Mod::get()->getSavedValue(std::to_string(level->m_levelID)+"hold-percent", 22.0f));
            log::info("Loaded: Go percent: {}", Mod::get()->getSavedValue(std::to_string(level->m_levelID)+"go-percent", 37.0f));
            log::info("Loaded: Super go percent: {}", Mod::get()->getSavedValue(std::to_string(level->m_levelID)+"supergo-percent", 80.0f));
        


        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // chat box
        m_fields->m_chatBox = CCLayerColor::create({0, 0, 0, 180}, 100.0f, 150.0f);
        m_fields->m_chatBox->setPosition(winSize.width - 110, 10); //pos
        m_fields->m_chatBox->setZOrder(20);
        this->addChild(m_fields->m_chatBox);
        
        // text
        m_fields->m_chatText = CCLabelBMFont::create("", "chatFont.fnt");
        m_fields->m_chatText->setScale(0.4f);
        m_fields->m_chatText->setAnchorPoint({0, 1});
        m_fields->m_chatText->setPosition(winSize.width - 105, 145);
        m_fields->m_chatText->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
        m_fields->m_chatText->setZOrder(101);
        this->addChild(m_fields->m_chatText);
        
        log::info("ChatGD: Init complete");
        
        this->schedule(schedule_selector(MyPlayLayer::checkProgress));
        
        return true;
    }
    
    void checkProgress(float dt) {
        float progress = this->getCurrentPercent();
        
        // NOOOOOOOO
        if (m_fields->m_isDeathSpamming) {
            m_fields->m_deathChatTimer += dt;
            
            if (m_fields->m_deathChatTimer >= 2.0f) {
                m_fields->m_isDeathSpamming = false;
                m_fields->m_deathChatTimer = 0;
            } else {
                m_fields->m_randomChatTimer += dt;
                if (m_fields->m_randomChatTimer >= m_fields->m_nextChatDelay) {
                    std::vector<std::string> deathMessages = {
                        chat("RIP"),
                        chat("NOOOO"),
                        chat("rippp"),
                        chat("F"),
                        chat("NOOOOOO"),
                        chat("rip bozo"),
                        chat("oof"),
                        chat("so close"),
                        chat("unlucky")
                    };
                    addChatMessage(deathMessages[rand() % deathMessages.size()]);
                    m_fields->m_randomChatTimer = 0;
                    m_fields->m_nextChatDelay = 0.1f + (rand() % 10) / 10.0f;
                }
            }
            return;
        }
        
        // hold
        // TODO: cfg 50% done
        if (progress >= m_fields->holdPercent && progress < m_fields->goPercent) {
            m_fields->m_randomChatTimer += dt;
            if (m_fields->m_randomChatTimer >= m_fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("holdlldldldl"),
                    chat("HOLD IT"),
                    chat("HOOOOOOOOLDDDDDDDDDDDD"),
                    chat("HOOOLD"),
                    chat("HOLDDDDDDDDDDDDDDDDDDD")
                };
                addChatMessage(messages[rand() % messages.size()]);
                m_fields->m_randomChatTimer = 0;
                m_fields->m_nextChatDelay = 0.1f + (rand() % 10) / 10.0f;
            }
        }
        // gooo
        // TODO: cfg 50% done
        else if (progress >= m_fields->goPercent && progress < m_fields->superGoPercent) {           
            m_fields->m_randomChatTimer += dt;
            if (m_fields->m_randomChatTimer >= m_fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("GOOOO"),
                    chat("LETS GOOOOO"),
                    chat("CMON"),
                    chat("GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"),
                };
                addChatMessage(messages[rand() % messages.size()]);
                m_fields->m_randomChatTimer = 0;
                m_fields->m_nextChatDelay = 0.1f + (rand() % 6) / 10.0f;
            }
        }
        // super go and i was here
        // TODO: cfg 50% done
        else if (progress >= m_fields->superGoPercent && progress < 99.9999f) {          
            m_fields->m_randomChatTimer += dt;
            if (m_fields->m_randomChatTimer >= m_fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("SUPER GOOOOOOOOOOOOOOOOOOOOOO"),
                    chat("SUPERGOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!"),
                    chat("I WAS HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"),
                    chat("CMONNNN"),
                    chat("GOOOOO GOOOOOO GOOOOOOOO")
                };
                addChatMessage(messages[rand() % messages.size()]);
                m_fields->m_randomChatTimer = 0;
                m_fields->m_nextChatDelay = 0.1f + (rand() % 4) / 10.0f;
            }
        } 
        // 100%: gg
        else if (progress > 99.9999f) {
            m_fields->m_randomChatTimer += dt;
            if (m_fields->m_randomChatTimer >= m_fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("GG"),
                    chat("GGS"),
                    chat("WWWWWWWWWWWWWWWWWWWWWWWW"),
                    chat("LETS GOOOOO"),
                    chat("WOOOOOOOOOOOOO")
                };
                addChatMessage(messages[rand() % messages.size()]);
                m_fields->m_randomChatTimer = 0;
                m_fields->m_nextChatDelay = 0.1f + (rand() % 3) / 10.0f;
            }
        }
    }
    
    void addChatMessage(std::string message) {
        std::string currentText = m_fields->m_chatText->getString();
        
        int lineCount = 0;
        for (char c : currentText) {
            if (c == '\n') lineCount++;
        }
        
        if (lineCount >= 19) {
            size_t pos = currentText.find('\n');
            if (pos != std::string::npos) {
                currentText = currentText.substr(pos + 1);
            }
        }
        
        std::string newText = currentText.empty() ? message : currentText + "\n" + message;
        m_fields->m_chatText->setString(newText.c_str());
    }
    
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        
        m_fields->m_isDeathSpamming = true;
        m_fields->m_deathChatTimer = 0;
        m_fields->m_randomChatTimer = 0;
    }
    
    void resetLevel() {
        PlayLayer::resetLevel();
        
        m_fields->m_randomChatTimer = 0;
        m_fields->m_isDeathSpamming = false;
        m_fields->m_deathChatTimer = 0;
    }
};

class ChatConfigPopup : public geode::Popup {
protected:
    geode::TextInput* m_textInput1 = nullptr;
    geode::TextInput* m_textInput2 = nullptr;
    geode::TextInput* m_textInput3 = nullptr;
    
    bool init(float width, float height) {
        if (!Popup::init(width, height))
            return false;
        
        this->setTitle("ChatGD Config");
        auto center = m_mainLayer->getContentSize() / 2;
        
        // box 1
        m_textInput1 = geode::TextInput::create(200.0f, "Hold %...");
        m_textInput1->setPosition({center.width, center.height + 40});
        m_textInput1->setFilter("0123456789");
        m_textInput1->setMaxCharCount(3);
        m_mainLayer->addChild(m_textInput1);
        
        // box 2
        m_textInput2 = geode::TextInput::create(200.0f, "Go %...");
        m_textInput2->setPosition({center.width, center.height});
        m_textInput2->setFilter("0123456789");
        m_textInput2->setMaxCharCount(3);
        m_mainLayer->addChild(m_textInput2);
        
        // box 3
        m_textInput3 = geode::TextInput::create(200.0f, "Super Go %...");
        m_textInput3->setPosition({center.width, center.height - 40});
        m_textInput3->setFilter("0123456789");
        m_textInput3->setMaxCharCount(3);
        m_mainLayer->addChild(m_textInput3);


        return true;
    }
    
public:
    static ChatConfigPopup* create() {
        auto ret = new ChatConfigPopup();
        if (ret->init(300.0f, 200.0f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void onClose(CCObject* sender) override {
        // get playlayer
        auto playLayer = PlayLayer::get();
        //get id
        int levelID = playLayer->m_level->m_levelID;
        // get vals
        std::string holdStr = m_textInput1->getString();
        std::string goStr = m_textInput2->getString();
        std::string superGoStr = m_textInput3->getString();
        
        // conv to int
        auto hold = geode::utils::numFromString<int>(holdStr);
        auto go = geode::utils::numFromString<int>(goStr);
        auto superGo = geode::utils::numFromString<int>(superGoStr);
        
        if (hold && go && superGo) {
            Mod::get()->setSavedValue(std::to_string(levelID)+"hold-percent", *hold);
            Mod::get()->setSavedValue(std::to_string(levelID)+"go-percent", *go);
            Mod::get()->setSavedValue(std::to_string(levelID)+"supergo-percent", *superGo);
        }
        
        geode::Popup::onClose(sender);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        
        log::info("PauseLayer customSetup called");
        
        // find opt menu (slightly fried way)
        CCNode* optionsButton = nullptr;
        CCMenu* parentMenu = nullptr;
        
        auto children = this->getChildren();
        if (children) {
            for (int i = 0; i < children->count(); i++) {
                auto child = static_cast<CCNode*>(children->objectAtIndex(i));
                if (!child) continue;
                
                if (auto menu = typeinfo_cast<CCMenu*>(child)) {
                    auto menuChildren = menu->getChildren();
                    if (!menuChildren) continue;
                    
                    for (int j = 0; j < menuChildren->count(); j++) {
                        auto menuChild = static_cast<CCNode*>(menuChildren->objectAtIndex(j));
                        if (!menuChild) continue;
                        
                        if (menuChild->getID() == "options-button") {
                            optionsButton = menuChild;
                            parentMenu = menu;
                            break;
                        }
                    }
                }
                if (optionsButton) break;
            }
        }
        
        if (optionsButton && parentMenu) {
            // make btn
            auto myButtonSprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            auto myButton = CCMenuItemSpriteExtra::create(
                myButtonSprite,
                this,
                menu_selector(MyPauseLayer::onMyButton)
            );
            
            // put below options
            auto optionsPos = optionsButton->getPosition();
            myButton->setPosition(optionsPos.x, optionsPos.y - 100);
            parentMenu->addChild(myButton);
        } else {
            log::error("Options button not found!");
        }
    }
    
    void onMyButton(CCObject*) {
        ChatConfigPopup::create()->show();
    }
};