#pragma once

#include <string.h>

#include "imgui.h"
#include "imgui_internal.h"

namespace ImPop {

// Enum for config item types
enum class ConfigType { Bool, Int, Float, String };

// Structure to hold configuration items
struct ConfigItem {
    const char* name;
    ConfigType type;
    union {
        bool bool_value;
        int int_value;
        float float_value;
    };
    const char* string_value;

    ConfigItem(const char* n, bool value) : name(n), type(ConfigType::Bool), bool_value(value) {}
    ConfigItem(const char* n, int value) : name(n), type(ConfigType::Int), int_value(value) {}
    ConfigItem(const char* n, float value) : name(n), type(ConfigType::Float), float_value(value) {}
    ConfigItem(const char* n, const char* value) : name(n), type(ConfigType::String), string_value(value) {}
};

static ConfigItem* config_items = nullptr;
static int config_items_count = 0;
static ImGuiStorage config_storage;

template <typename T>
inline void InitializeConfig(T& items) {
    config_items = (ConfigItem*)(&items);
    config_items_count = sizeof(T) / sizeof(ConfigItem);

    ImGuiSettingsHandler ini_handler;
    ini_handler.TypeName = "CustomSettings";
    ini_handler.TypeHash = ImHashStr("CustomSettings");
    ini_handler.ReadOpenFn = [](ImGuiContext*, ImGuiSettingsHandler*, const char* name) -> void* {
        if (strcmp(name, "Settings") == 0) return (void*)1;
        return nullptr;
    };
    ini_handler.ReadLineFn = [](ImGuiContext*, ImGuiSettingsHandler*, void*, const char* line) {
        char key_name[128];
        char value[128];
        if (sscanf(line, "%127[^=]=%127[^\n]", key_name, value) == 2) {
            for (int i = 0; i < config_items_count; ++i) {
                ConfigItem& item = config_items[i];
                if (strcmp(item.name, key_name) == 0) {
                    ImGuiID key = ImGui::GetID(item.name);
                    switch (item.type) {
                        case ConfigType::Bool:
                            item.bool_value = (strcmp(value, "true") == 0 || strcmp(value, "1") == 0);
                            config_storage.SetBool(key, item.bool_value);
                            break;
                        case ConfigType::Int:
                            item.int_value = atoi(value);
                            config_storage.SetInt(key, item.int_value);
                            break;
                        case ConfigType::Float:
                            item.float_value = static_cast<float>(atof(value));
                            config_storage.SetFloat(key, item.float_value);
                            break;
                        case ConfigType::String:
                            item.string_value = strdup(value);
                            config_storage.SetVoidPtr(key, (void*)item.string_value);
                            break;
                    }
                    break;
                }
            }
        }
    };
    ini_handler.WriteAllFn = [](ImGuiContext*, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf) {
        out_buf->appendf("[%s][Settings]\n", handler->TypeName);
        for (int i = 0; i < config_items_count; ++i) {
            ConfigItem& item = config_items[i];
            switch (item.type) {
                case ConfigType::Bool:
                    out_buf->appendf("%s=%s\n", item.name, item.bool_value ? "true" : "false");
                    break;
                case ConfigType::Int:
                    out_buf->appendf("%s=%d\n", item.name, item.int_value);
                    break;
                case ConfigType::Float:
                    out_buf->appendf("%s=%.3f\n", item.name, item.float_value);
                    break;
                case ConfigType::String:
                    out_buf->appendf("%s=%s\n", item.name, item.string_value);
                    break;
            }
        }
    };
    ImGui::GetCurrentContext()->SettingsHandlers.push_back(ini_handler);

    ImGui::LoadIniSettingsFromDisk(ImGui::GetIO().IniFilename);
}

inline void ConfigMenu(const char* name = "Config") {
    if (ImGui::BeginMenu(name)) {
        for (int i = 0; i < config_items_count; ++i) {
            ConfigItem& item = config_items[i];
            ImGuiID key = ImGui::GetID(item.name);
            switch (item.type) {
                case ConfigType::Bool:
                    if (ImGui::MenuItem(item.name, nullptr, &item.bool_value)) {
                        config_storage.SetBool(key, item.bool_value);
                    }
                    break;
                case ConfigType::Int:
                    if (ImGui::InputInt(item.name, &item.int_value)) {
                        config_storage.SetInt(key, item.int_value);
                    }
                    break;
                case ConfigType::Float:
                    if (ImGui::InputFloat(item.name, &item.float_value)) {
                        config_storage.SetFloat(key, item.float_value);
                    }
                    break;
                case ConfigType::String:
                    char buffer[256];
                    strncpy(buffer, item.string_value, sizeof(buffer));
                    if (ImGui::InputText(item.name, buffer, sizeof(buffer))) {
                        item.string_value = strdup(buffer);
                        config_storage.SetVoidPtr(key, (void*)item.string_value);
                    }
                    break;
            }
        }
        ImGui::EndMenu();
    }
}

} // namespace ImPop
