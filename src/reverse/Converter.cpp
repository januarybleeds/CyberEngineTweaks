#include "Converter.h"

#include "BasicTypes.h"
#include "LuaRED.h"

template<size_t i>
sol::object RecursiveInternalToLua(MetaArrayImpl<i>& arr, RED4ext::REDreverse::CScriptableStackFrame::CStackType& aResult, sol::state_view aLua)
{
    return sol::nil;
}

template<size_t i, typename HeadItem, typename... TailItems>
sol::object RecursiveInternalToLua(MetaArrayImpl<i, HeadItem, TailItems...>& arr, RED4ext::REDreverse::CScriptableStackFrame::CStackType& aResult, sol::state_view aLua)
{
    if (arr.MetaArrayEntry<i, HeadItem>::value.Is(aResult.type))
        return arr.MetaArrayEntry<i, HeadItem>::value.ToLua(aResult, aLua);

    return RecursiveInternalToLua<i + 1, TailItems...>(arr, aResult, aLua);
}

template<typename HeadItem, typename... TailItems>
sol::object InternalToLua(MetaArray<HeadItem, TailItems...>& arr, RED4ext::REDreverse::CScriptableStackFrame::CStackType& aResult, sol::state_view aLua)
{
    return RecursiveInternalToLua<0, HeadItem, TailItems...>(arr, aResult, aLua);
}

template<size_t i>
RED4ext::REDreverse::CScriptableStackFrame::CStackType RecursiveInternalToRED(MetaArrayImpl<i>& arr, sol::object aObject, RED4ext::REDreverse::CRTTIBaseType* apRtti, TiltedPhoques::Allocator* apAllocator)
{
    return {};
}

template<size_t i, typename HeadItem, typename... TailItems>
RED4ext::REDreverse::CScriptableStackFrame::CStackType RecursiveInternalToRED(MetaArrayImpl<i, HeadItem, TailItems...>& arr, sol::object aObject, RED4ext::REDreverse::CRTTIBaseType* apRtti, TiltedPhoques::Allocator* apAllocator)
{
    if (arr.MetaArrayEntry<i, HeadItem>::value.Is(apRtti))
        return arr.MetaArrayEntry<i, HeadItem>::value.ToRED(aObject, apRtti, apAllocator);

    return RecursiveInternalToRED<i + 1, TailItems...>(arr, aObject, apRtti, apAllocator);
}

template<typename HeadItem, typename... TailItems>
RED4ext::REDreverse::CScriptableStackFrame::CStackType InternalToRED(MetaArray<HeadItem, TailItems...>& arr, sol::object aObject, RED4ext::REDreverse::CRTTIBaseType* apRtti, TiltedPhoques::Allocator* apAllocator)
{
    return RecursiveInternalToRED<0, HeadItem, TailItems...>(arr, aObject, apRtti, apAllocator);
}

static MetaArray<
    LuaRED<int32_t, "Int32">,
    LuaRED<float, "Float">,
    LuaRED<bool, "Bool">,
    LuaRED<Quaternion, "Quaternion">,
    LuaRED<ItemID, "gameItemID">,
    LuaRED<TweakDBID, "TweakDBID">,
    LuaRED<CName, "CName">
> s_convertersMeta;

sol::object Converter::ToLua(RED4ext::REDreverse::CScriptableStackFrame::CStackType& aResult, sol::state_view aLua)
{
    return InternalToLua(s_convertersMeta, aResult, aLua);
}

RED4ext::REDreverse::CScriptableStackFrame::CStackType Converter::ToRED(sol::object aObject, RED4ext::REDreverse::CRTTIBaseType* apRtti, TiltedPhoques::Allocator* apAllocator)
{
    return InternalToRED(s_convertersMeta, aObject, apRtti, apAllocator);
}
