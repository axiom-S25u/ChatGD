#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/modify/MenuLayer.hpp>


using namespace geode::prelude;

static std::vector<std::string> GD_PLAYERS = {
    // Streamers
    "Michigun", "Viprin", "Riot", "Juniper", "Wulzy",
    "EVW", "Doggie", "Nexus", "AeonAir", "Tride",
    "Cyclic", "Knobbelboy", "Sunix", "Technical49", "Dorami",
    "SpaceUK", "Diamond", "Trick", "Zoink", "Nswish",
    "Cursed", "BlassCFB", "MiKhaXx", "Mullsy", "Luqualizer",
    "Npesta", "xanii", "BTD6", "Cataclysm",
    "Krazyman50", "Zobros", "Sea1997", "Pennutoh", "FunnyGame",
    "TrusTa", "RicoLP", "ViPriN", "ChaSe", "Lemons",
    "Vortrox",
    // Contriubtors
    "Axiom", "Human", "siniNight"
};

// static std::vector<bool> hasSpoken;

static std::string randomPlayer() {
    return GD_PLAYERS[rand() % GD_PLAYERS.size()];
}

static std::string chat(const std::string& msg) {
    return randomPlayer() + ": " + msg;
}

static std::string levelKey(int levelID, const char* suffix) {
    return std::to_string(levelID) + suffix;
}

static float loadPercentForLevel(int levelID, const char* suffix, float defaultValue) {
    auto key = levelKey(levelID, suffix);
    auto legacyValue = Mod::get()->getSavedValue<int>(key, static_cast<int>(defaultValue));
    return Mod::get()->getSavedValue<float>(key, static_cast<float>(legacyValue));
}

static bool loadDisabledForLevel(int levelID, const char* suffix, bool defaultValue) {
    auto key = levelKey(levelID, suffix);
    return Mod::get()->getSavedValue<bool>(key, defaultValue);
}

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        CCLayer* m_chatBox = nullptr;
        CCLabelBMFont* m_chatText = nullptr;
        float m_lastPercent = 0.0f;
        float m_randomChatTimer = 0.0f;
        float m_nextChatDelay = 0.5f;
        float m_deathChatTimer = 0.0f;
        float m_deathSpamDuration = 2.0f;
        bool m_isDeathSpamming = false;
        float holdPercent = 22;
        float goPercent = 37;
        float superGoPercent = 80;
        int att = 0;
        bool enabled = true;
    };

