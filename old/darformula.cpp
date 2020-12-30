//
// Created by dev on 2/27/2016.
//
#include "darformula.h"

using namespace std;
/*

*/
ElaroDARFormula::darformula::darformula() {
    _willRaiseMathematicalExceptions=false;

    srand((unsigned int) time(NULL));
}

/*darformula::FormulaTree::FormulaTree(unordered_map<string,double> * vars) {
    _vars=vars;
}

const string darformula::FormulaTree::toString() {
    return _root->toString();
}

darformula::FormulaTree::~FormulaTree() {
    delete _root;
}

double darformula::FormulaTree::calcValue() throw(UnxpctdVarException) {
    return _root->calcValue(_vars);
}*/

ElaroDARFormula::darformula::FormulaElement* ElaroDARFormula::darformula::makeFormula(ElaroDARFormula::FormulaTokens* formula) {
    stack<FormulaElement*> treeStack;
    for(std::vector<Token>::iterator it=formula->begin();it<formula->end();it++)
    {
        if(it->getLevel()==0)
        {
            char c = it->getElement()[0];
            if (c == '.' || isdigit(c))
            {
                treeStack.push(new SimpleElement(atof(it->getElement().c_str()), this));
            }
            else
            {
                treeStack.push(new SimpleElement(it->getElement(), this));
            }
        }
        else if(it->getLevel()<5)
        {
            FormulaElement *operand1, *operand2;
            operand2=treeStack.top();
			treeStack.pop();
            operand1=treeStack.top();
			treeStack.pop();
            treeStack.push((new BinaryElement(it->getElement(),operand1,operand2, this)));
        }
        else if(it->getLevel()==6)
        {
			if (it->getElement() == "sum" || it->getElement() == "mult")
			{
				FormulaElement * expression = treeStack.top();
				treeStack.pop();
				FormulaElement * limit = treeStack.top();
				treeStack.pop();
				FormulaElement * counter = treeStack.top();
				treeStack.pop();
				treeStack.push(new TernaryElement(it->getElement(), (SimpleElement *)counter, limit, expression, this));
			}else
			{
				FormulaElement *operand = treeStack.top();
				treeStack.pop();
				treeStack.push((new UnaryElement(it->getElement(), operand, this)));
			}
            

        }
        else if(it->getLevel()==5)
        {
            if(it->getElement()=="--")
            {
                FormulaElement *operand=treeStack.top();
				treeStack.pop();
                treeStack.push((new UnaryElement("-",operand,this)));

            }
        }

    }
	delete formula;
    return treeStack.top();
    
}


const string ElaroDARFormula::darformula::toString() {
    return _formula->toString();
}

ElaroDARFormula::darformula::~darformula() {
	delete _formula;
}

void ElaroDARFormula::darformula::putVar(string name,double value) {
   try{
       _variables.at(name)=value;
   }catch(out_of_range){
       _variables.emplace(name,value);
   }
}

double ElaroDARFormula::darformula::getVar(string name) {
    return _variables.at(name);
}

double ElaroDARFormula::darformula::calcValue() {
    return _formula->calcValue();
}

void ElaroDARFormula::darformula::enableMathExceptions(bool yes) {
    _willRaiseMathematicalExceptions=yes;
}

const vector<string> ElaroDARFormula::darformula::getAcceptedVars()
{
	return _acceptedVariableNames;
}

void ElaroDARFormula::darformula::setAcceptedVars(const string acceptedVars[],const int numberOfVars)
{
	for (int i = 0; i < numberOfVars; i++)
	{
		_acceptedVariableNames.push_back(acceptedVars[i]);
	}
}



void ElaroDARFormula::darformula::setFormula(string expression, string acceptedVars[], int numberOfVars) {
	this->setAcceptedVars(acceptedVars, numberOfVars);
    _formula = makeFormula(FormulaTokens::Tokenize(expression)->checkFormula()->checkVariables(_acceptedVariableNames)->makePostfix());
}

ElaroDARFormula::darformula::FormulaElement::~FormulaElement()
{
    
}

