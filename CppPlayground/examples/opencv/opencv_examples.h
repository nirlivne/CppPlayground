#pragma once

#if __has_include(<opencv2\opencv.hpp>)
#define HAS_OPENCV
#ifdef NDEBUG
#pragma comment(lib, "opencv_core451.lib")
#pragma comment(lib, "opencv_videoio451.lib")
#pragma comment(lib, "opencv_imgproc451.lib")
#pragma comment(lib, "opencv_highgui451.lib")
#pragma comment(lib, "opencv_features2d451.lib")
#pragma comment(lib, "opencv_imgcodecs451.lib")
#pragma comment(lib, "opencv_calib3d451.lib") // for drawkeypoints
#pragma comment(lib, "opencv_video451.lib") // for optical flow
#pragma comment(lib, "opencv_objdetect451.lib") // for optical flow

#else
#pragma comment(lib, "opencv_core451d.lib")
#pragma comment(lib, "opencv_videoio451d.lib")
#pragma comment(lib, "opencv_imgproc451d.lib")
#pragma comment(lib, "opencv_highgui451d.lib")
#pragma comment(lib, "opencv_features2d451d.lib")
#pragma comment(lib, "opencv_imgcodecs451d.lib")
#pragma comment(lib, "opencv_calib3d451d.lib") // for drawkeypoints
#pragma comment(lib, "opencv_video451d.lib") // for optical flow
#pragma comment(lib, "opencv_objdetect451d.lib") // for optical flow
#endif
#endif

constexpr bool has_opencv()
{
#ifdef HAS_OPENCV
    return true;
#else
    return false;
#endif
}

#ifdef HAS_OPENCV

void opencv_webcam_filters_test();
void cvplot_test();

#endif // HAS_BOOST