public:
    void reloadThresholds() {
        if (!m_level) return;
        auto fields = m_fields.self();
        fields->holdPercent = loadPercentForLevel(m_level->m_levelID, "hold-percent", 22.0f);
        fields->goPercent = loadPercentForLevel(m_level->m_levelID, "go-percent", 37.0f);
        fields->superGoPercent = loadPercentForLevel(m_level->m_levelID, "supergo-percent", 80.0f);
        fields->enabled = loadDisabledForLevel(m_level->m_levelID, "enabled", true);
    }
    
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        
        this->reloadThresholds();

        auto fields = m_fields.self();
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // chat box
        fields->m_chatBox = CCLayerColor::create({0, 0, 0, 180}, 100.0f, 150.0f);
        fields->m_chatBox->setPosition(winSize.width - 110, 10); //pos
        fields->m_chatBox->setZOrder(20);
        this->m_uiLayer->addChild(fields->m_chatBox);

        // text
        fields->m_chatText = CCLabelBMFont::create("", "chatFont.fnt");
        fields->m_chatText->setScale(0.4f);
        fields->m_chatText->setAnchorPoint({0, 1});
        fields->m_chatText->setPosition(winSize.width - 105, 145);
        fields->m_chatText->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
        fields->m_chatText->setZOrder(101);
        this->m_uiLayer->addChild(fields->m_chatText);
                
        this->schedule(schedule_selector(MyPlayLayer::checkProgress));
        
        return true;
    }
    
    void checkProgress(float dt) {
        auto fields = m_fields.self();
        float progress = this->getCurrentPercent();
        bool inPractice = this->m_isPracticeMode && !Mod::get()->getSettingValue<bool>("enabled-in-practice");
        bool visible = !inPractice && !fields->enabled;
        fields->m_chatBox->setVisible(visible);
        fields->m_chatText->setVisible(visible);
        
        if (!visible) return;
        // NOOOOOOOO
        if (fields->m_isDeathSpamming) {
            fields->m_deathChatTimer += dt;
            
            if (fields->m_deathChatTimer >= fields->m_deathSpamDuration) {
                fields->m_isDeathSpamming = false;
                fields->m_deathChatTimer = 0;
            } else {
                fields->m_randomChatTimer += dt;
                if (fields->m_randomChatTimer >= fields->m_nextChatDelay) {
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
                    fields->m_randomChatTimer = 0;
                }
            }
            return;
        }
        
        // hold
        if (progress >= fields->holdPercent && progress < fields->goPercent) {
            fields->m_randomChatTimer += dt;
            if (fields->m_randomChatTimer >= fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("holdlldldldl"),
                    chat("HOLD IT"),
                    chat("HOOOOOOOOLDDDDDDDDDDDD"),
                    chat("HOOOLD"),
                    chat("HOLDDDDDDDDDDDDDDDDDDD")
                };
                addChatMessage(messages[rand() % messages.size()]);
                fields->m_randomChatTimer = 0;
                float t = (progress - fields->holdPercent) / (fields->goPercent - fields->holdPercent);
                fields->m_nextChatDelay = 0.3f - (t * 0.25f);
            }
        }
        // gooo
        else if (progress >= fields->goPercent && progress < fields->superGoPercent) {
            fields->m_randomChatTimer += dt;
            if (fields->m_randomChatTimer >= fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("GOOOO"),
                    chat("LETS GOOOOO"),
                    chat("CMON"),
                    chat("GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"),
                };
                addChatMessage(messages[rand() % messages.size()]);
                fields->m_randomChatTimer = 0;
                float t = (progress - fields->goPercent) / (fields->superGoPercent - fields->goPercent);
                fields->m_nextChatDelay = 0.2f - (t * 0.17f);
            }
        }
        // super go
        else if (progress >= fields->superGoPercent && progress < 99.9999f) {
            fields->m_randomChatTimer += dt;
            if (fields->m_randomChatTimer >= fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("SUPER GOOOOOOOOOOOOOOOOOOOOOO"),
                    chat("SUPERGOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!"),
                    chat("I WAS HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"),
                    chat("CMONNNN"),
                    chat("GOOOOO GOOOOOO GOOOOOOOO")
                };
                addChatMessage(messages[rand() % messages.size()]);
                fields->m_randomChatTimer = 0;
                float t = (progress - fields->superGoPercent) / (99.9999f - fields->superGoPercent);
                fields->m_nextChatDelay = 0.15f - (t * 0.14f);
            }
        }
        // 100%: gg
        else if (progress > 99.9999f) {
            fields->m_randomChatTimer += dt;
            if (fields->m_randomChatTimer >= fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("GG"),
                    chat("GGS"),
                    chat("WWWWWWWWWWWWWWWWWWWWWWWW"),
                    chat("LETS GOOOOO"),
                    chat("WOOOOOOOOOOOOO")
                };
                addChatMessage(messages[rand() % messages.size()]);
                fields->m_randomChatTimer = 0;
                fields->m_nextChatDelay = 0.1f + (rand() % 3) / 10.0f;
            }
        } else {
            fields->m_randomChatTimer += dt;
            if (fields->m_randomChatTimer >= fields->m_nextChatDelay) {
                std::vector<std::string> messages = {
                    chat("Hi")
                };
                addChatMessage(messages[rand() % messages.size()]);
                fields->m_randomChatTimer = 0;
                fields->m_nextChatDelay = 0.5f;
            }
        }
    }
    
    static std::string wrapMessage(const std::string& msg, int maxChars = 28) {
        std::string result;
        int lineLen = 0;
        for (size_t i = 0; i < msg.size(); ) {
            size_t wordEnd = msg.find(' ', i);
            if (wordEnd == std::string::npos) wordEnd = msg.size();
            std::string word = msg.substr(i, wordEnd - i);

            while ((int)word.size() > maxChars) {
                int space = maxChars - lineLen;
                if (!result.empty() && lineLen > 0) result += '\n';
                else if (!result.empty()) {}
                result += word.substr(0, space);
                word = word.substr(space);
                result += '\n';
                lineLen = 0;
            }

            if (lineLen > 0 && lineLen + 1 + (int)word.size() > maxChars) {
                result += '\n';
                lineLen = 0;
            }
            if (lineLen > 0) { result += ' '; lineLen++; }
            result += word;
            lineLen += (int)word.size();

            i = wordEnd;
            while (i < msg.size() && msg[i] == ' ') i++;
        }
        return result;
    }

    void addChatMessage(std::string message) {
        auto fields = m_fields.self();
        std::string wrapped = wrapMessage(message);
        std::string currentText = fields->m_chatText->getString();

        int lineCount = 0;
        for (char c : currentText) {
            if (c == '\n') lineCount++;
        }

        int newLines = 0;
        for (char c : wrapped) {
            if (c == '\n') newLines++;
        }

        while (lineCount + newLines + 1 > 19) {
            size_t pos = currentText.find('\n');
            if (pos == std::string::npos) { currentText.clear(); lineCount = 0; break; }
            currentText = currentText.substr(pos + 1);
            lineCount--;
        }

        std::string newText = currentText.empty() ? wrapped : currentText + "\n" + wrapped;
        fields->m_chatText->setString(newText.c_str());
    }
    
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        auto fields = m_fields.self();
        // lwk fried fix but eh it works lol
        if(fields->att > 16 && !fields->m_isDeathSpamming) {
            fields->m_isDeathSpamming = true;
            float progress = this->getCurrentPercent();
            float t = progress / 100.0f;
            fields->m_deathSpamDuration = 2.0f + (t * t * 12.0f);
            fields->m_nextChatDelay = 0.5f - (t * 0.49f);
        }
        fields->att += 1;
    }
    
    void resetLevel() {
        PlayLayer::resetLevel();
    }
};

