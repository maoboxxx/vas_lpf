#include "test.h"
#include <iostream>
#include <vasDefined.h>
#include <vasVariant.h>
#include <vasPluginContext.h>

Test::Test()
{
    /*触发事件，在plugin.test插件中完成与plugin.demo插件之间的交互*/
    doTriggered();
}

Test::~Test()
{
    
}

void Test::doTriggered()
{
    /*触发事件*/
    VAS::vasVariant var;
    VAS::vasProperty property;
    
    var.set<std::string>("vas_lpf");
    property["name"] = var;

    var.set<unsigned int>(25);
    property["age"] = var;

    var.set<double>(3.1415);
    property["money"] = var;

    VAS_PLUGIN_CONTEXT->triggerEvent("group", "event", property);
}
