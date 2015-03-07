/** @file
    @brief Header

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

#ifndef INCLUDED_PathTree_h_GUID_8C6C691A_AAB1_4586_64DD_BD3F870C9071
#define INCLUDED_PathTree_h_GUID_8C6C691A_AAB1_4586_64DD_BD3F870C9071

// Internal Includes
#include <osvr/Routing/PathTree_fwd.h>
#include <osvr/Routing/Export.h>
#include <osvr/Routing/PathNode_fwd.h>

// Library/third-party includes
#include <boost/noncopyable.hpp>

// Standard includes
#include <string>

namespace osvr {
namespace routing {
    /// @addtogroup Routing
    /// @{

    /// @brief A tree representation, with path/url syntax, of the known OSVR
    /// system.
    class PathTree : boost::noncopyable {
      public:
        /// @brief Constructor
        OSVR_ROUTING_EXPORT PathTree();

        /// @brief Visitation
        /// @{
        /// @brief Visit the tree, starting at the root, with the given functor.
        template <typename F> void visitTree(F &functor) { functor(*m_root); }

        /// @brief Visit the tree, with const nodes, starting at the root, with
        /// the given functor.
        template <typename F> void visitConstTree(F &functor) const {
            functor(const_cast<PathNode const &>(*m_root));
        }
        /// @}

        /// @brief Returns the node indicated by the path, which must be
        /// absolute (begin with a /). Any non-existent nodes will be created
        /// with values of NullElement
        /// @throws exceptions::PathNotAbsolute, exceptions::EmptyPath,
        /// exceptions::EmptyPathComponent
        OSVR_ROUTING_EXPORT PathNode &getNodeByPath(std::string const &path);

      private:
        /// @brief Root node of the tree.
        PathNodePtr m_root;
    };
    /// @}

} // namespace routing
} // namespace osvr

#endif // INCLUDED_PathTree_h_GUID_8C6C691A_AAB1_4586_64DD_BD3F870C9071
