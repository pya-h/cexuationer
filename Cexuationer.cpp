/***********************************************************************
*																	   *
*				Software Name : Equation							   *
*				Programmer    : Pedram Mpn							   *
*				Last Update   : 2012/12/29 Sat.	04 : 17 PM			   *
*                                                                      *
***********************************************************************/

/*headers :*/
	#include <iostream>
	#include <ctype.h>
	#include <stdlib.h>
	#include <math.h>
	#include "include/console.h"
/*end*/

/*defines :*/
	#define NoUnbeknoun 1
	#define ManyUnbeknouns 2
	#define NotEquation 3
	#define UnknounOperator 4
	#define IlligalOperatorPosition 5
	#define PowerEquation 6
	#define NotNatural 7
	#define HighDegree 8
	#define null NULL
/*end*/

/*usings :*/
	using std::cout;
	using std::cin;
	using std::endl;
/*end*/

/*structure :*/
	struct Part
	{
		char o;
		double num;
		char uk;
		Part *nxt;
		Part *pre;
	};
/*end*/

/*funtion prototypes :*/
	bool IsOperator(char);
	void Show();
	bool BakhshPazir(double,double);
	void DeletePart(Part *);
	void CopyData(Part *,Part *);
	void ObtainDegree();
	void CheckErrors();
	void DeleteSpaces(char*);
	int CheckEquation(char*);
	void ArrangeArray(char*);
	void SwapPositions(char*,int);
	void ExtractParts(char*);
	void CopyToStruct(Part*,char*,int,int);
	int CheckDegreeOfEquation();
	bool DeleteTrashes();
	bool Power();
	bool DeleteZeroExps();
	bool OmitVents();
	bool MorD();
	bool OmitAlphaVents();
	bool MorP();
	bool MovePartsToOtherSide();
	void LastStep();
	bool MoveAllToLeft();
	bool ArrangePositions();
	bool FindPairs(double,double,double,int&,int&);
	bool BreakUp();
	void LastStep2();
	void DoInequality(char nor,char asign,double fst = 0.0,double snd = 0.0);
/*end*/

/*global variable*/
	Part *sl = new Part;
	Part *sr = new Part;
	float EquationDegree = 1.0f;
	char inequality = null;
/*end*/

int main()
{
	char *exp = new char[100];
	for(int i = 0;i < 100;i++)
		exp[i] = '\0';
	cin.getline(exp,100,'\n');
	cout<< endl << "--------------------------------------------------------------------------------\n";
	sl->pre = sr->nxt = nullptr;
	DeleteSpaces(exp);
	try
	{
		int error = CheckEquation(exp);
		if(NoUnbeknoun == error)
			throw "This equation has no unbeknoun.";
		else if(ManyUnbeknouns == error)
			throw "This equation has more than one unbeknoun.";
		else if(NotEquation == error)
			throw "The expression you entered, is not a equation.";
		else if(UnknounOperator == error)
			throw "An unknoun operator has been found in your equation.";
		else if(IlligalOperatorPosition == error)
			throw "One of the operators is in the illigal position.";
		else if(PowerEquation == error)
			throw "This Equation is Power Equation and can't be calculate.";
	}
	catch(char *msg)
	{
		cout<<"\n\nAn error has been occured : \n\t"<<msg<<"\n\nPress Enter to exit...";
		getch();
		delete [] exp;
		exp = nullptr;
		return 0;
	}
	ArrangeArray(exp);
	ExtractParts(exp);
	delete [] exp;
	exp = nullptr;
	cout<<endl;
	Show();
	while(DeleteTrashes());
	while(OmitVents());
	while(Power());
	while(DeleteZeroExps());
	while(MorD());
	while(OmitAlphaVents());
	while(MorP());
	ObtainDegree();
	while(MovePartsToOtherSide());
	CheckErrors();
	if(EquationDegree == 1)
		LastStep();
	else if(EquationDegree == 2)
	{
		while(MoveAllToLeft());
		while(ArrangePositions());
		if(BreakUp() == false)
			LastStep2();
	}
	Part *t = sl;
	while(t != nullptr)
	{
		Part *t2 = t->nxt;
		t->nxt = t->pre = nullptr;
		delete t;
		t = t2;
	}
	cout<< endl << "--------------------------------------------------------------------------------\n";
	getch();
	return 0;
}

bool IsOperator(char t)
{
	if(t != '*' && t != '/' && t != '+' && t != '-' && t != '=' && t != '<' && t != '>' && t != '^' && t != '(' && t != ')')
		return false;
	return true;
}

void Show()
{
	Part *c = sl;
	while(c != nullptr)
	{
		if(c->uk == null)
			cout<<c->num;
		else if(c->num == -1)
			cout<<'-';
		else if(c->num != 1)
			cout<<c->num;
		if(c->uk != null)
			cout<<c->uk;
		if(c->o != null)
			cout<<' '<<c->o<<' ';
		c = c->nxt;
	}
	cout<<endl<<endl;
}

bool BakhshPazir(double a,double b)
{
	const double r = a > b ? a / b : b / a;
	if(r == static_cast<long long>(r) && a != 1.0 && b != 1.0)
		return true;
	return false;
}

void DeletePart(Part *t)
{
	t->nxt = t->pre = nullptr;
	delete t;
	t = nullptr;
}

void CopyData(Part *dst,Part *src)
{
	if(src != sl && src->pre->o != '^')
		dst->num = abs(src->num);
	else
		dst->num = src->num;
	dst->uk = src->uk;
}

void ObtainDegree()
{
	Part *c = sl;
	while(c != nullptr)
	{
		if(c->o == '^' && c->uk != null && c->nxt->num != EquationDegree)
			EquationDegree = static_cast<float>(c->nxt->num);
		c = c->nxt;
	}
}

void CheckErrors()
{
	try
	{
		unsigned int error = CheckDegreeOfEquation();
		if(NotNatural == error)
			throw "The Degree of the equation must be a Natural number.";
		else if(HighDegree == error)
			throw "The degree of the equation must be one or two.";
		else if(NoUnbeknoun == error)
			throw "This equation has no unbeknoun.";
	}
	catch(char *msg)
	{
		cout<<"\n\nAn error has been occured : \n\t"<<msg<<"\n\nPress Enter to exit...";
		getch();
		exit(0);
	}
}

