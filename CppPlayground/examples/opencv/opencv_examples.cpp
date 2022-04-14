#include "register_items.h"
#include "categories.h"
#include "opencv_examples.h"

#ifdef HAS_OPENCV

#include "CvWindowWrapper.h"
#include "filters.h"
#include <opencv2\opencv.hpp>

#define CVPLOT_HEADER_ONLY
#include <CvPlot/cvplot.h>

void cvplot_test()
{
    auto fontface = cv::FONT_HERSHEY_PLAIN;
    auto scale = 1.0;
    auto color = cv::Scalar { 0, 0, 0 };
    auto line_thickness = 1;
    auto line_type = cv::LINE_AA;

    // auto axes = CvPlot::plot(std::vector<double>{ 3, 3, 4, 6, 4, 3 }, "-o");
    // cv::Mat mat = axes.render(300, 400);
    // cv::imwrite(R"(c:\temp\test_plot_1.png)",mat);

    std::vector<double> x_movement { 0, 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 20000, 25000, 30000, 35000 };
    std::vector<double> y_accuracy { 0, -16.47850318, -36.68605253, -57.09180804, -79.42407163, -101.3751891, -120.7437324, -143.2834484, -160.9329664, -194.0910771, -214.3749757, -223.5510808, -227.8593686 };

    std::vector<double> x_slope { 0, 35000 };
    std::vector<double> y_slope { -20, -270 };

    auto axes = CvPlot::makePlotAxes();
    auto& slope = axes.create<CvPlot::Series>(x_slope, y_slope, "b-"); // draw black (k) circle bullets (o)
    slope.setColor(cv::Scalar(200, 200, 200));
    axes.create<CvPlot::Series>(x_movement, y_accuracy, "g-"); // draw green (g) solid line (-)
    axes.create<CvPlot::Series>(x_movement, y_accuracy, "ko"); // draw black (k) circle bullets (o)

    axes.title("Accuracy[um] Vs. Movement[um]");
    axes.yLabel("Accuracy [um]");
    axes.xLabel("Movement [um]");

    cv::Mat mat = axes.render(350, 750);

    cv::putText(mat, "y = -0.00071x - 21.884", { 100, 260 }, fontface, scale, color, line_thickness, line_type);
    cv::putText(mat, "R^2 = 0.9277", { 100, 280 }, fontface, scale, color, line_thickness, line_type);

    cv::imwrite(R"(c:\temp\test_plot_ft.png)", mat);
}

//void opencv_webcam_filters_test()
ADD_MENU_ITEM(
    opencv_filters,
    "opencv filters on webcam stream",
    category_libraries) //category_opencv)
{

    auto WinDoG = CreateDifferenceOfGaussianWindow("Difference of Gaussian (DoG)", { 0, 0 });
    auto WinAdaptiveThreshold = CreateAdaptiveTresholdWindow("Adaptive threshold", { 640, 0 });
    auto WinCannyTest = CreateCannyWindow("Canny edge detector class", { 1280, 0 });
    auto WinORB = CreateORBWindow("ORB keypoints detector", { 640, 480 });
    auto WinOpticalFlow = CreateOpticalFlowWindow("Optical flow", { 0, 480 });
    // auto WinSIFT = CreateSIFTWindow("SIFT keypoints detector", { 1280,480 });
    auto WinFace = CreateFaceWindow("Face Detection", { 1280, 480 });
    auto WinPerspective = CreatePerspectiveWindow("Perspective", { 200, 480 });

    // auto window = CreateORBWindow("ORB");
    // auto window = CreateAdaptiveTresholdWindow("Adaptive threshold", { 0,0 });
    // auto window = CreateSIFTWindow("SIFT keypoints detector", { 0,0 });
    // auto window = CreateOpticalFlowWindow("Optical flow", { 0,0 });

    /////////////////////////////////////////////////////
    // Capture, filter and show
    /////////////////////////////////////////////////////

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "cannot open camera";
    }

    auto filter_and_show = [](auto& cvWin, const auto& image) {
        cvWin.setImage(image);
        cvWin.show();
    };

    while (true) {

        cv::Mat src; // , src_gray;
        cap >> src;
        // cv::cvtColor(src, src_gray, cv::COLOR_RGB2GRAY);

        // filter_and_show(WinCannyTest, src);
        // filter_and_show(WinDoG, src);
        // filter_and_show(WinAdaptiveThreshold, src);
        // filter_and_show(WinSIFT, src);
        filter_and_show(WinORB, src);
        filter_and_show(WinOpticalFlow, src);
        // filter_and_show(WinFace, src);
        // filter_and_show(WinPerspective, src);

        // filter_and_show(window, src);

        int k = cv::waitKey(25);
        if (k == 27) // # close on ESC key
        {
            cv::destroyAllWindows();
            break;
        }
    }

    return;
}

#endif // HAS_OPENCV
