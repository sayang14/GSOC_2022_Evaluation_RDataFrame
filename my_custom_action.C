#include <ROOT/RDataFrame.hxx>
#include <memory>
 
template<typename T>

class InfiniteSum final : public ROOT::Detail::RDF::RActionImpl<class InfiniteSum<T>> 
{
public:
using Result_t = T;

private:
   std::vector<T> fSums;
   //T fSums;
   int fnslots;
   std::shared_ptr<T> fResultSum;
   
   void onetosum(const T &x, T &sum) {
      T y = 1-x;   
      sum = 1/y;
      //sum = (x*(x+1)) / 2;
   }
   
public:
   InfiniteSum(InfiniteSum &&) = default;
   InfiniteSum(const InfiniteSum &) = delete;
   
   InfiniteSum(const std::shared_ptr<T> &r) : fResultSum(r)
   {
      fnslots = ROOT::IsImplicitMTEnabled() ? ROOT::GetThreadPoolSize() : 1;
      fSums.resize(fnslots,0);
   }
   
   std::shared_ptr<Result_t> GetResultPtr() const { return fResultSum; }
   
   void Initialize() {}
   void InitTask(TTreeReader *, unsigned int) {}
   
   void Exec(unsigned int slot, T x)
   {
      onetosum(x,fSums[slot]);
   }
   
   template <typename V=T, std::enable_if_t<ROOT::Internal::RDF::IsDataContainer<V>::value, int> = 0>
   void Exec(unsigned int slot, const T &vs)
   {
      for (auto &&v : vs) {
         Exec(slot, v);
      }
   }
   
   void Finalize() 
   {
      T sum(0);
      for (int i = 0; i < fnslots; ++i) {
         onetosum(fSums[i],sum);
         *fResultSum = fSums[i];
      }
      //*fResultSum = sum;
   }
   
   std::string GetActionName(){
      return "InfiniteSum";
   }
}; 
 
void my_custom_action() {
   ROOT::EnableImplicitMT(2);
   ROOT::RDataFrame d(128);
   
   //double n;
   //std::cout << "Enter the ratio for the infinte sum should be within (-1,1): " << std::endl;
   //std::cin>>n;
   auto input = []() { return gRandom->Uniform(-1, 1); };
 
   //auto dd = d.Define("x", "(rdfentry_ %2 == 0) ? 0.5 : -0.6");
   auto dd = d.Define("x",input);
   
   auto ptr = std::make_shared<double>();
   InfiniteSum<double> helper(ptr);
   
   ROOT::RDF::RResultPtr<double> resultPtr;
   
   if(input!=0){auto Result = dd.Book<double>(std::move(helper), {"x"});
   resultPtr = Result;}
   //auto plain  = dd.Sum<int>({"x"});
   
   std::cout << "The final sum is :"<< *resultPtr << " " << std::endl;
   
   //auto d1 = dd.Display("");
   //d1->Print();
}





