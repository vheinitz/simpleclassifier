#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
//#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/ml/ml.hpp>

#include <list>

//using namespace cv::ml;
using namespace cv;

struct ResultRange
{
    float _from;
    float _to;
	int _resultClass;
    QString _outputText;
    ResultRange(float from = -999999, float to=99999, int resClass=-1, QString t=QString()):
        _from(from), _to(to), _resultClass(resClass), _outputText(t){}
};


class classifier
{
	QMap<int, ResultRange> _resultRanges;
	CvSVM *_svmClassifier;
    //CvRTrees *_rtClassifier;
	//CvGBTrees *_gbClassifier;
	//CvNormalBayesClassifier *_nbClassifier;

public:
    classifier();
    void setData(QList<QStringList> data, QStringList responses);
    bool setResultTypeInfo( int rtype, QString rinfo );
    void train();
    int predict(std::list<float> values, bool normalize = true);
    void test();
    float getMinFor(int idx);
    float getMaxFor(int idx);
    float getAvgFor(int idx);
    float getMdataMinFor(int idx);
    float getMdataMaxFor(int idx);

    int getOkCnt() const { return _trainOk; }
    int getErrCnt() const { return _trainErr; }
	
	int _trainOk;
    int _trainErr;
private:
    Mat _normMin;
    Mat _normMax;
    Mat _normAvg;
    Mat _inputData;
    Mat _mdata;
    Mat _mdataMin;
    Mat _mdataMax;
    Mat _mresponses;
   
    int _resultType;
    QString _resultInfo;

};

#endif // CLASSIFIER_H
