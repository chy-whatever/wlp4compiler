#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <vector>
using namespace std;


typedef map<string, pair<vector<string>, map<string, pair<string, int>>> > T;
T tst;
int index = 0;
int f = 0;
string helpname;
string procname;
int delnum;
int newnum;
class erro {};

class node {
public:
    string r;
    vector< string > token;
    vector< node > vec;
};


//determine whether it is terminate
int Ter(string str, string ter[])
{
    for (int i = 0; i < 35; i++) {
        if (str == ter[i]) {
            return 1;
        }
    }
    return 0;
}


// build the parse tree
node build(string ter[]) {
    string rule;
    getline(cin, rule);
    node pt;
    pt.r = rule;
    istringstream iss(rule);
    string s;
    while (iss >> s) {
        pt.token.push_back(s);
    }
    if (Ter(pt.token[0], ter) == false) {
        int len = pt.token.size();
        for (int i = 1; i < len; i++) {
            pt.vec.push_back(build(ter));
        }
    }
    return pt;
}


//build the symbol table 
void bst(node pt, string name) {
    int e = 110;
    for (auto it = pt.vec.begin(); it != pt.vec.end(); ++it) {
        //inser function expcept wain
        if (it->r == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            //get fubction name;
            if (tst.find(it->vec[1].token[1]) == tst.end()) {
                name = it->vec[1].token[1];
                tst.insert(make_pair(name, pair<vector<string>, map<string, pair<string, int>>>()));
            }
            else {
                throw e;
            }
        }
        else  if (it->r == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            //insert wain function
            if (tst.find("wain") == tst.end()) {
                name = "wain";
                tst.insert(make_pair("wain", pair<vector<string>, map<string, pair<string, int>>>()));
                if (it->vec[3].vec[0].r == "type INT STAR") {
                    tst[name].first.push_back("int*");
                }
                else if (it->vec[3].vec[0].r == "type INT") {
                    tst[name].first.push_back("int");
                }


                if (it->vec[5].vec[0].r == "type INT STAR") {
                    tst[name].first.push_back("int*");
                }
                else if (it->vec[5].vec[0].r == "type INT") {
                    tst[name].first.push_back("int");
                }
                cout << "sw $1, 0($29)" << endl;
                cout << "sub $30, $30, $4" << endl;
                cout << "sw $2, -4($29)" << endl;
                cout << "sub $30, $30, $4" << endl;

            }
            else {
                throw e;
            }
        }
        else if (it->r == "dcl type ID") {
            //insert wain parameter
            if (tst[name].second.find(it->vec[1].token[1]) == tst[name].second.end()) {
                if (it->vec[0].r == "type INT") {
                    tst[name].second.insert(make_pair(it->vec[1].token[1], make_pair("int", index)));
                }
                else
                    if (it->vec[0].r == "type INT STAR") {
                        tst[name].second.insert(make_pair(it->vec[1].token[1], make_pair("int*", index)));
                    }
                index -= 4;
            }
            else {
                throw e;

            }


        }
        else if (it->r == "paramlist dcl COMMA paramlist") {
            //insert parameter and variable
            if (it->vec[0].vec[0].r == "type INT STAR") {
                tst[name].first.push_back("int*");
            }
            else if (it->vec[0].vec[0].r == "type INT") {
                tst[name].first.push_back("int");
            }
        }if (it->r == "paramlist dcl") {
            //insert parameter and variable
            if (it->vec[0].vec[0].r == "type INT STAR") {
                tst[name].first.push_back("int*");
            }
            else if (it->vec[0].vec[0].r == "type INT") {
                tst[name].first.push_back("int");
            }
        }
        bst(*it, name);
    }

}


