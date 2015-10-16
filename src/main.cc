#include "random_forest.h"

int main()
{
	//directory that contains our training images
	string train_path="../train/pic";

	RandomForest rf_regressor;
	rf_regressor.BuildForest(train_path);

	string test_path="../test/pic/depth";
	ImageTable *test_image_table;
	test_image_table->LoadImages(test_path);

	vector <vector<PixelInfo> > predict_result = rf_regressor.Predict(test_image_table);


	bool validation=true;
	if(validation){
		double error=rf_regressor.Evaluate(predict_result, test_image_table);
		cout<<error<<endl;
	}
	return 0;
}