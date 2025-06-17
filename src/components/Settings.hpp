#pragma once

#include <QSettings>
#include <QString>
#include <QVariant>
#include <mutex>

#include "../common/Macros.hpp"
#include "../common/Settings.hpp"

namespace {
const QString SETTINGS_FILE_NAME = "settings";
}

namespace SeekerStone::Components {
namespace _Settings {
using InnerGroup = Common::Settings::InnerGroup<QVariant>;

template <typename ValueType>
class Entry : public Common::Settings::Entry<ValueType, QVariant> {
    public:
        explicit Entry(const std::string& key, const ValueType& defaultValue, Common::Settings::Group<QVariant>& group)
            : Common::Settings::Entry<ValueType, QVariant>(key, defaultValue, group, Entry::getValueFromContainer,
                                                           Entry::createValueContainer) {
        }

    private:
        static ValueType getValueFromContainer(const QVariant& container) {
            return container.value<ValueType>();
        }

        static QVariant createValueContainer(const ValueType& value) {
            return QVariant(value);
        }
};
} // namespace _Settings

#define SETTINGS_GROUP_BEGIN(name)                                   \
    class SettingsGroupClass_##name : public _Settings::InnerGroup { \
        public:                                                      \
            using _Settings::InnerGroup::InnerGroup;

#define SETTINGS_GROUP_END(name) \
    }                            \
    ;                            \
    SettingsGroupClass_##name name{#name, *this};

#define SETTINGS_ENTRY(name, type, defaultValue) _Settings::Entry<type> name{#name, defaultValue, *this};

class Settings : public Common::Settings::RootGroup<QVariant> {
    public:
        bool     exists(std::vector<std::string_view>& keyStack) const override;
        QVariant get(std::vector<std::string_view>& keyStack, const QVariant& defaultValue) const override;
        void     set(std::vector<std::string_view>& keyStack, const QVariant& value) override;

    private:
        QSettings          settings{APP_NAME, SETTINGS_FILE_NAME};
        mutable std::mutex settingsMutex;

        static QString fullKey(std::vector<std::string_view>& keyStack);

    public:
        SETTINGS_GROUP_BEGIN(general0);
        SETTINGS_ENTRY(option1, bool, true);
        SETTINGS_ENTRY(option2, int, 5);
        SETTINGS_ENTRY(option3, QString, "fdsa");
        SETTINGS_GROUP_BEGIN(group1);
        SETTINGS_ENTRY(option1, bool, true);
        SETTINGS_ENTRY(option2, int, 5);
        SETTINGS_ENTRY(option3, QString, "fdsa");
        SETTINGS_GROUP_BEGIN(group2);
        SETTINGS_ENTRY(option1, bool, true);
        SETTINGS_ENTRY(option2, int, 5);
        SETTINGS_ENTRY(option3, QString, "fdsa");
        SETTINGS_GROUP_END(group2);
        SETTINGS_GROUP_END(group1);
        SETTINGS_GROUP_END(general0);
};
} // namespace SeekerStone::Components
