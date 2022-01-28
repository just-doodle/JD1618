/*
    JD1618  Copyright (C) 2021 Just Doodle
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

*/

#include "CPU1618.h"

struct CPU1618::Memory mem;
CPU1618::CPU cpu;

CPU1618::CPU1618()
{
	cpu.I = 0;
}

CPU1618::~CPU1618()
{
}

void CPU1618::Memory::InitializeMEM()
{
	for (uint32_t i = 0; i < MAX_MEM; i++)
	{
		RAM[i] = 0;
	}
}

void CPU1618::CPU::ResetCPU(Memory& mem)
{
	PC = 0xFFFC;
	SP = 0x0100;
	C = Z = I = D = B = V = N = E = 0;
	A = 0;
	X = 0;
	Y = 0;
	Q = 0;
	K = 0;

	mem.InitializeMEM();
}

uint8_t CPU1618::CPU::FetchByte(uint32_t& ClockCycles, Memory& mem)
{
	uint8_t Result = mem[PC];
	PC++;
	ClockCycles--;

	return Result;
}

uint8_t CPU1618::CPU::ReadByte(uint32_t& ClockCycles, uint8_t Address, Memory& mem)
{
	uint8_t Result = mem[Address];
	ClockCycles--;

	return Result;
}

void CPU1618::CPU::ExecuteFromMemory(Memory& mem)
{
	bool isFaulted = true;
	bool isInterruptsenabled = false;

	uint32_t ClockCycles = 1;
	uint8_t Interrupt = 0;
	uint8_t PC1 = (uint8_t)((PC & 0xFF00) >> 8);
	uint8_t PC2 = (uint8_t)(PC & 0x00FF);
	std::cout << "Register PC = 0x" << hex(PC1) << hex(PC2) << "\n";

	while (ClockCycles > 0)
	{
		if (I == 0)
		{
			isInterruptsenabled = false;
		}
		else
		{
			isInterruptsenabled = true;
		}
		ClockCycles = 1;
		uint8_t INSTRUCTION = FetchByte(ClockCycles, mem);
		//ClockCycles = 0;
		switch (INSTRUCTION)
		{
		case INS_MV: case INS_ADD: case INS_SUB:
		{
			ClockCycles = 5;
		}break;
		case INS_JMP: case INS_INT: case INS_JZ: case INS_JNZ: case INS_JE: case INS_JNE: case INS_INC: case INS_DEC: case INS_CMP:
		{
			ClockCycles = 4;
		}break;
		case INS_STI: case INS_CLI: case INS_NDE: case INS_SDE: case INS_HLT:
		{
			ClockCycles = 3;
		}break;
		default:
		{
			ClockCycles = 0;
		}break;
		}
		char* insname = NULL;
		uint8_t ADDRESS = 0;
		uint8_t REG = 0;
		

		switch (INSTRUCTION)
		{
		case INS_MV:
		{
			insname = (char*)"MOV";
			uint8_t destination = FetchByte(ClockCycles, mem);
			uint8_t source = FetchByte(ClockCycles, mem);

			if (destination == 0x01)
			{
				if (source == 0x02)
				{
					A = X;
					X = 0;
				}
				else if (source == 0x03)
				{
					A = Y;
					Y = 0;
				}
				else if (source == 0x04)
				{
					A = Q;
					Q = 0;
				}
				else if (source == 0x05)
				{
					A = K;
					K = 0;
				}
				else
				{
					if (OF == 0x00)
						A = mem.RAM[source];
					else
						A = mem.RAM[source * OF];
				}
			}
			else if (destination == 0x02)
			{
				if (source == 0x01)
				{
					X = A;
					A = 0;
				}
				else if (source == 0x03)
				{
					X = Y;
					Y = 0;
				}
				else if (source == 0x04)
				{
					X = Q;
					Q = 0;
				}
				else if (source == 0x05)
				{
					X = K;
					K = 0;
				}
				else
				{
					if (OF == 0x00)
						X = mem.RAM[source];
					else
						X = mem.RAM[source * OF];
				}
			}
			else if (destination == 0x03)
			{
				if (source == 0x01)
				{
					Y = A;
					A = 0;
				}
				else if (source == 0x02)
				{
					Y = X;
					X = 0;
				}
				else if (source == 0x04)
				{
					Y = Q;
					Q = 0;
				}
				else if (source == 0x05)
				{
					Y = K;
					K = 0;
				}
				else
				{
					if (OF == 0x00)
						Y = mem.RAM[source];
					else
						Y = mem.RAM[source * OF];
				}
			}
			else if (destination == 0x04)
			{
				if (source == 0x01)
				{
					Q = A;
					A = 0;
				}
				else if (source == 0x02)
				{
					Q = X;
					X = 0;
				}
				else if (source == 0x03)
				{
					Q = Y;
					Q = 0;
				}
				else if (source == 0x05)
				{
					Q = K;
					K = 0;
				}
				else
				{
					if (OF == 0x00)
						Q = mem.RAM[source];
					else
						Q = mem.RAM[source * OF];
				}
			}
			else if (destination == 0x05)
			{
				if (source == 0x01)
				{
					OF = A;
					A = 0;
				}
				else if (source == 0x02)
				{
					OF = X;
					X = 0;
				}
				else if (source == 0x03)
				{
					OF = Y;
					Y = 0;
				}
				else if (source == 0x04)
				{
					OF = Q;
					Q = 0;
				}
				else
				{
					OF = mem.RAM[source];
				}
			}
			else
			{
				if (source == 0x01)
				{
					if (OF == 0x00)
						mem.RAM[destination] = A;
					else
						mem.RAM[destination * OF] = A;
				}
				else if (source == 0x02)
				{
					if (OF == 0x00)
						mem.RAM[destination] = X;
					else
						mem.RAM[destination * OF] = X;
				}
				else if (source == 0x03)
				{
					if (OF == 0x00)
						mem.RAM[destination] = Y;
					else
						mem.RAM[destination * OF] = Y;
				}
				else if (source == 0x04)
				{
					if (OF == 0x00)
						mem.RAM[destination] = Q;
					else
						mem.RAM[destination * OF] = Q;
				}
				else
				{
					mem.RAM[destination] = mem.RAM[source];
				}
			}
			ClockCycles--;
		}break;
		case INS_JMP:
		{
			insname = (char*)"JMP";
			ADDRESS = FetchByte(ClockCycles, mem);
			if (OF == 0x00)
				PC = ADDRESS;
			else
				PC = ADDRESS * OF;
			ClockCycles--;
		}break;
		case INS_JZ:
		{
			insname = (char*)"JZ";
			ADDRESS = FetchByte(ClockCycles, mem);
			if (Z == 1)
			{
				if (OF == 0x00)
					PC = ADDRESS;
				else
					PC = ADDRESS * OF;
			}
			ClockCycles--;
		}break;
		case INS_JNZ:
		{
			insname = (char*)"JNZ";
			ADDRESS = FetchByte(ClockCycles, mem);
			if (Z == 0)
			{
				if (OF == 0x00)
					PC = ADDRESS;
				else
					PC = ADDRESS * OF;
			}
			ClockCycles--;
		}
		break;
		case INS_JE:
		{
			insname = (char*)"JE";
			ADDRESS = FetchByte(ClockCycles, mem);
			if (E == 1)
			{
				if (OF == 0x00)
					PC = ADDRESS;
				else
					PC = ADDRESS * OF;
			}
			ClockCycles--;
		}break;
		case INS_JNE:
		{
			insname = (char*)"JNE";
			ADDRESS = FetchByte(ClockCycles, mem);
			if (E == 0)
			{
				if (OF == 0x00)
					PC = ADDRESS;
				else
					PC = ADDRESS * OF;
			}
			ClockCycles--;
		}break;
		case INS_DEC:
		{
			insname = (char*)"DEC";
			REG = FetchByte(ClockCycles, mem);
			if (REG == 0x01)
			{
				A--;
				Z = (A == 0);
				N = (A & 0B10000000) > 0;
			}
			else if (REG == 0x02)
			{
				X--;
				Z = (X == 0);
				N = (X & 0B10000000) > 0;
			}
			else if (REG == 0x03)
			{
				Y--;
				Z = (Y == 0);
				N = (Y & 0B10000000) > 0;
			}
			else if (REG == 0x04)
			{
				Q--;
				Z = (Q == 0);
				N = (Q & 0B10000000) > 0;
			}
			ClockCycles--;
		}break;
		case INS_INC:
		{
			insname = (char*)"INC";
			REG = FetchByte(ClockCycles, mem);
			if (REG == 0x01)
				A++;
			else if (REG == 0x02)
				X++;
			else if (REG == 0x03)
				Y++;
			else if (REG == 0x04)
				Q++;
			ClockCycles--;
		}break;
		case INS_ADD:
		{
			insname = (char*)"ADD";
			uint8_t Dest = FetchByte(ClockCycles, mem);
			uint8_t Sour = FetchByte(ClockCycles, mem);

			if (Dest == 0x01)
			{
				if (Sour == 0x02)
					A = X + A;
				else if (Sour == 0x03)
					A = Y + A;
			}
			else if (Dest == 0x02)
			{
				if (Sour == 0x01)
					X = A + X;
				else if (Sour == 0x03)
					X = Y + X;
			}
			else if (Dest == 0x03)
			{
				if (Sour == 0x01)
					Y = A + Y;
				else if (Sour == 0x02)
					Y = X + Y;
			}
			ClockCycles--;
		}break;
		case INS_SUB:
		{
			insname = (char*)"SUB";
			uint8_t D = FetchByte(ClockCycles, mem);
			uint8_t S = FetchByte(ClockCycles, mem);

			if (D == 0x01)
			{
				if (S == 0x02)
				{
					A = X - A;
					Z = (A == 0);
					N = (A & 0B10000000) > 0;
				}
				else if (S == 0x03)
				{
					A = Y - A;
					Z = (A == 0);
					N = (A & 0B10000000) > 0;
				}
			}
			else if (D == 0x02)
			{
				if (S == 0x01)
				{
					X = A - X;
					Z = (X == 0);
					N = (X & 0B10000000) > 0;
				}
				else if (S == 0x03)
				{
					X = Y - X;
					Z = (X == 0);
					N = (X & 0B10000000) > 0;
				}
			}
			else if (D == 0x03)
			{
				if (S == 0x01)
				{
					Y = A - Y;
					Z = (Y == 0);
					N = (Y & 0B10000000) > 0;
				}
				else if (S == 0x02)
				{
					Y = X - Y;
					Z = (Y == 0);
					N = (Y & 0B10000000) > 0;
				}
			}
			ClockCycles--;
		}break;
		case INS_CMP:
		{
			insname = (char*)"CMP";
			uint8_t SO = FetchByte(ClockCycles, mem);
			uint8_t DE = FetchByte(ClockCycles, mem);

			if (SO == 0x01 & DE == 0x02)
			{
				E = (A == X);
			}
			else if (SO == 0x01 & DE == 0x03)
			{
				E = (A == Y);
			}
			else if (SO == 0x02 & DE == 0x01)
			{
				E = (X == A);
			}
			else if (SO == 0x02 & DE == 0x03)
			{
				E = (X == Y);
			}
			else if (SO == 0x03 & DE == 0x01)
			{
				E = (Y == A);
			}
			else if (SO == 0x03 & DE == 0x02)
			{
				E = (Y == X);
			}
			else if(DE == 0x01)
			{
				E = (A == SO);
			}
			else if (SO == 0x01)
			{
				E = (A == DE);
			}
			else if (DE == 0x02)
			{
				E = (X == SO);
			}
			else if (SO == 0x02)
			{
				E = (X == DE);
			}
			else if (DE == 0x03)
			{
				E = (Y == SO);
			}
			else if (SO == 0x03)
			{
				E = (Y == DE);
			}
			else
			{
				E = 10;
			}
			ClockCycles--;
		}break;
		case 0x00:
		{
			std::cout << "The instruction in memory address 0x" << hex(PC) << " cannot be executed because it is zero\n";
                        insname = (char*)"ZOP";
			//isFaulted = true;
		}break;
		case INS_INT:
		{
			Interrupt = FetchByte(ClockCycles, mem);
			if (isInterruptsenabled)
			{
				insname = (char*)"INT";
				switch (Interrupt)
				{
					case INT_PC:
					{
						isFaulted = true;
						switch (Y)
						{
						case 0xA:
						{
							std::cout << "\n";
						}break;
						default:
						{
							std::cout << (char)Y;
						}break;
						}
					}break;
					case INT_SRP:
					{
						std::cout << "status registers :\n"
								  << "C = " << (C ? "1" : "0") << "\n"
								  << "E = " << (E ? "1" : "0") << "\n"
								  << "Z = " << (Z ? "1" : "0") << "\n"
								  << "I = " << (I ? "1" : "0") << "\n"
								  << "D = " << (D ? "1" : "0") << "\n"
								  << "B = " << (B ? "1" : "0") << "\n"
								  << "V = " << (V ? "1" : "0") << "\n"
								  << "N = " << (N ? "1" : "0") << "\n";
					}break;
					case INT_GKP:
					{
						std::cin >> K;
					}break;
					case INT_CLS:
					{
                        #ifdef __linux__
						system("clear");
                        #pragma message ( "OS is linux" )
                        #else
                        #pragma message ( "OS is not linux" )
                        system("cls");
                        #endif
					}break;
				}
			}
		}break;
		case INS_CLI:
		{
			insname = (char*)"CLI";
			I = 0;
		}break;
		case INS_STI:
		{
			insname = (char*)"STI";
			I = 1;
		}break;
		case INS_NOP:
		{
            insname = (char*)"NOP";
			//isFaulted = true;
		}break;
		case INS_HLT:
		{
			isFaulted = true;
			ResetCPU(mem);
			char l = NULL;
			std::cin >> l;
		}break;
		case INS_SDE:
		{
			insname = (char*)"SDE";
			isFaulted = false;
		}break;
		case INS_NDE:
		{
			insname = (char*)"NDE";
			isFaulted = true;
		}
		default:
		{
			std::cout << "The instruction 0x" << hex(INSTRUCTION) << " is not implemented.\n";
			insname = (char*)"IOC";
		}break;
		}
		if (!isFaulted)
		{
			PC1 = (uint8_t)((PC & 0xFF00) >> 8);
			PC2 = (uint8_t)(PC & 0x00FF);
			if(insname != "INT")
				std::cout << "The instruction is 0x" << hex(INSTRUCTION) << " | " << insname << " \n";
			else
			{
				std::cout << "The instruction is 0x" << hex(INSTRUCTION) << " | " << insname  << " " << hex(Interrupt) << "h \n";
			}
			std::cout << "Register A = 0x" << hex(A) << "\n";
			std::cout << "Register X = 0x" << hex(X) << "\n";
			std::cout << "Register Y = 0x" << hex(Y) << "\n";
			std::cout << "Register Q = 0x" << hex(Q) << "\n";
			std::cout << "Offset Register = 0x" << hex(OF) << "\n";
			std::cout << "Register PC = 0x" << hex(PC1) << hex(PC2) << "\n";
		}

	}
}

