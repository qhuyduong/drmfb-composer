// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2019 Stephan Gerhold

#define LOG_TAG "drmfb-framebuffer-qti"

#include <android-base/logging.h>
#include <drm/drm_fourcc.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <system/graphics.h>

#include <gralloc_priv.h>
#include "DrmFramebufferImporter.h"

namespace android {
namespace hardware {
namespace graphics {
namespace composer {
namespace V2_1 {
namespace drmfb {
namespace qti {

namespace {
void addFramebuffer(int fd, const private_handle_t *handle, uint32_t* id) {
    uint32_t handles[1] = {};
    uint32_t pitches[1] = {static_cast<uint32_t>(handle->width * 4)};
    uint32_t offsets[1] = {handle->offset};

    if (drmPrimeFDToHandle(fd, handle->fd, &handles[0])) {
        PLOG(ERROR) << "Failed to get handle for prime fd " << handle->fd;
        return;
    }

    if (drmModeAddFB2(fd, handle->width, handle->height, DRM_FORMAT_XRGB8888,
                      handles, pitches, offsets, id, 0)) {
        PLOG(ERROR) << "drmModeAddFB2 failed";
    }
}
}

bool addFramebuffer(int fd, buffer_handle_t buffer, uint32_t* id) {
    const private_handle_t *handle = PRIV_HANDLE_CONST(buffer);

    if (private_handle_t::validate(handle) < 0) {
        return false;
    }

    addFramebuffer(fd, handle, id);
    return true;
}

}  // namespace qti
}  // namespace drmfb
}  // namespace V2_1
}  // namespace composer
}  // namespace graphics
}  // namespace hardware
}  // namespace android
