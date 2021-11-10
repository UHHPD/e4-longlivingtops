#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>
#include <cmath>

class Data {
 public:
  Data(const std::string& filename);
  Data(std::vector<double> m_data,std::vector<double> m_bins,std::vector<double> m_errors);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return m_bins[i]-(m_bins[i]-m_bins[i+1])/2; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_errors[i];  }
  
  int check(const Data& in, int n) const;
  Data operator+(const Data& in) const;
  
  //get&set
  std::string getName() const { return name;  }
  std::vector<double> getBins() const {return m_bins;}

 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;
  std::string name;
};

#endif
