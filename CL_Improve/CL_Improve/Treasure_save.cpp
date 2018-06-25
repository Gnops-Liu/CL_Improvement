#include "tester.h"

void Treasure_save::TreasureBox::build(int depth)
{
	Treasure_box.SetSize(depth);
	for (int i = 0; i < depth; i++)
		Treasure_box.GetAt(i) = "";
	Ruby = "";
}

void Treasure_save::TreasureBox::destroy()
{
	Treasure_box.RemoveAll();
}

void Treasure_save::TreasureBox::resize(int depth)
{
	destroy();
	build(depth);
}

void Treasure_save::SaveTreasure::_int(char* label, int coin)
{
	tmp.Format("%d, ", coin);
	if (strstr(Treasure_box.GetAt(0), label) == NULL)
	{
		Treasure_index[label] = 0;
		Treasure_box.GetAt(Treasure_index[label]) += label;
		Treasure_box.GetAt(Treasure_index[label]) += ", ";
	}
	Treasure_index[label] ++;
	Treasure_box.GetAt(Treasure_index[label]) += tmp;
}

void Treasure_save::SaveTreasure::_double(char* label,double coin)
{
	tmp.Format("%lf, ", coin);
	if (strstr(Treasure_box.GetAt(0), label) == NULL)
	{
		Treasure_index[label] = 0;
		Treasure_box.GetAt(Treasure_index[label]) += label;
		Treasure_box.GetAt(Treasure_index[label]) += ", ";
	}
	Treasure_index[label]++;
	Treasure_box.GetAt(Treasure_index[label]) += tmp;
}

void Treasure_save::SaveTreasure::_string(char* label, char *coin)
{
	tmp.Format("%s, ", coin);
	if (strstr(Treasure_box.GetAt(0), label) == NULL)
	{
		Treasure_index[label] = 0;
		Treasure_box.GetAt(Treasure_index[label]) += label;
		Treasure_box.GetAt(Treasure_index[label]) += ", ";
	}
	Treasure_index[label]++;
	Treasure_box.GetAt(Treasure_index[label]) += tmp;
}

void Treasure_save::SaveTreasure::_PF(char* label, bool coin)
{
	if (coin)	tmp = "P, ";
	else		tmp = "F, ";
	if (strstr(Treasure_box.GetAt(0), label) == NULL)
	{
		Treasure_index[label] = 0;
		Treasure_box.GetAt(Treasure_index[label]) += label;
		Treasure_box.GetAt(Treasure_index[label]) += ", ";
	}
	Treasure_index[label]++;
	Treasure_box.GetAt(Treasure_index[label]) += tmp;
}

void Treasure_save::SaveTreasure::_hex(char* label, long coin)
{
	tmp.Format("0x%X,", coin);
	if (strstr(Treasure_box.GetAt(0), label) == NULL)
	{
		Treasure_index[label] = 0;
		Treasure_box.GetAt(Treasure_index[label]) += label;
		Treasure_box.GetAt(Treasure_index[label]) += ", ";
	}
	Treasure_index[label]++;
	Treasure_box.GetAt(Treasure_index[label]) += tmp;
}

bool Treasure_save::TreasureBox::dig(char* Path)
{
	Ruby = "";
	for (int i = 0; i < Treasure_box.GetSize(); i++)
	{
		if (Treasure_box.GetAt(i) != "")
		{
			Ruby += Treasure_box.GetAt(i);
			Ruby += "\n";
		}
	}
	fileout.open(Path, ios::out);
	if (!fileout)
	{
		output("file not open correctly!!");
		return 0;
	}
	fileout << Ruby << endl;
	fileout.close();
	return 1;
}

