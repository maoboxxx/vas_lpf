#include <mutex>
#include "vasPluginContext.h"
#include "vasPlugin.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <dlfcn.h>
#ifdef __cplusplus
}
#endif

typedef vasPlugin *(*P_CREATE_PLUGIN_FUNC)();

vasPluginContext::vasPluginContext()
{
    
}

vasPluginContext::~vasPluginContext()
{
    
}

vasPluginContext *vasPluginContext::getInstance()
{
    static vasPluginContext *s_pInstance = nullptr;
    static std::mutex s_mutex;
    std::unique_lock<std::mutex> lock(s_mutex);
    if (!s_pInstance) {
        s_pInstance = new vasPluginContext();
    }
    return s_pInstance;
}

std::string vasPluginContext::install(const std::string& pluginPath, std::string* pErrStr)
{
    std::string pgId;
    do {
        if (pluginPath.empty()) { 
            VAS_ERROR_SET(pErrStr, "plugin's path is empty !");
            break;
        }
        
        void *pHandle = dlopen(pluginPath.c_str(), RTLD_LAZY);
        if (!pHandle) {
            VAS_ERROR_SET(pErrStr, dlerror());
            break;
        }
        
        P_CREATE_PLUGIN_FUNC pCreate = (P_CREATE_PLUGIN_FUNC)dlsym(pHandle, "createPlugin");
        if (!pCreate) {
            VAS_ERROR_SET(pErrStr, dlerror());
            break;
        }
        
        vasPlugin *pPlugin = pCreate();
        if (!pPlugin) {
            VAS_ERROR_SET(pErrStr, "get plugin info failed !");
            break;
        }
        
        pgId = pPlugin->getPluginId();
        
        VAS::vasPluginInfo_St pluginInfo;
        pluginInfo._pHandle = pHandle;
        pluginInfo._pluginPath = pluginPath;
        pluginInfo._pPlugin = pPlugin;
        pluginInfo._state = VAS::VAS_PS_INSTALLED;
        
        m_pluginsMap[pgId] = pluginInfo;
        
        VAS_INFO_SET(pErrStr, "no error !");
        
    } while (0);
    return pgId;
}

bool vasPluginContext::unInstall(const std::string &pluginId, std::string *pErrStr)
{
    bool rslt = false;
    do {
        
        if (pluginId.empty() || m_pluginsMap.find(pluginId) == m_pluginsMap.end()) {
            VAS_ERROR_SET(pErrStr, "don't find the plugin id !");
            break;
        }
        
        VAS::vasPluginInfo_St &pluginInfo = m_pluginsMap.find(pluginId)->second;
        if (!pluginInfo._pPlugin || !pluginInfo._pHandle) {
            VAS_ERROR_SET(pErrStr, "an error occurred with Uninstall !");
            break;
        }
        
        if (pluginInfo._state == VAS::VAS_PS_STARTING) {
            pluginInfo._pPlugin->stop();
        }
        
        delete pluginInfo._pPlugin;
        pluginInfo._pPlugin = nullptr;
        
        dlclose(pluginInfo._pHandle);
        pluginInfo._pHandle = nullptr;
        pluginInfo._pluginPath.clear();
        pluginInfo._state = VAS::VAS_PS_UNINSTALLED;
        
    } while (0);
    return rslt;
}