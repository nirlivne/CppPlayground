#pragma once
#include <functional>
#include <opencv2\opencv.hpp>

class CvWindowWrapper {
public:
    using filter_fn = std::function<cv::Mat(const cv::Mat&, const CvWindowWrapper&)>;

    CvWindowWrapper(std::string window_name, const cv::Point& pos = { -1, -1 })
        : mWindowName(std::move(window_name))
    {
        cv::namedWindow(mWindowName, cv::WINDOW_NORMAL);
        if (pos.x >= 0 && pos.y >= 0) {
            cv::moveWindow(mWindowName, pos.x, pos.y);
        } else { // -1,-1
            // cv::setWindowProperty(mWindowName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
        }
    }

    void setImage(const cv::Mat& image)
    {
        mPrev = mImage;
        mImage = image;
    }

    const cv::Mat& getImage() const { return mImage; }
    const cv::Mat& getPrevImage() const { return mPrev; }

    cv::Mat filter()
    {
        if (mFnFilter) {
            cv::Mat filteredImage = mFnFilter(mImage, *this); // run filter
            return filteredImage;
        }

        return mImage;
    }

    void show()
    {
        cv::Mat filteredImage = filter(); // run filter
        cv::imshow(mWindowName, filteredImage);
    }

    void addSlider(const std::string& name, int pos = 127, int min = 0, int max = 255)
    {
        cv::createTrackbar(name, mWindowName, 0, max);
        cv::setTrackbarMin(name, mWindowName, min);
        cv::setTrackbarPos(name, mWindowName, pos);
    }

    int getSliderValue(const std::string& name) const
    {
        return cv::getTrackbarPos(name, mWindowName);
    }

    std::string getWindowName() const { return mWindowName; }
    void setFilter(filter_fn fn) { mFnFilter = fn; }

private:
    cv::Mat mImage;
    cv::Mat mPrev;
    std::string mWindowName;
    filter_fn mFnFilter;
};