void DeleteSpaces(char *str)
{
	for(int i = 0;str[i];i++)
	{
		if(str[i] == ' ')
		{
			for(int j = i + 1;j < (strlen(str) + 1);j++)
			{
				if(str[j] != ' ')
				{
					str[i] = str[j];
					str[j] = ' ';
					break;
				}
			}
		}
	}
	str[strlen(str) + 1] = '\0';
}

int CheckEquation(char *str)
{
	if(str[0] == '\0')
		return NotEquation;
	for(int i = 0;str[i];i++)
		if((str[i] == '=' || str[i] == '<' || str[i] == '>') && str[i+1] == '\0')
			return NotEquation;
	bool hasEqual = false;
	for(int i = 0;str[i];i++)
		if(str[i] == '=' || str[i] == '<' || str[i] == '>')
			hasEqual = true;
	if(hasEqual == false)
		return NotEquation;
	for(int i = 0;str[i];i++)
	{
		if(!isalpha(str[i]) && !isdigit(str[i]) && !IsOperator(str[i]) && str[i] != '.')
			return UnknounOperator;
	}
	int NumberOfUnbeknouns = 0;
	char u = null;
	for(int i = 0;str[i];i++)
	{
		if(isalpha(str[i]))
		{
			str[i] = toupper(str[i]);
			if(u == null)
			{
				u = str[i];
				NumberOfUnbeknouns++;
			}
			if(u != str[i])
				NumberOfUnbeknouns++;
		}
	}
	if(NumberOfUnbeknouns == 0)
		return NoUnbeknoun;
	else if(NumberOfUnbeknouns > 1)
		return ManyUnbeknouns;
	for(int i = 0;str[i];i++)
		if(IsOperator(str[i]) && str[i] != '.' && str[i] != '(' && str[i+1] == ')' 
			&& (str[i+1] == '*' || str[i+1] == '/' || str[i+1] == '=' || str[i+1] == '^' || str[i+1] == '<' || str[i+1] == '>'))
				return IlligalOperatorPosition;
	for(int i = 0;str[i];i++)
	{
		if(str[i] == '^')
		{
			int j;
			for(j = i + 1;isdigit(str[j]);j++);
			if(isalpha(str[j]))
				return PowerEquation;
		}
	}
	for(int i = 0;str[i];i++)
	{
		if(str[i] == '^' && str[i+1] == '\0')
		{
			str[i] = ' ';
			DeleteSpaces(str);
		}
	}
	return 0;
}

void ArrangeArray(char *str)
{
	for(int i = 0;str[i];i++)
	{
		if(str[i+1] == '(')
		{
			if(IsOperator(str[i]) && str[i] != '.')
			{
				str[strlen(str) + 1] = '\0';
				for(int len = strlen(str);len > i;len--)
					str[len] = str[len-1];
				str[i+1] = '1';
			}
		}
	}
	for(int i = 0;str[i];i++)
	{
		if(isdigit(str[i]) && i != 0  && isalpha(str[i-1]))
		{
			int ii = i + 1;
			if(!str[ii])
				break;
			while(isdigit(str[ii]))
				ii++;
			if(isalpha(str[ii]))
			{
				str[strlen(str) + 1] = '\0';
				for(int len = strlen(str);len > i;len--)
					str[len] = str[len-1];
				str[i] = '*';
			}
		}
	}
	for(int i = 0;str[i];i++)
		if(isalpha(str[i]) && isdigit(str[i+1]))
			SwapPositions(str,i);
	if(str[0] == '+')
		str[0] = ' ';
	if(IsOperator(str[strlen(str) - 1]))
		str[strlen(str) - 1] = ' ';
	for(int i = 0;str[i];i++)
	{
		if(str[i] == '.' && !isdigit(str[i+1]))
			str[i] = ' ';
	}
	for(int i = 0;str[i];i++)
	{
		if(IsOperator(str[i]) && str[i+1] == '+')
			str[i+1] = ' ';
		else if(str[i] == '+' && str[i+1] == '-')
		{
			str[i] = '-';
			str[i+1] = ' ';
		}
	}
	DeleteSpaces(str);
}

void SwapPositions(char *str,int m)
{
	if(m != 0 && isdigit(str[m-1]))
	{
		str[strlen(str) + 1] = '\0';
		for(int len = strlen(str);len > m;len--)
			str[len] = str[len-1];
		str[m] = '*';
		m += 2;
	}
	else
		m++;
	for(;str[m] && isdigit(str[m]);m++)
	{
		char t = str[m-1];
		str[m-1] = str[m];
		str[m] = t;
	}
}

void ExtractParts(char *str)
{
	Part *c = sl;
	for(int i = 0;str[i];i++)
	{
		int j;
		for(j = i + 1;str[j] && !IsOperator(str[j]);j++);
		CopyToStruct(c,str,i,j);
		if(c->o != '=' && c->o != '<' && c->o != '>' && c->o != null)
		{
			c->nxt = new Part;
			c->nxt->pre = c;
			c->nxt->nxt = nullptr;
			c = c->nxt;
		}
		else
			c->nxt = nullptr;
		if(str[j] == '=' || str[j] == '<' || str[j] == '>')
		{
			c->nxt = sr;
			sr->pre = c;
			c = sr;
		}
		i = j;
	}
	c->o = null;
}

void CopyToStruct(Part *c,char *str,int s,int e)
{
	double num = 0.0;
	bool fstIsNeg = false;
	if(str[s] == '-')
	{
		fstIsNeg = true;
		s++;
	}
	if(isalpha(str[s]))
	{
		num = fstIsNeg == false ? 1.0 : -1.0;
	}
	else
	{

		double ten = 1.0;
		int i;
		for(i = s + 1;i < e && !isalpha(str[i]) && str[i] != '.';i++)
			ten *= 10.0;
		int i2 = s;
		do
		{
			num += static_cast<double>((str[i2] - 48) * ten);
			i2++;
		}while((ten /= 10) >= 1);
		if(str[i] == '.')
		{
			ten = 1.0;
			int ii;
			for(ii = i + 1;ii < e && !isalpha(str[ii]);ii++)
				ten *= 0.1;
			i2 = ii - 1;
			do
			{
				num += static_cast<double>((str[i2] - 48) * ten);
				i2--;
			}while((ten *= 10) < 1);
		}
		if(fstIsNeg == true)
			num *= -1;
	}
	c->num = num;
	c->o = str[e];IsOperator(str[e]) ? str[e] : null;
	c->uk = isalpha(str[e-1]) ? str[e-1] : null;
}

