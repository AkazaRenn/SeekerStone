#pragma once

#include <string>
#include <vector>

namespace Common::Settings {

template <typename ValueContainer>
class Group {
    public:
        virtual bool exists(std::vector<std::string_view>& keyStack) const = 0;

        virtual ValueContainer get(std::vector<std::string_view>& keyStack,
                                   const ValueContainer&          defaultValue) const = 0;

        virtual void set(std::vector<std::string_view>& keyStack, const ValueContainer& value) = 0;
};

template <typename ValueContainer>
using RootGroup = Group<ValueContainer>;

template <typename ValueContainer>
class InnerGroup : public Group<ValueContainer> {
    public:
        explicit InnerGroup(const std::string& _key, Group<ValueContainer>& _parentGroup)
            : key(_key)
            , parentGroup(_parentGroup) {
        }

        bool exists(std::vector<std::string_view>& keyStack) const override {
            keyStack.push_back(key);
            return parentGroup.exists(keyStack);
        }

        ValueContainer get(std::vector<std::string_view>& keyStack, const ValueContainer& defaultValue) const override {
            keyStack.push_back(key);
            return parentGroup.get(keyStack, defaultValue);
        }

        void set(std::vector<std::string_view>& keyStack, const ValueContainer& value) override {
            keyStack.push_back(key);
            parentGroup.set(keyStack, value);
        }

    private:
        const std::string      key;
        Group<ValueContainer>& parentGroup;
};

template <typename ValueType, typename ValueContainer>
class Entry {
    public:
        explicit Entry(const std::string& _key, const ValueType& _defaultValue, Group<ValueContainer>& _parentGroup,
                       const std::function<ValueType(const ValueContainer&)>& _valueFromContainer,
                       const std::function<ValueContainer(const ValueType&)>& _createValueContainer)
            : key(_key)
            , parentGroup(_parentGroup)
            , valueFromContainer(_valueFromContainer)
            , createValueContainer(_createValueContainer)
            , value(getValue(_defaultValue)) {
            if (!exists()) {
                saveValue();
            }
        }

        operator ValueType() const {
            return value;
        }

        Entry& operator=(const ValueType& _value) {
            value = _value;
            saveValue();
            return *this;
        }

    private:
        const std::string                                     key;
        Group<ValueContainer>&                                parentGroup;
        const std::function<ValueType(const ValueContainer&)> valueFromContainer;
        const std::function<ValueContainer(const ValueType&)> createValueContainer;
        ValueType                                             value;

        ValueType getValue(const ValueType& defaultValue) const {
            std::vector<std::string_view> keyStack{key};
            return valueFromContainer(parentGroup.get(keyStack, createValueContainer(defaultValue)));
        }

        void saveValue() {
            std::vector<std::string_view> keyStack{key};
            parentGroup.set(keyStack, createValueContainer(value));
        }

        bool exists() {
            std::vector<std::string_view> keyStack{key};
            return parentGroup.exists(keyStack);
        }
};
} // namespace Common::Settings
