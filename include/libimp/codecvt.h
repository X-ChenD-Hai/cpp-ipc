/**
 * @file libimp/codecvt.h
 * @author mutouyun (orz@orzz.org)
 * @brief Character set conversion interface
 * @date 2022-08-07
 */
#pragma once

#include <string>
#include <cstddef>

#include "libimp/def.h"
#include "libimp/export.h"

LIBIMP_NAMESPACE_BEG_

/**
 * @brief The transform between UTF-8/16/32
 * 
 * @param des  The target string pointer can be nullptr
 * @param dlen The target string length can be 0
 */
template <typename CharT, typename CharU>
LIBIMP_EXPORT std::size_t cvt_cstr(CharT const *src, std::size_t slen, CharU *des, std::size_t dlen) noexcept;

template <typename CharT, typename TraitsT, typename AllocT, 
          typename CharU, typename TraitsU, typename AllocU>
void cvt_sstr(std::basic_string<CharT, TraitsT, AllocT> const &src, std::basic_string<CharU, TraitsU, AllocU> &des) {
  std::size_t dlen = cvt_cstr(src.c_str(), src.size(), (CharU *)nullptr, 0);
  if (dlen == 0) {
    des.clear();
    return;
  }
  des.resize(dlen);
  cvt_cstr(src.c_str(), src.size(), &des[0], des.size());
}

LIBIMP_NAMESPACE_END_