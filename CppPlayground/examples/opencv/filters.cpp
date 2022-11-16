#include "opencv_examples.h"
#include <string>

#ifdef HAS_OPENCV
#include "CvWindowWrapper.h"
#include "filters.h"
#include <opencv2\opencv.hpp>

/////////////////////////////////////////////////////
// Canny edge detector
/////////////////////////////////////////////////////

CvWindowWrapper CreateCannyWindow(std::string window_name, const cv::Point& pos)
{

    std::string tresholdSliderName = "Threshold";
    std::string ratioSliderName = "Ratio";
    std::string kernelSliderName = "kernel";

    CvWindowWrapper OutWindow { window_name, pos };
    OutWindow.addSlider(tresholdSliderName, 40);
    // OutWindow.addSlider(ratioSliderName, 3, 0, 6);
    // OutWindow.addSlider(kernelSliderName, 1, 1, 3);

    OutWindow.setFilter([=](const cv::Mat& image, const CvWindowWrapper& window) -> cv::Mat {
        int threshold = window.getSliderValue(tresholdSliderName);
        int ratio = 3; // window.getSliderValue(ratioSliderName); // 3;
        int kernel_size = 3; // window.getSliderValue(kernelSliderName) * 2 + 1; // must be in [3,5,7] // 3;;

        cv::UMat uimage, blured, detectedEdges;
        image.copyTo(uimage);
        cv::GaussianBlur(uimage, blured, cv::Size(3, 3), 3, 0);
        cv::Canny(blured, detectedEdges, threshold, threshold * ratio, kernel_size);

        // Apply mask

        cv::Mat outImage { cv::Scalar::all(0) };
        cv::Mat temp { ~image };
        temp.copyTo(outImage, detectedEdges);
        outImage = ~outImage;

        std::string text = "Canny " + std::to_string(threshold);
        cv::putText(outImage, text, { 20, 20 }, cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);

        return outImage;
    });

    return OutWindow;
}

/////////////////////////////////////////////////////
// Adaptive threshold filter
/////////////////////////////////////////////////////

CvWindowWrapper CreateAdaptiveTresholdWindow(std::string window_name, const cv::Point& pos)
{

    std::string blockSizeSliderName = "Block size";

    CvWindowWrapper OutWindow { "Adaptive threshold", pos };

    OutWindow.addSlider(blockSizeSliderName, 11, 1, 50);

    OutWindow.setFilter([=](const cv::Mat& image, const CvWindowWrapper& window) -> cv::Mat {
        int blocksize = window.getSliderValue(blockSizeSliderName);
        blocksize += (blocksize % 2) + 1;

        cv::Mat mask, image_gray;
        cv::cvtColor(image, image_gray, cv::COLOR_RGB2GRAY);
        cv::adaptiveThreshold(image_gray, mask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blocksize, 5);

        // Apply mask on original image
        cv::Mat outImage { cv::Scalar::all(0) };
        cv::Mat temp { ~image };
        temp.copyTo(outImage, ~mask);
        outImage = ~outImage;

        std::string text = "Adaptive threshold ";
        cv::putText(outImage, text, { 20, 20 }, cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);

        return outImage;
    });

    return OutWindow;
}

/////////////////////////////////////////////////////
// Difference of Gaussian (DoG)
/////////////////////////////////////////////////////

CvWindowWrapper CreateDifferenceOfGaussianWindow(std::string window_name, const cv::Point& pos)
{
    CvWindowWrapper OutWindow { window_name, pos };
    OutWindow.setFilter([](const cv::Mat& image, const CvWindowWrapper& window) -> cv::Mat {
        cv::Mat mask, image_gray;
        cv::cvtColor(image, image_gray, cv::COLOR_RGB2GRAY);

        // Difference of Gaussian (DoG)
        cv::Mat g1, g2;
        cv::GaussianBlur(image_gray, g1, cv::Size(3, 3), 2);
        cv::GaussianBlur(image_gray, g2, cv::Size(9, 9), 25);

        cv::Mat DoG = g1 - g2;
        cv::Mat DoG_norm;
        cv::normalize(DoG, DoG_norm, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        cv::threshold(DoG_norm, mask, 16, 255, cv::THRESH_BINARY_INV);

        // Apply mask on original image
        cv::Mat outImage { cv::Scalar::all(0) };
        cv::Mat temp { ~image };
        temp.copyTo(outImage, ~mask);
        outImage = ~outImage;

        std::string text = "DoG ";
        cv::putText(outImage, text, { 20, 20 }, cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);

        return outImage;

        // return out;
    });

    return OutWindow;
}

/////////////////////////////////////////////////////
// Scale Invariant Feature Tracking - SIFT
/////////////////////////////////////////////////////

CvWindowWrapper CreateSIFTWindow(std::string window_name, const cv::Point& pos)
{
    CvWindowWrapper OutWindow { window_name, pos };
    OutWindow.setFilter([=](const cv::Mat& image, const CvWindowWrapper& window) -> cv::Mat {
        cv::UMat outImage;
        image.copyTo(outImage);
        cv::Ptr<cv::Feature2D> f2d = cv::SIFT::create(1000, 5, 0.01); // options.FeatNum, options.OctNum, options.ContrastTresh, options.EdgeTresh, options.Sig);
        std::vector<cv::KeyPoint> keyPoints;
        f2d->detect(outImage, keyPoints);
        // cv::drawKeypoints(imagegray, keyPoints, imageout, { 0,255,0 }); //, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        for (auto elem : keyPoints) {
            cv::circle(outImage, elem.pt, 1, { 0, 255, 0 }, -1);
        }

        std::string text = "SIFT ";
        cv::putText(outImage, text, { 20, 20 }, cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);

        return outImage.getMat(cv::ACCESS_READ).clone();
    });

    return OutWindow;
}

