/*
    JD1618  Copyright (C) 2021 Just Doodle
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

*/

#include "CPU1618.h"
#include <ncurses.h>
#include <stdlib.h>
#include <iostream>

int main()
{
	CPU1618::Memory mem;
	CPU1618::CPU cpu;
	cpu.ResetCPU(mem);
	bool inDebug = false;
	while (true)
	{
		std::cout << "e to execute | r to reset | l to load | c to clear screen | x to exit\n";
		char l;
		std::cin >> l;
		if (l == 'e')
		{
			std::cout << "Executing instructions from memory\n";
			cpu.ExecuteFromMemory(mem);
		}
		else if (l == 'r')
		{
			std::cout << "Resetting the cpu\n";
			cpu.ResetCPU(mem);
		}
		else if (l == 'l')
		{
			//mem.RAM[0x0070] = 0xA;
			std::cout << "Loading ROM to memory\n";
/*
			mem.RAM[0xFFFC] = 0x40; // JMP
			mem.RAM[0xFFFD] = 0x8F;
			mem.RAM[0x0070] = 0xA;
			mem.RAM[0x8F] = 0x30; //MV
			mem.RAM[0x90] = 0x01;
			mem.RAM[0x91] = 0x70;
			mem.RAM[0x92] = 0x50; //DEC
			mem.RAM[0x93] = 0x01;

			mem.RAM[0x94] = 0x42; //JNZ
			mem.RAM[0x95] = 0x92;
			mem.RAM[0x96] = 0x00;
			*/
			CPU1618::System s;
			if (!inDebug)
			{
				std::cout << "Enter the name of the file you want to load :- ";
				const size_t BUFFER_SIZE = 1024;
				char fn[BUFFER_SIZE] = {0};
				std::cin >> fn;
				if (fn != "DEBUG")
				{
					s.LoadFiletoMemory(fn, mem);
				}
				else
				{
					s.LoadFiletoMemory("Text.rom", mem);
				}
				std::cout << "ROM " << fn << " loaded\n";
			}
			else
			{
				s.LoadFiletoMemory("Text.rom", mem);
				std::cout << "ROM Text.rom loaded\n";
			}

		}
		else if(l == 'c')
		{
			system("clear");
		}
		else if (l == 'x')
		{
			break;
		}
	}
	return 0;
}
