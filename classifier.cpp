#include "classifier.h"


#include <qdebug.h>
using namespace cv;

classifier::classifier():
    _resultType(-1),
    _trainOk(0),
    _trainErr(0)
{ 

}


void classifier::setData(QList<QStringList> data, QStringList responses)
{

    qDebug() << responses;
	foreach (QStringList r, data)
	{
		qDebug() << r;
	}
    int ninst = data.size();
    

    if (ninst<1 && ninst != responses.size())
    {
        return; // TODO Error format
    }
    int nfeatures = data.at(0).size();
    foreach( QStringList l , data)
    {
        if (nfeatures!=l.size())
        {
            return; // TODO Error format
        }
    }

    float responseMin=std::numeric_limits<bool>::max();
    float responseMax=std::numeric_limits<bool>::min();
    foreach( QString v, responses)
    {
        float fv = v.toFloat();
        if ( responseMin > fv )
        {
            responseMin = fv;
        }
        else if ( responseMax < fv )
        {
            responseMax = fv;
        }
    }

    
    QStringList lines = _resultInfo.split("\n");
    int nclasses=0;
	int cl;
	QString clName;
    foreach( QString l, lines)
    {
		float rangemin = std::numeric_limits<int>::min();
		float rangemax = std::numeric_limits<int>::max();
        l = l.trimmed();
        if (l.isEmpty())
            continue;
        if (l.startsWith("#"))
            continue;
        QStringList items = l.split(":");
        if (items.size() != 4)
            continue;
		if (_resultType == 0 || _resultType == 2 ) // TODO enum
		{

            if ( !items.at(0).trimmed().isEmpty() )
            {
                rangemin = items.at(0).toFloat();
            }

			if ( !items.at(1).trimmed().isEmpty() )
            {
                rangemax = items.at(1).toFloat();
            }

            cl = items.at(2).toInt();
			clName = items.at(3);

        }       

		else if (_resultType == 1 ) // TODO enum
		{
			cl = items.at(0).toInt();
			clName = items.at(1);
			float step = (responseMax - responseMin) /2;
		}

		else if (_resultType == 3 ) // TODO enum
		{
			cl = items.at(0).toInt();
			clName = items.at(1);
			float step = (responseMax - responseMin) /2;
		}

		_resultRanges[cl] = ResultRange( rangemin, rangemax, cl, clName );
    }
   

    _inputData = Mat(ninst, nfeatures, CV_32FC1);
    _mresponses = Mat(ninst, 1, CV_32FC1);

	_inputData  *= 0;
    _mresponses *= 0;

    int idxInst=0;
    foreach( QStringList l, data)
    {
        int idxFeat=0;
        foreach( QString v, l)
        {
            _inputData.at<float>(idxInst,idxFeat) = v.toFloat();
            idxFeat++;
        }
        idxInst++;
    }

    idxInst=0;
    foreach( QString v, responses)
    {
        float fv = v.toFloat();
        bool classAssigned=false;
       
		foreach( ResultRange rr,  _resultRanges.values())
        {
			if ( fv >=rr._from && fv < rr._to )
            {
				_mresponses.at<float>(idxInst++,0) = rr._resultClass;
				qDebug() << fv << " ->" << rr._resultClass ;
                classAssigned=true;
                break;
            }
        }
		if (!classAssigned)
		{
			_mresponses.at<float>(idxInst++,0) = fv;
			//qDebug() <<"Error" ;
		}
    }

    reduce(_inputData,_normMin,0,CV_REDUCE_MIN);
    reduce(_inputData,_normMax,0,CV_REDUCE_MAX);
    reduce(_inputData,_normAvg,0,CV_REDUCE_AVG);
    _mdata = _inputData.clone();

}

bool classifier::setResultTypeInfo( int rtype, QString rinfo )
{
   _resultType=rtype;
   _resultInfo = rinfo;
   return false;
}

