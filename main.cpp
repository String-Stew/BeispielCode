#include <iostream>
#include <vector>
#include <string>


using namespace std;

struct data {
    std::vector<std::string> nums;
    std::vector<std::string> op;
};


std::vector<string> split(const string &text);
bool compareStack(std::string origOp, std::string newOp);
bool isnum(std::string op);
std::vector<string> postFix(std::vector<string> calc);
void checkStack(data &opStack);

double postFixCalc(std::vector<string> postFix);
double add(double a, double b);
double sub(double a, double b);
double div(double a, double b);
double mult(double a, double b);
void refresh(std::vector<double> &vec, double val);
int main()
{
    /*
    BUG: if -x is in stack and + is to be added
    you get -+ in stack. HERE check the weights again. + = - therefore
    push - to numbers and keep +
    */
    using namespace std;
    //int a =10;
    //string b ="test+test2/hello";
    //cout <<((a < 20) ? 40:30);
    string test;
    vector<string> test2 = split("2+3*(2-3)");
    vector<string> test3 = split("3-1-1+2");
    cin >> test;
    vector<string> test4 = split(test);
    vector <string> a = postFix(test4);
    for(auto x: a) {
        cout << x;
    }
    cout << endl;
    cout <<"result: " << postFixCalc(a) << endl;
}

std::vector<string> split(const string &text) {
    std::vector<string> ret;
    int pos = 0;
    int endPos;
    string chars = {"()+-*/"};
    //size_t instead of int?
    for(int i=0; i <=text.length(); i++) {
        for(int a=0; a <=chars.length(); a++){
            if(text.substr(i, 1)== chars.substr(a,1)) {
                //cout << text.substr(pos, i-pos) << "\tOperator: " << chars.substr(a,1) << endl;
                if(text.substr(pos, i-pos)!=""){ret.push_back(text.substr(pos, i-pos));}
                if(chars.substr(a,1)!=""){ret.push_back(chars.substr(a,1));}
                pos = i+1;    
            }
        } 
    }

    return ret;
    
}

bool compareStack(std::string origOp, std::string newOp) {
    using namespace std;
    //Map would be the better solution, but my primary goal is it to work
    vector<string> ops {"+","-","*","/","(",")"};
    vector<int> opValue {2,2,3,3,0,1};
    int val1, val2;
    for(int i = 0; i<=ops.size()-1; i++) {
        if(origOp == ops.at(i)) {val1=opValue.at(i);}
        if(newOp == ops.at(i)) {val2=opValue.at(i);}
    }
    if(val1 >= val2) {return true;}
    if(val1 < val2) {return false;}
}

bool isnum(std::string op) {
    try {
        stod(op);
        return true;
    } catch(std::invalid_argument) {
        return false;
    }
}

void checkStack(data &stacks, std::string x) {
    //std::vector<string> tmp;
    if(compareStack(stacks.op.back(), x)) {
        stacks.nums.push_back(stacks.op.back());
        stacks.op.pop_back();
        stacks.op.push_back(x);
    } else {
        stacks.op.push_back(x);
    }
}
std::vector<string> postFix(std::vector<string> calc) {
    data stacks;
    for(auto x: calc) {
        if(isnum(x)) {
            stacks.nums.push_back(x);
        } else {
            if(stacks.op.size() == 0) {
                stacks.op.push_back(x);
            } else {
                //first check if ( or )
                //prevent switching if anything is before ( or ) when true
                //cout << stacks.op.back() << " -> " << x << endl;
                int flag = 1;
                if(x.compare(")") ==0 || x.compare("(") ==0) {flag = 0;}
                if(compareStack(stacks.op.back(), x) && flag == 1) {
                    stacks.nums.push_back(stacks.op.back());
                    stacks.op.pop_back();
                    checkStack(stacks, x);
                    //stacks.op.push_back(x);
                } else {
                    if(x.compare(")") == 0) {
                        for(int i = stacks.op.size()-1; i >=0; i--) {
                            if(stacks.op.at(i).compare("(") != 0) {
                                stacks.nums.push_back(stacks.op.at(i));
                                stacks.op.pop_back();
                            } else {stacks.op.pop_back();}
                        }
                    } else {
                        stacks.op.push_back(x);
                    } 
                }


            }
        }
        
    }
    for(int i = stacks.op.size()-1; i>=0; i--) {
        stacks.nums.push_back(stacks.op.at(i));
    }
    return stacks.nums;
}

double postFixCalc(std::vector<string> postFix) {
    using namespace std;
    vector<double> nums;
    vector<string> ops {"+","-","*","/"};
    int opNum;
    double tmp;
    for(auto x: postFix) {
        if(isnum(x)) {
            nums.push_back(stod(x));
        } else {
            for(int i = 0 ; i<=ops.size()-1; i++) {
                if(x == ops.at(i)) {opNum = i;}
            }
            //cout << opNum << endl;
            switch(opNum) {
                case 0:
                    tmp = add(nums.at(nums.size()-2), nums.at(nums.size()-1));
                    refresh(nums, tmp);
                    break;
                case 1:
                    tmp = sub(nums.at(nums.size()-2), nums.at(nums.size()-1));
                    refresh(nums,tmp);
                    break;
                case 2:
                    tmp = mult(nums.at(nums.size()-2), nums.at(nums.size()-1));
                    refresh(nums,tmp);
                    break;
                case 3:
                    tmp = div(nums.at(nums.size()-2), nums.at(nums.size()-1));
                    refresh(nums,tmp);
                    break;
            }
        }
    }
    return nums.at(0);
}


double add(double a, double b) {
    return a+b;
}
double sub(double a, double b) {
    return a-b;
}
double div(double a, double b){
    return a/b;
}
double mult(double a, double b){
    return a*b;
}

void refresh(std::vector<double> &vec, double val) {
    if(vec.size()-1 >= 1) {
        vec.pop_back();
        vec.pop_back();
        vec.push_back(val);
    }
}