
#include "Settings.hpp"

namespace SeekerStone::Components {

bool Settings::exists(std::vector<std::reference_wrapper<const std::string>>& keyStack) const {
    return settings.contains(fullKey(keyStack));
}

QVariant Settings::get(std::vector<std::reference_wrapper<const std::string>>& keyStack,
                       const QVariant&                                         defaultValue) const {
    return settings.value(fullKey(keyStack), defaultValue);
}

void Settings::set(std::vector<std::reference_wrapper<const std::string>>& keyStack, const QVariant& value) {
    settings.setValue(fullKey(keyStack), value);
}

QString Settings::fullKey(std::vector<std::reference_wrapper<const std::string>>& keyStack) {
    QStringList parts;
    for (auto it = keyStack.rbegin(); it != keyStack.rend(); it++)
        parts << QString::fromStdString(*it);
    return parts.join('/');
}
} // namespace SeekerStone::Components
