#pragma once

#include <map>
#include <string>
#include <functional>

/*** macro ***/

#ifndef VAS_DISABLED_COPY
    #define VAS_DISABLED_COPY(ClassName) \
        ClassName(const ClassName &) = delete; \
        ClassName &operator=(const ClassName &) = delete;
#endif /*VAS_DISABLED_COPY*/

/**
 * @brief VAS_PLUGIN 插件的构造
 *        在插件vasPlugin的派生类中必须使用此宏来替代构造函数
 * @example: 
 *        VAS_PLUGIN(PluginTest, "plugin.test")
 *        其中PluginTest为类名，"plugin.test"为该插件ID
 */
#ifndef VAS_PLUGIN
    #define VAS_PLUGIN(ClassName, pluginId) \
        ClassName(const std::string &pgId) : VAS::vasPlugin(pgId) {} \
        ~ClassName() {}
#endif /*VAS_PLUGIN*/

/**
 * @brief VAS_REGIST_PLUGIN_ID 插件的注册
 *        必须在插件vasPlugin的派生类cpp文件的末尾使用VAS_REGIST_PLUGIN_ID
 * @example:
 *        VAS_REGIST_PLUGIN_ID(PluginTest, "plugin.test")
 *        其中PluginTest为类名，"plugin.test"为该插件ID
 */
#ifndef VAS_REGIST_PLUGIN_ID
    #define VAS_REGIST_PLUGIN_ID(ClassName, pgId) \
        extern "C" VAS::vasPlugin *createPlugin() \
        { \
            return new ClassName(pgId); \
        }
#endif /*VAS_REGIST_PLUGIN_ID*/

/*error set*/
#ifndef VAS_ERROR_SET
    #define VAS_ERROR_SET(pStr, error) \
        if (pStr && error) { *pStr = std::string("[ERROR] ") + error; }
#endif /*VAS_ERROR_SET*/

/*info set*/
#ifndef VAS_INFO_SET
    #define VAS_INFO_SET(pStr, info) \
        if (pStr && info) { *pStr = std::string("[INFO] ") + info; }
#endif /*VAS_INFO_SET*/

/*empty string*/
#ifndef VAS_EMPTY_STR_SET
    #define VAS_EMPTY_STR_SET(pStr) \
        if (pStr) { *pStr = std::string(""); }
#endif /*VAS_EMPTY_STR_SET*/


/**
 * @brief VAS_USER_INTERFACE_CLASS 接口类说明
 */
#ifndef VAS_USER_INTERFACE_CLASS
    #define VAS_USER_INTERFACE_CLASS class
#endif

/**
 * @brief VAS_PRIVATE_IMPL_CLASS 实现类说明
 */
#ifndef VAS_PRIVATE_IMPL_CLASS
    #define VAS_PRIVATE_IMPL_CLASS class
#endif

#ifndef VAS_DECLARE_FRIEND_CONTEXT
    #define VAS_DECLARE_FRIEND_CONTEXT friend class VAS::vasPluginContext;
#endif


/*namespace VAS*/
namespace VAS {

/*** declaration ***/
class vasPlugin;
class vasVariant;

typedef std::map<std::string, VAS::vasVariant> vasProperty;
typedef std::function<void(vasProperty)>       vasEvent;

/*** enums ***/
enum vasPluginState_Em {
    VAS_PS_UNINSTALLED = 0,
    VAS_PS_INSTALLED,
    VAS_PS_STARTING,
    VAS_PS_STOPPING,
};

/*** structs ***/
struct vasPluginInfo_St
{
    vasPluginInfo_St()
        : _pHandle(nullptr), _pPlugin(nullptr) {}
    
    void              *_pHandle;
    std::string        _pluginPath;
    vasPlugin         *_pPlugin;
    vasPluginState_Em  _state;
};

} /*namespace VAS*/