ElaroDARFormula::darformula::SimpleElement::SimpleElement(string var, darformula *belonging) {
    _var=var;
	_val = nan("NaN");
    _belongsTo=belonging;
}

ElaroDARFormula::darformula::SimpleElement::SimpleElement(double val, darformula *belonging) {
    _val=val;
    _var="";
    _belongsTo=belonging;
}

ElaroDARFormula::darformula::SimpleElement::~SimpleElement() {

}

const string ElaroDARFormula::darformula::SimpleElement::toString() {
    return isnan(_val)? _var : to_string(_val);
}

bool ElaroDARFormula::darformula::SimpleElement::isVariable() {
	return _var != "";
}

double ElaroDARFormula::darformula::SimpleElement::calcValue() {
    if(_var=="")
    {
        return _val;
    }
    else if(_var=="r")
    {
        return (double)rand()/(double)RAND_MAX;
    }
    else if(_var=="PI"||_var=="Pi"||_var=="pI"||_var=="pi")
    {
        return M_PI;
    }
    else if(_var=="E"||_var=="e")
    {
        return M_E;
    }
    else {
        try {
            return _belongsTo->_variables.at(_var);
        }catch (out_of_range oor)
        {
            throw UnxpctdVarException(_var+" is not initialized");
        }
    }
}


ElaroDARFormula::darformula::UnaryElement::UnaryElement(string op, darformula::FormulaElement *operand, darformula *belonging) {
    _operand=operand;
    _operator=op;
	_belongsTo = belonging;
}

ElaroDARFormula::darformula::UnaryElement::~UnaryElement() {
    delete _operand;
}

const string ElaroDARFormula::darformula::UnaryElement::toString() {
    return _operator+"("+_operand->toString()+")";
}

double ElaroDARFormula::darformula::UnaryElement::calcValue() {
    if(_operator=="-")
        return -_operand->calcValue();
    if(_operator=="sin")
        return sin(_operand->calcValue());
    if(_operator=="cos")
        return cos(_operand->calcValue());
    if(_operator=="sinh")
        return sinh(_operand->calcValue());
    if(_operator=="cosh")
        return cosh(_operand->calcValue());
    if(_operator=="tanh")
        return tanh(_operand->calcValue());
    if(_operator=="atan")
        return atan(_operand->calcValue());
    if(_operator=="abs")
        return abs(_operand->calcValue());
    if(_operator=="ceil")
        return ceil(_operand->calcValue());
    if(_operator=="floor")
        return floor(_operand->calcValue());
    else {
        double cache=_operand->calcValue();
        if (_operator =="sqrt")
        {
            if(cache < 0 && !_belongsTo->_willRaiseMathematicalExceptions)
                return 0;
            return sqrt(cache);
        }
        else if(_operator=="log")
        {
            if(cache <= 0 && !_belongsTo->_willRaiseMathematicalExceptions)
                return 0;
            return log10(cache);
        }
        else if(_operator=="ln")
        {
            if(cache <= 0 && !_belongsTo->_willRaiseMathematicalExceptions)
                return 0;
            return log(cache);
        }
        else if(_operator=="tan")
        {
            if(cos(cache)==0 && !_belongsTo->_willRaiseMathematicalExceptions)
                return 0;
            return tan(cache);
        }
        else if(_operator=="asin")
        {
            if((cache>1 || cache<-1) && !_belongsTo->_willRaiseMathematicalExceptions)
                return 0;
            return asin(cache);
        }
        else if(_operator=="acos")
        {
            if((cache>1 || cache<-1) && !_belongsTo->_willRaiseMathematicalExceptions)
                return 0;
            return acos(cache);
        }
        else throw runtime_error("Unexpected unary operator : "+this->toString());

    }
}


ElaroDARFormula::darformula::BinaryElement::BinaryElement(string op, darformula::FormulaElement *first,
                                         darformula::FormulaElement *second, darformula *belonging) {
	_operator = op;
	_op1 = first;
	_op2 = second;
	_belongsTo = belonging;
}

