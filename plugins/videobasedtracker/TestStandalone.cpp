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
#include "VideoBasedTracker.h"
#include "HDKLedIdentifierFactory.h"
#include "CameraParameters.h"
#include "HDKData.h"

// Library/third-party includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifndef _WIN32
#include <libuvc/libuvc.h>
#endif

// Standard includes
#include <iostream>

using osvr::vbtracker::CameraParameters;

class Main {
  public:
    Main() : m_camera(0) {

#ifdef _WIN32
        int height = 0;
        int width = 0;
	if (!m_camera.isOpened()) {
            std::cerr << "Couldn't open camera" << std::endl;
            return;
        }

        height = static_cast<int>(m_camera.get(CV_CAP_PROP_FRAME_HEIGHT));
        width = static_cast<int>(m_camera.get(CV_CAP_PROP_FRAME_WIDTH));
	
        // See if this is an Oculus camera by checking the dimensions of
        // the image.  This camera type improperly describes its format
        // as being a color format when it is in fact a mono format.
        bool isOculusCamera = (width == 376) && (height == 480);
        if (isOculusCamera) {
            std::cerr << "This standalone app doesn't have Oculus support."
                      << std::endl;
            return;
        }

        std::cout << "Got image of size " << width << "x" << height
                  << ", Format " << m_camera.get(CV_CAP_PROP_FORMAT)
                  << ", Mode " << m_camera.get(CV_CAP_PROP_MODE) << std::endl;
#else
	uvc_context_t *ctx;
	uvc_error_t res;
	uvc_device_t *dev;
	uvc_device_handle_t *devh;
	uvc_stream_ctrl_t ctrl;

	res = uvc_init(&ctx, NULL);

	if (res < 0) {
	  uvc_perror(res, "uvc_init");
	  return;
	}

	puts("UVC initialized");

	res = uvc_find_device(
	    ctx, &dev,
	    0, 0, NULL);

	if (res < 0) {
	  uvc_perror(res, "uvc_find_device");
	} else {
	  puts("Device found");

	  res = uvc_open(dev, &devh);

	  if (res < 0) {
	    uvc_perror(res, "uvc_open");
	  } else {
	    puts("Device opened");
	    //uvc_print_diag(devh, stderr);
	  }
	  res = uvc_get_stream_ctrl_format_size(
          devh, &ctrl, /* result stored in ctrl */
          UVC_FRAME_FORMAT_YUYV, /* YUV 422, aka YUV 4:2:2. try _COMPRESSED */
          640, 480, 30 /* width, height, fps */
      );

      /* Print out the result */
      uvc_print_stream_ctrl(&ctrl, stderr);
	}
	
	int width = 640;
	int height = 480;
#endif


        /// @todo Come up with actual estimates for camera and distortion
        /// parameters by calibrating them in OpenCV.
        double cx = width / 2.0;
        double cy = height / 2.0;
        double fx = 700.0; // XXX This needs to be in pixels, not mm
        double fy = fx;
        CameraParameters camParams(fx, fy, cv::Size(width, height));
        m_vbtracker.addSensor(osvr::vbtracker::createHDKLedIdentifier(0),
                              camParams,
                              osvr::vbtracker::OsvrHdkLedLocations_SENSOR0,
                              osvr::vbtracker::OsvrHdkLedDirections_SENSOR0);
        m_vbtracker.addSensor(osvr::vbtracker::createHDKLedIdentifier(1),
                              camParams,
                              osvr::vbtracker::OsvrHdkLedLocations_SENSOR1,
                              osvr::vbtracker::OsvrHdkLedDirections_SENSOR1);
        m_valid = true;
    }

    /// @return true if the app should exit.
    bool update() {
#ifdef _WIN32
        if (!m_valid || !m_camera.isOpened()) {
#else
        if (!m_valid) {
#endif
            return true;
        }
        if (!m_camera.grab()) {
            // No frame available.
            return false;
        }

        if (!m_camera.retrieve(m_frame, m_channel)) {
            m_valid = false;
            return true;
        }

        auto tv = osvr::util::time::getNow();
        //==================================================================
        // Convert the image into a format we can use.
        // TODO: Consider reading in the image in gray scale to begin with
        cv::cvtColor(m_frame, m_imageGray, CV_RGB2GRAY);

        return m_vbtracker.processImage(
            m_frame, m_imageGray, tv,
            [&](OSVR_ChannelCount sensor, OSVR_Pose3 const &pose) {
                std::cout << "Sensor " << sensor << ": Translation "
                          << pose.translation << " rotation " << pose.rotation
                          << std::endl;
            });
    }

  private:
    bool m_valid = false;
    int m_channel = 0;
    cv::VideoCapture m_camera;

    osvr::vbtracker::VideoBasedTracker m_vbtracker;
    cv::Mat m_frame;
    cv::Mat m_imageGray;
};
int main() {
    Main mainObj;
    bool done = false;
    while (!done) {
        done = mainObj.update();
    }
    return 0;
}
