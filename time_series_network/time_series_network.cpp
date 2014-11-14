// time_series_network.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "math.h"
#include "F:/Projets-C++/neural_network/neural_network/Neural_Network.h"
#include "F:/Projets-C++/neural_network/neural_network/Back_prop.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{

	vector<int> scheme = { 3, 1, 1 };
	double alpha = 2.;
	double stop_crit = 0.00001;
	double lambda = 0.0;

	string path = "F:/Projets-C++/neural_training_test/sinus2_training.csv";
	string save_path = "F:/Projets-C++/time_series_network/cost_evol.out";
	vector<vector<double>> training_inputs;
	vector<vector<double>> training_outputs;

	vector<double> cost_vect;




	ifstream file(path.c_str());

	if (file)
	{
		double x, y;
		string input;
		vector<double> in, out;
		vector<double> temp;


		getline(file, input);


		int it = 0;
		while (getline(file, input))
		{
			stringstream ss(input);

			while (ss >> x)
			{
				

				ss >> y;

				if (it < 3)
				{
					temp.push_back(y);
					++it;
				}
				else
				{
					out = { y };
					training_outputs.push_back(out);

					training_inputs.push_back(temp);

					temp.clear();
					it = 0;
				}
			}
		}
		cout << "end reading \n";
	}
	else
	{
		cout << "ERROR in opening file : " << path.c_str() << endl;
		exit(1);
	}


	for (size_t j = 0; j < training_inputs.size(); ++j)
	{
		for (size_t i = 0; i < training_inputs[j].size(); ++i)
		{
			cout << training_inputs[j][i] << " ";
		}
		cout << endl;
	}

	for (size_t j = 0; j < training_outputs.size(); ++j)
	{
		for (size_t i = 0; i < training_outputs[j].size(); ++i)
		{
			cout << training_outputs[j][i] << " ";
		}
		cout << endl;
	}

	cout << "sizes " << training_inputs.size() << " " << training_inputs.size() << endl;


	// network
	Network network;
	network.build_network(scheme, 0, 111);

	//training
	Back_prop back_prop(network,lambda);
	back_prop.training(training_inputs, training_outputs, alpha, stop_crit, save_path);


	//-------------------------------------------------------------------------------------------------

	vector<vector<double>> weights;

	weights = back_prop.get_mod_weights();
	network.set_allWeights(weights);

	save_path = "F:/Projets-C++/time_series_network/trained_sin2.out";
	double a;
	vector<vector<double>> x, y;
	vector<double> dat;
	vector<double> o;

	int it = 0;
	for (int i = -100; i < 100; ++i)
	{

		if (it < 3)
		{
			a = pow(sin( i*20. / 200. ),3);
			dat.push_back(a);
			
			++it;
		}
		else
		{
			network.set_inputs(dat);
			network.forward_prop();

			o = network.get_outputs();
			y.push_back(o);

			dat.clear();
			it = 0;

		}
		
	}

	cout << "end test" << endl;

	ofstream sfile(save_path.c_str());

	if (sfile)
	{
		for (size_t i = 0; i < y.size(); ++i)
		{
			sfile << i << ", " << y[i][0] << endl;
		}
	}
	else
	{
		cout << "ERROR can't open file to save \n";
	}



	return 0;
}

