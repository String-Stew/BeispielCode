#include <iostream>
#include <vector>
#include <string>

//globale definition von namespace std, achtung kann zu namespace konflikten führen.
//liber immer local in den Funktionen definieren.
using namespace std;

//Eigenen "Variable typen definieren" für handhabung der Stacks
struct data {
    std::vector<std::string> nums;
    std::vector<std::string> op;
};

//Vordefinierung aller funktionen
//*******************************

//Teilt eingabe nach operatoren und nummern auf
std::vector<string> split(const string &text);
//prüft die Reinfolge der operatoren +=- *=/ +<*
bool compareStack(std::string origOp, std::string newOp);
//Prüfung ob es sich um eine nummer oder einen Operator handelt
bool isnum(std::string op);
//Geteilte eingabe wird zu Postfix umgestellt <---- Haupteil des Codes
std::vector<string> postFix(std::vector<string> calc);
//Funktion zur überprüfung der Richtigkeit des Stacks.
void checkStack(data &opStack);

//Berechnet das Endergebnis vom PostFix.
double postFixCalc(std::vector<string> postFix);
//Hier addition, subtraktion, division, multiplikation
double add(double a, double b);
double sub(double a, double b);
double div(double a, double b);
double mult(double a, double b);
//und eine Funktion zum aktualliesieren des nummern stacks
void refresh(std::vector<double> &vec, double val);
//ENDE der vordefinierungen
//*************************

//Hauptfunktion eines jeden Programmes.
int main()
{

    using namespace std;
    //benutzer eingabe
    string test;    //Definition eines Strings
    cin >> test;    //Speicherung der eingabe in die Variable test || Typ String
    //ende der benutzer eingabe

    vector<string> test4 = split(test); //benutzer eingabe wird in nummern und Operatoren(-+/...) aufgeteilt
    vector <string> a = postFix(test4); //Eben aufgeteilte eingabe wird nun nach Postfix Sortiert
    for(auto x: a) { //code um durch den gesamten inhalt eines Vectors zu gehen
        cout << x; // Ausgabe des postfixes
    }
    cout << endl; // beendigung der zeile des Postfixes
    cout <<"result: " << postFixCalc(a) << endl; //Ergebniss wird berechnet und ausgegeben
}

