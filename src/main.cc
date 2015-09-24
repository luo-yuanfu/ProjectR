#include "random_forest.h"

int main()
{
	//directory that contains our training images
	string path="~/Pictures/";

	RandomForest rf_regressor;
	rf_regressor.BuildForest(path);

	string filename="...";
	rf_regressor.Predict(filename);

	return 0;
}