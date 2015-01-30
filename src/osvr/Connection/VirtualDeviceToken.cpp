/** @file
    @brief Implementation

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include "VirtualDeviceToken.h"
#include <osvr/Connection/ConnectionDevice.h>
#include <osvr/Util/Verbosity.h>
#include <osvr/Util/GuardInterfaceDummy.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace connection {
    VirtualDeviceToken::VirtualDeviceToken(std::string const &name)
        : DeviceToken(name) {}

    VirtualDeviceToken::~VirtualDeviceToken() {}

    bool
    VirtualDeviceToken::m_callWhenSafeToSend(std::function<void()> &callback) {
        callback();
        return true;
    }

    void VirtualDeviceToken::m_sendData(util::time::TimeValue const &timestamp,
                                        MessageType *type,
                                        const char *bytestream, size_t len) {
        m_getConnectionDevice()->sendData(timestamp, type, bytestream, len);
    }

    GuardPtr VirtualDeviceToken::m_getSendGuard() {
        return GuardPtr(new util::DummyGuard);
    }

    void VirtualDeviceToken::m_connectionInteract() {}
} // namespace connection
} // namespace osvr