ElaroDARFormula::darformula::BinaryElement::~BinaryElement() {
    delete _op1;
    delete _op2;
}

const string ElaroDARFormula::darformula::BinaryElement::toString() {
    return "("+_op1->toString()+_operator+_op2->toString()+")";
}

double ElaroDARFormula::darformula::BinaryElement::calcValue() {
	if (_operator == "+")
	{
		return _op1->calcValue() + _op2->calcValue();
	}
	if (_operator == "-") {
		return _op1->calcValue() - _op2->calcValue();
	}
	if (_operator == "*")
	{
		double cache = _op1->calcValue();
		if (cache == 0)
			return 0;
		else
			return cache * _op2->calcValue();
	}
	if (_operator == "/")
	{
		double cache = _op2->calcValue();
		if (cache == 0 && !_belongsTo->_willRaiseMathematicalExceptions)
			return 0;
		else
			return _op1->calcValue() / cache;
	}
	if (_operator == "%")
	{
		double cache2 = _op2->calcValue();
		if (cache2 == 0 && !_belongsTo->_willRaiseMathematicalExceptions)
			return 0;
		else
		{
			return fmod(_op1->calcValue(), cache2);
		}
	}
	if (_operator == "^")
	{
		double cache = pow(_op1->calcValue(), _op2->calcValue());
		if (isnan(cache) && !_belongsTo->_willRaiseMathematicalExceptions)
		{
			return 0;
		}
		return cache;
	}
	if (_operator == "=")
		return (abs(_op1->calcValue() - _op2->calcValue()) < MARGIN_OF_ERROR) ? 1 : 0;
	if (_operator == "!=")
		return (abs(_op1->calcValue() - _op2->calcValue()) > MARGIN_OF_ERROR) ? 1 : 0;
	if (_operator == ">") {
		double cache1 = _op1->calcValue();
		double cache2 = _op2->calcValue();
		return (abs(cache1 - cache2) > MARGIN_OF_ERROR && cache1 > cache2) ? 1 : 0;
	}
	if (_operator == "<")
	{
		double cache1 = _op1->calcValue();
		double cache2 = _op2->calcValue();
		return (abs(cache1 - cache2) > MARGIN_OF_ERROR && cache1 < cache2) ? 1 : 0;
	}
	throw runtime_error("Unexpected binary operator : " + this->toString());
}

ElaroDARFormula::darformula::TernaryElement::TernaryElement(string op, SimpleElement * counter, 
                    FormulaElement * limit, FormulaElement * expression, darformula * belonging)
{
	_operator = op;
	_limit = limit;
	_expression = expression;
	if (counter->toString() == "r")
		throw UnxpctdVarException("Conflict between counting variable and the random variable");
	else if (!counter->isVariable())
		throw UnxpctdVarException("Assigned number instead of variable");
	_counter = counter;
	_belongsTo = belonging;
}

ElaroDARFormula::darformula::TernaryElement::~TernaryElement() {
	delete _expression;
	delete _limit;
	delete _counter;
}

const string ElaroDARFormula::darformula::TernaryElement::toString() {
	return _operator + "(" + _counter->toString() + "," + _limit->toString() + "," + _expression->toString() + ")";
}

double ElaroDARFormula::darformula::TernaryElement::calcValue() {
	double limit = _limit->calcValue();

	if (_operator == "sum") {
		double result = 0.0;
		for(double i = 1;i<=limit;i++)
		{
			_belongsTo->putVar(_counter->toString(), i);
			result += _expression->calcValue();
		}
		_belongsTo->_variables.erase(_counter->toString());
		return result;
	}
	else if (_operator == "mult") {
		double result = 1;
		for (double i = 1; i <= limit; i++)
		{
			_belongsTo->putVar(_counter->toString(), i);
			result *= _expression->calcValue();
		}
		_belongsTo->_variables.erase(_counter->toString());
		return result;
	}
	else
		throw runtime_error("Unexpected Ternary Operator : " + this->toString());
}