int CheckDegreeOfEquation()
{
	Part *c = sl;
	while(c != nullptr)
	{
		if(c->uk != null)
			break;
		c = c->nxt;
	}
	if(c == nullptr)
		return NoUnbeknoun;
	ObtainDegree();
	if(EquationDegree != static_cast<int>(EquationDegree) || EquationDegree < 0)
		return NotNatural;
	if(EquationDegree > 2.0f)
		return HighDegree;
	if(EquationDegree == 2.0f && sr->pre->o != '=')
	{
		inequality = sr->pre->o;
		sr->pre->o = '=';
		cout<< endl <<"--------------------------------------------------------------------------------\n";
		Show();
	}
	return 0;
}

bool DeleteTrashes()
{
	Part *c = sl;
	while(c != nullptr)
	{
		if(c->num == 0.0 && c->uk != null)
		{
			if(c->o == '^')
			{
				Part *t = c->nxt;
				c->o = t->o;
				c->nxt = c->nxt->nxt;
				if(c->nxt != nullptr)
					c->nxt->pre = c;
				DeletePart(t);
				t = nullptr;
			}
			c->uk = null;
		}
		c = c->nxt;
	}
	c = sl;
	while(c != nullptr)
	{
		if(c->o == '^' && c->nxt->num == 1)
		{
			c = c->nxt;
			c->pre->o = c->o;
			c->pre->nxt = c->nxt;
			if(c->nxt != nullptr)
				c->nxt->pre = c->pre;
			DeletePart(c);
			return true;
		}
		c = c->nxt;
	}
	c = sl;
	while(c != nullptr)
	{
		if(c->num == 0 && c->uk != null && c->o == '^')
		{
			if(c->nxt->nxt != sr && c->nxt->nxt != nullptr)
			{

					if(c != sr && c != sl)
						c->pre->o = c->nxt->o;
					else
					{
						if(c->nxt->o == '-')
							c->nxt->nxt->num *= -1;
					}
					if(c != sl)
						c->pre->nxt = c->nxt->nxt;
				if(sl == c)
					sl = c->nxt->nxt;
				else if(sr == c)
					sr = c->nxt->nxt;
				if(c->nxt->nxt != nullptr)
					c->nxt->nxt->pre = c->pre;
				DeletePart(c->nxt);
				DeletePart(c);
			}
			else
			{
				Part *t = c->nxt;
				c->o = c->nxt->o;
				c->uk = null;
				if(c->nxt->nxt != nullptr)	
					c->nxt->nxt->pre = c;
				c->nxt = c->nxt->nxt;
				DeletePart(t);
				t = nullptr;
			}
			Show();
			return true;
		}
		c = c->nxt;
	}
	c = sl;
	while(c != nullptr)
	{
		if(sl->nxt != sr && ((c->num == 0.0 && c->uk != null) || 
			((c == sl || c->pre->o == '-' || c->pre->o == '+') && c != sr && 
				c->o != '*' && c->o != '/' && c->num == 0 && c->o != '^')))
		{
			if(c->nxt != nullptr)
				c->nxt->pre = c->pre;
			if(c->pre != nullptr)
			{
				c->pre->nxt = c->nxt;
				c->pre->o = c->o;
			}
			if(c == sl)
				sl = c->nxt;
			DeletePart(c);
			Show();
			return true;
		}
		c = c->nxt;
	}
	c = sl;
	while(c != nullptr)
	{
		if(c->uk != null)
			break;
		c = c->nxt;
	}
	if(c == nullptr)
	{
		cout<<"\n\nAn error has been occured :\n\tThis equation has no unbeknouns.\n\nPress Enter to exit...";
		getch();
		exit(0);
	}
	c = nullptr;
	return false;
}

bool Power()
{
	Part *c = sl;
	while(c != nullptr && (c->o != '^' || c->nxt->o == '^' || (c->uk != null && c->nxt->num != 0 && c->nxt->num != 1)))
		c = c->nxt;
	if(c != nullptr)
	{
		if(c->uk == null || c->nxt->num == 1 || c->nxt->num == 0)
		{
			if(c->nxt->num != 0)
				c->num = pow(c->num,c->nxt->num);
			else
				c->uk = null;
			c->o = c->nxt->o;
			Part *t = c->nxt;
			c->nxt = t->nxt;
			if(t->nxt != nullptr)
				t->nxt->pre = t->pre;
			DeletePart(t);
			Show();
			while(DeleteTrashes());
			return true;
		}
	}
	return false;
}

bool DeleteZeroExps()
{
	Part *c = sl;
	while(c != nullptr && c->num != 0.0)
		c = c->nxt;
	if(c == nullptr)
		return false;
	Part *t = c;
	if(t->o == '*' || (t != sl && t->pre->o == '*'))
	{
		while(t != sl && (t->pre->o == '*' || t->pre->o == '^'))
			t = t->pre;
		while(c->o == '*' || c->o == '^')
			c = c->nxt;
		t->num = 0.0;
		t->uk = null;
		t->o = c->o;
		t->nxt = c->nxt;
		if(c->nxt != nullptr)
			c->nxt->pre = t;
		while(c->pre != t)
			c = c->pre;
		Part *tt;
		while(c != t->nxt)
		{
			tt = c;
			c = c->nxt;
			DeletePart(tt);
		}
		Show();
		DeleteTrashes();
		return true;
	}
	return false;
}

bool OmitVents()
{
	Part *c = sl;
	while(c != sr && c->o != '/')
		c = c->nxt;
	if(c != sr)
	{
		if(c->nxt->o == '*')
		{
			if(BakhshPazir(c->nxt->num,c->nxt->nxt->num) == true)
			{
				if(c->nxt->num > c->nxt->nxt->num)
				{
					c->nxt->num /= c->nxt->nxt->num;
					c->nxt->nxt->num = 1.0;
				}
				else
				{
					c->nxt->nxt->num /= c->nxt->num;
					c->nxt->num = 1.0;
				}
				Show();
				return true;
			}
		}
		else if(c != sl && c->pre->o == '*')
		{
			if(BakhshPazir(c->pre->num,c->nxt->num) == true)
			{
				if(c->pre->num > c->nxt->num)
				{
					c->pre->num /= c->nxt->num;
					c->nxt->num = 1.0;
				}
				else
				{
					c->nxt->num /= c->pre->num;
					c->pre->num = 1.0;
				}
				Show();
				return true;
			}
			else if(c->nxt->o == '/' && BakhshPazir(c->pre->num,c->nxt->nxt->num) == true)
			{
				if(c->pre->num > c->nxt->nxt->num)
				{
					c->pre->num /= c->nxt->nxt->num;
					c->nxt->num = 1.0;
				}
				else
				{
					c->nxt->nxt->num /= c->pre->num;
					c->pre->num = 1.0;
				}
				Show();
				return true;
			}
		}
	}
	return false;
}

