/** @file
    @brief Implementation

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2014 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include <osvr/Routing/PathElementTypes.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace routing {
    namespace elements {
        void AliasElement::setTarget(std::string const &targetPath) {
            /// @todo validation?
            m_target = targetPath;
        }

        std::string const &AliasElement::getTarget() const { return m_target; }
    } // namespace elements
} // namespace routing
} // namespace osvr