/////////////////////////////////////////////////////
// ORB
/////////////////////////////////////////////////////

CvWindowWrapper CreateORBWindow(std::string window_name, const cv::Point& pos)
{
    CvWindowWrapper OutWindow { window_name, pos };
    OutWindow.setFilter([=](const cv::Mat& image, const CvWindowWrapper& window) -> cv::Mat {
        cv::UMat outImage;
        image.copyTo(outImage);

        cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();

        std::vector<cv::KeyPoint> keyPoints;
        detector->detect(outImage, keyPoints);
        // cv::drawKeypoints(outImage, keyPoints, outImage, { 0,255,0 }, cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        for (auto elem : keyPoints) {
            cv::circle(outImage, elem.pt, 1, { 0, 255, 0 }, -1);
        }

        std::string text = "ORB ";
        cv::putText(outImage, text, { 20, 20 }, cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);

        return outImage.getMat(cv::ACCESS_READ).clone();
    });

    return OutWindow;
}

/////////////////////////////////////////////////////
// Face Detection
/////////////////////////////////////////////////////

CvWindowWrapper CreateFaceWindow(std::string window_name, const cv::Point& pos)
{
    CvWindowWrapper OutWindow { window_name, pos };
    OutWindow.setFilter([=](const cv::Mat& image, const CvWindowWrapper& window) -> cv::Mat {
        cv::UMat outImage;
        image.copyTo(outImage);

        cv::UMat image_gray;
        cv::cvtColor(outImage, image_gray, cv::COLOR_RGB2GRAY);

        auto faceCascadePath = "./haarcascade_frontalface_default.xml";
        cv::CascadeClassifier faceCascade;
        faceCascade.load(faceCascadePath);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(image_gray, faces);

        for (size_t i = 0; i < faces.size(); i++) {
            int x1 = faces[i].x;
            int y1 = faces[i].y;
            int x2 = faces[i].x + faces[i].width;
            int y2 = faces[i].y + faces[i].height;

            cv::Point p1 { faces[i].x, faces[i].y };
            cv::Point p2 { faces[i].x + faces[i].width, faces[i].y + faces[i].height };

            cv::rectangle(outImage, p1, p2, cv::Scalar(0, 255, 0));
        }

        return outImage.getMat(cv::ACCESS_READ).clone();
    });

    return OutWindow;
}

/*
# Load the cascade
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
# Read the input image
img = cv2.imread('test.jpg')
# Convert into grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# Detect faces
faces = face_cascade.detectMultiScale(gray, 1.1, 4)
# Draw rectangle around the faces
for (x, y, w, h) in faces:
    cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
# Display the output
cv2.imshow('img', img)
*/

/////////////////////////////////////////////////////
// Optical Flow
/////////////////////////////////////////////////////

// prevable mixin

