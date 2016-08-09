/** @file
@brief Header

Must be c-safe!

@date 2014

@author
Sensics, Inc.
<http://sensics.com/osvr>
*/

/*
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
*/

#ifndef INCLUDED_DisplayStateC_h_GUID_F9715DE4_2649_4182_0F4C_D62121235D5F
#define INCLUDED_DisplayStateC_h_GUID_F9715DE4_2649_4182_0F4C_D62121235D5F

/* Internal Includes */
#include <osvr/Util/APIBaseC.h>

/* Library/third-party includes */
/* none */

/* Standard includes */
/* none */

OSVR_EXTERN_C_BEGIN

/** @addtogroup UtilMath
@{
*/
/** @brief A structure defining a 2D vector, which represents position
*/
typedef struct OSVR_DisplayState {
    int placeholder;
    // @todo fill in here
} OSVR_DisplayState;

/** @} */

OSVR_EXTERN_C_END

#ifdef __cplusplus
template <typename StreamType>
inline StreamType &operator<<(StreamType &os, OSVR_DisplayState const &display) {
    os << "display(" << display.placeholder << ")";
    return os;
}
#endif

#endif // INCLUDED_DisplayStateC_h_GUID_F9715DE4_2649_4182_0F4C_D62121235D5F
