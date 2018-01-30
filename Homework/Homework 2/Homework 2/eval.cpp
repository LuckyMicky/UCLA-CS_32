//
//  eval.cpp
//  Homework 2
//
//  Created by SHUDAXUAN on 2/7/17.
//  Copyright © 2017 SHUDAXUAN. All rights reserved.
//

#include <stack>
#include <string>
#include <cassert>
#include <iostream>

using namespace std;


int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    postfix = "";
    string nonspaceinfix = "";
    stack<char> s;
    for(int i = 0; i < infix.size(); i++)
    {
        if (infix[i] != ' ')
            nonspaceinfix += infix[i];
    }
    
    if (nonspaceinfix == "")
        return 1;
    if (nonspaceinfix[0] == '&' || nonspaceinfix[0] == '|')
        return 1;
    if (nonspaceinfix[nonspaceinfix.size() - 1] != ')' && !isdigit(nonspaceinfix[nonspaceinfix.size() - 1]))
        return 1;
    int numl = 0, numr = 0;
    for (int i = 0; i < nonspaceinfix.size(); i++)
    {
        
        char ch = nonspaceinfix[i];
        if (ch == '(')
            numl++;
        if (ch == ')')
            numr++;
        if (numl < numr)
            return 1;
    }
    if (numl != numr)
        return 1;
    for (int i = 0; i < nonspaceinfix.size() - 1; i++)
    {
        char ch = nonspaceinfix[i];
        if (ch == '&' || ch == '|')
            if (nonspaceinfix[i+1] != '(' && nonspaceinfix[i+1] != '!' && !isdigit(nonspaceinfix[i+1]))
                return 1;
        if (ch == '(')
            if (nonspaceinfix[i+1] != '!' && nonspaceinfix[i+1] != '(' && !isdigit(nonspaceinfix[i+1]))
                return 1;
    }
    for (int i = 0; i < infix.size(); i++)
    {
        switch (infix[i])
        {
            case '0': postfix += '0'; break;
            case '1': postfix += '1'; break;
            case '2': postfix += '2'; break;
            case '3': postfix += '3'; break;
            case '4': postfix += '4'; break;
            case '5': postfix += '5'; break;
            case '6': postfix += '6'; break;
            case '7': postfix += '7'; break;
            case '8': postfix += '8'; break;
            case '9': postfix += '9'; break;
            case '(': s.push('('); break;
            case ')':
                while (s.top() != '(') {
                    postfix += s.top();
                    s.pop();
                }
                s.pop();
                break;
            case '!':
                while (!s.empty() && s.top() != '(' && s.top() == '!'){
                    postfix += s.top();
                    s.pop();
                }
                      s.push('!');
                break;
            case '&':
                while (!s.empty() && s.top() != '(' && (s.top() == '!' || s.top() == '&')){
                    postfix += s.top();
                    s.pop();
                }
                s.push('&');
                break;

            case '|':
                while (!s.empty() && s.top() != '(' && (s.top() == '!' || s.top() == '&' || s.top() == '|')){
                    postfix += s.top();
                    s.pop();
                }
                s.push('|');
            break;
        }
    }
        while (!s.empty())
        {
            postfix += s.top();
            s.pop();
        }
        
        stack<bool> operand;
        bool temp_result;
        for (int i = 0; i < postfix.size(); i++)
        {
            if (isdigit(postfix[i]))
            {
                operand.push(values[postfix[i] - '0']);
            }
            else
            {
                if (postfix[i] == '!')
                {
                    bool operand1 = operand.top();
                    operand.pop();
                    temp_result = !operand1;
                    operand.push(temp_result);
                }
                else{
                bool operand2 = operand.top();
                operand.pop();
                bool operand1 = operand.top();
                operand.pop();
                if (postfix[i] == '&')
                    temp_result = operand1 && operand2;
                if (postfix[i] == '|')
                    temp_result = operand1 || operand2;
                operand.push(temp_result);
                }
            }
            
        }
    if (operand.size() != 1)
        return 1;
    result = operand.top();
    return 0;
}

int main()
{
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0 &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    cout << "Passed all tests" << endl;
    
}
