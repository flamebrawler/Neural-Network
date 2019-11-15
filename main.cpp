#include <iostream>
#include <fstream>
#include "Parser.h"
#include "NeuralNetwork.h"
#include "BmpParser.h"
#include <math.h>

using namespace std;

/*
 * prints to cout the results of the input when put therough the network
 * n is the network to run
 * input is the array that will be inputted to the network to when it runs
 */
void printResult(NeuralNetwork& n, const double* input) {
	double* result;
	n.run(input);
	result = n.getResult();
	for (int i = 0; i < n.getOutputs(); i++) {
		cout << result[i] << endl;
	}
	delete[] result;
}

int main(int arg, char* argv[]) {
	bool run = 0;//if the user just wants to run the network
	bool learn = 0; //whether the network will run or learn
	bool append = 0; //whether the  program will use another network as a baseline
	bool tocout = 0; //if the user didn't input any file whether or not it will print it to cout
	string nnwfile = "names2.nnw"; //the file inputted or by default outputted
	string outfile = nnwfile; //the file outputted
	string organizerfile = "names.og"; //the file holding the locations of files and interpratations

	int pos = 1; //position in argv
	if (arg > 1) { //use command line
		//check for organizer file
		if (contains(argv[pos], ".og")) {
			organizerfile = argv[pos];
			learn = 1;
			pos++;

			//check for input network file
			if (arg > pos && contains(argv[pos], ".nnw")) {	//if there is a nnw file
				nnwfile = argv[pos];

				pos++;
				//check for output network file
				if (arg > pos && contains(argv[pos], ".nnw")) {
					outfile = argv[pos];
					pos++;
					append = 1;
				} else
					outfile = nnwfile;
				//check if user wants to add onto the network
				if (arg > pos
						&& (contains(argv[pos], "yes")
								|| contains(argv[pos], "true")
								|| contains(argv[pos], "1")
								|| contains(argv[pos], "yep"))) {
					append = 1;
					pos++;
				}
				if (arg > pos)
					cerr << "not all inputted arguments were used: "
							<< (arg - pos) << " arguments remain"<<endl;
			} else {
				cout
						<< "no .nnw file is inputted so network will be printed to standard output as standard text"
						<< endl;
				tocout = 1;
			}
		} else {
			if (contains(argv[pos], ".nnw")) { //if user wants to run network
				nnwfile = argv[pos];
				run = 1;
				pos++;
				if (arg > pos)
					cerr << "not all inputted arguments were used: "
							<< (arg - pos) << " arguments remain"<<endl;
			} else { //if nothing useful is inputted or in the wrong order
				cerr
						<< "a .og file containing locations of files used and their interpratations are required for the network to learn\n";
				cerr
						<< "Or a .nnw file is require to run the input if learning is not intended\n\n";
				cerr
						<< "Please input in the format:\n\t<.og file> <.nnw file> optional:<output .nnw file>(defauts to other file)\n\t optional: <whether to rewrite or add to existing network>(yes, true,1,yep or other for no)(defaults at no)";
				cerr
						<< "Or:\n\t<.nnw file> <string or file to use as input for network>";
				return 1;
			}
		}

	}
	if (learn) {
		match* list;
		int num = 0;
		list = load(organizerfile, num);
		supervised net;
		if (append) { //read from a .nnw file
			cout << "improving an existing network" << endl;
			ifstream in(nnwfile.c_str(), ios::in | ios::binary);//load a file to use as baseline
			if (in.fail()) {
				cerr << nnwfile << " file not opened";
				exit(0);
			}

			net.readNNW(in);
			in.close();
		} else {//no file given
			string temp;
			int hidden, hiddennodes;

			//input the size of the network
			cout << "input number of hidden layers" << endl;
			getline(cin, temp);
			hidden = (int) getD(temp);
			cout << "input number of nodes in the layers" << endl;
			getline(cin, temp);
			hiddennodes = (int) getD(temp);

			supervised s(list[0].in, hiddennodes, list[0].out, hidden);//use the data inputted
			net.copyInfo(s);
			net.copyVals(s);
		}
		net.learn(list, num);//teach itself using the data

		if (!tocout) {//print the network if no network is given
			ofstream f(outfile.c_str(), ios::out | ios::binary);
			f << net;
			f.close();
		} else
			cout << net;

		delete[] list;
	} else if (run) {//if user just wants to run the network

		supervised words;//load the file to use
		ifstream in(nnwfile.c_str(), ios::in | ios::binary);
		words.readNNW(in);
		in.close();
		string name;
		cout << "input text or a file" << endl;
		getline(cin,name);
		double* data = convertfile(name,words.getInputs());//check if its a file and use it if it is
		if(!data)
			data = wordtonum(name,words.getInputs()/26);//use as text if not recognized
		printResult(words, data);

	} else {

		cout
				<< "to use properly please input in command line in the format:\n\t<.og file> <.nnw file> optional:<output .nnw file>(defauts to other file)\n\t optional: <whether to rewrite or add to existing network>(yes, true,1,yep or other for no)(defaults at no)";
		cout
				<< "Or:\n\t<.nnw file> <string or file to use as input for network>"
				<< endl;

		cout << "xor: " << endl;
		supervised nn(2, 5, 1, 2);

		double input[2] = { 1, 0 };//inputs of the xor
		double input2[2] = { 0, 1 };
		double input3[2] = { 1, 1 };
		double input4[2] = { 0, 0 };
		double *inputs[] = { input, input2, input3, input4 };

		for (int i = 0; i < 1000; i++) {//teach it
			nn.learn(inputs[0], 1);
			nn.learn(inputs[1], 1);
			nn.learn(inputs[2], 0.0);
			nn.learn(inputs[3], 0.0);
		}

		for (int i = 0; i < 4; i++) {//print out the reslts
			cout << inputs[i][0] << " " << inputs[i][1] << ": ";
			printResult(nn, inputs[i]);
		}
		cout << "gender identifier" << endl;

		supervised names;
		ifstream in(nnwfile.c_str(), ios::in | ios::binary);
		if (!in.fail())//check for error
			names.readNNW(in);
		else {
			cerr << "file not loaded";
			return 1;
		}
		char name[14];
		cout << "input a name" << endl;
		cin.getline(name, names.getInputs() / 26);//check if it is a boy name or a girl name based on previous data
		printResult(names, wordtonum(name, names.getInputs() / 26));
	}

	return 0;

}
