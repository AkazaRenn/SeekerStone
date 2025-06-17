#include "Settings.hpp"

namespace SeekerStone::Components {
bool Settings::exists(std::vector<std::string_view>& keyStack) const {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return settings.contains(fullKey(keyStack));
}

QVariant Settings::get(std::vector<std::string_view>& keyStack, const QVariant& defaultValue) const {
    std::lock_guard<std::mutex> lock(settingsMutex);
    return settings.value(fullKey(keyStack), defaultValue);
}

void Settings::set(std::vector<std::string_view>& keyStack, const QVariant& value) {
    std::lock_guard<std::mutex> lock(settingsMutex);
    settings.setValue(fullKey(keyStack), value);
}

QString Settings::fullKey(std::vector<std::string_view>& keyStack) {
    QStringList parts;
    for (auto it = keyStack.rbegin(); it != keyStack.rend(); it++) {
        parts << it->data();
    }
    return parts.join('/');
}
} // namespace SeekerStone::Components
