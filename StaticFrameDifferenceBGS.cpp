#include "StaticFrameDifferenceBGS.h"

StaticFrameDifferenceBGS::StaticFrameDifferenceBGS() : firstTime(true), enableThreshold(true), threshold(15), showOutput(true)
{
  std::cout << "StaticFrameDifferenceBGS()" << std::endl;
}

StaticFrameDifferenceBGS::~StaticFrameDifferenceBGS()
{
  std::cout << "~StaticFrameDifferenceBGS()" << std::endl;
}

void StaticFrameDifferenceBGS::setBackgroundRef(const cv::Mat &img_bkg)
{
  img_bkg.copyTo(img_background);
}

void StaticFrameDifferenceBGS::process(const cv::Mat &img_input, cv::Mat &img_output)
{
  if(img_input.empty() || img_background.empty())
    return;

  loadConfig();

  if(firstTime)
    saveConfig();

  cv::absdiff(img_input, img_background, img_foreground);

  if(enableThreshold)
    cv::threshold(img_foreground, img_foreground, threshold, 255, cv::THRESH_BINARY);

  if(showOutput)
    cv::imshow("Static Frame Difference", img_foreground);

  img_foreground.copyTo(img_output);

  firstTime = false;
}

void StaticFrameDifferenceBGS::saveConfig()
{
  CvFileStorage* fs = cvOpenFileStorage("./config/StaticFrameDifferenceBGS.xml", 0, CV_STORAGE_WRITE);

  cvWriteInt(fs, "enableThreshold", enableThreshold);
  cvWriteInt(fs, "threshold", threshold);
  cvWriteInt(fs, "showOutput", showOutput);

  cvReleaseFileStorage(&fs);
}

void StaticFrameDifferenceBGS::loadConfig()
{
  CvFileStorage* fs = cvOpenFileStorage("./config/StaticFrameDifferenceBGS.xml", 0, CV_STORAGE_READ);
  
  enableThreshold = cvReadIntByName(fs, 0, "enableThreshold", true);
  threshold = cvReadIntByName(fs, 0, "threshold", 15);
  showOutput = cvReadIntByName(fs, 0, "showOutput", true);

  cvReleaseFileStorage(&fs);
}