//determine the type of pt
string typeOf(node pt, string name, int num) {
    if (pt.r == "test expr LT expr" || pt.r == "test expr GT expr" || pt.r == "test expr LE expr" || pt.r == "test expr GE expr" || pt.r == "test expr NE expr" || pt.r == "test expr EQ expr") {
        if (typeOf(pt.vec[0], name, num) == typeOf(pt.vec[2], name, num)) {
            return "int";
        }
        else {
            cerr << "ERROR7" << endl;
            return "whatever7";
        }
    }
    else  if (pt.token[0] == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
        if (typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else {
            cerr << "ERROR" << endl;
            return "a";
        }
    }
    else  if (pt.token[0] == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
        if (typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else {
            cerr << "ERROR" << endl;
            return "b";
        }
    }
    else  if (pt.token[0] == "NULL") {
        //type of this token
        return "int*";
    }
    else  if (pt.token[0] == "ID") {
        //type of this token

        return tst[name].second[pt.token[1]].first;

    }
    else if (pt.token[0] == "NUM") {
        //return typr
        return "int";

    }
    else if (pt.r == "dcl type ID") {
        if (pt.vec[0].vec.size() == 1) {
            return "int";
        }
        else {
            return "int*";
        }
    }
    else if (pt.r == "expr term") {
        //return term type
        return typeOf(pt.vec[0], name, num);
    }
    else if (pt.r == "term factor") {
        // retiurn factor type
        return typeOf(pt.vec[0], name, num);
    }
    else if (pt.r == "factor ID") {
        //retrun type
        return typeOf(pt.vec[0], name, num);
    }
    else if (pt.r == "factor NUM") {
        // return num
        return typeOf(pt.vec[0], name, num);
    }
    else if (pt.r == "lvalue ID") {
        return typeOf(pt.vec[0], name, num);
    }
    else if (pt.r == "lvalue LPAREN lvalue RPAREN") {
        return typeOf(pt.vec[1], name, num);
    }
    else if (pt.r == "factor LPAREN expr RPAREN") {
        return typeOf(pt.vec[1], name, num);
    }
    else if (pt.r == "test expr EQ expr") {
        if (typeOf(pt.vec[0], name, num) == typeOf(pt.vec[2], name, num)) {
            return typeOf(pt.vec[0], name, num);
        }
        else {//maybe int ot int*
            cerr << "ERROR6" << endl;
            return "whatever6";
        }
    }
    else if (pt.r == "factor AMP lvalue") {
        if (typeOf(pt.vec[1], name, num) == "int") {
            return "int*";
        }
        else {
            cerr << "ERROR2" << endl;
            return "whatever2";
        }
    }
    else if (pt.r == "factor NULL") {
        return "int*";

    }
    else if (pt.r == "lvalue STAR factor") {
        if (typeOf(pt.vec[1], name, num) == "int*") {
            return "int";
        }
        else {
            cerr << "ERROR1" << endl;
            return "whateverasa1";

        }

    }
    else if (pt.r == "factor STAR factor") {

        if (typeOf(pt.vec[1], name, num) == "int*") {
            return "int";
        }
        else {
            cerr << "ERROR1" << endl;
            return "whatever1as2";
        }
    }
    else if (pt.r == "dcls dcls dcl BECOMES NULL SEMI") {
        if (typeOf(pt.vec[1].vec[1], name, num) == "int*") {
            return typeOf(pt.vec[1], name, num);
        }
        else {
            cerr << "ERROR5" << endl;
            return "whatever5";
        }
    }
    else if (pt.r == "dcls dcls dcl BECOMES NUM SEMI") {
        if (typeOf(pt.vec[1].vec[1], name, num) == "int") {
            return typeOf(pt.vec[1], name, num);
        }
        else {
            cerr << "ERROR5" << endl;
            return "whatever5";
        }
    }
    else if (pt.r == "factor NEW INT LBRACK expr RBRACK") {
        if (typeOf(pt.vec[3], name, num) == "int") {
            return "int*";
        }
        else {
            cerr << "ERROR10" << endl;
            return "whatever11";
        }

    }
    else if (pt.r == "statement lvalue BECOMES expr SEMI") {
        if (typeOf(pt.vec[0], name, num) == typeOf(pt.vec[2], name, num)) {
            return typeOf(pt.vec[0], name, num);
        }
        else {
            cerr << "ERROR11" << endl;
            return "whatever12";
        }
    }
    else if (pt.r == "factor ID LPAREN arglist RPAREN") {
        return "int";
    }
    else if (pt.r == "arglist expr COMMA arglist") {
        if (f < tst[helpname].first.size() && tst[helpname].first[f] == typeOf(pt.vec[0], name, num)) {
            f++;
            return typeOf(pt.vec[2], name, num);
        }
        else {
            cerr << "ERROR113" << endl;
            return "whatever12s2";
        }
    }
    else if (pt.r == "arglist expr") {
        num--;
        if (f == (tst[helpname].first.size() - 1)) {
            if (num != 0) {
                cout << 1;
            }
            if (tst[helpname].first[f] == typeOf(pt.vec[0], name, num)) {
                return typeOf(pt.vec[0], name, num);
            }
            else {
                cout << helpname << f << endl;
                cerr << "ERROR111" << endl;
                return "whatever12ss2";
            }
        }
        else {
            cerr << "ERROR115" << endl;
            return "whatever12ss2";
        }
    }
    else if (pt.r == "factor ID LPAREN RPAREN") {
        return "int";
    }
    else if (pt.r == "expr expr PLUS term") {
        if (typeOf(pt.vec[0], name, num) == "int" && typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else if (typeOf(pt.vec[0], name, num) == "int*" && typeOf(pt.vec[2], name, num) == "int*") {
            cerr << "ERROR14" << endl;
            return "whatever1178911";
        }
        else {
            return "int*";
        }

    }
    else if (pt.r == "expr expr MINUS term") {
        if (typeOf(pt.vec[0], name, num) == "int" && typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else if (typeOf(pt.vec[0], name, num) == "int*" && typeOf(pt.vec[2], name, num) == "int*") {
            return "int";
        }
        else if (typeOf(pt.vec[0], name, num) == "int*" && typeOf(pt.vec[2], name, num) == "int") {
            return "int*";
        }
        else {
            cerr << "ERROR15" << endl;
            return "whatever1178911";
        }
    }
    else if (pt.r == "term term STAR factor") {
        if (typeOf(pt.vec[0], name, num) == "int" && typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else {
            cerr << "ERROR16" << endl;
            return "whatever1111s1";
        }
    }
    else if (pt.r == "term term PCT factor") {

        if (typeOf(pt.vec[0], name, num) == "int" && typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else {
            cerr << "ERROR16" << endl;
            return "whatever1111d1";
        }
    }
    else if (pt.r == "term term SLASH factor") {
        if (typeOf(pt.vec[0], name, num) == "int" && typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else {
            cerr << "ERROR16" << endl;
            return "whatever111s11";
        }
    }
    else if (pt.r == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        //return is int
        if (typeOf(pt.vec[9], name, num) != "int") {
            cerr << "ERROR3" << endl;
            return "whatever3";
        }
        else {
            return typeOf(pt.vec[9], name, num);
        }

    }
    else  if (pt.r == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {

        if (typeOf(pt.vec[11], name, num) != "int" || typeOf(pt.vec[5], name, num) != "int") {
            cerr << "ERROR4" << endl;
            return "whatever4";
        }
        else {
            return typeOf(pt.vec[11], name, num);
        }
    }
    else if (pt.r == "statement PRINTLN LPAREN expr RPAREN SEMI") {
        if (typeOf(pt.vec[2], name, num) != "int") {
            cerr << "ERROR8" << endl;
            return "whatever8";
        }
    }
    else if (pt.r == "statement DELETE LBRACK RBRACK expr SEMI") {
        if (typeOf(pt.vec[3], name, num) != "int*") {
            cerr << "ERROR9" << endl;
            return "whatever10";
        }

    }
    else  if (pt.token[0] == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
        if (typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else {
            cerr << "ERROR" << endl;
            return "a";
        }


    }
    else  if (pt.token[0] == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {

        if (typeOf(pt.vec[2], name, num) == "int") {
            return "int";
        }
        else {
            cerr << "ERROR" << endl;
            return "b";
        }


    }
    else {
        return "what";
    }

}



int ifnum = 0;
int loop = 0;

//pop register
void pop(int regist) {
    cout << "add $30, $30, $4" << endl;
    cout << "lw $" << regist << ", -4($30)" << endl;
}

//push register
void push(int regist) {
    cout << "sw $" << regist << ", -4($30)" << endl;
    cout << "sub $30, $30, $4" << endl;
}

//code register
void code(string s, string name) {
    cout << "lw $3, " << tst[name].second[s].second << "($29)" << endl;
}

//generate mipcode
void mipcode(node t, string name, int testnum, int loopnum) {
    if (t.r == "start BOF procedures EOF" || t.r == "factor LPAREN expr RPAREN") {
        mipcode(t.vec[1], name, testnum, loopnum);
    }
    else if (t.r == "expr term" || t.r == "term factor" || t.r == "procedures main" || t.r == "factor ID" || t.r == "factor NUM") {
        mipcode(t.vec[0], name, testnum, loopnum);
    }
    else if (t.r == "statements statements statement") {

        mipcode(t.vec[0], name, testnum, loopnum);
        mipcode(t.vec[1], name, testnum, loopnum);

    }
    else if (t.r == "dcls dcls dcl BECOMES NUM SEMI") {

        mipcode(t.vec[0], name, testnum, loopnum);
        cout << "lis $3" << endl << ".word " << t.vec[3].token[1] << endl;
        cout << "sw $3, " << tst[name].second[t.vec[1].vec[1].token[1]].second << "($29)" << endl;

    }
    else if (t.r == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {

        name = "wain";
        mipcode(t.vec[8], name, testnum, loopnum);
        mipcode(t.vec[9], name, testnum, loopnum);
        mipcode(t.vec[11], name, testnum, loopnum);
        cout << "add $30, $29, $4" << endl << "jr $31" << endl;

    }

    else if (t.r == "statement lvalue BECOMES expr SEMI") {
        mipcode(t.vec[2], name, testnum, loopnum);
        if (t.vec[0].r == "lvalue STAR factor") {
            mipcode(t.vec[0], name, testnum, loopnum);
            cout << "add $6, $3, $0" << endl;
            mipcode(t.vec[2], name, testnum, loopnum);
            cout << "sw $3, 0($6)" << endl;


        }
        else {
            cout << "sw $3, " << tst[name].second[t.vec[0].vec[0].token[1]].second << "($29)" << endl;
        }

    }
    else if (t.r == "expr expr PLUS term" || t.r == "expr expr MINUS term" || t.r == "term term STAR factor" || t.r == "term term SLASH factor" || t.r == "term term PCT factor") {

        mipcode(t.vec[0], name, testnum, loopnum);
        push(3);
        mipcode(t.vec[2], name, testnum, loopnum);
        pop(5);

        if (t.r == "expr expr PLUS term") {
            if (typeOf(t.vec[0], name, 0) == "int*" && typeOf(t.vec[2], name, 0) == "int")
            {
                cout << "mult $3, $4" << endl;
                cout << "mflo $3" << endl;
            }          
            else if (typeOf(t.vec[0], name, 0) == "int" && typeOf(t.vec[2], name, 0) == "int*")//int + int*
            {
                cout << "mult $5, $4" << endl;
                cout << "mflo $5" << endl;
            }

            cout << "add $3, $5, $3" << endl;
        }
        else if (t.r == "expr expr MINUS term") {
            if (typeOf(t.vec[0], name, 0) == "int*" && typeOf(t.vec[2], name, 0) == "int")
            {
                cout << "mult $3, $4" << endl;
                cout << "mflo $3" << endl;
                cout << "sub $3, $5, $3" << endl;
            }          
            else if (typeOf(t.vec[0], name, 0) == "int*" && typeOf(t.vec[2], name, 0) == "int*")//int* - int*
            {
                cout << "sub $3, $5, $3" << endl;
                cout << "div $3, $4" << endl;
                cout << "mflo $3" << endl;
            }
            else
            {
                cout << "sub $3, $5, $3" << endl;
            }
        }
        else if (t.r == "term term SLASH factor") {
            cout << "div $5, $3" << endl;
            cout << "mflo $3" << endl;
        }
        else if (t.r == "term term STAR factor") {
            cout << "mult $5, $3" << endl;
            cout << "mflo $3" << endl;
        }
        else if (t.r == "term term PCT factor") {
            cout << "div $5, $3" << endl;
            cout << "mfhi $3" << endl;
        }

    }
    else if (t.r == "statement PRINTLN LPAREN expr RPAREN SEMI") {
        mipcode(t.vec[2], name, testnum, loopnum);
        cout << "add $1, $3, $0" << endl;
        push(31);
        cout << "lis $5" << endl << ".word print" << endl << "jalr $5" << endl;
        pop(31);

    }
    else if (t.token[0] == "NUM") {
        cout << "lis $3" << endl << ".word " << t.token[1] << endl;
    }
    else if (t.token[0] == "ID") {
        code(t.token[1], name);
    }
    else if (t.r == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
        int a = loop;
        cout << "loop" << loop << ":" << endl;
        mipcode(t.vec[2], name, testnum, loopnum);
        cout << "beq $3, $0, endWhile" << loop << endl;
        loop++;
        mipcode(t.vec[5], name, testnum, loopnum);
        int b = loop;
        loop = a;
        cout << "beq $0, $0, loop" << loop << endl;
        cout << "endWhile" << loop << ":" << endl;
        loop = b + 1;
    }
    else if (t.r == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
        int a = ifnum;
        mipcode(t.vec[2], name, testnum, loopnum);
        cout << "beq $3, $0, else" << ifnum << endl;
        ifnum++;
        mipcode(t.vec[5], name, testnum, loopnum);


        cout << "beq $0, $0, endif" << a << endl;

        cout << "else" << a << ":" << endl;
        ifnum++;
        mipcode(t.vec[9], name, testnum, loopnum);

        int b = ifnum;
        ifnum = a;
        cout << "endif" << ifnum << ":" << endl;

        ifnum = b + 1;
    }
    else if (t.token[0] == "test") {

        mipcode(t.vec[0], name, testnum, loopnum);
        push(3);
        mipcode(t.vec[2], name, testnum, loopnum);
        pop(5);

        if (t.r == "test expr LT expr") {
            if (typeOf(t.vec[0], name, 0) == "int")
            {
                cout << "slt $3, $5, $3" << endl;
            }
            else if (typeOf(t.vec[0], name, 0) == "int*")
            {

                cout << "sltu $3, $5, $3" << endl;
            }

        }
        else if (t.r == "test expr GT expr") {
            if (typeOf(t.vec[0], name, 0) == "int")
            {

                cout << "slt $3, $3, $5" << endl;
            }
            else if (typeOf(t.vec[0], name, 0) == "int*")
            {

                cout << "sltu $3, $3, $5" << endl;
            }


        }
        else if (t.r == "test expr NE expr") {
            if (typeOf(t.vec[0], name, 0) == "int")
            {
                cout << "slt $6, $3, $5; $6 = $3 < $5" << endl;
                cout << "slt $7, $5, $3; $7 = $5 < $3" << endl;
            }
            else if (typeOf(t.vec[0], name, 0) == "int*")
            {
                cout << "sltu $6, $3, $5; $6 = $3 < $5" << endl;
                cout << "sltu $7, $5, $3; $7 = $5 < $3" << endl;
            }

            cout << "add $3, $6, $7" << endl;
        }
        else if (t.r == "test expr EQ expr") {
            if (typeOf(t.vec[0], name, 0) == "int")
            {
                cout << "slt $6, $3, $5; $6 = $3 < $5" << endl;
                cout << "slt $7, $5, $3; $7 = $5 < $3" << endl;
            }
            else if (typeOf(t.vec[0], name, 0) == "int*")
            {
                cout << "sltu $6, $3, $5; $6 = $3 < $5" << endl;
                cout << "sltu $7, $5, $3; $7 = $5 < $3" << endl;
            }

            cout << "add $3, $6, $7" << endl;
            cout << "sub $3, $11, $3" << endl;
        }
        else if (t.r == "test expr GE expr") {
            if (typeOf(t.vec[0], name, 0) == "int")
            {
                cout << "slt $3, $5, $3" << endl;
            }
            else if (typeOf(t.vec[0], name, 0) == "int*")
            {
                cout << "sltu $3, $5, $3" << endl;
            }

            cout << "sub $3, $11, $3" << endl;
        }
        else if (t.r == "test expr LE expr") {
            if (typeOf(t.vec[0], name, 0) == "int")
            {
                cout << "slt $3, $3, $5" << endl;
            }
            else if (typeOf(t.vec[0], name, 0) == "int*")
            {
                cout << "sltu $3, $3, $5" << endl;
            }
            cout << "sub $3, $11, $3" << endl;
        }

    }

    else if (t.r == "dcls dcls dcl BECOMES NULL SEMI") {

        mipcode(t.vec[0], name, testnum, loopnum);
        cout << "add $3, $11, $0" << endl;
        cout << "sw $3, " << tst[name].second[t.vec[1].vec[1].token[1]].second << "($29)" << endl;


    }
    else if (t.r == "factor NULL") {

        cout << "add $3, $0, $11" << endl;

    }
    else if (t.r == "factor STAR factor") {

        mipcode(t.vec[1], name, testnum, loopnum);
        cout << "lw $3, 0($3)" << endl;

    }
    else if (t.r == "factor AMP lvalue") {

        mipcode(t.vec[1], name, testnum, loopnum);

    }
    else if (t.r == "lvalue STAR factor") {

        mipcode(t.vec[1], name, testnum, loopnum);

    }
    else if (t.r == "lvalue ID") {

        cout << "lis $3" << endl;
        cout << ".word " << tst[name].second[t.vec[0].token[1]].second << endl;
        cout << "add $3, $3, $29" << endl;

    }
    else if (t.r == "factor NEW INT LBRACK expr RBRACK") {
        mipcode(t.vec[3], name, testnum, loopnum);
        cout << "add $1, $3, $0" << endl;
        push(31);
        cout << "lis $5" << endl << ".word new" << endl << "jalr $5" << endl;
        pop(31);
        cout << "bne $3, $0, 1" << endl;
        cout << "add $3, $11, $0" << endl;

    }
    else if (t.r == "statement DELETE LBRACK RBRACK expr SEMI") {
        int a = delnum;
        delnum++;
        mipcode(t.vec[3], name, testnum, loopnum);
        cout << "beq $3, $11, skipDelete" << a << endl;
        cout << "add $1, $3, $0" << endl;
        push(31);
        cout << "lis $5" << endl << ".word delete" << endl << "jalr $5" << endl;
        pop(31);
        cout << "skipDelete" << a << ":" << endl;

    }
    else if (t.r == "factor ID LPAREN RPAREN")
    {
        procname = t.vec[0].token[1];
        //Push $29 and 31
        push(29);
        push(31);
        //Call procedure
        cout << "lis $5" << endl;
        cout << ".word " << procname << endl;
        cout << "jalr $5" << endl;
        //pop $31 and $29
        pop(31);
        pop(29);
    }
    else if (t.r == "statements statements statement") {
        mipcode(t.vec[0], name, testnum, loopnum);
        mipcode(t.vec[1], name, testnum, loopnum);

    }
    else if (t.r == "procedures procedure procedures") {
        mipcode(t.vec[1], name, testnum, loopnum);
        mipcode(t.vec[0], name, testnum, loopnum);
    }
    else if (t.r == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        procname = t.vec[1].token[1];
        cout << t.vec[1].token[1] << ":" << endl;
        cout << "sub $29, $30, $4" << endl;
        mipcode(t.vec[6], name, testnum, loopnum);
        push(5);
        push(6);
        push(7);
        mipcode(t.vec[7], name, testnum, loopnum);
        mipcode(t.vec[9], name, testnum, loopnum);
        pop(7);
        pop(6);
        pop(5);
        cout << "add $29, $30, $4" << endl;
        cout << "jr $31" << endl;
    }
}


int main() {
    try {
        //import the thing we need
        cout << ".import init" << endl;
        cout << ".import print" << endl;
        cout << ".import new" << endl;
        cout << ".import delete" << endl;
        cout << "lis $4" << endl << ".word 4" << endl;
        cout << "lis $11" << endl << ".word 1" << endl << "sub $29, $30, $4" << endl;
        int e = 110;
        string name = "a";
        node pt;
        string ter[35];
        fstream c;
        c.open("ter.txt");
        string temp;
        for (int i = 0; i < 35; i++)
        {
            c >> temp;
            ter[i] = temp;
        }
        c.close();
        pt = build(ter);
        bst(pt, name);
        for (int i = 0; i < (tst["wain"].second.size() - 2); i++) {
            cout << "sub $30, $30, $4" << endl;
        }
        if (tst["wain"].first.front() == "int") {
            cout << "add $2, $0, $0" << endl;
        }
        push(31);
        cout << "lis $5" << endl << ".word init" << endl << "jalr $5" << endl;
        pop(31);
        //get asm
        mipcode(pt, "whatever", 0, 0);
    }
    catch (int e) {
        cerr << "ERROR" << endl;
    }

}
