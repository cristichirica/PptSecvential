
#include "pch.h"
#include <iostream>
#include "mpi.h"
#include <fstream>
#include <string>
#include <ctime>
#include <vector> 
#include "Helpers.h"
#include <chrono>
using namespace std;

void ActionParentProcess() {

	vector<Monom> polyResult;
	unsigned int n = 20;
	unsigned int k = 1;
	auto begin = chrono::high_resolution_clock::now();
	while (k!=n+1)
	{
		ifstream file("poly"+to_string(k)+".txt");
		int grad,coef;
		while (file >> grad)
		{
			if (file >> coef)
			{
				Monom curent;
				curent.grad = grad;
				curent.coef = coef;
				bool gasitGrad = false;
				for (int i = 0; i < polyResult.size() && gasitGrad==false; i++)
				{
					if (polyResult[i].grad == curent.grad) {
						gasitGrad = true;
						polyResult[i].coef = polyResult[i].coef + curent.coef;
					}
				}
				if (gasitGrad == false) {
					polyResult.push_back(curent);
				}
				
			}
		}
		k++;
		file.close();
	}
	auto end = chrono::high_resolution_clock::now();
	WriteResultToFile("poly-sum.txt", polyResult);
	cout << "\n" << chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
}

int main()
{
	/*for (int i = 0; i < 20; i++)
	{
		CreateRandomNumbersFile("poly"+to_string(i)+".txt", 500, 1, 500);
	}*/

	int rank;

	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		std::cout << "Err MPI init\n";
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		ActionParentProcess();
	}

	MPI_Finalize();
}


