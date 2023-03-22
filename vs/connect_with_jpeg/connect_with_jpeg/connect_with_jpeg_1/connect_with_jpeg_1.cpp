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
    Try connecting to an i-PRO camera with JPEG.
    HTTPで接続してJPEGの連続表示で i-PRO カメラと接続してみる

[Details]
    Let's try first.
    まずはやってみる

[Author]
    kinoshita hidetoshi (木下英俊)

[Library install]
    opencv:     See "https://i-pro-corp.github.io/Programing-Items/cpp_vs/install_opencv.html"
    libcurl:    See "https://i-pro-corp.github.io/Programing-Items/cpp_vs/connect_camera/connect_with_jpeg.html"
======================================================================================
*/

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <curl/curl.h>


const std::string user_id       = "user-id";         // Change to match your camera setting
const std::string user_pw       = "password";        // Change to match your camera setting
const std::string host          = "192.168.0.10";    // Change to match your camera setting
const std::string resolution    = "1920";
const std::string winname       = "VIDEO";           // Window title
const std::string cameraURL     = "http://" + host + "/cgi-bin/camera?resolution=" + resolution;


/// <summary>
/// A curl callback function. Call this function when data is received.
/// </summary>
/// <param name="ptr">ptr points to the delivered data.</param>
/// <param name="size">size is always 1.</param>
/// <param name="nmemb">the size of the deliverd data is nmemb.</param>
/// <param name="userdata">Pointer to buffer specified by CURLOPT_WRITEDATA</param>
/// <returns>
/// Return the number of bytes actually taken care of.
/// If that amount differs from the amount passed to your callback function, it will signal an error condition to the library.
/// This will cause the transfer to get aborted and the libcurl function used will return CURLE_WRITE_ERROR.
/// </returns>
/// <detail>See "https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html" for details.</detail>
size_t onReceive(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::vector<char>* receiveBuffer = (std::vector<char>*)userdata;
    const size_t sizes = size * nmemb;

    receiveBuffer->insert(receiveBuffer->end(), (char*)ptr, (char*)ptr + sizes);

    return sizes;
}


/// <summary>
/// Receiving one JPEG image from the i-PRO camera.
/// </summary>
/// <param name="url">CGI command for 1 shot JPEG</param>
/// <param name="receiveBuffer">A variable to store the received JPEG image</param>
/// <retval>true: Success to get JPEG image.</retval>
/// <retval>false: Failed to get JPEG image.</retval>
bool get_jpeg_image(const std::string url, std::vector<char>& receiveBuffer) {
    CURL*       curl = NULL;
    CURLcode    res;

    // initialize curl.
    curl = curl_easy_init();
    if (!curl) {
        printf("curl_easy_init() failed...\n");
        return false;
    }

    // curl option for digest authentication
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
    std::string user_id_pw = user_id + ":" + user_pw;
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_id_pw.c_str());

    // curl option
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &receiveBuffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);

    // execute
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed...%s\n", curl_easy_strerror(res));
        return false;
    }
    curl_easy_cleanup(curl);
    return true;
}


/// <summary>
/// main function.
/// </summary>
/// <returns>Always returns EXIT_SUCCESS(0).</returns>
int main() {
    cv::Mat recv_data;
    bool isDataReceived;
    char ret;
    std::vector<char> receiveBuffer;

    while (true) {

        // get image data by HTTP
        isDataReceived = get_jpeg_image(cameraURL, receiveBuffer);
        if (!isDataReceived) {
            std::cout << "get_jpeg_image() failed..." << std::endl;
            break;
        }

        // JPEG decode
        cv::Mat frame = cv::imdecode(receiveBuffer, cv::IMREAD_UNCHANGED);

        // Clear all received data.
        receiveBuffer.clear();

        // Please modify the value to fit your PC screen size.
        resize(frame, frame, cv::Size(), 0.5, 0.5);                     // Setting by magnification.

        // Display video.
        cv::imshow(winname, frame);

        ret = (char)cv::waitKey(1);     // necessary to display the video by cv::imshow().

        // Press the "q" key to finish.
        if (ret == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