std::vector<string> split(const string &text) {
    /*
        Viel text leichte funktion.
        alles ist eine zahl bis du eines der zeichen in chars getroffen wird.
        beim ersten zeichen ist die erste nummer position 0 - zur getroffenen stelle.
        danach wird pos auf i gesetzt und alles mit dem nächsten zeichen wiederholt.

        Extra info:
        Erste loop = Der Haupttext
        Zweite loop läuft für jedes zeichen im Haupttext
    */
   //Vector für die geteilte eingabe: 1+2*3 wird zu {"1","+","2","*","3"}
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

//Vergleicht die werte von operatoren
bool compareStack(std::string origOp, std::string newOp) {
    using namespace std;
    //Hier kann auch eine Map verwendet werden
    //Aber ich habe ein array für die zeichen und ein Array mit den richtigen werten für jedes zeichen
    //z.B. + = 2 -=2 *=2 ....
    vector<string> ops {"+","-","*","/","(",")"};
    vector<int> opValue {2,2,3,3,0,1};
    int val1, val2;
    //finde die werte für den Operator im stack und dem zu hinzuzufügenden
    for(int i = 0; i<=ops.size()-1; i++) {
        if(origOp == ops.at(i)) {val1=opValue.at(i);}
        if(newOp == ops.at(i)) {val2=opValue.at(i);}
    }
    //Vergleiche die Werte
    if(val1 >= val2) {return true;}
    if(val1 < val2) {return false;}
}

bool isnum(std::string op) {
    //hier trickse ich ein bisschen da stod() ein fehler ausgibt wenn
    // z.B. stod("+") benutzt wird. stod() funktioniert nur mit nummern
    //stod convertiert string nummern zu double (double ist eine komma zahl)
    //wenn ein fehler geworfen wird wird der teil nach dem catch() ausgeführt.
    //Ansonsten nur return true;
    try {
        stod(op);
        return true;
    } catch(std::invalid_argument) {
        return false;
    }
}
//für einen speziellen fall.
/*
    wenn der stack +* ist und ein - hinzu kommt.
    stack wird zu +- und das * wird zu den num stack geschrieben
    +- muss allerdings ebenso nach den regeln bearbeitet werden
    +- -> + und das - wird zu den num stack geschrieben
*/
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

//Hier ist das herz des Gesamten codes. Diese funktion benötigt alle funktionen
std::vector<string> postFix(std::vector<string> calc) {
    data stacks;
    //vorhin genant {"1","+","2","*","3"} ist hier calc.
    //for(auto x: calc)... geht eintrag für eintrag vor. Erster eintag 1 zweiter + usw.
    for(auto x: calc) {
        //wenn x eine nummer ist dann wird x zu den nummern geschickt.
        //hier währe es 1 danach wird das + geprüft und das + wirft ein false sprich der 
        //else code wird ausgeführt
        if(isnum(x)) {
            stacks.nums.push_back(x);
        } else {
            //wenn noch nicht im op stack ist schreibe x
            if(stacks.op.size() == 0) {
                stacks.op.push_back(x);
            } else {
                //wenn berreits ein operator im stack ist
                //die nächsten zwei zeilen sind so geschrieben da es probleme mit den && und den || operatoren gab
                int flag = 1;
                if(x.compare(")") ==0 || x.compare("(") ==0) {flag = 0;}
                //wenn x > oder gleich des letzten eintrages im opStack ist
                if(compareStack(stacks.op.back(), x) && flag == 1) {
                    //schreibe hintersten wert von opStack zu numStack.
                    stacks.nums.push_back(stacks.op.back());
                    //lösche letzten eintrag im OpStack
                    stacks.op.pop_back();
                    //hier die funktion von weiter oben.
                    checkStack(stacks, x);
                    //stacks.op.push_back(x);
                } else {
                    //hier wird geprüft ob x = ) ist und wenn ja werden alle zeichen im opStack
                    // bis zum nächsten ( in den numStack geschrieben
                    if(x.compare(")") == 0) {
                        for(int i = stacks.op.size()-1; i >=0; i--) {
                            if(stacks.op.at(i).compare("(") != 0) {
                                stacks.nums.push_back(stacks.op.at(i));
                                stacks.op.pop_back();
                                //entfernt (
                            } else {stacks.op.pop_back();}
                        }
                    } else {
                        //wenn beides nicht gegeben ist x > endeOpStack -> x wird zum opStack Geschrieben
                        stacks.op.push_back(x);
                    } 
                }


            }
        }
        
    }
    //letzte überbleibsel werde zum numStack geschrieben
    for(int i = stacks.op.size()-1; i>=0; i--) {
        stacks.nums.push_back(stacks.op.at(i));
    }
    //{"1","+","2","*","3"} ist hier zu {"1","2","3","+","*"} geworden
    return stacks.nums;
}

double postFixCalc(std::vector<string> postFix) {
    //bekommt {"1","2","3","+","*"} in Variable postFix
    using namespace std;
    //neuer nummern Stack
    vector<double> nums;
    //vorbereitung für Konvertierung von + zu 0 und / zu 3
    vector<string> ops {"+","-","*","/"};
    int opNum;
    double tmp;
    for(auto x: postFix) {
        if(isnum(x)) {
            //hier wird eine nummer erkannt und von string zum typen Double convertiert und
            //zum numstack geschrieben.
            nums.push_back(stod(x));
        } else {
            //wenn es keine nummer ist. wird hier der operant zu einer nummer
            //wenn x == einer der ops wird opNum zu z.B. 0 für ein +
            for(int i = 0 ; i<=ops.size()-1; i++) {
                if(x == ops.at(i)) {opNum = i;}
            }
            //cout << opNum << endl;
            //Hier werden die operationen ausgeführt
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
                    //{"1","2","3","*","+"}
                    //hier ist der num stack 1 2 3 und beim * wird 2 * 3 berrechnet
                    tmp = mult(nums.at(nums.size()-2), nums.at(nums.size()-1));
                    //num stack wird aktualliesiert
                    //{"1","2","3"} -> {"1","6"}
                    refresh(nums,tmp);
                    //beendet den switch und spring somit zum nächsten zeichen
                    //hier + also {"1", "6"} wird bei add ausgefürt (6+1)
                    break;
                case 3:
                    tmp = div(nums.at(nums.size()-2), nums.at(nums.size()-1));
                    refresh(nums,tmp);
                    break;
            }
        }
    }
    //wenn nichts mehr zu verarbeiten da ist, ist der erste eintrag im numstack das Ergebnis.
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