void CPU1618::System::SystemReset(Memory& mem, CPU& cpu)
{
	cpu.ResetCPU(mem);
}

uint8_t* hex_decode(const char* in, size_t len, uint8_t* out)
{
	unsigned int i, t, hn, ln;

	for (t = 0, i = 0; i < len; i += 2, ++t) {

		hn = in[i] > '9' ? in[i] - 'A' + 10 : in[i] - '0';
		ln = in[i + 1] > '9' ? in[i + 1] - 'A' + 10 : in[i + 1] - '0';

		out[t] = (hn << 4) | ln;
	}
	return out;
}

void CPU1618::System::LoadFiletoMemory(const char* filename, Memory& mem)
{
	std::ifstream ifl(filename);

	char ex1[] = "\0\0\0\0\0\0\0\0";
	char ex2[] = "\0\0\0\0\0\0\0\0";
	char ex3[] = "\0\0\0\0\0\0\0\0";

	while (ifl >> ex1 >> ex2 >> ex3)
	{
		//std::cout << "fst = " << ex1 << " sec = " << ex2 << " trd = " << ex3 << "\n";
		for (int i = 0; i != 3; i++)
		{
			memme(i, mem, ex1, ex2, ex3);
		}
	}
}

void CPU1618::System::memme(int i, Memory& mem, char ex1[], char ex2[], char ex3[])
{
	uint8_t ins1[1];
	uint8_t ins2[1];
	uint8_t ins3[1];

	

	hex_decode(ex1, strlen(ex1), ins1);
	hex_decode(ex2, strlen(ex2), ins2);
	hex_decode(ex3, strlen(ex3), ins3);
	uint8_t FRAM[10];
	uint8_t fp = 0;
	if (fp == 10)
	{
		for (int i = 0; i != 10; i++)
		{
			FRAM[i] = 0;
		}
	}
	switch (i)
	{
		case 0:
		{
			if(ins1[0] != 0x00)
			{
				if (ins1[0] == 0x60)
				{
					if (ins3[0] == 0x00)
					{
						maddr = ins2[0];
						std::cout << "maddr = 0x" << hex(maddr) << "\n";
					}
					else
					{
						maddr = (ins2[0] * ins3[0]);
						std::cout << "maddr1 = 0x" << hex(maddr) << "\n";
					}
					
				}
				else if (ins1[0] != 0x60)
				{
					mem.RAM[maddr] = ins1[0];
					maddr++;
					std::cout << "INS1 0x" << hex(maddr) << " = 0x" << hex(ins1[0]) << "\n";
				}

				FRAM[fp] = ins1[0];
				fp++;
			}
		}break;
		case 1:
		{
			if (ins2[0] != 0x00)
			{
				FRAM[fp] = ins2[0];
				fp++;
				if (ins1[0] != 0x60)
				{
					mem.RAM[maddr] = ins2[0];
					std::cout << "INS2 0x" << hex(maddr) << " = 0x" << hex(ins2[0]) << "\n";
					maddr++;
				}
			}
		}break;
		case 2:
		{
			if (ins3[0] != 0x00)
			{
				FRAM[fp] = ins3[0];
				fp++;
				if (ins1[0] != 0x60)
				{
					mem.RAM[maddr] = ins3[0];
					std::cout << "INS3 0x" << hex(maddr) << " = 0x" << hex(ins3[0]) << "\n";
					maddr++;
				}
			}
		}break;
	}
}