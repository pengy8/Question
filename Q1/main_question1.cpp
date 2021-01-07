// Q1.cpp

#include <iostream>
using namespace std;

class ExpandPowersOf{
   private:
      const int& m;
   public:
      ExpandPowersOf(int n) : m(n){
      //constructor
        if (m==0){
            cout <<"1"<<endl;
        }
        else{ 
            
            if (n<0){
                cout <<"1/";
                n = abs(n);
            }
            
            int mFact = 1; 
            for (int i = 2; i <= n; i++){ 
                mFact *= i;
            }
            int miFact = mFact; 
            int iFact = 1;
            
            // loop to display the series 
            for (int i = 0; i < n+1; i++) { 
          
                // display the series 
                if (i==n){
                    cout <<"q^"<<i<<")"<<endl;
                }
                else{ 
                    if (i==0){
                        cout <<"(p^"<<n-i<<"+";
                    }
                    else
                    {
                        cout << (mFact)/(miFact * iFact)<<"p^"<<n-i<<"q^"<<i<<"+";
                    }
                
                    miFact /= (n-i);
                    iFact *= (i+1);
                }
          
            } 
        }  
   }
};

int main() {
   ExpandPowersOf(-2);
   return 0;
}