class ChatConfigPopup : public geode::Popup {
protected:
    geode::TextInput* m_textInput1 = nullptr;
    geode::TextInput* m_textInput2 = nullptr;
    geode::TextInput* m_textInput3 = nullptr;
    CCMenuItemToggler* m_enableToggle = nullptr;
    
    bool init(float width, float height) {
        if (!Popup::init(width, height))
            return false;
        
        this->setTitle("ChatGD Config");
        auto center = m_mainLayer->getContentSize() / 2;

        float holdPercent = 22.0f;
        float goPercent = 37.0f;
        float superGoPercent = 80.0f;
        bool enabled = true;
        if (auto playLayer = PlayLayer::get(); playLayer && playLayer->m_level) {
            auto levelID = playLayer->m_level->m_levelID;
            holdPercent = loadPercentForLevel(levelID, "hold-percent", 22.0f);
            goPercent = loadPercentForLevel(levelID, "go-percent", 37.0f);
            superGoPercent = loadPercentForLevel(levelID, "supergo-percent", 80.0f);
            enabled = loadDisabledForLevel(levelID, "enabled", true);
        }
        
        // box 1
        auto label1 = cocos2d::CCLabelBMFont::create("Hold %:", "bigFont.fnt");
        label1->setPosition({center.width - 120, center.height + 40});
        label1->setScale(0.3f);
        m_mainLayer->addChild(label1);

        m_textInput1 = geode::TextInput::create(200.0f, "");
        m_textInput1->setPosition({center.width + 30, center.height + 40});
        m_textInput1->setFilter("0123456789");
        m_textInput1->setMaxCharCount(3);
        m_textInput1->setString(std::to_string(static_cast<int>(holdPercent)));
        m_mainLayer->addChild(m_textInput1);

        // box 2
        auto label2 = cocos2d::CCLabelBMFont::create("Go %:", "bigFont.fnt");
        label2->setPosition({center.width - 127, center.height + 0});
        label2->setScale(0.3f);
        m_mainLayer->addChild(label2);

        m_textInput2 = geode::TextInput::create(200.0f, "");
        m_textInput2->setPosition({center.width + 30, center.height + 0});
        m_textInput2->setFilter("0123456789");
        m_textInput2->setMaxCharCount(3);
        m_textInput2->setString(std::to_string(static_cast<int>(goPercent)));
        m_mainLayer->addChild(m_textInput2);

        // box 3
        auto label3 = cocos2d::CCLabelBMFont::create("Super Go %:", "bigFont.fnt");
        label3->setPosition({center.width - 111, center.height - 40});
        label3->setScale(0.3f);
        m_mainLayer->addChild(label3);

        m_textInput3 = geode::TextInput::create(200.0f, "");
        m_textInput3->setPosition({center.width + 30, center.height - 40});
        m_textInput3->setFilter("0123456789");
        m_textInput3->setMaxCharCount(3);
        m_textInput3->setString(std::to_string(static_cast<int>(superGoPercent)));
        m_mainLayer->addChild(m_textInput3);

        // toggle
        auto label4 = cocos2d::CCLabelBMFont::create("Enabled:", "bigFont.fnt");
        label4->setPosition({center.width - 118, center.height - 80});
        label4->setScale(0.3f);
        m_mainLayer->addChild(label4);

        auto toggleMenu = CCMenu::create();
        toggleMenu->setPosition({center.width + 30, center.height - 80});
        m_enableToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(ChatConfigPopup::onToggle), 0.6f
        );
        m_enableToggle->toggle(!enabled);
        toggleMenu->addChild(m_enableToggle);
        m_mainLayer->addChild(toggleMenu);

