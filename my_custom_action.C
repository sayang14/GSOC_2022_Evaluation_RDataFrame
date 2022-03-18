//Implementing Infinte sum custom action starting from 1. 
// Sum of Infinite GP When |r| < 1
// Let us derive a formula for the same. Consider a GP whose first term is 'a' and the common ratio is 'r' where |r| < 1. Then the sum of its infinite terms is:
// S = a + ar + ar2 + ar3 + ...___ (1)
// Multiply both sides by 'r':

// rS = ar + ar2 + ar3 + ...___ (2)

// Subtracting (2) from (1):

// S - rS = a

// S (1 - r) = a

// Dividing both sides by (1 - r),

// S = a / (1 - r)
// This is the formula for infinite GP when r is less than 1.
//In this program a = 1;
//So S = 1 / (1 - r)  when |r| < 1.

#include <ROOT/RDataFrame.hxx>
#include <memory>
 
template<typename T>

class InfiniteSum final : public ROOT::Detail::RDF::RActionImpl<class InfiniteSum<T>> 
{
public:
using Result_t = T;

private:
   std::vector<T> fSums;
   int fnslots;
   std::shared_ptr<T> fResultSum;
   
   void onetosum(const T &x, T &sum) {
      T y = 1-x;   
      sum = 1/y;
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
         //*fResultSum = fSums[i];
      }
      *fResultSum = fSums[fnslots-1];
   }
   
   std::string GetActionName(){
      return "InfiniteSum";
   }
}; 
 
void my_custom_action() {
   ROOT::EnableImplicitMT(2);
   ROOT::RDataFrame d(128);
   
   auto input = []() { return gRandom->Uniform(-1, 1); };
 
   auto dd = d.Define("x",input);
   
   auto ptr = std::make_shared<double>();
   InfiniteSum<double> helper(ptr);
   
   auto Result = dd.Book<double>(std::move(helper), {"x"});
   
   std::cout << "The final sum is :"<< *Result << " " << std::endl;
   
}





