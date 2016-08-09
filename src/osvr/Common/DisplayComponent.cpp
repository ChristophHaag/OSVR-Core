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
#include <osvr/Common/DisplayComponent.h>
#include <osvr/Common/BaseDevice.h>
#include <osvr/Common/Serialization.h>
#include <osvr/Common/Buffer.h>
#include <osvr/Util/Verbosity.h>

// Library/third-party includes

namespace osvr {
    namespace common {

        namespace messages {
            class DisplayRecord::MessageSerialization {
            public:
                MessageSerialization(OSVR_DisplayState const &display,
                    OSVR_ChannelCount sensor)
                    : m_display(display), m_sensor(sensor) {}

                MessageSerialization() {}

                template <typename T> void processMessage(T &p) {
                    p(m_display);
                    p(m_sensor);
                }
                DisplayData getData() const {
                    DisplayData ret;
                    ret.sensor = m_sensor;
                    ret.display = m_display;
                    return ret;
                }

            private:
                OSVR_DisplayState m_display;
                OSVR_ChannelCount m_sensor;
            };
            const char *DisplayRecord::identifier() {
                return "com.osvr.display.displayrecord";
            }
        } // namespace messages

        shared_ptr<DisplayComponent>
            DisplayComponent::create(OSVR_ChannelCount numChan) {
            shared_ptr<DisplayComponent> ret(new DisplayComponent(numChan));
            return ret;
        }

        DisplayComponent::DisplayComponent(OSVR_ChannelCount numChan)
            : m_numSensor(numChan) {}

        void
        DisplayComponent::sendDisplayData(OSVR_DisplayState display,
                                          OSVR_ChannelCount sensor,
                                          OSVR_TimeValue const &timestamp) {

            Buffer<> buf;
            messages::DisplayRecord::MessageSerialization msg(display, sensor);
            serialize(buf, msg);

            m_getParent().packMessage(buf, displayRecord.getMessageType(),
                timestamp);
        }

        int VRPN_CALLBACK
            DisplayComponent::m_handleDisplayRecord(void *userdata,
            vrpn_HANDLERPARAM p) {
            auto self = static_cast<DisplayComponent *>(userdata);
            auto bufReader = readExternalBuffer(p.buffer, p.payload_len);

            messages::DisplayRecord::MessageSerialization msg;
            deserialize(bufReader, msg);
            auto data = msg.getData();
            auto timestamp = util::time::fromStructTimeval(p.msg_time);

            for (auto const &cb : self->m_cb) {
                cb(data, timestamp);
            }
            return 0;
        }

        void DisplayComponent::registerDisplayHandler(DisplayHandler handler) {
            if (m_cb.empty()) {
                m_registerHandler(&DisplayComponent::m_handleDisplayRecord,
                    this, displayRecord.getMessageType());
            }
            m_cb.push_back(handler);
        }
        void DisplayComponent::m_parentSet() {
            m_getParent().registerMessageType(displayRecord);
        }

    } // namespace common
} // namespace osvr