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
    Try connecting to an i-PRO camera with RTSP.
    HTTPで接続してJPEGの連続表示で i-PRO カメラと接続してみる

[Details]
    Let's try first.
    まずはやってみる

[Author]
    Yomogida Yasuo (蓬田 康雄)

[Library install]
    sudo apt update -y && sudo apt upgrade -y
    sudo apt install build-essential libopencv-dev cmake -y
    sudo apt install libcurl4-openssl-dev -y 
    sudo apt update -y && sudo apt upgrade -y

======================================================================================
*/

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <curl/curl.h>

const std::string user_id     = "user-id";         // Change to match your camera setting
const std::string user_pw     = "password";        // Change to match your camera setting
const std::string host        = "192.168.0.10";    // Change to match your camera setting
const std::string winname     = "VIDEO";           // Window title

const std::string cameraURL   = "http://" + host + "/cgi-bin/camera";

// data receive callback function

/// <summary>
/// onReceive function
/// </summary>
/// <returns>segment size of recive data</returns>
size_t onReceive(void* ptr, size_t size, size_t nmemb, void* userdata) {

    std::vector<char>* receiveBuffer = (std::vector<char>*)userdata;
    const size_t sizes = size * nmemb;

    receiveBuffer->insert(receiveBuffer->end(), (char*)ptr, (char*)ptr + sizes);

    return sizes;
}


/// <summary>
/// main function
/// </summary>
/// <returns>Always returns EXIT_SUCCESS(0).</returns>
int main() {

    char ret;
    std::vector<char> receiveBuffer;
    CURL* curl = NULL;
    CURLcode res;

    while (true) {

        // initialize curl.
        curl = curl_easy_init();
        if (!curl) {
            printf("curl_easy_init() failed...\n");
            break;
        }

        // curl option for digest authentication
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
        std::string user_id_pw = user_id + ":" + user_pw;
        curl_easy_setopt(curl, CURLOPT_USERPWD, user_id_pw.c_str());

        // curl option
        curl_easy_setopt(curl, CURLOPT_URL, cameraURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &receiveBuffer); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);

        // execute
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf("curl_easy_perform() failed...%s\n", curl_easy_strerror(res));
            break;
        }
        curl_easy_cleanup(curl); 

        // JPEG decode
        cv::Mat frame = cv::imdecode(receiveBuffer, cv::IMREAD_UNCHANGED);

        // Clear all received data.
        receiveBuffer.clear();

        // Please modify the value to fit your PC screen size.
        resize(frame, frame, cv::Size(), 0.5, 0.5); // Setting by magnification.

        // Display video.
        cv::imshow(winname, frame);

        ret = (char)cv::waitKey(1); // necessary to display the video by cv::imshow().

        // Press the "q" key to finish.
        if (ret == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();

    return EXIT_SUCCESS;

}
