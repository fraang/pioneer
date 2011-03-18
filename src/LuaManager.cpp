#include "LuaManager.h"

std::auto_ptr<LuaManager> LuaManager::s_instance;

LuaManager::LuaManager() : m_lua(NULL) {
    m_lua = lua_open();

    luaL_openlibs(m_lua);

    // initialise built in stuff here
}

LuaManager::~LuaManager() {
    lua_close(m_lua);
}
