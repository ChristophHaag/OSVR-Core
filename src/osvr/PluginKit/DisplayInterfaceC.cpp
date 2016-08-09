/** @file
@brief Implementation

@date 2015

@author
Sensics, Inc.
<http://sensics.com/osvr>

*/

// Copyright 2015 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include <osvr/PluginKit/DisplayInterfaceC.h>
#include <osvr/Connection/DeviceInitObject.h>
#include <osvr/Connection/DeviceToken.h>
#include <osvr/PluginHost/PluginSpecificRegistrationContext.h>
#include <osvr/Common/DisplayComponent.h>
#include "HandleNullContext.h"
#include <osvr/Util/Verbosity.h>
#include <osvr/Connection/DeviceInterfaceBase.h>

// Library/third-party includes
// - none

// Standard includes
// - none

struct OSVR_Display_DeviceInterfaceObject
    : public osvr::connection::DeviceInterfaceBase {
    osvr::common::DisplayComponent *display;
};

OSVR_ReturnCode osvrDeviceDisplayConfigure(
    OSVR_INOUT_PTR OSVR_DeviceInitOptions opts,
    OSVR_OUT_PTR OSVR_Display_DeviceInterface *iface,
    OSVR_IN OSVR_ChannelCount numSensors) {

    OSVR_PLUGIN_HANDLE_NULL_CONTEXT("osvrDeviceDisplayConfigure", opts);
    OSVR_PLUGIN_HANDLE_NULL_CONTEXT("osvrDeviceDisplayConfigure", iface);
    OSVR_Display_DeviceInterface ifaceObj =
        opts->makeInterfaceObject<OSVR_Display_DeviceInterfaceObject>();
    *iface = ifaceObj;
    auto display = osvr::common::DisplayComponent::create(numSensors);
    ifaceObj->display = display.get();
    opts->addComponent(display);
    return OSVR_RETURN_SUCCESS;
}

OSVR_ReturnCode osvrDeviceDisplayReportData(
    OSVR_IN_PTR OSVR_Display_DeviceInterface iface,
    OSVR_IN_PTR OSVR_DisplayState displayData,
    OSVR_IN OSVR_ChannelCount sensor,
    OSVR_IN_PTR OSVR_TimeValue const *timestamp) {
    auto guard = iface->getSendGuard();
    if (guard->lock()) {
        iface->display->sendDisplayData(displayData, sensor, *timestamp);
        return OSVR_RETURN_SUCCESS;
    }

    return OSVR_RETURN_FAILURE;
}