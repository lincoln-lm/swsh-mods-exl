#pragma once
template <typename Ret, typename... Args>
inline static Ret external(long addr, Args... args) {
    return reinterpret_cast<Ret(*)(Args...)>(exl::util::modules::GetTargetOffset(addr))(args...);
}
template <typename Ret, typename... Args>
inline static Ret external_absolute(long addr, Args... args) {
    return reinterpret_cast<Ret(*)(Args...)>(static_cast<uintptr_t>(addr))(args...);
}
template <typename Ret>
inline static Ret read_main(long addr) {
    return *reinterpret_cast<Ret(*)>(exl::util::modules::GetTargetOffset(addr));
}
