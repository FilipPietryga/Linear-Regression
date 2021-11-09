#include <iostream>
#include <vector>
#include <exception>
#include <iterator>

class DiffrentSizeVectorsException : public std::exception {
public:
	DiffrentSizeVectorsException(const char* error) 
		: std::exception(error) {
	}
	const char* what() const throw () {
		return "The Vectors should not have different sizes!";
	}
};

//a template function to calculate sums of 
//elements in the random type vectors
template<class T>
T toSum(std::vector<T> &values) {
	T sum = 0;
	
	//iterate through the values and sum them
	//using the iterator loop
	for (auto it = values.begin(); it != values.end(); it++) {
		sum += *it;
	}
	return sum;
}

//a template function to calculate means of 
//elements in the random type vectors
template<class T>
T mean(std::vector<T> &values) {

	//using the toSum method from above
	T su = toSum<T>(values);
	int size = values.size();
	T mean = su / size;
	return mean;
}

//a template function to calculate an array of
//multiplications of elements from the two random
//type vectors
template<class T>
std::vector<T> multiply(std::vector<T> &vecA, std::vector<T> &vecB) throw (DiffrentSizeVectorsException) {
	std::vector<T> results;

	//both vectors are assumed to have
	//the same size
	if (vecA.size() != vecB.size()) 
		throw DiffrentSizeVectorsException("Error! Both vectors should be the same size.");
	
	//calculate the results of the multiplication
	//of both vectors by each value of both of them
	//separately
	for (unsigned i = 0; i < vecA.size(); i++) {
		results.push_back(vecA[i] * vecB[i]);
	}
	return results;
}

//a template function to estimate slope
//of the linear function that might be as 
//close as possible to the one "drawn" by the points
//made out of dependent and independent vars
template<class T>
T estimate_slope(T idDiffOfSquareSum, T iiDiffOfSquareSum) {
	T slope = idDiffOfSquareSum / iiDiffOfSquareSum;
	return slope;
}

//a template function to estimate intercept
//of the linear function that might be as 
//close as possible to the one "drawn" by the points
//made out of dependent and independent vars
template<class T>
T estimate_intercept(T &idDiffOfSquareSum, T &iiDiffOfSquareSum, T &dependentMean, T &independentMean) {
	T slope = idDiffOfSquareSum / iiDiffOfSquareSum;
	T intercept = dependentMean - slope * independentMean;
	return intercept;
}

//a template function to predict the value
//as if it was on the linear function
//that might be as close as possible to the 
//one "drawn" by the points made out of
//dependent and independent vars
template<class T>
T predict(T &slope, T &intercept, T &independent) {
	T prediction = slope * independent + intercept;
	return prediction;
}

int main()
{
	//our datasets 
	//independent = X
	//dependent = Y
	std::vector<float> independent{0,1,2,3,4,5};
	std::vector<float> dependent{50,100,150,200,250,300};

	//mean values
	float independentMean = mean<float>(independent);
	float dependentMean = mean<float>(dependent);

	//numbers of values in the datasets 
	//(assumed there is equal number of them 
	//in both datasets)
	int num_of_vals = independent.size();

	//placeholders for multiplication vectors
	std::vector<float> idMultiplication;
	std::vector<float> iiMultiplication;

	try {

		//calculating the  
		//multiplication vectors of both
		//independent x dependent 
		//and independent squared
		idMultiplication = multiply<float>(independent, dependent);
		iiMultiplication = multiply<float>(independent, independent);

	}
	catch (DiffrentSizeVectorsException& exception) {
		std::cerr << exception.what() << std::endl;
		std::cerr << "Change your vectors so that they have the same size" << std::endl;
		return EXIT_FAILURE;
	}

	//the substrahend needed to calculate the difference
	//so that we can obtain the divident and the divider
	//that are needed later
	float idDiffSubtrahend = num_of_vals * independentMean * dependentMean;
	float iiDiffSubtrahend = num_of_vals * independentMean * independentMean;

	//said divider and the divider
	float idDiffOfSquareSum = toSum<float>(idMultiplication) - idDiffSubtrahend;
	float iiDiffOfSquareSum = toSum<float>(iiMultiplication) - iiDiffSubtrahend;

	//calculating the slope
	float slope = estimate_slope<float>(idDiffOfSquareSum, iiDiffOfSquareSum);

	//calculating the intercept 
	//passing parameters needed
	float intercept = estimate_intercept<float>(idDiffOfSquareSum, iiDiffOfSquareSum, dependentMean, independentMean);

	//the display of the regression for the range of 20 first natural numbers
	//representing the group of points that fall on the line made
	//by the regression operation 
	for (float i = 0; i < 20; i++) {
		std::cout << "(" << i << ", " << predict<float>(slope, intercept, i) << ")" << std::endl;
	}
}