CvWindowWrapper CreateOpticalFlowWindow(std::string window_name, const cv::Point& pos)
{
    CvWindowWrapper OutWindow { window_name, pos };

    OutWindow.setFilter([](const cv::Mat& image_input, const CvWindowWrapper& window) -> cv::Mat {
        auto prev = window.getPrevImage();
        if (prev.empty())
            return image_input;

        cv::UMat image_gray, prev_gray, image_out, flowu;
        cv::Mat flow;
        cvtColor(image_input, image_gray, cv::COLOR_RGB2GRAY);
        cvtColor(prev, prev_gray, cv::COLOR_RGB2GRAY);
        image_input.copyTo(image_out);

        // image_input.copyTo(image_gray);
        // prev.copyTo(prev_gray);

        cv::calcOpticalFlowFarneback(prev_gray, image_gray, flowu, 0.5, 3, 15, 3, 5, 1.2, 0);
        flowu.copyTo(flow);
        // cv::cartToPolar()

        double bb_thresh = 5;
        cv::Point p_top_left { 1000, 1000 }, p_bottom_right { -1, -1 };

        // cv::calcOpticalFlowFarneback(prev, image, flow, .4, 1, 12, 2, 8, 1.2, 0);
        for (int y = 0; y < image_input.rows; y += 15) {
            for (int x = 0; x < image_input.cols; x += 15) {
                // get the flow from y, x position * 3 for better visibility
                const cv::Point2f flowatxy = flow.at<cv::Point2f>(y, x) * 1;

                cv::Point p1 { x, y };
                cv::Point p2 { cvRound(x + flowatxy.x), cvRound(y + flowatxy.y) };
                auto dist = cv::norm(p1 - p2);

                // draw line at flow direction
                cv::line(image_out, p1, p2, cv::Scalar(0, 255, 0));

                if (dist > bb_thresh) {
                    p_top_left.x = std::min(p1.x, p_top_left.x);
                    p_top_left.y = std::min(p1.y, p_top_left.y);
                    p_bottom_right.x = std::max(p1.x, p_bottom_right.x);
                    p_bottom_right.y = std::max(p1.y, p_bottom_right.y);
                    // draw initial point
                    circle(image_out, p1, 2, cv::Scalar(255, 255, 0), -1);
                } else {
                    // draw initial point
                    circle(image_out, p1, 1, cv::Scalar(0, 0, 255), -1);
                }
            }
        }

        cv::rectangle(image_out, p_top_left, p_bottom_right, cv::Scalar(0, 255, 0));

        std::string text = "Flow ";
        cv::putText(image_out, text, { 20, 20 }, cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);

        return image_out.getMat(cv::ACCESS_READ).clone();
    });

    return OutWindow;
}

/////////////////////////////////////////////////////
// Face Detection
/////////////////////////////////////////////////////

CvWindowWrapper CreatePerspectiveWindow(std::string window_name, const cv::Point& pos)
{
    CvWindowWrapper OutWindow { window_name, pos };
    OutWindow.setFilter([=](const cv::Mat& image, const CvWindowWrapper& window) -> cv::Mat {
        cv::UMat outImage;
        image.copyTo(outImage);

        cv::UMat imgGray, imgBlur, imgCanny, imgDil, imgWarp, imgWarpGray;
        cv::cvtColor(outImage, imgGray, cv::COLOR_RGB2GRAY);
        cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3, 0);
        cv::Canny(imgBlur, imgCanny, 25, 75);
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::dilate(imgCanny, imgDil, kernel);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(imgDil, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); ++i) {
            auto area = cv::contourArea(contours[i]);
            if (area > 15000) {
                std::vector<cv::Point> conPoly;
                float peri = cv::arcLength(contours[i], true);
                cv::approxPolyDP(contours[i], conPoly, 0.02 * peri, true);
                if (conPoly.size() == 4) {
                    cv::Point2f src[4] = { conPoly[0], conPoly[1], conPoly[2], conPoly[3] };
                    cv::Point2f dst[4] = { { 0, 0 }, { 0, 220 }, { 400, 220 }, { 400, 0 } };
                    auto tmat = cv::getPerspectiveTransform(src, dst);
                    cv::warpPerspective(outImage, imgWarp, tmat, { 400, 220 });
                    cv::cvtColor(imgWarp, imgWarpGray, cv::COLOR_RGB2GRAY);

                    cv::UMat mask;
                    const auto blocksize = 11;
                    cv::adaptiveThreshold(imgWarpGray, mask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blocksize, 5);

                    cv::imshow("xxx", mask);

                    int k = 0;
                    for (auto p : conPoly) {
                        k++;
                        cv::circle(outImage, p, 2, { 255, 0, 255 }, 2, cv::FILLED);
                        cv::putText(outImage, std::to_string(p.x) + "," + std::to_string(p.y) + " " + std::to_string(k), p, cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);
                    }

                    // cv::drawContours(outImage, contours, i, { 0,255,0 });
                    auto rect = cv::boundingRect(contours[i]);
                    // cv::rectangle(outImage, rect, { 255,0,255 });
                    cv::putText(outImage, std::to_string(area), rect.tl(), cv::FONT_HERSHEY_PLAIN, 1.0, { 0, 255, 0 }, 1, cv::LINE_AA);

                    // imgWarp.copyTo(outImage(cv::Rect(0, 0, imgWarp.cols, imgWarp.rows)));
                }
            }
        }

        return outImage.getMat(cv::ACCESS_READ).clone();
    });

    return OutWindow;
}

#endif