bool MorD()
{
	Part *c = sl;
	while(c != nullptr)
	{
		if(c->o == '*' || c->o == '/')
		{
			/*a / 0*/
				if(c->o == '/' && c->nxt->num == 0.0)
				{
					cout<<"\n\nAn error has been occured : \n\t";
					if(c != sl && c->pre->o == '^')
					{
						if(c->pre->num != 1.0)
							cout<<c->pre->num<<c->pre->uk<<" ^ ";
						else
							cout<<c->pre->uk<<" ^ ";
					}
					cout<<c->num;
					if(c->uk != null)
						cout<<c->uk<<' ';
					else
						cout<<' ';
					if(c->num == 0.0)
						cout<<"/ 0 : Is a dusky expression.";
					else
						cout<<"/ 0 : Is a undefined expression.";
					cout<<"\n\nPress Enter to exit...";
					getch();
					exit(0);
				}
			/*end a / 0*/
			if((c == sl || c->pre->o != '^') && c->uk == null && c->o == '/' && c->nxt->uk != null)
			{
				c = c->nxt;
				while(c->o == '^')
					c = c->nxt;
				c = c->nxt;
				continue;
			}
			if(c == sl || c == sr || c->pre->o != '^')
			{
				if(c->o == '*')
					c->num *= c->nxt->num;
				else
					c->num /= c->nxt->num;
				if((c->uk == null && c->nxt->uk == null) || (c->uk == null && c->nxt->uk != null) || 
					c->uk != null && c->nxt->uk == null)
				{
					c->uk = c->nxt->uk != null ? c->nxt->uk : c->uk;
					c->o = c->nxt->o;
					Part *t = c->nxt;
					c->nxt = t->nxt;
					if(t->nxt != nullptr)
						t->nxt->pre = c;
					DeletePart(t);
					while(DeleteTrashes());
					Show();
					while(OmitVents());
					return true;
				}
				else
				{
					if(c->o == '*')
					{
						c->o = '^';
						if(c->nxt->o == '^')
						{
							c->nxt->num = c->nxt->nxt->num + 1;
							c->nxt->uk = null;
							c->nxt->nxt->num = 0.0;
							c->nxt->o = '+';
						}
						else
						{
							c->nxt->num = 2;
							c->nxt->uk = null;
						}
					}
					else
					{	
						if(c->nxt->o == '^')
						{
							c->o = '^';
							c->nxt->num = 1 - c->nxt->nxt->num;
							c->nxt->uk = null;
							c->nxt->nxt->num = 0.0;
							c->nxt->o = '+';
						}
						else
						{
							c->o = '+';
							c->nxt->num = 0;
							c->uk = null;
							c->nxt->uk = null;
						}
					}
					if(DeleteTrashes() == false)
						Show();
					while(OmitVents());
					return true;
				}
			}
			else
			{
				if(c->o == '*')
					c->pre->num *= c->nxt->num;
				else
					c->pre->num /= c->nxt->num;
				if(c->nxt->uk != null)
				{
					if(c->nxt->o != '^')
					{
						if(c->o == '*')
							c->num++;
						else
							c->num--;
						c->o = c->nxt->o;
						Part *t = c->nxt;
						if(c->nxt->nxt != nullptr)
							c->nxt->nxt->pre = c;
						c->nxt = c->nxt->nxt;
						DeletePart(t);
					}
					else
					{
						bool equal = false;
						if(c->o == '*')
							c->num += c->nxt->nxt->num;
						else
						{
							if(c->num != c->nxt->nxt->num)
								c->num -= c->nxt->nxt->num;
							else
							{
								c->pre->uk = c->nxt->uk = null;
								equal = true;
							}
						}
						c->o = c->nxt->nxt->o;
						Part *t = c->nxt;
						if(t->nxt->nxt != nullptr)
							t->nxt->nxt->pre = c;
						c->nxt = t->nxt->nxt;
						DeletePart(t->nxt);
						DeletePart(t);
						if(equal == true)
						{
							c->pre->o = c->o;
							c->pre->nxt = c->nxt;
							if(c->nxt != nullptr)
								c->nxt->pre = c->pre;
							DeletePart(c);
						}
					}
				}
				else
				{
					Part *t = c->nxt;
					c->o = c->nxt->o;
					c->nxt = c->nxt->nxt;
					if(c->nxt != nullptr)
						c->nxt->pre = c;
					DeletePart(t);
					t = nullptr;
				}
				DeleteTrashes();
				Show();
				if(CheckDegreeOfEquation() == NoUnbeknoun)
				{
					cout<<"\n\nAn error has been occured : \n\tThis equation has no unbeknoun.\n\nPress Enter to exit...";
					getch();
					exit(0);
				}
				while(OmitVents());
				return true;
			}
		}
		c = c->nxt;
	}
	return false;
}

