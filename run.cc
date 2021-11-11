#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

std::string tostr(int i)
{
 if (i==0) return "A";
 if (i==1) return "B";
 if (i==2) return "C";
 if (i==3) return "D";
 return "";
}
int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  const Data datA("exp_A");
  const Data datB("exp_B");
  const Data datC("exp_C");
  const Data datD("exp_D");
  vector<Data> all;
  all.push_back(datA);
  all.push_back(datB);
  all.push_back(datC);
  all.push_back(datD);
  // here is the data from experiment A
  cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
       << endl;
  //cout << "measurement of experiment A in bin 27: " << datA.measurement(27)  << endl;
  for (Data dat:all)
  {
  	cout << "measurement of experiment "<< dat.getName().at(4) <<" in bin 27: " << dat.measurement(27)
       << " pb" << endl;
  }
  // 1d
  int k=27;
  int n=1;
  for (int i=0; i<4; i++)
  {
  	for (int j=i+1; j<4; j++)
  	{
  		double deltasig=abs(all.at(j).measurement(k)-all.at(i).measurement(k));
  		double sigdeltasig=sqrt(all.at(j).error(k)*all.at(j).error(k)+all.at(i).error(k)*all.at(i).error(k));
  		if (deltasig<=n*sigdeltasig)
  		{
  			cout << tostr(i)<<":"<< tostr(j)<<  " Smoll"<< endl;
  		}
  		else
  		{
  			cout << tostr(i)<<":"<< tostr(j)<<  " big"<< endl;
  		}
  	}

  }
   // 1d_2
  n=1;
  for (int i=0; i<4; i++)
  {
  	for (int j=i+1; j<4; j++)
  	{
  		int difbin=all.at(i).check(all.at(j),n);
  		cout<< "n="<< n<< "   Messung " << tostr(i)<<":"<< tostr(j)<< "   =   " << difbin << endl;
  	}


  }


 //Data datAll("exp_A");
//exp 2. a
n=2;
for (int i=0; i<4; i++)
{
  int divs=all.at(i).deviationModel(n);
  std::cout<< "\nn="<< n<<" Messung " << tostr(i) << ":   " <<  divs << '\n';
  std::cout << "############################" << '\n';
}
std::cout << "Expected for n=2: "<<  (1-0.9545)*all.at(1).size()<< '\n';
n=3;
for (int i=0; i<4; i++)
{
  int divs=all.at(i).deviationModel(n);
  std::cout<< "\nn="<< n<<" Messung" << tostr(i) << ":   " << divs << '\n';
  std::cout << "############################" << '\n';
}
std::cout << "Expected for n=3: "<<  (1-0.9973)*all.at(1).size()<< '\n';


// Ex2.b and c
 Data sum = datA+datB+datC+datD;
 for (int i=0; i<4; i++)
 {
   std::cout<<" Messung" << tostr(i) << "  Chi²/dof:   " << all.at(i).calcchipdof() << '\n';
 }
 std::cout<<" Messung Combined"  << "  Chi²/dof:   " << sum.calcchipdof() << '\n';

 for (int i=0; i<4; i++)
 {
   all.at(i).print("Mes"+tostr(i)+".txt");
 }
sum.print("MesSum.txt");
  return 0;
}
