#pragma once

class CvWindowWrapper;
template <typename BASE>
class PrevableCvWindowWrapper;

CvWindowWrapper CreateCannyWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
CvWindowWrapper CreateAdaptiveTresholdWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
CvWindowWrapper CreateDifferenceOfGaussianWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
CvWindowWrapper CreateSIFTWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
CvWindowWrapper CreateOpticalFlowWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
CvWindowWrapper CreateORBWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
CvWindowWrapper CreateFaceWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
CvWindowWrapper CreatePerspectiveWindow(std::string window_name, const cv::Point& pos = { -1, -1 });