bool OmitAlphaVents()
{
	Part *c = sl;
	while(c != nullptr && (c->uk != null || c->o != '/' || c->nxt->uk == null))
		c = c->nxt;
	if(c == nullptr)
		return false;
	Part *t = new Part;
	t->pre = nullptr;
	t->nxt = sl;
	sl->pre = t;
	CopyData(t,c->nxt);
	if(c->nxt->o == '^')
	{
		Part *cc = new Part;
		cc->nxt = sl;
		t->nxt = cc;
		cc->pre = t;
		sl->pre = cc;
		t->o = '^';
		CopyData(cc,c->nxt->nxt);
	}
	if(t->o != '^')
		t->o = '(';
	else
		t->nxt->o = '(';
	sl = t;
	t = sr;
	while(t->nxt != nullptr)
		t = t->nxt;
	t->o = ')';
	Show();
	if(sl->o != '^')
		sl->o = '*';
	else
		sl->nxt->o = '*';
	t->o = null;
	t = sl;
	while(1)
	{
		while(t->o == '*' || t->o == '/' || t->o == '^')
			t = t->nxt;
		if(t->nxt == nullptr)
			break;
		Part *tt = new Part;
		tt->pre = t;
		tt->nxt	= t->nxt;
		t->nxt = tt;
		tt->nxt->pre = tt;
		CopyData(tt,sl);
		if(sl->o != '^')
			tt->o = '*';
		else
		{
			tt->o = '^';
			Part *tmp = new Part;
			tmp->pre = tt;
			tmp->nxt = tt->nxt;
			tt->nxt = tmp;
			tmp->nxt->pre = tmp;
			CopyData(tmp,sl->nxt);
			tmp->o = '*';
		}
		if(sr == tt->nxt || (tt->o == '^' && tt->nxt->nxt == sr))
			sr = tt;
		t = t->nxt;
	}
	Show();
	while(MorD());
	return true;
}

bool MorP()
{
	Part *c = sl;
	Part *sr2 = sr;
	while(c != nullptr)
	{
		Part *cc = c->nxt;
		while(cc != sr2)
		{
			if(c->o != '^')
			{
				if(cc->o != '^' && ((c->uk == cc->uk && (c == sl || c->pre->o != '^') && cc->pre->o != '^')
					|| c->num == 0 || cc->num == 0))
				{
					if(cc->pre->o == '+')
					{
						if(c == sl || c == sr || c->pre->o == '+')
							c->num += cc->num;
						else
							c->num = cc->num > c->num ? cc->num - c->num : c->num - cc->num;
					}
					else
					{
						if(c == sl || c == sr || c->pre->o == '+')
						{
							c->num = cc->num > c->num ? cc->num - c->num : c->num - cc->num;
							if(c != sl && c != sr)
								c->pre->o = '-';
							else
								c->num *= -1;
						}
						else if(c->pre->o == '-')
							c->num += cc->num;
					}
					cc->pre->o = cc->o;
					if(c->uk != cc->uk)
						c->uk = c->uk != null ? c->uk : cc->uk;
					cc->pre->nxt = cc->nxt;
					if(cc->nxt != nullptr)
						cc->nxt->pre = cc->pre;
					DeletePart(cc);
					Show();
					while(DeleteTrashes());
					return true;
				}
			}
			else
			{
				if(c->uk == cc->uk && cc->o == c->o && cc->nxt->num == c->nxt->num)
				{
					if(cc->pre->o == '+')
						c->num += cc->num;
					else
						c->num -= cc->num;
					cc->pre->o = cc->nxt->o;
					cc->pre->nxt = cc->nxt->nxt;
					if(cc->nxt->nxt != nullptr)
						cc->nxt->nxt->pre = cc->pre;
					DeletePart(cc->nxt);
					DeletePart(cc);
					Show();
					while(DeleteTrashes());
					return true;
				}
			}
			cc = cc->nxt;
		}
		c = c->nxt;
		if(c == sr)
			sr2 = nullptr;
	}
	return false;
}

bool MovePartsToOtherSide()
{
	Part *c = sr;
	if(sl->uk == null && sl->nxt == sr && sr->uk != null && sr->nxt == nullptr)
	{
		c = sl;
		sl->nxt = nullptr;
		sl->pre = sr;
		sr->nxt = sl;
		sr->pre = nullptr;
		sl = sr;
		sr = c;
		sl->o = '=';
		sr->o = null;
		Show();
		return false;
	}
	while(c != nullptr && c->uk == null)
		c = c->nxt;
	if(c != nullptr && c->uk != null)
	{
		Part *t;
		if(c->o == '^')
			t = c->nxt;
		else
			t = c;
		if(c == sr)
		{
			if(t->nxt == nullptr)
			{
				t->nxt = new Part;
				t->nxt->pre = t;
				t->nxt->nxt = nullptr;
				t->o = '+';
				t->nxt->num = 0;
				t->nxt->o = t->nxt->uk = null;
			}
			sr = t->nxt;
		}
		if(c != sl && c->pre->o == '-')
			c->num *= -1;
		if(c->pre->o != '=' && c->pre->o != '<' && c->pre->o != '>')
			c->pre->o = t->o;
		else if(t->o == '-')
			t->nxt->num *= -1;
		c->num *= -1;
		c->pre->nxt = t->nxt;
		if(t->nxt != nullptr)
			t->nxt->pre = c->pre;
		t->nxt = sl;
		c->pre = null;
		sl->pre = t;
		if(sl->num < 0)
		{
			t->o = '-';
			sl->num *= -1;
		}
		else
			t->o = '+';
		sl = c;
		Show();
		MorP();
		return true;
	}
	if(EquationDegree != 1)
		return false;
	c = sl;
	while((c->uk != null || (c != sl && c->pre->o == '^') || c->o == '^') && c != sr)
		c = c->nxt;
	if(c != sr && c->uk == null && c->num != 0)
	{
		Part *cc = sr;
		if(c->o == '-')
			c->nxt->num *= -1;
		if(c->pre == sl)
		{
			if(c->pre->o == '-')
				c->num *= -1;
			c->pre->o = c->o;
		}
		while(cc->nxt != nullptr)
			cc = cc->nxt;
		if(c == sl)
		{
			sl = c->nxt;
			if(c->nxt == sr)
			{
				c->pre = new Part;
				c->pre->nxt = c;
				c->pre->pre = nullptr;
				c->pre->num = 0;
				c->pre->uk = null;
				sl = c->pre;
			}
		}
		c->num *= -1;
		if(c->nxt == sr)
			c->pre->o = c->o;
		if(c->pre != nullptr)
			c->pre->nxt = c->nxt;
		c->o = null;
		c->nxt->pre = c->pre;
		c->nxt = nullptr;
		cc->nxt = c;
		c->pre = cc;
		cc->o = '+';
		if(c->num < 0)
		{
			c->num *= -1;
			cc->o = '-';
		}
		Show();
		MorP();
		return true;
	}
	return false;
}

void LastStep()
{
	if(sl->num != 1)
	{
		if((sl->o == '<' || sl->o == '>') && sl->num < 0)
		{
			if(sl->o == '<')
				sl->o = '>';
			else
				sl->o = '<';
		}
		sr->num /= sl->num;
		if(sr->num == -0)
			sr->num = 0;
		sl->num /= sl->num;
		Show();
	}
}

