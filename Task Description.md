Task Description:

Create a custom RDataFrame action 

RDataFrame lets users inject their custom logic in the data processing loop in several ways. 
The most advanced and most flexible method is to use the Book method to inject a user-defined helper class in RDataFrame. 
An example tutorial for that feature is at https://root.cern.ch/doc/master/df018__customActions_8C.html  . 
For this last exercise, create your own custom RDataFrame action and a sample program that uses it. 
The custom action can do whatever you want, an idea could be to have it print the values of a specific set of columns. 
 
For extra points, the custom action could: 
- prompt the user to press Enter before continuing processing every N entries processed. 
- let the user specify a condition (e.g. via a C++11 lambda expression) that must be satisfied for the custom action to run. 
- correctly print its output (or anyway work correctly) even in multi-thread runs. To this end, the first parameter passed to the `Exec` method, i.e. the `slot` parameter, can be used to distinguish different calls from different threads. 
