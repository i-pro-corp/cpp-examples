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
    Try connecting to an i-PRO camera with MJPEG by curl.
    RTSP で i-PRO カメラと接続してみる

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
const std::string framerate   = "15";              // Frame rate (indispensable param)

const std::string cameraURL = "http://" + host + "/cgi-bin/nphMotionJpeg?Framerate=" + framerate;

// data receive callback function
size_t onReceive(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    //important static flag
    static bool isSOI = false;

    //receivebuffer handover
    std::vector<char>* receiveBuffer = (std::vector<char>*)userdata;

    const size_t segsize = size * nmemb;   

    //Find SOI/EOI and get position and raise a flag
    int PosS=0;//start point
    int PosE=0;//end point
    int status=0; //0:Non,1:EOI,2:SOI,3:EOI->SOI
    bool isJPEG=false;


    //Finding SOI and EOI
    for(int i=0;i<segsize-1;i++){
        //find EOI
        if(*((char*)ptr+i)==(char)0xFF && *((char*)ptr+i+1)==(char)0xD9 && isSOI==true){
            isSOI=false;
            PosE=i+1;//eoi position
            status=1;
        }

        //find SOI
        if(*((char*)ptr+i)==(char)0xFF && *((char*)ptr+i+1)==(char)0xD8 && isSOI==false){
            isSOI=true;
            PosS=i; //soi positon
            if(status==1){ //EOI -> neer -> SOI
                status=3;
            }else{
                status=2;
            }           
            break; //start JPEG Image
        }
        
    }   


    //Fill jpeg data into receiveBuffer
    if(isSOI==true && status==0){
        receiveBuffer->insert(receiveBuffer->end(), (char*)ptr, (char*)ptr+segsize);
    }else if(status==2){
        receiveBuffer->insert(receiveBuffer->end(), (char*)ptr+PosS, (char*)ptr+segsize);
    }else if(status==1 || status==3){
        receiveBuffer->insert(receiveBuffer->end(), (char*)ptr, (char*)ptr+PosE);
        isJPEG=true; //complete JPEG 
    }


    //display JPEG
    if(isJPEG==true){
        try{
            // JPEG decore
            cv::Mat frame = cv::imdecode(*receiveBuffer, cv::IMREAD_UNCHANGED);

            //clear recv_buffer
            receiveBuffer->clear();

            // Please modify the value to fit your PC screen size.
            resize(frame, frame, cv::Size(), 0.5, 0.5);

            cv::imshow(winname, frame); //Display video.

            if((char)cv::waitKey(1) >= 0) { // wait for any key
                return 0; 
            } 
        }catch(...){
            //clear recv_buffer
            receiveBuffer->clear();           
        }
    }

    //Fill jpeg data into receiveBuffer
    if(status==3){ //EOI -> neer -> SOI : recv_buf need to add data
        receiveBuffer->insert(receiveBuffer->end(), (char*)ptr+PosS, (char*)ptr+segsize);    
    }

    return segsize;
}


int main(int argc, char* argv[])
{
    
    std::vector<char> receiveBuffer;
    CURL* curl = NULL;
    CURLcode res;    

    //init curl processing
    curl = curl_easy_init();
    if (!curl) {
        printf("curl_easy_init() failed...\n");
        return 0;
    }

    //for digest
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
    std::string user_id_pw =user_id + ":" + user_pw;
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_id_pw.c_str());

    //curl option
    curl_easy_setopt(curl, CURLOPT_URL, cameraURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &receiveBuffer); 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);

    //In the mjpeg case, curl_easy_perform continues
    res = curl_easy_perform(curl);

    //curl processing finished
    curl_easy_cleanup(curl);
    receiveBuffer.clear();

    cv::destroyAllWindows();

    return 0;
}
