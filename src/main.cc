#include "random_forest.h"
#include <ctime>
int main()
{
	clock_t t = clock();
	//directory that contains our training images
	string train_path="./train/train_set";

	RandomForest rf_regressor;
	rf_regressor.BuildForest(train_path);

	cout<<"finish training"<<endl;
	t = clock() - t;
	cout<<"Training time: "<<(double)t/CLOCKS_PER_SEC<<endl;

	t = clock();
	string test_path="./test/test_set/depth";//"../train/pic/depth";
  	string test_image_ext  = ".png";
  	string test_label_path ="./test/label";
 	ImageTable*	test_image_table=new ImageTable();
  	test_image_table->LoadImages(test_path,test_label_path,test_image_ext);//load images to image table

  	cout<<"finish loading testing images"<<endl<<"testing "<<test_image_table->images_.size()<<" images"<<endl;

//	ImageTable *test_image_table;
//	test_image_table->LoadImages(test_path);

	vector <vector<PixelInfo> > predict_result = rf_regressor.Predict(test_image_table);

	cout<<"Finish testing"<<endl;
	t = clock() - t;
	cout<<"Test time: "<<(double)t/CLOCKS_PER_SEC<<endl;

	cout<<"begin evaluation: "<<endl;
	bool validation=true;
	if(validation){
		double error=rf_regressor.Evaluate(predict_result, test_image_table);
		cout<<error<<endl;
	}
	cout<<"finish evaluation: "<<endl;

	cout<<"the prediction(left) and the true label(right)"<<endl;
	for(int i=0; i<predict_result.size(); i++){
		vector<PixelInfo> label_for_one_image=test_image_table->images_.at(i)->joints;
		vector<PixelInfo> prediction_of_ith_image = predict_result.at(i);
		
		double joints_error=0;
		for(int j=0; j<label_for_one_image.size(); j++){	
			cout<<prediction_of_ith_image.at(j).x<<"\t"<<label_for_one_image.at(j).x<<endl;
			cout<<prediction_of_ith_image.at(j).y<<"\t"<< label_for_one_image.at(j).y<<endl;
			cout<<prediction_of_ith_image.at(j).depth<<"\t"<< label_for_one_image.at(j).depth<<endl;
		}
		cout<<endl;
	}

	return 0;
}