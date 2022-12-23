#include "Libraries.h"

namespace IN
{
	IN GetIn(wchar_t infile[])
	{
		ifstream inFile(infile);

		if (inFile.fail())
		{
			throw ERROR_THROW(110);
		}

		IN inStruct;
		inStruct.size = 0;
		inStruct.lines = 1;
		inStruct.ignor = 0;

		unsigned char tempChar;

		int positionInTheString = 0,
			line = 1;

		char tempWord[IN_WORD_MAX_LEN];
		short wordLength = 0;

		while ((!inFile.eof()) && (tempChar = inFile.get())) {
			if (wordLength >= IN_WORD_MAX_LEN)
			{
				throw ERROR_THROW_IN(114, line, positionInTheString);
			}

			if (tempChar == '"')
			{
				tempWord[wordLength++] = tempChar;
				do {
					if (wordLength >= IN_WORD_MAX_LEN)
					{
						throw ERROR_THROW_IN(114, line, positionInTheString);
					}
					if (tempChar == IN_CODE_ENDL)
					{
						throw ERROR_THROW_IN(114, line, positionInTheString);
					}
					tempChar = inFile.get();
					tempWord[wordLength++] = tempChar;
					inStruct.size++;
				} while (tempChar != '"');
				tempWord[wordLength] = '\0';
				inStruct.Words.push({ tempWord, line });
				wordLength = 0;
				inStruct.size += 1;
				continue;
			}

			if (tempChar == IN_CODE_SPACE || inStruct.code[(int)tempChar] == IN::S || tempChar == IN_CODE_ENDL)
			{
				if (tempWord != NULL && wordLength != 0)
				{
					tempWord[wordLength] = '\0';
					inStruct.Words.push({ tempWord, line });
					wordLength = 0;
				}

				if (tempChar == IN_CODE_SPACE)
				{
					inStruct.ignor += 1;
				}

				if (inStruct.code[(int)tempChar] == IN::S)
				{
					char word[2];
					word[0] = tempChar;
					word[1] = '\0';
					inStruct.Words.push({ word, line });
					wordLength = 0;
				}

				if (tempChar == IN_CODE_ENDL)
				{
					inStruct.lines++;
					line += 1;
					positionInTheString = 0;
				}
			}
			else if (inStruct.code[(int)tempChar] != IN::F)
			{
				positionInTheString++;
				tempWord[wordLength++] = tempChar;
				inStruct.size++;
			}
			else
			{
				throw ERROR_THROW_IN(111, inStruct.lines, positionInTheString);
			}

			if (inStruct.size >= IN_MAX_LEN_TEXT)
			{
				throw ERROR_THROW(113);
			}
		}
		inFile.close();
		return inStruct;
	}
}