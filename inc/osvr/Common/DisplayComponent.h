/** @file
@brief Header

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

#ifndef INCLUDED_DisplayComponent_h_GUID_A831481F_0257_449A_6404_3B789BD89CEF
#define INCLUDED_DisplayComponent_h_GUID_A831481F_0257_449A_6404_3B789BD89CEF

// Internal Includes
#include <osvr/Common/Export.h>
#include <osvr/Common/DeviceComponent.h>
#include <osvr/Common/SerializationTags.h>
#include <osvr/Util/ChannelCountC.h>
#include <osvr/Util/ClientReportTypesC.h>

// Library/third-party includes
#include <vrpn_BaseClass.h>

// Standard includes
// - none

namespace osvr {
    namespace common {

        struct DisplayData {
            OSVR_ChannelCount sensor;
            OSVR_DisplayState display;
        };

        namespace messages {
            class DisplayRecord : public MessageRegistration<DisplayRecord> {
            public:
                class MessageSerialization;

                static const char *identifier();
            };

        } // namespace messages

        /// @brief BaseDevice component
        class DisplayComponent : public DeviceComponent {
        public:
            /// @brief Factory method
            ///
            /// Required to ensure that allocation and deallocation stay on the same
            /// side of a DLL line.
            static OSVR_COMMON_EXPORT shared_ptr<DisplayComponent>
                create(OSVR_ChannelCount numSensor = 1);

            /// @brief Message from server to client, containing display data.
            messages::DisplayRecord displayRecord;

            OSVR_COMMON_EXPORT void
            sendDisplayData(OSVR_DisplayState display,
                            OSVR_ChannelCount sensor,
                            OSVR_TimeValue const &timestamp);

            typedef std::function<void(DisplayData const &,
                                       util::time::TimeValue const &)>
                                       DisplayHandler;
            OSVR_COMMON_EXPORT void registerDisplayHandler(DisplayHandler cb);

        private:
            DisplayComponent(OSVR_ChannelCount numChan);
            virtual void m_parentSet();

            static int VRPN_CALLBACK
                m_handleDisplayRecord(void *userdata, vrpn_HANDLERPARAM p);

            void m_checkFirst(OSVR_DisplayState const &display);

            OSVR_ChannelCount m_numSensor;
            std::vector<DisplayHandler> m_cb;
            bool m_gotOne;
        };

    } // namespace common
} // namespace osvr

#endif // INCLUDED_DisplayComponent_h_GUID_A831481F_0257_449A_6404_3B789BD89CEF
