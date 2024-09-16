/**
 * \file libipc/platform/win/shm_impl.h
 * \author mutouyun (orz@orzz.org)
 */
#pragma once

#include <string>
#include <cstddef>

#include "libimp/log.h"
#include "libimp/system.h"
#include "libpmr/new.h"
#include "libipc/shm.h"

#include "mmap_impl.h"
#include "close_handle.h"

LIBIPC_NAMESPACE_BEG_

using namespace ::LIBIMP;
using namespace ::LIBPMR;

result<shm_t> shm_open(std::string name, std::size_t size, mode::type type) noexcept {
  LIBIMP_LOG_();
  auto h = mmap_open(name, size, type);
  if (*h == NULL) {
    log.error("failed: mmap_open(name = ", name, ", size = ", size, ", type = ", type, ").");
    return h.error();
  }
  auto mem = mmap_memof(*h);
  if (*mem == NULL) {
    log.error("failed: mmap_memof(", *h, ").");
    winapi::close_handle(*h);
    return mem.error();
  }
  auto sz = mmap_sizeof(*mem);
  if (!sz) {
    log.error("failed: mmap_sizeof(", *mem, ").");
    winapi::close_handle(*h);
    return sz.error();
  }
  return $new<shm_handle>(std::move(name), *sz, *mem, *h);
}

result<void> shm_close(shm_t h) noexcept {
  LIBIMP_LOG_();
  if (h == nullptr) {
    log.error("shm handle is null.");
    return std::make_error_code(std::errc::invalid_argument);
  }
  auto shm = static_cast<shm_handle *>(h);
  auto ret = mmap_release(shm->h_fmap, shm->memp);
  $delete(shm);
  return ret;
}

LIBIPC_NAMESPACE_END_