        return true;
    }

    void onToggle(CCObject*) {}
    
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
        if (!playLayer || !playLayer->m_level) {
            log::error("ChatGD: Could not save config");
            geode::Popup::onClose(sender);
            return;
        }
        //get id
        int levelID = playLayer->m_level->m_levelID;
        // get vals
        std::string holdStr = m_textInput1->getString();
        std::string goStr = m_textInput2->getString();
        std::string superGoStr = m_textInput3->getString();
        
        Mod::get()->setSavedValue(levelKey(levelID, "hold-percent"), geode::utils::numFromString<float>(holdStr).unwrapOrDefault());
        Mod::get()->setSavedValue(levelKey(levelID, "go-percent"), geode::utils::numFromString<float>(goStr).unwrapOrDefault());
        Mod::get()->setSavedValue(levelKey(levelID, "supergo-percent"), geode::utils::numFromString<float>(superGoStr).unwrapOrDefault());

        Mod::get()->setSavedValue(levelKey(levelID, "enabled"), !m_enableToggle->isToggled());

        if (auto basePlayLayer = typeinfo_cast<PlayLayer*>(playLayer)) {
            static_cast<MyPlayLayer*>(basePlayLayer)->reloadThresholds();
        }
        
        geode::Popup::onClose(sender);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) {
            log::error("right-button-menu not found");
            return;
        }

        auto myButtonSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
        auto myButton = CCMenuItemSpriteExtra::create(
            myButtonSprite,
            this,
            menu_selector(MyPauseLayer::onMyButton)
        );

        rightMenu->addChild(myButton);
        static_cast<CCMenu*>(rightMenu)->updateLayout();
    }
    
    void onMyButton(CCObject*) {
        ChatConfigPopup::create()->show();
    }
};

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        std::thread([]() {
            auto res = web::WebRequest().getSync("https://badges.hiimjasmine00.com/developer");
            if (!res.ok()) {
                log::error("Failed to fetch dev list: {}", res.code());
                return;
            }
            auto arr = res.json().unwrapOr(matjson::Value{})
                .asArray().unwrapOr(std::vector<matjson::Value>{});
            for (auto& entry : arr) {
                GD_PLAYERS.push_back(entry["name"].asString().unwrapOrDefault());
            }
            std::sort(GD_PLAYERS.begin(), GD_PLAYERS.end());
            GD_PLAYERS.erase(std::unique(GD_PLAYERS.begin(), GD_PLAYERS.end()), GD_PLAYERS.end());
        }).detach();
        // hasSpoken.assign(GD_PLAYERS.size(), false);
        return true;
    }
};