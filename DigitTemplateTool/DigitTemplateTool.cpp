// DigitTemplateTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <shellapi.h>
#include <conio.h>
#include <string>

using namespace tinyxml2;
using namespace std;

bool CopyFile(TCHAR* pTo, TCHAR* pFrom)
{
	SHFILEOPSTRUCT FileOp = { 0 };
	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	FileOp.pFrom = pFrom;
	FileOp.pTo = pTo;
	FileOp.wFunc = FO_COPY;
	return SHFileOperation(&FileOp) == 0;
}

int main()
{
	cout << "Please select the mode : 1.FileMode 2. ConsoleMode" << endl;
	int iMode = 0;
	while (iMode != 1 && iMode!= 2)
	{
		cin >> iMode;
		cin.ignore(1);
		if (iMode != 1 && iMode != 2)
		{
			cout << "Mode wrong!!! Please select again : 1.FileMode 2. ConsoleMode" << endl;
		}
		else if (iMode == 2)
		{
			cout << "Ff you want to delete one template, please input : !d" << endl;
		}
	}

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile("SCDTMList.xml");
	if (err == XML_SUCCESS)
	{
		if (!doc.NoChildren())
		{
			tinyxml2::XMLElement* element = doc.FirstChildElement("SC_DigitTemplateModelList");
			if (element != nullptr)
			{
				if (!element->NoChildren())
				{
					ofstream out;
					if (iMode == 1)
					{
						out.open(L"DigitTemplate.txt", ios::out | ios::trunc);
					}
					else if (iMode == 2)
					{
						CopyFile(_T("SCDTMListbak.xml\0"), _T("SCDTMList.xml\0"));
					}

					tinyxml2::XMLElement* childeElement = element->FirstChildElement();
					while (childeElement)
					{
						string V = childeElement->Attribute("V");
						if (V == "x")
						{
							int id = childeElement->IntAttribute("ID");
							int w = childeElement->IntAttribute("W");
							int h = childeElement->IntAttribute("H");
							string M = childeElement->Attribute("M");
							string::iterator itB = M.begin();
							string::iterator itE = M.end();
							for (string::iterator it = itB; it != itE; ++it)
							{
								if (*it == '0')
								{
									*it = ' ';
								}
							}

							if (iMode == 1)
							{
								out << "====================" << endl;
								out << "       ID = " << id << "         " << endl;
								out << "====================" << endl;
								for (int i = 0; i < h; i++)
								{
									string sub = M.substr(i*w, w);
									out << sub.c_str() << endl;;
								}
							}
							else if (iMode == 2)
							{
								cout << "====================" << endl;
								cout << "       ID = " << id << "         " << endl;
								cout << "====================" << endl;
								for (int i = 0; i < h; i++)
								{
									string sub = M.substr(i*w, w);
									cout << sub.c_str() << endl;;
								}
								cout << "This digit template is : " << endl;
								//char res[1024] = {0};
								//cin >> res;
								string line;
								getline(cin, line);
								if (line == "!d")
								{
									element->DeleteChild(childeElement);
								}
								else
								{
									childeElement->SetAttribute("V", line.c_str());
								}


								if (iMode == 2)
								{
									doc.SaveFile("SCDTMList.xml");
								}
							}

						}

						childeElement = childeElement->NextSiblingElement();
					}

					if (iMode == 2)
					{
						doc.SaveFile("SCDTMList.xml");
					}
				}
			}
		}
		cout << "There is no digit to recognise." << endl;
	}
	else
	{
		cout << "Cann't load SCDTMList.xml" << endl;
	}

	cout << "Please input any key to exit..." << endl;
	
	getch();

	return 0;
}


