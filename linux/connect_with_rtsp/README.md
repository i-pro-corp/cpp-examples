# connect_with_rtsp

## Abstract / 概要

We have posted the source code that connects the PC and i-PRO camera with RTSP (H.264/H.265) and displays images using GNU and OpenCV.
The content does not include OpenCV. You need to install OpenCV yourself and configure it properly for your project.

GNU の c++ と OpenCV を使って、PC と i-PRO カメラを RTSP (H.264/H.265) で接続して映像表示するソースコードを掲載します。
こちらの内容に OpenCV を含みません。ご自身で OpenCV をインストールを行い、プロジェクトへ適切に設定する必要があります。

---

## Linked article / 連動記事

We will introduce articles that work in conjunction with the source code that is stored in this folder.

このフォルダに保存するソースコードと連動する記事を紹介します。

- [RTSP で画像を取得する (c++/linux)](https://i-pro-corp.github.io/Programing-Items/cpp_linux/connect_camera/connect_with_rtsp.html) :link:
- [i-PRO - Programing Items](https://i-pro-corp.github.io/Programing-Items) :link:

---

## Environment / 環境

We describe the environment we used to do the development and evaluation.

我々が開発および評価を行うために使用した環境を以下に記載します。

| Item       | Content                   | Version  |
|:-----------|:--------------------------|:---------|
| Compiler   | GNU                       | 9.4.0    |
| Compiler   | build-essensial           | 12.8     |
| Compiler   | CMake                     | 3.16.3   |
| Library    | OpenCV                    | 4.6.0    |
| OS         | ubuntu                    | 20.04 LTS|
