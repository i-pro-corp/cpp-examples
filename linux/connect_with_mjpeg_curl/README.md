# connect_with_mjpeg

## Abstract / 概要

We have posted the source code that connects the PC and i-PRO camera with MJPEG (Motion JPEG) and displays images using GNU and OpenCV.
The content does not include OpenCV. You need to install OpenCV yourself and configure it properly for your project.

linux GNU の c++ と OpenCV を使って、PC と i-PRO カメラを MJPEG (Motion JPEG) で接続して映像表示するソースコードを掲載します。
こちらの内容に OpenCV を含みません。ご自身で OpenCV をインストールを行い、プロジェクトへ適切に設定する必要があります。

---

## Linked article / 連動記事

We will introduce articles that work in conjunction with the source code that is stored in this folder.

このフォルダに保存するソースコードと連動する記事を紹介します。

- [MJPEG で画像を取得する (c++/Linux)](https://i-pro-corp.github.io/Programing-Items/cpp_linux/connect_camera/connect_with_mjpeg.html) :link:
- [i-PRO - Programing Items](https://i-pro-corp.github.io/Programing-Items) :link:

---

## Environment / 環境

We describe the environment we used to do the development and evaluation.

我々が開発および評価を行うために使用した環境を以下に記載します。

| Item       | Content                   | Version                  |
|:-----------|:--------------------------|:-------------------------|
| Compiler   | GNU                       | 9.4.0                    |
| Compiler   | build-essensial           | 12.8                     |
| Compiler   | CMake                     | 3.16.3                   |
| Library    | OpenCV                    | 4.6.0                    |
| Library    | libcurl4                  | 7.68.0-1ubuntu2.14 amd64 |  
| OS         | ubuntu                    | 20.04 LTS                |
