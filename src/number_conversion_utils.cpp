#include "number_conversion_utils.h"

#include <math.h>
using std::min;
using std::max;

// const string fixedWidthSpace = "\xe3\x80\x80"; // Full width space
const string fixedWidthSpace = "\x20"; // ASCII space
// const string fixedWidthSpace = "\x20\x20"; // 2 ASCII spaces

string intToString(int in)
{
	string out;
	bool negative = false;
	
	if (in < 0)
	{
		negative = true;
		in *= -1;
	}
	
	do
	{
		out = (char)((in % 10) + '0') + out;
		in /= 10;
	}
	while (in > 0);
	
	if (negative)
		out = "-" + out;
	
	return out;
}

string doubleToString(double in)
{
	long long a=in;
	long long b=(in-a)*10000000000;
	if (b<0)
		b*=-1;
	if (b%10==9)
		b+=1;
	while (b>0 && !(b%10))
		b/=10;
	return intToString(a)+(b ? "."+intToString(b) : "");
}

int stringToInt(string in)
{
	int out = 0;
	
	for (int i = 0; i < (int)in.size(); i++)
	{
		if (in[i] >= '0' && in[i] <= '9')
		{
			out = out * 10 + in[i] - '0';
		}
		else if (in[i] == '.')
			break;
	}
	
	if (in.size() > 0 && in[0] == '-')
		out *= -1;
	
	return out;
}

unsigned long long stringToUnsignedLongLong(string in)
{
	unsigned long long out = 0;
	
	for (int i = 0; i < (int)in.size(); i++)
	{
		if (in[i] >= '0' && in[i] <= '9')
		{
			out = out * 10 + in[i] - '0';
		}
		else if (in[i] == '.')
			break;
	}
	
	return out;
}

double stringToDouble(string in)
{
	double out = 0;
	int divider = 1;
	
	for (int i = 0; i < (int)in.size(); i++)
	{
		if (divider == 1)
		{
			if (in[i] >= '0' && in[i] <= '9')
				out = out * 10 + in[i] - '0';
			else if (in[i] == '.')
				divider = 10;
		}
		else
		{
			if (in[i] >= '0' && in[i] <= '9')
			{
				out += (double)(in[i] - '0') / divider;
				divider *= 10;
			}
		}
	}
	
	if (in.size() > 0 && in[0] == '-')
		out *= -1;
	
	return out;
}

const vector<string> verticalBarStrs = {fixedWidthSpace, "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"};
const vector<string> horizontalBarStrs = {fixedWidthSpace, "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█"};
const vector<string> pieChartStrs = {fixedWidthSpace, "◜", "◠", "◠", "○", "○", "◔", "◑", "◕", "●"};
const vector<string> dotStrs = {fixedWidthSpace, "◌", "○", "◎", "◉", "●"};
const vector<string> circleSpinnerStrs = {"◜", "◝", "◞", "	◟"};

string verticalBar(double val)
{
	int i=min(floor(val * verticalBarStrs.size()), (double)verticalBarStrs.size()-1);
	return verticalBarStrs[i];
}

string doubleAsPercent(double val)
{
	return
		(val >= 1 ? intToString(val) : " ") +
		(val * 10 >= 1 ? intToString((int)(val*10) % 10) : " ") +
		intToString((int)(val*100) % 10) +
		"%";
}

string horizontalBar(double val, int charWidth)
{
	string out;
	
	int before, after, index;
	
	before = (int)floor(charWidth * val);
	index = min(((charWidth * val) - before) * horizontalBarStrs.size(), (double)verticalBarStrs.size()-1);
	after = charWidth - before - 1;
	
	for (int i = 0; i < before; i++)
	{
		out += "█";
	}
	
	out += horizontalBarStrs[index];
	
	for (int i = 0; i < after; i++)
	{
		out += fixedWidthSpace;
	}
	
	return out;
}

string pieChart(double val)
{
	int i=min(floor(val * pieChartStrs.size()), (double)pieChartStrs.size()-1);
	return pieChartStrs[i];
}

string dot(double val)
{
	int i=min(floor(val * dotStrs.size()), (double)dotStrs.size()-1);
	return dotStrs[i];
}