void classifier::train()
{
    Mat normalizedData = _mdata.clone();
    reduce(_mdata,_mdataMin,0,CV_REDUCE_MIN);
    reduce(_mdata,_mdataMax,0,CV_REDUCE_MAX);


    for( int f=0; f < _mdata.cols; f++)
    {
        for( int r=0; r < _mdata.rows; r++)
        {
            float x = _mdata.at<float>(r,f);
            float min = _mdataMin.at<float>(0,f);
            float max = _mdataMax.at<float>(0,f);

            normalizedData.at<float>(r,f) = (x - min) / (  max - min );
        }
    }

    float split=.75;

	Mat trainData = normalizedData( Rect(0,0,normalizedData.cols,normalizedData.rows*split) );
	Mat trainLabels = _mresponses( Rect(0,0,1,_mresponses.rows*split) );

    //Mat testData = _mdata( Rect(0,normalizedData.rows*split,normalizedData.cols,normalizedData.rows - normalizedData.rows*split) );
	Mat testData = normalizedData( Rect(0,normalizedData.rows*split,normalizedData.cols,normalizedData.rows - normalizedData.rows*split) );
    Mat testLabels = _mresponses( Rect(0,normalizedData.rows*split,1,normalizedData.rows - normalizedData.rows*split) );

    _svmClassifier = new CvSVM;
	CvSVMParams params = _svmClassifier->get_params();
	params.kernel_type = CvSVM::RBF;
	//params.kernel_type = CvSVM::LINEAR;
	params.svm_type = CvSVM::C_SVC;
	params.gamma = 0.1;
	params.C = 10.0;
	params.nu = 0.5;
	params.p = 0.1;

	_svmClassifier->train(trainData, trainLabels, Mat(), Mat(), params);
	
    _trainOk=0;
    _trainErr=0;
    for( int r=0; r < testData.rows; r++)
    {
        Mat dataRow = testData(cv::Rect(0,r,normalizedData.cols,1));
		QString tmpData;
		std::list<float> values;
		for( int c=0; c < dataRow.cols; c++)
		{
			values.push_back( dataRow.at<float>(0,c) );
			tmpData += QString(" %1 ").arg(dataRow.at<float>(0,c),4);
		}
        int res = predict(values, false);
		
        float soll = _mresponses.at<float>(r,0);

		tmpData += QString(" : %1 - %2 ").arg(soll).arg(res);
		qDebug() << tmpData;

        //if ( qAbs(soll - ist ) < 2  )
        if ( soll == res )
            _trainOk++;
        else
            _trainErr++;

        qDebug()<< r<<int(_mresponses.at<float>(r,0))<<" Pr:" << res << _trainOk <<" "<<_trainErr << _trainOk*100 / (_trainOk+_trainErr);
    }
}


float classifier::getMinFor(int idx)
{
    return _normMin.at<float>(0,idx);
}

float classifier::getMdataMinFor(int idx)
{
    return _mdataMin.at<float>(0,idx);
}

float classifier::getMaxFor(int idx)
{
    return _normMax.at<float>(0,idx);
}

float classifier::getMdataMaxFor(int idx)
{
    return _mdataMax.at<float>(0,idx);
}

float classifier::getAvgFor(int idx)
{
    return _normAvg.at<float>(0,idx);
}

int classifier::predict( std::list<float> values, bool normalize)
{
    Mat sampleMat = Mat(1, values.size(), CV_32F);
    int idx=0;
	QString tmpDbg;
    foreach (float x, values)
    {
		float normX = x;
		if (normalize)
		{
			float min = _mdataMin.at<float>(0,idx);
			float max = _mdataMax.at<float>(0,idx);
			normX = (x - min) / (  max - min );
		}
		tmpDbg += QString(" %1").arg(normX);
		sampleMat.at<float>(0,idx) = normX;
        idx++;
    }
    float response = _svmClassifier->predict(sampleMat);
	qDebug() <<"Predict: "<< tmpDbg << " => " << response;
    return response;
}
