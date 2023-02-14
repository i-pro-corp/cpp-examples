/*
======================================================================================
Copyright 2023 i-PRO Co., Ltd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissionsand
limitations under the License.
======================================================================================
*/

/*
======================================================================================

[Abstract]
    Try connecting to an i-PRO camera with Motion JPEG.
    Motion JPEG で i-PRO カメラと接続してみる

[Details]
    Let's try first.
    まずはやってみる

[Author]
    kinoshita hidetoshi (木下英俊)

[Library install]
    opencv:     You need OpenCV. See "https://i-pro-corp.github.io/Programing-Items/cpp_vs/install_opencv.html"

======================================================================================
*/


#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>


const std::string user_id    = "user_id";       // Change to match your camera setting
const std::string user_pw    = "user_pw";       // Change to match your camera setting
const std::string host       = "192.168.0.10";  // Change to match your camera setting
const std::string winname    = "VIDEO";         // Window title
const std::string resolution = "1920x1080";     // Resolution
const std::string framerate  = "15";            // Frame rate

const std::string url = "http://" + user_id + ":" + user_pw + "@" + host + "/cgi-bin/nphMotionJpeg?Resolution=" + resolution + "&Quality=Standard&Framerate=" + framerate ;

int main(int argc, const char* argv[])
{
    cv::VideoCapture cap(url);
    cv::Mat frame;
    char ret;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        // Please modify the value to fit your PC screen size.
        resize(frame, frame, cv::Size(), 0.5, 0.5);                                     // Setting by magnification.

        // Display video.
        cv::imshow(winname, frame);

        ret = (char)cv::waitKey(1);     // necessary to display the video by cv::imshow().

        // Press the "q" key to finish.
        if (ret == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
