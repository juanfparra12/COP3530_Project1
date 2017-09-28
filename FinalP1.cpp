#include <iostream>
#include <string>
#include <cmath>
#include <cctype>
#include <unordered_map>
using namespace std;

const double Pi = 3.14169;
const double e = 2.718;


//Creates singular linkedList to erase/insert/traverse values from the opsStack class
class Node
{
    public:
        int orderOfPrecedence;
        char value;
        Node *next;

};

//Make a opsStack class to store operators later on
class opsStack
{
    public:
        Node *head;
        void eraseOps();
        void insertOps(char op, int precedence);
};

//erases the node
void opsStack::eraseOps()
{
    if (head != NULL)
    {
        Node *x = head;
        head = head->next;
        delete x;
    }
}

//inserts the operators and their respective precedences(3 highest - 1 lowest)
void opsStack::insertOps(char op, int precedence)
{
    Node *x = new Node;
    x->value = op;
    x->orderOfPrecedence = precedence;
    x->next = head;
    head = x;
}

//Creates singular linkedList to erase/insert/traverse values from the numLetterStack class
//This Linkedlist is for the double values
class numLetterNode
{
    public:
        double value;
        numLetterNode *next;
};

//Make a numLetterStack class to store double numbers later on
class numLetterStack
{
    public:
        numLetterNode *head;
        void eraseNumsLetter();
        void insertNumsLetter(double num);
};

//erases the node
void numLetterStack::eraseNumsLetter()
{
    if (head != NULL)
    {
        numLetterNode *x = head;
        head = head->next;
        delete x;
    }
}

//inserts the double numbers
void numLetterStack::insertNumsLetter(double number)
{
    numLetterNode *x = new numLetterNode;
    x->value = number;
    x->next = head;
    head = x;
}

//Specifiying the function of each operator
double specifyOps(double num1, double num2, char op)
{
    if (op == '^')
    {
        return pow(num2, num1);
    }
    if (op == '*')
    {
        return num2 * num1;
    }
    if (op == '/')
    {
        return num2 / num1;
    }
    if (op == '+')
    {
        return num2 + num1;
    }
    if (op == '-')
    {
        return num2 - num1;
    }

}

