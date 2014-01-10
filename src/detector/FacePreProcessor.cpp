/**
 * @file    FacePreProcessor.cpp
 *
 * @date    01/10/2014 12:48:37 AM
 * @brief
 *
 * @details
 *
 */

#include "FacePreProcessor.h"

using std::string;
using cv::Mat;

namespace emotime {

  void FacePreProcessor::init(string faceDetectorConfig, string eyesDetectorConfig, int
      width, int height, double nwidths, double nlambdas, double nthetas) {

    this->imgsize.width = width;
    this->imgsize.height = height;
    this->nwidths = nwidths;
    this->nlambdas = nlambdas;
    this->nthetas = nthetas;
    if (eyesDetectorConfig.size() > 0) {
      this->facedet = FaceDetector(faceDetectorConfig, eyesDetectorConfig);
    } else {
      this->facedet = FaceDetector(faceDetectorConfig);
    }
    this->gaborbank.fillGaborBank(this->nwidths, this->nlambdas, this->nthetas);
  }

  FacePreProcessor::FacePreProcessor(string faceDetectorConfig, string
      eyesDetectorConfig, int width, int height, double nwidths, double
      nlambdas, double nthetas) {
    init(faceDetectorConfig, eyesDetectorConfig, width, height, nwidths,
        nlambdas, nthetas);
  }

  FacePreProcessor::FacePreProcessor(string faceDetectorConfig, int width, int
      height, double nwidths, double nlambdas, double nthetas) {
    init(faceDetectorConfig, "", width, height, nwidths, nlambdas, nthetas);
  }

  FacePreProcessor::~FacePreProcessor() {

  }

  bool FacePreProcessor::preprocess(Mat& img, Mat& featvec) {
    Mat face, filtered, fvec;
    bool res;
    res = this->extractFace(img, face);
    if (res) {
      res = this->filterImage(face, filtered);
      if (res) {
        res = this->toFeaturesVector(filtered, featvec);
      }
    }
    return res;
  }

  bool FacePreProcessor::extractFace(Mat& src, Mat& out) {
    Mat face;
    bool faceFound=this->facedet.detect(src, face);
    if (!faceFound){
      return false;
    }
    face.copyTo(out); 
    return true;
  }

  bool FacePreProcessor::filterImage(Mat& src, Mat& out) {
    Mat resized, filtered;
    resize(src, resized, this->imgsize, 0, 0, CV_INTER_AREA);
    filtered = this->gaborbank.filterImage(resized);
    filtered.copyTo(out);
    return true;
  }

  bool FacePreProcessor::toFeaturesVector(Mat& src, Mat& out) {
    Mat feat;
    feat = src.reshape(1/*chan*/, 1/*rows*/);
    feat.copyTo(out);
    return true;
  }
}
