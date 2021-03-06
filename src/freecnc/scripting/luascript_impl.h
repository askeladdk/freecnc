#ifndef LUASCRIPT_IMPL_H
#error Internal header file do not include
#endif

namespace LuaScriptPriv {
    // Find the length of the array of registrations
    template<class T>
    size_t length(LuaScript::Reg<T>* methods)
    {
        size_t ret = 0;
        while (methods->name) {
            ++ret;
            ++methods;
        }
        return ret;
    }

    // All of the bound member functions go through this function.
    // Retrieves the this pointer and the pointer to member function from the
    // closure and completes the call to the member function.
    template<class T, class Tfunc>
    int luabinder(lua_State* L)
    {
        T* obj = static_cast<T*>(lua_touserdata(L, lua_upvalueindex(1)));
        union { Tfunc func; char data[sizeof(Tfunc)]; } s;

        memcpy(s.data, lua_touserdata(L, lua_upvalueindex(2)), sizeof(Tfunc));
        return (obj->*(s.func))(L);
    }
}

template<class T>
void LuaScript::register_methods(const char* name, T* obj, Reg<T>* methods)
{
    size_t num_methods = length(methods);

    lua_createtable(L, num_methods, 0);
    for (; num_methods--; ++methods) {
        lua_pushstring(L, methods->name);
        push_member_function(obj, methods->func);
        lua_settable(L, -3);
    }
    lua_setglobal(L, name);
}

template<class T>
void LuaScript::register_methods(T* obj, Reg<T>* methods)
{
    for (; methods->name; ++methods) {
        push_member_function(obj, methods->func);
        lua_setglobal(L, methods->name);
    }
}

// Store the this pointer and the pointer to member function inside a closure
// associated with a call to specialisation of luabinder.
template<class T>
void LuaScript::push_member_function(T* obj, int (T::*func)(lua_State*))
{
    typedef int (T::*Tfunc)(lua_State*);

    lua_pushlightuserdata(L, obj);
    void* buf = lua_newuserdata(L, sizeof(Tfunc));
    // This approach means we aren't affected by the size of the pointer
    // to member function being variable depending on various factors.
    union { Tfunc func; char data[sizeof(Tfunc)]; } s;
    s.func = func;
    memcpy(buf, s.data, sizeof(Tfunc));
    lua_pushcclosure(L, LuaScriptPriv::luabinder<T, Tfunc>, 2);
}