//function that evaluates the mathematical expressions of the user
string mathematicalExpressions(string userStringInput, unordered_map<string, double>  *x)
{
    //Instantiate the two stacks made previously
    opsStack *operatorStack = new opsStack();
    numLetterStack *numberLet = new numLetterStack();

    //For loop that will evaluate a string char by char until it reaches the string length.
    for (unsigned int i = 0; i < userStringInput.size(); i++)
    {
        //Used to remove any whitespace
        if (userStringInput[i] == ' ')
        {
            i++;
        }

        //test cases where char is a digits(0-9) or decimal point
        if (isdigit(userStringInput[i]) || userStringInput[i] == '.')
        {
            string numberString;
            numberString += userStringInput[i];

            //loops if the subsequent chars are still digits(0-9) or decimal point
            while (isdigit(userStringInput[i + 1]) || userStringInput[i + 1] == '.')
            {
                numberString += userStringInput[i + 1];
                i++;
            }

            //converts the string into a double and inserts it in the number stack
            double num = stod(numberString);
            numberLet->insertNumsLetter(num);
        }

        //test cases where char is a alphabetic letter(lower or uppercase)
        if (isalpha(userStringInput[i]))
        {
            string letter;
            letter += userStringInput[i];

            //loops if the subsequent chars are still alphabetic letters
            while (isalpha(userStringInput[i + 1]))
            {
                letter += userStringInput[i + 1];
                i++;
            }

            //Error handling to check whether the string was stored in the hashmap
            if (x->count(letter) == 0)
            {
                return "Undeclared-Variable";
            }

            //store string into number stack
            else
            {
                numberLet->insertNumsLetter(x->at(letter));
            }
        }

        //test cases where char is a exponent(^)
        if (userStringInput[i] == '^')
        {
            //insert ^ into operator stack and its precedence level
            operatorStack->insertOps('^', 3);
        }

        //test cases where char is a multiplier(*)
        if (userStringInput[i] == '*')
        {
            if (operatorStack->head != NULL)
            {
                //the operator precedence must be 2 or more if not order of operations
                //will mess up in the evaluation
                while (operatorStack->head->orderOfPrecedence >= 2)
                {
                    //creates new head in numberLet to later store the result
                    double num2 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    double num1 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    //creates new head in operatorStack to later store the new operator
                    //and precedence level
                    char op = operatorStack->head->value;
                    operatorStack->eraseOps();

                    //evalutes the result from specifyOps function
                    double result = specifyOps(num1, num2, op);
                    numberLet->insertNumsLetter(result);

                    if (operatorStack->head == NULL)
                    {
                        break;
                    }
                }
            }
            //insert * into operator stack and its precedence level
            operatorStack->insertOps('*', 2);
        }

        //test cases where char is a divisor(/)
        if (userStringInput[i] == '/')
        {
            if (operatorStack->head != NULL)
            {
                //the operator precedence must be 2 or more if not order of operations
                //will mess up in the evaluation
                while (operatorStack->head->orderOfPrecedence>= 2)
                {
                    //creates new head in numberLet to later store the result
                    double num1 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    double num2 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    //creates new head in operatorStack to later store the new operator
                    //and precedence level
                    char op = operatorStack->head->value;
                    operatorStack->eraseOps();

                    //When the denominator is not a zero
                    if (num1 != 0)
                    {
                         //evalutes the result from specifyOps function
                        double result = specifyOps(num1, num2, op);
                        numberLet->insertNumsLetter(result);

                        if (operatorStack->head == NULL)
                        {
                            break;
                        }
                    }

                    //Error handling for when the denominator is a zero
                    else
                    {
                        return "Division-By-Zero";
                    }
                }
            }

            //insert / into operator stack and its precedence level
            operatorStack->insertOps('/', 2);
        }

        //test cases where char is a addition(+)
        if (userStringInput[i] == '+')
        {
            if (operatorStack->head != NULL)
            {
                //the operator precedence must be 1 or more if not order of operations
                //will mess up in the evaluation.
                while (operatorStack->head->orderOfPrecedence >= 1)
                {
                    //creates new head in numberLet to later store the result
                    double num1 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    double num2 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    //creates new head in operatorStack to later store the new operator
                    //and precedence level
                    char op = operatorStack->head->value;
                    operatorStack->eraseOps();

                     //evalutes the result from specifyOps function
                    double result = specifyOps(num1, num2, op);
                    numberLet->insertNumsLetter(result);

                    if (operatorStack->head == NULL)
                    {
                        break;
                    }
                }
            }

            //insert + into operator stack and its precedence level
            operatorStack->insertOps('+', 1);
        }

        //test cases where char is a subtraction(-)
        if (userStringInput[i] == '-')
        {
            if (operatorStack->head != NULL)
            {
                //the operator precedence must be 1 or more if not order of operations
                //will mess up in the evaluation.
                while (operatorStack->head->orderOfPrecedence >= 1)
                {
                    //creates new head in numberLet to later store the result
                    double num1 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    double num2 = numberLet->head->value;
                    numberLet->eraseNumsLetter();

                    //creates new head in operatorStack to later store the new operator
                    //and precedence level
                    char op = operatorStack->head->value;
                    operatorStack->eraseOps();

                    //evalutes the result from specifyOps function
                    double result = specifyOps(num1, num2, op);
                    numberLet->insertNumsLetter(result);

                    if (operatorStack->head == NULL)
                    {
                        break;
                    }
                }
            }
            //insert - into operator stack and its precedence level
            operatorStack->insertOps('-', 1);
        }

        //test cases where char is a left parenthesis (
        if (userStringInput[i] == '(')
        {
            operatorStack->insertOps('(', 0);
        }

        //test cases where char is a right parenthesis )
        if (userStringInput[i] == ')')
        {
            //loops if there is no NEW left parentheis ( insertied in the operatorStack
            while (operatorStack->head->value != '(')
            {
                //creates new head in numberLet to later store the result
                double num1 = numberLet->head->value;
                numberLet->eraseNumsLetter();

                double num2 = numberLet->head->value;
                numberLet->eraseNumsLetter();

                //creates new head in operatorStack to later store the new operator
                //and precedence level
                char op = operatorStack->head->value;
                operatorStack->eraseOps();

                 //When the denominator is not a zero or the operator is not /
                if (num1 != 0 || op != '/')
                {
                     //evalutes the result from specifyOps function
                    double result = specifyOps(num1, num2, op);
                    numberLet->insertNumsLetter(result);
                }

                //Error handling for when the denominator is a zero
                else
                {
                    return "Division-By-Zero";
                }
            }
            //Erases head node of the operatorStack
            operatorStack->eraseOps();
        }


    }


    while (operatorStack->head != NULL)
    {
        //creates new head in numberLet to later store the result
        double num1 = numberLet->head->value;
        numberLet->eraseNumsLetter();

        double num2 = numberLet->head->value;
        numberLet->eraseNumsLetter();

        //creates new head in operatorStack to later store the new operator
        //and precedence level
        char op = operatorStack->head->value;
        operatorStack->eraseOps();

        //When the denominator is not a zero or the operator is not /
        if (num1 != 0 || op != '/')
        {
            double d = specifyOps(num1, num2, op);
            numberLet->insertNumsLetter(d);
        }

        //Error handling for when the denominator is a zero
        else
        {
            return "Division-By-Zero";
        }
    }

    //creates a string out of head value in the numberLet
    return to_string(numberLet->head->value);
}


