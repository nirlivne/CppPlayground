#pragma once

#if __has_include(<boost\version.hpp>)
#define HAS_BOOST
#endif

constexpr bool has_boost()
{
#ifdef HAS_BOOST
    return true;
#else
    return false;
#endif
}

#ifdef HAS_BOOST

/// <summary>
/// Use boot::combine to range eloop on multiple containers at once
/// </summary>
// void boost_loop_combine();

#endif // HAS_BOOST