bool MoveAllToLeft()
{
	Part *c = sr;
	if(c->num != 0)
	{
		if(sl->nxt == sr && sr->nxt == nullptr)
		{
			c = sl;
			sl->nxt = nullptr;
			sl->pre = sr;
			sr->nxt = sl;
			sr->pre = nullptr;
			sl = sr;
			sr = c;
			sl->o = '=';
			sr->o = null;
			Show();
			return false;
		}
		if(c == sr)
		{
			if(c->nxt == nullptr)
			{
				c->nxt = new Part;
				c->nxt->pre = c;
				c->nxt->nxt = nullptr;
				c->o = '+';
				c->nxt->num = 0;
				c->nxt->o = null;
				c->nxt->uk = null;
			}
			sr = c->nxt;
		}
		if(c != sl && c->pre->o == '-')
			c->num *= -1;
		if(c->pre->o != '=' && c->pre->o != '<' && c->pre->o != '>')
			c->pre->o = c->o;
		else if(c->o == '-')
			c->nxt->num *= -1;
		c->num *= -1;
		c->pre->nxt = c->nxt;
		if(c->nxt != nullptr)
			c->nxt->pre = c->pre;
		c->nxt = sl;
		c->pre = null;
		sl->pre = c;
		if(sl->num < 0)
		{
			c->o = '-';
			sl->num *= -1;
		}
		else
			c->o = '+';
		sl = c;
		Show();
		MorP();
		return true;
	}
	return false;
}

bool ArrangePositions()
{
	Part *c = sl;
	if(sl->o != '^')
	{
		while(c != nullptr && c->o != '^')
			c = c->nxt;
		if(c != nullptr)
		{
			if(c->pre->o == '-')
				c->num *= -1;
			c->pre->o = c->nxt->o;
			c->pre->nxt = c->nxt->nxt;
			c->nxt->nxt->pre = c->pre;
			c->pre->pre = c->nxt;
			c->pre = nullptr;
			c->nxt->nxt = sl;
			if(sl->num < 0)
			{
				sl->num *= -1;
				c->nxt->o = '-';
			}
			else
				c->nxt->o = '+';
			sl = c;
			Show();
			return true;
		}
	}
	else if(sl->nxt->nxt->uk == null)
	{
		if(sl->nxt->nxt != sr && sl->nxt->nxt->nxt != sr)
		{
			c = sl->nxt->nxt->nxt;
			if(c->pre->o == '-')
				c->num *= -1;
			if(sl->nxt->o == '-')
				c->pre->num *= -1;
			c->pre->o = c->o;
			c->pre->nxt = c->nxt;
			c->nxt->pre = c->pre;
			c->pre = sl->nxt;
			sl->nxt->nxt = c;
			c->nxt = sr->pre;
			if(c->num < 0)
			{
				c->num *= -1;
				c->pre->o = '-';
			}
			else
				c->pre->o = '+';
			if(c->nxt->num < 0)
			{
				c->o = '-';
				c->nxt->num *= -1;
			}
			else
				c->o = '+';
			Show();
			return true;
		}
	}
	return false;
}

bool FindPairs(double a,double b,double c,int& i,int& j)
{
	i = j = -100;
	if((b / a) != static_cast<int>(b / a))
		return false;
	for(;i <= 100;i++)
	{
		for(j = i;j <= 100;j++)
		{
			if((i + j) == (b / a) && (i * j) == c)
				return true;
		}
	}
	return false;
}

