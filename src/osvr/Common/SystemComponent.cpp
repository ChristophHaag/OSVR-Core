/** @file
    @brief Implementation

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2015 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include <osvr/Common/SystemComponent.h>
#include <osvr/Common/BaseDevice.h>
#include <osvr/Util/MessageKeys.h>
#include <osvr/Common/Serialization.h>
#include <osvr/Common/Buffer.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace common {
    namespace messages {
        class RoutesFromServer::MessageSerialization {
          public:
            MessageSerialization(std::string const &str = std::string())
                : m_str(str) {}

            template <typename T> void processMessage(T &p) {
                p(m_str, serialization::StringOnlyMessageTag());
            }

          private:
            std::string m_str;
        };
        const char *RoutesFromServer::identifier() {
            return util::messagekeys::routingData();
        }

        const char *AppStartupToServer::identifier() {
            return "com.osvr.system.appstartup";
        }

        class ClientRouteToServer::MessageSerialization {
          public:
            MessageSerialization(std::string const &str = std::string())
                : m_str(str) {}

            template <typename T> void processMessage(T &p) {
                p(m_str, serialization::StringOnlyMessageTag());
            }

          private:
            std::string m_str;
        };
        const char *ClientRouteToServer::identifier() {
            return "com.osvr.system.updateroutetoserver";
        }
    } // namespace messages

    const char *SystemComponent::deviceName() {
        return util::messagekeys::systemSender();
    }

    shared_ptr<SystemComponent> SystemComponent::create() {
        shared_ptr<SystemComponent> ret(new SystemComponent);
        return ret;
    }

    SystemComponent::SystemComponent() {}

    void SystemComponent::sendRoutes(std::string const &routes) {
        Buffer<> buf;
        messages::RoutesFromServer::MessageSerialization msg(routes);
        serialize(buf, msg);
        m_getParent().packMessage(buf, routesOut.getMessageType());
    }

    void SystemComponent::registerRoutesHandler(vrpn_MESSAGEHANDLER handler,
                                                void *userdata) {
        m_registerHandler(handler, userdata, routesOut.getMessageType());
    }

    void SystemComponent::sendClientRouteUpdate(std::string const &route) {
        Buffer<> buf;
        messages::ClientRouteToServer::MessageSerialization msg(route);
        serialize(buf, msg);
        m_getParent().packMessage(buf, routeIn.getMessageType());
    }

    void SystemComponent::registerClientRouteUpdateHandler(
        vrpn_MESSAGEHANDLER handler, void *userdata) {
        m_registerHandler(handler, userdata, routeIn.getMessageType());
    }

    void SystemComponent::m_parentSet() {
        m_getParent().registerMessageType(routesOut);
        m_getParent().registerMessageType(appStartup);
        m_getParent().registerMessageType(routeIn);
    }
} // namespace common
} // namespace osvr