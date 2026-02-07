#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

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
    };
    
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        
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
        
        // addChatMessage("Chat loaded!");
        log::info("ChatGD: Init complete!");
        
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
        
        // 22-37%: hold
        // TODO: cfg
        if (progress >= 22.0f && progress < 37.0f) {
            // addChatMessage(chat("hold"));
            // addChatMessage(chat("HOLD"));
            // addChatMessage(chat("hooold"));
            
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
        // 37-80%: gooo
        // TODO: cfg
        else if (progress >= 37.0f && progress < 80.0f) {
            // addChatMessage(chat("GOOOOOOO"));
            // addChatMessage(chat("LETS GOOOO"));
            // addChatMessage(chat("GOOOOOOOO"));            
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
        // 80-100%: super go and i was here
        // TODO: cfg
        else if (progress >= 80.0f && progress < 99.9f) {
            // addChatMessage(chat("SUPER GO!!!"));
            // addChatMessage(chat("I WAS HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"));
            // addChatMessage(chat("SUPERGOOOOOOOOOOOOOOOOOOO"));
            // addChatMessage(chat("I WAS HERE!!!!!!!!!!!!!!!!!!!!!!!"));            
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
        else if (progress > 99.9f) {
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