bool BreakUp()
{
	double& a = sl->num;
	/*check (a+b)^2 */
		if(static_cast<long long>(pow(a,0.5)) == pow(a,0.5))
		{
			if(sl->nxt->nxt != sr->pre && sl->nxt->nxt != sr && sl->num > 0.0 && sl->nxt->nxt->o != '-')
			{
				double& b = sl->nxt->nxt->num;
				double& c = sr->pre->num;
				if(static_cast<long long>(pow(c,0.5)) == pow(c,0.5))
				{
					cout<< endl << "--------------------------------------------------------------------------------\n";
					if(b == (2 * pow(a,0.5) * pow(c,0.5)))
					{
						a = pow(a,0.5);
						c = pow(c,0.5);
						if(a != 1.0)
							cout<<'('<<a<<sl->uk<<' ';
						else
							cout<<'('<<sl->uk<<' ';
						if(sl->nxt->o == '-')
							cout<<"- ";
						else
							cout<<"+ ";
						cout<<c<<") ^ 2 = 0"<<endl<<endl;
						if(a != 1.0)
							cout<<a<<sl->uk<<' ';
						else
							cout<<sl->uk<<' ';
						if(sl->nxt->o == '-')
							cout<<"- ";
						else
							cout<<"+ ";
						cout<<c<<" = 0\n\n";
						if(sl->nxt->o == '-')
							c *= -1;
						if(a == 1.0)
							cout<<sl->uk<<" = "<<c * -1<<endl<<endl;
						else
						{
							cout<<a<<sl->uk<<" = "<<c * -1<<endl<<endl;
							cout<<sl->uk<<" = "<<(c * -1) / a<<endl<<endl;
						}
						if(inequality != null)
							DoInequality('1','+',(c * -1) / a);
						return true;
					}
				}
			}
		}
	/*end*/

	/*check (a-b)(a+b) */
		if(sl->nxt->o == '-' && sl->nxt->nxt == sr->pre)
		{
			double& b = sr->pre->num;
			if(static_cast<long long>(pow(a,0.5)) == pow(a,0.5) && static_cast<long long>(pow(b,0.5)) == pow(b,0.5))
			{
				cout<< endl << "--------------------------------------------------------------------------------\n";
				a = pow(a,0.5);
				b = pow(b,0.5);
				if(a != 1.0)
					cout<<'('<<a<<sl->uk<<' ';
				else
					cout<<'('<<sl->uk<<' ';
				cout<<"+ "<<b<<") (";
				if(a != 1.0)
					cout<<a<<sl->uk<<' ';
				else
					cout<<sl->uk<<' ';
				cout<<"- "<<b<<") = 0\n\n";
				cout<< endl << "--------------------------------------------------------------------------------\n";
				if(a != 1.0)
					cout<<a<<sl->uk<<' ';
				else
					cout<<sl->uk<<' ';
				cout<<"+ "<<b<<" = 0\n\n";
				if(a == 1.0)
					cout<<sl->uk<<" = "<<b * -1<<endl<<endl;
				else
				{
					cout<<a<<sl->uk<<" = "<<b * -1<<endl<<endl;
					cout<<sl->uk<<" = "<<(b * -1) / a<<endl<<endl;
				}
				cout<< endl << "--------------------------------------------------------------------------------\n";
				if(a != 1.0)
					cout<<a<<sl->uk<<' ';
				else
					cout<<sl->uk<<' ';
				cout<<"- "<<b<<" = 0\n\n";
				if(a == 1.0)
					cout<<sl->uk<<" = "<<b<<endl<<endl;
				else
				{
					cout<<a<<sl->uk<<" = "<<b<<endl<<endl;
					cout<<sl->uk<<" = "<<b / a<<endl<<endl;
				}
				cout<< endl << "--------------------------------------------------------------------------------\n";
				cout<<sl->uk<<"1 = "<<(b * -1) / a<<" , "<<sl->uk<<"2 = "<<b / a<<endl<<endl;
				if(inequality != null)
				{
					const double r1 = (b * -1) / a;
					const double r2 = b / a;
					DoInequality('2','+',r1 < r2 ? r1 : r2,r1 > r2 ? r1 : r2);
				}
				return true;
			}
		}
	/*end*/	

	/*check (b-a)(b+a) */
		if(sl->num < 0 && sl->nxt->o == '+' && sl->nxt->nxt == sr->pre)
		{
			a *= -1;
			double& b = sr->pre->num;
			if(static_cast<long long>(pow(a,0.5)) == pow(a,0.5) && static_cast<long long>(pow(b,0.5)) == pow(b,0.5))
			{
				cout<< endl << "--------------------------------------------------------------------------------\n";
				a = pow(a,0.5);
				b = pow(b,0.5);
				cout<<'('<<b<<" + ";
				if(a != 1.0)
					cout<<a<<sl->uk<<')';
				else
					cout<<sl->uk<<')';
				cout<<" ("<<b<<" - ";
				if(a != 1.0)
					cout<<a<<sl->uk<<')';
				else
					cout<<sl->uk<<')';
				cout<<" = 0\n\n";
				cout<< endl << "--------------------------------------------------------------------------------\n";
				cout<<b<<" + ";
				if(a != 1.0)
					cout<<a<<sl->uk;
				else
					cout<<sl->uk;
				cout<<" = 0\n\n";
				if(a == 1.0)
					cout<<sl->uk<<" = "<<b * -1<<endl<<endl;
				else
				{
					cout<<a<<sl->uk<<" = "<<b * -1<<endl<<endl;
					cout<<sl->uk<<" = "<<(b * -1) / a<<endl<<endl;
				}
				cout<< endl << "--------------------------------------------------------------------------------\n";
				cout<<b<<" - ";
				if(a != 1.0)
					cout<<a<<sl->uk;
				else
					cout<<sl->uk;
				cout<<" = 0\n\n";
				if(a == 1.0)
					cout<<'-'<<sl->uk<<" = "<<b * -1<<endl<<endl<<sl->uk<<" = "<<b<<endl<<endl;
				else
				{
					cout<<'-'<<a<<sl->uk<<" = "<<b * -1<<endl<<endl;
					cout<<sl->uk<<" = "<<b / a<<endl<<endl;
				}
				cout<< endl << "--------------------------------------------------------------------------------\n";
				cout<<sl->uk<<"1 = "<<(b * -1) / a<<" , "<<sl->uk<<"2 = "<<b / a<<endl<<endl;
				if(inequality != null)
				{
					const double r1 = (b * -1) / a;
					const double r2 = b / a;
					DoInequality('2','-',r1 < r2 ? r1 : r2,r1 > r2 ? r1 : r2);
				}
				return true;
			}
		}
	/*end*/	
	
	/*check (x+a)(x+b) */
		if(static_cast<long long>(pow(a,0.5)) == pow(a,0.5))
		{
			if(sl->nxt->nxt != sr->pre && sl->nxt->nxt != sr && sl->num > 0.0)
			{
				double& b = sl->nxt->nxt->num;
				double& c = sr->pre->num;
				if(static_cast<long long>(c) == c)
				{
					if(sl->nxt->o == '-')
						b *= -1;
					if(sr->pre->pre->o == '-')
						c *= -1;
					int i = 0,j = 0;
					a = pow(a,0.5);
					if(FindPairs(a,b,c,i,j) == false)
						return false;
					if(a != 1.0)
						cout<<'('<<a<<sl->uk<<' ';
					else
						cout<<'('<<sl->uk<<' ';
					if(i > 0)
						cout<<"+ ";
					else
						cout<<"- ";
					cout<<(i > 0 ? i : i * -1)<<") (";
					if(a != 1.0)
						cout<<a<<sl->uk<<' ';
					else
						cout<<sl->uk<<' ';
					if(j > 0)
						cout<<"+ ";
					else
						cout<<"- ";
					cout<<(j > 0 ? j : j * -1)<<") = 0\n\n";
					cout<< endl << "--------------------------------------------------------------------------------\n";
					if(a != 1.0)
						cout<<a<<sl->uk<<' ';
					else
						cout<<sl->uk<<' ';
					if(i > 0)
						cout<<"+ ";
					else
						cout<<"- ";
					cout<<(i > 0 ? i : i * -1)<<" = 0\n\n";
					if(a == 1.0)
						cout<<sl->uk<<" = "<<i * -1<<endl<<endl;
					else
					{
						cout<<a<<sl->uk<<" = "<<i * -1<<endl<<endl;
						cout<<sl->uk<<" = "<<(i * -1) / a<<endl<<endl;
					}
					cout<< endl << "--------------------------------------------------------------------------------\n";
					if(a != 1.0)
						cout<<a<<sl->uk<<' ';
					else
						cout<<sl->uk<<' ';
					if(j > 0)
						cout<<"+ ";
					else
						cout<<"- ";
					cout<<(j > 0 ? j : j * -1)<<" = 0\n\n";
					if(a == 1.0)
						cout<<sl->uk<<" = "<<j * -1<<endl<<endl;
					else
					{
						cout<<a<<sl->uk<<" = "<<j * -1<<endl<<endl;
						cout<<sl->uk<<" = "<<(j * -1) / a<<endl<<endl;
					}
					cout<< endl << "--------------------------------------------------------------------------------\n";
					cout<<sl->uk<<"1 = "<<(i * -1) / a<<" , "<<sl->uk<<"2 = "<<(j * -1) / a<<endl<<endl;
					if(inequality != null)
					{
						const double r1 = (i * -1) / a;
						const double r2 = (j * -1) / a;
						DoInequality('2','+',r1 < r2 ? r1 : r2,r1 > r2 ? r1 : r2);
					}
					return true;
				}
			}
		}
	/*end*/
	return false;
}

