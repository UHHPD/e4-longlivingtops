#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <vector>

using namespace std;

Data::Data(std::vector<double> m_data,std::vector<double> m_bins,std::vector<double> m_errors) :
	m_data(m_data),m_bins(m_bins),m_errors(m_errors)
{


}

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }


  // read in data from file: m_siz  errors
  for (int i = 0; i < size; ++i) {
    double errors;
    file >> errors;
    m_errors.push_back(errors);
  }



  // done! close the file
  file.close();

  assertSizes();
  name=filename;
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::check(const Data& in, int n) const{
	int difbin=0;
	for (int k=0; k<size();k++)
	{
		double deltasig=abs(measurement(k)-in.measurement(k));
  		double sigdeltasig=sqrt(error(k)*error(k)+in.error(k)*in.error(k));
  		if (deltasig>n*sigdeltasig)
  		{
  			difbin+=1;
  		}
	}
	return difbin;
}
double Data::background(double x,double alpha,double beta, double gamma, double delta) const
{
	return alpha+beta*x+gamma*exp(-delta*x);
}
double Data::calcchipdof() const
{
	double chi=0;
	for (int i=0; i<size();i++)
	{
		chi+=pow(measurement(i)-background(binCenter(i)),2)/(pow(error(i),2));
	}
	return (chi/(size()-4));
}
void Data::print(std::string filename) const
{
	ofstream out(filename);
	for (int i=0; i<size();i++)
	{
		out << binCenter(i)<< "\t" << measurement(i)<< "\t"<< error(i)<< endl;
	}
	out.close();

}

int Data::deviationModel(int n) const
{
	int deviation=0;
	std::cout << "\n############################" << '\n';
	std::cout << "Messung "<< getName().at(4) << '\n';
	for (int i=0; i<size();i++)
	{
		if (abs(measurement(i)-background(binCenter(i)))>n*error(i))
		{
			std::cout << "x=" << i << "\t";
			deviation+=1;
		}
	}
	return deviation;
}

Data Data::operator+(const Data& in) const
{
	vector<double> new_data;
	vector<double> new_bins;
	vector<double> new_errors;
	if (check(in,2)<3)
	{
	for (int k=0; k<size();k++)
	{
		double omega1=1/(pow(error(k),2));
		double omega2=1/(pow(in.error(k),2));
		new_data.push_back((omega1*measurement(k)+omega2*in.measurement(k))/(omega1+omega2));
		new_errors.push_back(sqrt(1/(omega1+omega2)));

	}
	Data outputData(new_data,getBins(),new_errors);
	return outputData;
	}
	else
	{
	cout<< "not compatible in 2 sigma" << endl;
	return Data({},{},{});
	}

}
