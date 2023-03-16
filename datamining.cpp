
#include "stdafx.h"
#include "SampleSet.h"
#include "Model.h"
int main(int argc, _TCHAR* argv[])
{
	int i;
	std::list<std::string> mltypes;
	//mltypes.push_back(CV_TYPE_NAME_ML_ANN_MLP);

	//-mltypes.push_back(CV_TYPE_NAME_ML_KNN);
	//mltypes.push_back(CV_TYPE_NAME_ML_NBAYES);
	//-mltypes.push_back(CV_TYPE_NAME_ML_BOOSTING);
	//mltypes.push_back(CV_TYPE_NAME_ML_TREE);
	
	//mltypes.push_back(CV_TYPE_NAME_ML_CNN);
	mltypes.push_back(CV_TYPE_NAME_ML_RTREES);
	//mltypes.push_back(CV_TYPE_NAME_ML_ERTREES);
	//mltypes.push_back(CV_TYPE_NAME_ML_GBT);
	
	mltypes.push_back(CV_TYPE_NAME_ML_SVM);


	int n=0; 
	for (std::list<std::string>::iterator it = mltypes.begin(); it != mltypes.end(); it++)
	{
		Model model((*it).c_str());
		cout << (*it) <<endl;
		
		if (*it == CV_TYPE_NAME_ML_ANN_MLP)
		{
			model.SetPara(CvANN_MLP_TrainParams(cvTermCriteria(CV_TERMCRIT_ITER,100,0.001),CvANN_MLP_TrainParams::BACKPROP, 0.001));
		}
		else if(*it == CV_TYPE_NAME_ML_KNN)
		{
			
		}
		else if(*it == CV_TYPE_NAME_ML_SVM)
		{
			CvSVMParams params;
					params.kernel_type = CvSVM::RBF;
					params.svm_type = CvSVM::C_SVC;
					params.gamma = 1.0;
					params.C = 1.0;
					params.nu = 0.5;
					params.p = 0.1;
			model.SetPara(params);
		}
		else if(*it == CV_TYPE_NAME_ML_BOOSTING)
		{
			//CvBoostParams params(CvBoost::REAL, 100, 0.95, 5, false, 0 );
			CvBoostParams params(CvBoost::DISCRETE, 100, 0.95, 2, false, 0);
			model.SetPara(params);
		}
		else if(*it == CV_TYPE_NAME_ML_RTREES)
		{
			CvRTParams params( 10, 2, 0, false, 16, 0, true, 0, 100, 0, CV_TERMCRIT_ITER );
			model.SetPara(params);
		}

		

		model.Train("_model.datset");

		model.Save( ((*it) + std::string(".model")).c_str() );
		
		//model.Load("svm.model");
		model.Predict("_model.datset", "err.datset");
		
		//scanf("%d", &i);

	}

	scanf("%d", &i);

	
	return 0;

}