void LastStep2()
{
	cout<<"--------------------------------------------------------------------------------";
	double a = sl->num;
	double b = 0;
	double c = 0;
	Part *t = sl->nxt->nxt;
	while(t != sr)
	{
		if(t->uk != null)
			b = t->pre->o == '-' ? t->num * -1 : t->num;
		else
			c = t->pre->o == '-' ? t->num * -1 : t->num;
		t = t->nxt;
	}
	char uk = sl->uk;
	double delta = (b * b) - (4 * a * c);
	cout<<"\ndelta = (b ^ 2) - 4ac\ndelta =  ("<<b<<" ^ 2) - 4 * "<<a<<" * "<<c;
	cout<<"\ndelta = ("<<b * b<<") - ("<<4 * a * c<<")\ndelta = "<<delta<<endl<<endl;
	cout<<"--------------------------------------------------------------------------------" << endl;
	if(delta < 0)
	{
		cout<<"\n\n\tdelta is less than zero and this equation has no root\n\n";
		if(inequality != null)
			DoInequality('0',a > 0 ? '+' : '-');
		else
			cout<<"Press Enter to exit...\n\n";
		return ;
	}
	else if(delta == 0)
	{
		cout<<uk<<" = (-b + sqrt(delta)) / (2a)\n";
		cout<<uk<<" = (-("<<b<<") + sqrt("<<delta<<")) / (2 * "<<a<<")"<<endl;
		b *= -1;
		delta = pow(delta,0.5);
		a *= 2;
		cout<<uk<<" = ("<<b<<" + "<<delta<<") / "<<a<<endl;
		cout<<uk<<" = "<<b+delta<<" / "<<a<<" = "<<(b+delta != 0 ? (b+delta)/a : 0)<<endl<<endl;
		if(inequality != null)
			DoInequality('1',a > 0 ? '+' : '-',b+delta != 0 ? (b+delta)/a : 0);
	}
	else
	{
		double r1 = 0.0,r2 = 0.0;
		const double sndDe = delta;
		cout<<uk<<"1 = (-b + sqrt(delta)) / (2a)\n";
		cout<<uk<<"1 = (-("<<b<<") + sqrt("<<delta<<")) / (2 * "<<a<<")"<<endl;
		b *= -1;
		delta = pow(delta,0.5);
		a *= 2;
		cout<<uk<<"1 = ("<<b<<" + "<<delta<<") / "<<a<<endl;
		cout<<uk<<"1 = "<<b+delta<<" / "<<a<<" = "<<(b+delta != 0 ? (b+delta)/a : 0)<<endl;
		r1 = b+delta != 0 ? (b+delta)/a : 0;

		cout<< endl << "--------------------------------------------------------------------------------" << endl;
		b *= -1;
		a /= 2;
		cout<<uk<<"2 = (-b - sqrt(delta)) / (2a)\n";
		cout<<uk<<"2 = (-("<<b<<") - sqrt("<<sndDe<<")) / (2 * "<<a<<")"<<endl;
		b *= -1;
		a *= 2;
		cout<<uk<<"2 = ("<<b<<" - "<<delta<<") / "<<a<<endl;
		cout<<uk<<"2 = "<<b-delta<<" / "<<a<<" = "<<(b-delta != 0 ? (b-delta)/a : 0)<<endl;

		cout<< endl << "--------------------------------------------------------------------------------" << endl;
		cout<<uk<<"1 = "<<(b+delta != 0 ? (b+delta)/a : 0)<<","<<uk<<"2 = "<<(b-delta != 0 ? (b-delta)/a : 0)<<endl<<endl;
		r2 = b-delta != 0 ? (b-delta)/a : 0;
		if(inequality != null)
			DoInequality('2',a > 0 ? '+' : '-',r1 < r2 ? r1 : r2,r1 > r2 ? r1 : r2);
	}
}

void DoInequality(char nor,char asign,double fst,double snd)
{
	cout<< endl << "--------------------------------------------------------------------------------\n";
	if(nor == '0')
	{
		if(asign == '+')
		{
			if(inequality == '>')
				cout<<"Answers : (-oo , +oo)\tor\tR";
			else
				cout<<"Answers : { }";
		}
		else
		{
			if(inequality == '<')
				cout<<"Answers : (-oo , +oo)\tor\tR";
			else
				cout<<"Answers : { }";
		}
	}
	else if(nor == '1')
	{
		if(asign == '+')
		{
			if(inequality == '>')
				cout<<"Answers : (-oo , "<<fst<<") + ("<<fst<<" , +oo)\tor\tR - {"<<fst<<"}";
			else
				cout<<"Answers : { }";
		}
		else
		{
			if(inequality == '<')
				cout<<"Answers : (-oo , "<<fst<<") + ("<<fst<<" , +oo)\tor\tR - {"<<fst<<"}";
			else
				cout<<"Answers : { }";
		}
	}
	else
	{
		if(asign == '+')
		{
			if(inequality == '>')
				cout<<"Answers : (-oo , "<<fst<<") + ("<<snd<<" , +oo)\tor\tR - ["<<fst<<" , "<<snd<<"]";
			else
				cout<<"Answers : ("<<fst<<" , "<<snd<<")\tor\t{X | "<<fst<<" < X < "<<snd<<"}";
		}
		else
		{
			if(inequality == '<')
				cout<<"Answers : (-oo , "<<fst<<") + ("<<snd<<" , +oo)\tor\tR - ["<<fst<<" , "<<snd<<"]";
			else
				cout<<"Answers : ("<<fst<<" , "<<snd<<")\tor\t{X | "<<fst<<" < X < "<<snd<<"}";
		}
	}
	cout<<"\n\n";
}