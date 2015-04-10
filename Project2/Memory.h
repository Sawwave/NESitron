#pragma once
class Memory
{
public:
	/*
	Stack -> 0x0100 to 0x01FF, and the SP will wrap around if it goes past.
	General Purpose Ram -> 0x0200 - 0x07FF, mirrored at 0x0800 to 0x1FFF.
	PPU registers -> 0x2000 to 0x2007, mirrored at 0x2008 to 0x4000.
	Direct Memory Access -> 0x4000 to 0x401f
	expansion ROM -> 0x4020 to 0x5FFF
	SRAM save data ->0x6000 to 0x7FFF
	6502 Cartridge memory starts at 0x8000. To start off, use this as the offset for a jmp command.
	//if memory location < 0x2000, mem location &= 0x07ff;
	nes 'RAM' is bottom 2K (0x0200) of memory.
	else if( mem < 0x4020)



	//interrupt priority is reset is first, then NMI, and IRQ is last.
	*/




	Memory(void);
	~Memory(void);
};