int main()
{
   //Instantiate a new unordered_map from the previous unordered_map
    unordered_map<string, double>  *x = new unordered_map<string, double>();

    bool terminateProgam = false;
    while (!terminateProgam)
    {
        string userInput;
        getline(cin, userInput);

        //Terminates the program if the user wishes to end
        if (userInput.substr(0, 4) == "quit")
        {
            terminateProgam = true;
            break;
        }

        //Used when defining variables
        if (userInput.substr(0, 3) == "let")
        {
            //i = 4 because chars 0-2 are taken by "let" and the
            //char 3 is a space or " "
            int i = 4;

            //loops if the subsequent chars are still alphabetic letters
            while (isalpha(userInput[i + 1]))
            {
                i++;
            }

            //the name of the user defined variable
            string userVariable = userInput.substr(4, i - 3);

            //loops until the = is reached in the input
            while (userInput[i] != '=')
            {
                i++;
            }

            //creates a string of the result of the user defined variable
            string userVariableResult = userInput.substr((i + 1), (userInput.size() - i - 1));

            //Ensures that the value of string userVariableResult is not alphabetic letter
            //has to be a number!
            if (!isalpha(mathematicalExpressions(userVariableResult, x)[0]))
            {
                //creates new string that stores the value of the userDefinedVariable
                string result = mathematicalExpressions(userVariableResult, x);

                 //stores string userVariable in the hashmap if not stored yet
                if (x->count(userVariable) == 0)
                {
                    //Since string and double are different types needed pair
                    //to place them in the hashmap
                    pair<string, double> finalVariable(userVariable, stod(result));
                    x->insert(finalVariable);
                }

                //if there is already a userVariable in the hashmap, make it equal the result
                //have to convert the string result into a number value
                else
                {
                    x->at(userVariable) = stod(result);
                }
            }

            //printing the value of the userVariable if the user just typed the userVariable name
            else
            {
				bool pastDecimal = false;
				string final = mathematicalExpressions(userInput, x);
				while((final.back() == '0' || final.back() == '.') && !pastDecimal)
				{
					if(final.back() == '.')
						pastDecimal = true;
					final.pop_back();

				}
                cout << final << endl;
            }
        }

        //evalute results for anything that does not involve user defined variable
        else
        {
			bool pastDecimal = false;
			string final = mathematicalExpressions(userInput, x);
			while((final.back() == '0' || final.back() == '.') && !pastDecimal)
			{
				if(final.back() == '.')
					pastDecimal = true;
				final.pop_back();
			}
            cout << final << endl;
        }
    }
    return 0;
}
