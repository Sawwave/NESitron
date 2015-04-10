#pragma once
#include <stdint.h>
class Nes6502
{
public:
	Nes6502(void);
	~Nes6502(void);
	/*
	/ There are 13 Different adressing types, as enumerated below
	/ Implicit (1 byte, 2 cycles)
	/ This address type has an implicit target, and only takes up one byte, just for the opcode
	/
	/ Accumulator (1 Byte, 2 cycles)
	/	This type is the same as implicit, but only targets the accumulator register
	/
	/ Immediate (2 bytes, 2 cycles)
	/	This type uses a hex byte as given in the byte after the opcode and does something with it.
	/
	/ Zero Page (2 bytes, 3-5 cycles)
	/	given a byte after the opcode, the 6502 creates a 16-bit number with the most significant hex equaling 0, so #0x52 becomes #0x0052.
	/	the value at this address is used.
	/
	/ Zero Page, X (2 bytes)
	/	Same as zero page, but adds the value in X register to the value given. This wraps around 0x0100, so 0xff + 0x10 = 0x0f;
	/	the value at address 0x000f would then be used for the instruction.
	/
	/ Zero Page, Y (2 bytes)
	/	Same as Zero Page, X, but adds Y to given register.
	/
	/ Relative (2 byte)
	/	Used only for branching. Takes a second byte in the form sxxxxxxx, where s is the sign bit and x is a 7-bit integer.
	/	if bit 7 is 1, the 7-bit number is negative.
	/
	/ Absolute (3 bytes)
	/	A 16-bit address is given, and the value contained there is used.
	/
	/ Absolute, X (3 bytes)
	/	adds X register to the given 16-bit address and uses value contained at that address.
	/
	/ Absolute, Y (3 bytes)
	/	adds Y register to the given 16-bit address and uses value contained at that address.
	/
	/ Indirect (3 bytes)
	/	looks up the value at the given 16-bit address, treats that value as an address. Used in Jump commands to obtain the address needed.
	/
	/ Indexed Indirect
	/	Same as Indirect, but adds X register to the given 16-bit number.
	/
	/ Indirect Indexed
	/	Same as Indirect , but adds Y to the address FOUND at the given 16-bit register.
	*/
	//default values for the addressing types.
	const uint8_t IMMEDIATE_CYCLE_COUNT = 2;
	const uint8_t ZERO_PAGE_CYCLE_COUNT = 3;
	const uint8_t ZERO_PAGE_X_Y_CYCLE_COUNT = 4;
	const uint8_t ABSOLUTE_CYCLE_COUNT = 4;
	const uint8_t INDIRECT_X_CYCLE_COUNT = 6;
	const uint8_t INDIRECT_Y_CYCLE_COUNT = 5;
	const uint8_t SET_CLEAR_FLAG_CYCLE_COUNT = 2;
	const uint8_t REGISTER_TRANSFER_CYCLE_COUNT = 2;

	/*ADD operations
	/	Adds the given byte Operator to the accumulator, adds 1 if carry bit is set, and stores it into the accumulator.
	/carry bit then set to equal (result >255)
	/	overflow flag is to ((Accumulator^result)&(Operator^result))&0x80,
	take the status byte and logical AND it with (M^result)&(N^result)&0x80, I think.
	/	if the result is zero, set the zero flag, otherwise cleared
	/ set the negative flag to equal the last bit of the result
	/ if indirect y, or absolute x or y are used, add 1 cycle if page boundry is crossed.
	*/
	const uint8_t ADD_IMMEDIATE_OPCODE = 0x69;	//2 bytes
	const uint8_t ADD_ZERO_PAGE_OPCODE = 0x65; //2 bytes
	const uint8_t ADD_ZERO_PAGE_X_OPCODE = 0x75; //2 bytes
	const uint8_t ADD_ABSOLUTE_OPCODE = 0x60; //3 bytes
	const uint8_t ADD_ABSOLUTE_X_OPCODE = 0x70;//3 bytes
	const uint8_t ADD_ABSOLUTE_Y_OPCODE = 0x79; //3 bytes
	const uint8_t ADD_INDIRECT_X_OPCODE = 0x61; //2 bytes
	const uint8_t ADD_INDIRECT_Y_OPCODE = 0x61; //2 bytes

	/*SBC (Subtract With Carry)
	/Subtracts the given byte Operator from the accumulator, and stores that in the accumulator.
	/if the carry bit is cleared, it subtracts 1 more.
	/	overflow flag is set to ((Accumulator ^ result) &(Accumulator ^ Operator ))
	/	if the result is zero, set the zero flag, otherwise cleared
	/ set the negative flag to equal the last bit of the result
	/ if indirect y, or absolute x or y are used, add 1 cycle if page boundry is crossed.
	*/
	const uint8_t SUBTRACT_IMMEDIATE_OPCODE = 0xe9;
	const uint8_t SUBTRACT_ZERO_PAGE_OPCODE = 0xe5;
	const uint8_t SUBTRACT_ZERO_PAGE_X_OPCODE = 0xf5;
	const uint8_t SUBTRACT_ABSOLUTE_OPCODE = 0xed;
	const uint8_t SUBTRACT_ABSOLUTE_X_OPCODE = 0xfd;
	const uint8_t SUBTRACT_INDIRECT_X_OPCODE = 0xe1;
	const uint8_t SUBTRACT_INDIRECT_Y_OPCODE = 0xf1;

	/*AND operations
	/	AND memory with accumulator
	set zero flag if result = 0, otherwise cleared.
	set negative flag if last bit is 1, otherwise cleared
	put result into AC
	/ if indirect y, or absolute x or y are used, add 1 cycle if page boundry is crossed.
	*/
	const uint8_t AND_IMMEDIATE_OPCODE = 0x29; //2 bytes
	const uint8_t AND_ZERO_PAGE_OPCODE = 0x25; //2 bytes
	const uint8_t AND_ZERO_PAGE_X_OPCODE = 0x35; //2 bytes
	const uint8_t AND_ABSOLUTE_OPCODE = 0x2d; //3 bytes
	const uint8_t AND_ABSOLUTE_X_OPCODE = 0x3d; //3 bytes//add 1 if boundry crossed
	const uint8_t AND_ABSOLUTE_Y_OPCODE = 0x39; //3 bytes//add 1 if boundry crossed
	const uint8_t ADD_INDIRECT_X_OPCODE = 0x21;//2 bytes
	const uint8_t AND_INDIRECT_Y_OPCODE = 0x31; //2 bytes//add 1 if boundry crossed

	/*LSA and ALS (Left Shift and Right Shift)
	/Left shift or Right Shift X by one bit.
	/the bit shifted out is moved into the carry.
	/the newly freed bit it set to 0.
	/zero flag is set if result = 0, otherwise cleared.
	/negative flag is set if last bit of result = 1, clared otherwise.
	*/
	const uint8_t L_SHIFT_ACCUMULATOR_OPCODE = 0x0a; 
	const uint8_t L_SHIFT_ZERO_PAGE_OPCODE = 0x06; 
	const uint8_t L_SHIFT_ZERO_PAGE_X_OPCODE = 0x16; 
	const uint8_t L_SHIFT_ABSOLUTE_OPCODE = 0x0e; 
	const uint8_t L_SHIFT_ABSOLUTE_X_OPCODE = 0x1e; 
	const uint8_t R_SHIFT_ACCUMULATOR_OPCODE = 0x4a;
	const uint8_t R_SHIFT_ZERO_PAGE_OPCODE = 0x46;
	const uint8_t R_SHIFT_ZERO_PAGE_X_OPCODE = 0x56;
	const uint8_t R_SHIFT_ABSOLUTE_OPCODE = 0x4e;
	const uint8_t R_SHIFT_ABSOLUTE_X_OPCODE = 0x5e;

	const uint8_t SHIFT_ACCUMULATOR_CYCLE_COUNT = 2;
	const uint8_t SHIFT_ZERO_PAGE_CYCLE_COUNT = 5;
	const uint8_t SHIFT_ZERO_PAGE_X_CYCLE_COUNT = 6;
	const uint8_t SHIFT_ABSOLUTE_CYCLE_COUNT = 6;
	const uint8_t SHIFT_ABSOLUTE_X_CYCLE_COUNT = 7;

	/*ROL and ROR (Rotate Left and Rotate Right)
	/roates the byte either left or right. carry is put into the newly freed bit,
	/and the bit pushed out is moved into carry.
	/negative flag is set if the last bit of the result is 1, otherwise cleared.
	*/
	const uint8_t ROTATE_LEFT_ACCUMULATOR_OPCODE = 0x2a;
	const uint8_t ROTATE_LEFT_ZERO_PAGE_OPCODE = 0x26;
	const uint8_t ROTATE_LEFT_ZERO_PAGE_X_OPCODE = 0x36;
	const uint8_t ROTATE_LEFT_ABSOLUTE_OPCODE = 0x2e;
	const uint8_t ROTATE_LEFT_ABSOLUTE_X_OPCODE = 0x3e;
	const uint8_t ROTATE_RIGHT_ACCUMULATOR_OPCODE = 0x6a;
	const uint8_t ROTATE_RIGHT_ZERO_PAGE_OPCODE = 0x66;
	const uint8_t ROTATE_RIGHT_ZERO_PAGE_X_OPCODE = 0x76;
	const uint8_t ROTATE_RIGHT_ABSOLUTE_OPCODE = 0x6e;
	const uint8_t ROTATE_RIGHT_ABSOLUTE_X_OPCODE = 0x7e;

	const uint8_t ROTATE_ZERO_PAGE_CYCLE_COUNT = 5;
	const uint8_t ROTATE_ZERO_PAGE_X_CYCLE_COUNT = 6;
	const uint8_t ROTATE_ABSOLUTE_CYCLE_COUNT = 6;
	const uint8_t ROTATE_ABSOLUTE_X_CYCLE_COUNT = 7;


	/*CMP (Compare)
	/Compares the accumulator with memory value at given address.
	/zero flag is set if accumulator = memory, otherwise cleared
	/carry flag is set if accumulator >= memory, otherwise cleared
	/negative flag is set if bit 7 of (accumulator - memory) is 1, otherwise cleared
	/ if indirect y, or absolute x or y are used, add 1 cycle if page boundry is crossed.
	*/
	const uint8_t COMPARE_IMMEDIATE_OPCODE = 0xc9;
	const uint8_t COMPARE_ZERO_PAGE_OPCODE = 0xc5;
	const uint8_t COMPARE_ZERO_PAGE_X_OPCODE = 0xd5;
	const uint8_t COMPARE_ABSOLUTE_OPCODE = 0xcd;
	const uint8_t COMPARE_ABSOLUTE_X_OPCODE = 0xdd;
	const uint8_t COMPARE_ABSOLUTE_Y_OPCODE = 0xd9;
	const uint8_t COMPARE_INDIRECT_X_OPCODE = 0xc1;
	const uint8_t COMPARE_INDIRECT_Y_OPCODE = 0xd1;

	/*CPX (Compare With X)
	/ Compares X register with value at given memory address
	/zero flag is set if X = memory, otherwise cleared
	/carry flag is set if X >= memory, otherwise cleared
	/negative flag is set if bit 7 of (X - memory) is 1, otherwise cleared
	*/
	const uint8_t COMPARE_X_IMMEDIATE_OPCODE = 0xe0;
	const uint8_t COMPARE_X_ZERO_PAGE_OPCODE = 0xe4;
	const uint8_t COMPARE_X_ABSOLUTE_OPCODE = 0xec;
	//cycle counts are the same for CMP (compare) instruction.

	/*CPX (Compare With Y)
	/ Compares Y register with value at given memory address
	/zero flag is set if Y = memory, otherwise cleared
	/carry flag is set if Y >= memory, otherwise cleared
	/negative flag is set if bit 7 of (Y - memory) is 1, otherwise cleared
	*/
	const uint8_t COMPARE_Y_IMMEDIATE_OPCODE = 0xe0;
	const uint8_t COMPARE_Y_ZERO_PAGE_OPCODE = 0xe4;
	const uint8_t COMPARE_Y_ABSOLUTE_OPCODE = 0xec;
	//cycle counts are the same for CMP (compare) instruction.

	/*DEC (Decrement Memory)
	/ subtracts 1 from value at given memory address.
	/ zero flag is set if result = 0, otherwise cleared.
	/ negative flag is set if result's last bit is 1, otherwise cleared.
	*/
	const uint8_t DECREMENT_ZERO_PAGE_OPCODE = 0xc6;
	const uint8_t DECREMENT_ZERO_PAGE_X_OPCODE = 0xd6;
	const uint8_t DECREMENT_ABSOLUTE_OPCODE = 0xce;
	const uint8_t DECREMENT_ABSOLUTE_X_OPCODE = 0xde;

	const uint8_t DECREMENT_ZERO_PAGE_CYCLE_COUNT = 5;
	const uint8_t DECREMENT_ZERO_PAGE_X_CYCLE_COUNT =6;
	const uint8_t DECREMENT_ABSOLUTE_CYCLE_COUNT = 6;
	const uint8_t DECREMENT_ABSOLUTE_X_CYCLE_COUNT = 7;

	/*DEX and DEY (Decrement X register and Decrement Y register)
	/subtracts 1 from X register.
	/ zero flag is set if result = 0, otherwise cleared.
	/ negative flag is set if result's last bit is 1, otherwise cleared.
	*/
	const uint8_t DECREMENT_X_OPCODE = 0xca;
	const uint8_t DECREMENT_Y_OPCODE = 0x88;
	const uint8_t DECREMENT_X_CYCLE_COUNT = 2;
	const uint8_t DECREMENT_Y_CYCLE_COUNT = 2;

	/*EOR (Exclusive OR)
	/ takes Accumulator register XOR memory value, and stores it in accumulator.
	/ zero flag is set if result = 0, otherwise cleared.
	/ negative flag is set if result's last bit is 1, otherwise cleared.
	/ if indirect y, or absolute x or y are used, add 1 cycle if page boundry is crossed.
	*/
	const uint8_t XOR_IMMEDIATE_OPCODE = 0x49;
	const uint8_t XOR_ZERO_PAGE_OPCODE = 0x45;
	const uint8_t XOR_ZERO_PAGE_X_OPCODE = 0x55;
	const uint8_t XOR_ABSOLUTE_OPCODE = 0x4d;
	const uint8_t XOR_ABSOLUTE_X_OPCODE = 0x5d;
	const uint8_t XOR_ABSOLUTE_Y_OPCODE = 0x59;
	const uint8_t XOR_INDIRECT_X_OPCODE = 0x41;
	const uint8_t XOR_INDIRECT_Y_OPCODE = 0x51;

	/*INC (Increment Memory)
	/adds 1 to the value at the given memory location
	/zero flag is set if result = 0, otherwise cleared
	/negative flag is set if result's last bit is 1, otherwise cleared.
	*/
	const uint8_t INCREMENT_ZERO_PAGE_OPCODE = 0xe6;
	const uint8_t INCREMENT_ZERO_PAGE_X_OPCODE = 0xf6;
	const uint8_t INCREMENT_ABSOLUTE_OPCODE = 0xee;
	const uint8_t INCREMENT_ABSOLUTE_X_OPCODE = 0xfe;

	const uint8_t INCREMENT_ZERO_PAGE_CYCLE_COUNT = 5;
	const uint8_t INCREMENT_ZERO_PAGE_X_CYCLE_COUNT = 6;
	const uint8_t INCREMENT_ABSOLUTE_CYCLE_COUNT = 6;
	const uint8_t INCREMENT_ABSOLUTE_X_CYCLE_COUNT = 7;

	/*INX and INY (Increment X and Increment Y)
	/adds 1 to X register or Y register.
	/zero flag is set if result = 0, otherwise cleared
	/negative flag is set if result's last bit is 1, otherwise cleared.
	*/
	const uint8_t INCREMENT_X_OPCODE = 0xe8;
	const uint8_t INCREMENT_Y_OPCODE = 0xc8;
	const uint8_t INCREMENT_X_CYCLE_COUNT = 2;
	const uint8_t INCREMENT_Y_CYCLE_COUNT = 2;

	/*JMP (JUMP)
	/sets the program counter to the location at the given address
	/in indirect mode, the next two bytes determine the memory address that contains the JUMP target.
	*/
	const uint8_t JUMP_ABSOLUTE_OPCODE = 0x4c;
	const uint8_t JUMP_INDIRECT_OPCODE = 0x6c;
	const uint8_t JUMP_ABSOLUTE_CYCLE_COUNT = 3;
	const uint8_t JUMP_INDIRECT_CYCLE_COUNT = 5;

	/*JSR (Jump to Subroutine)
	/JSR pushes the address(minus one) of the return point onto the stack and sets the PC to the target memory address.
	/first pushes most significant byte, then least.
	*/
	const uint8_t JUMP_SUBROUTINE_ABSOLUTE_OPCODE = 0x20;
	const uint8_t JUMP_SUBROUTINE_ABSOLUTE_CYCLE_COUNT = 6;

	/*LDA (Load Accumulator)
	/ loads a byte into the accumulator.
	/zero flag is set if result = 0, otherwise cleared
	/negative flag is set if result's last bit is 1, otherwise cleared.
	/ if indirect y, or absolute x or y are used, add 1 cycle if page boundry is crossed.
	*/
	const uint8_t LOAD_ACCUMULATOR_IMMEDIATE_OPCODE = 0xa9;
	const uint8_t LOAD_ACCUMULATOR_ZERO_PAGE_OPCODE = 0xa5;
	const uint8_t LOAD_ACCUMULATOR_ZERO_PAGE_X_OPCODE = 0xb5;
	const uint8_t LOAD_ACCUMULATOR_ABSOLUTE_OPCODE = 0xad;
	const uint8_t LOAD_ACCUMULATOR_ABSOLUTE_X_OPCODE = 0xbd;
	const uint8_t LOAD_ACCUMULATOR_ABSOLUTE_Y_OPCODE = 0xb9;
	const uint8_t LOAD_ACCUMULATOR_INDIRECT_X_OPCODE = 0xa1;
	const uint8_t LOAD_ACCUMULATOR_INDIRECT_Y_OPCODE = 0xb1;

	/*LDX and LDY (Load X and Load Y)
	/Loads a byte into either X or Y.
	/zero flag is set if result = 0, otherwise cleared
	/negative flag is set if result's last bit is 1, otherwise cleared.
	//if load x absolute y or load y absolute x are used, add 1 cycle if page is crossed.
	*/
	const uint8_t LOAD_X_IMMEDIATE_OPCODE = 0xa2;
	const uint8_t LOAD_X_ZERO_PAGE_OPCODE = 0xa6;
	const uint8_t LOAD_X_ZERO_PAGE_Y_OPCODE = 0xb6;
	const uint8_t LOAD_X_ABSOLUTE_OPCODE = 0xae;
	const uint8_t LOAD_X_ABSOLUTE_Y_OPCODE = 0xbe;
	const uint8_t LOAD_Y_IMMEDIATE_OPCODE = 0xa0;
	const uint8_t LOAD_Y_ZERO_PAGE_OPCODE = 0xa4;
	const uint8_t LOAD_Y_ZERO_PAGE_X_OPCODE = 0xb4;
	const uint8_t LOAD_Y_ABSOLUTE_OPCODE = 0xac;
	const uint8_t LOAD_Y_ABSOLUTE_X_OPCODE = 0xbc;

	/*STA (Store Accumulator)
	/stores the accumulator byte into a place in memory.*/

	const uint8_t STORE_ACCUMULATOR_ZERO_PAGE_OPCODE = 0x85;
	const uint8_t STORE_ACCUMULATOR_ZERO_PAGE_X_OPCODE = 0x95;
	const uint8_t STORE_ACCUMULATOR_ABSOLUTE_OPCODE = 0x8d;
	const uint8_t STORE_ACCUMULATOR_ABSOLUTE_X_OPCODE = 0x9d;
	const uint8_t STORE_ACCUMULATOR_ABSOLUTE_Y_OPCODE = 0x99;
	const uint8_t STORE_ACCUMULATOR_INDIRECT_X_OPCODE = 0x81;
	const uint8_t STORE_ACCUMULATOR_INDIRECT_Y_OPCODE = 0x91;

	const uint8_t STORE_ACCUMULATOR_ABSOLUTE_X_CYCLE_COUNT = 5;
	const uint8_t STORE_ACCUMULATOR_ABSOLUTE_Y_CYCLE_COUNT = 5;
	const uint8_t STORE_ACCUMULATOR_INDIRECT_Y_CYCLE_COUNT = 6;

	/*STX and STY (Store X Register and Store Y Register)
	/stores the X or Y register at the given place in memory*/
	const uint8_t STORE_X_ZERO_PAGE_OPCODE = 0x86;
	const uint8_t STORE_X_ZERO_PAGE_Y_OPCODE = 0x96;
	const uint8_t STORE_X_ABSOLUTE_OPCODE = 0x8e;
	const uint8_t STORE_Y_ZERO_PAGE_OPCODE = 0x84;
	const uint8_t STORE_Y_ZERO_PAGE_X_OPCODE = 0x94;
	const uint8_t STORE_Y_ABSOLUTE_OPCODE = 0x8c;

	/*TAX, TAY, TXA, TYA, TSX, TXS  (Transfer Accu->X, Accu->Y, X->Accu, Y->Accu , SP->X, X->SP) */
	const uint8_t TRANSFER_ACCUMULATOR_TO_X_OPCODE = 0xaa;
	const uint8_t TRANSFER_ACCUMULATOR_TO_Y_OPCODE = 0xa8;
	const uint8_t TRANSFER_X_TO_ACCUMULATOR_OPCODE = 0x8a;
	const uint8_t TRANSFER_Y_TO_ACCUMULATOR_OPCODE = 0x98;
	const uint8_t TRANSFER_STACK_POINTER_TO_X_OPCODE = 0xba;
	const uint8_t TRANSFER_X_TO_STACK_POINTER_OPCODE = 0x9a;


	/*NOP (No Operation)
	/*simple 2 cycle no op
	*/
	const uint8_t NO_OP_OPCODE = 0xea;
	const uint8_t NO_OP_CYCLE_COUNT = 2;

	/*ORA (Inclusive Or )*
	/ Inclusive Or with accumulator and given byte.
	/zero flag is set if result = 0, otherwise cleared
	/negative flag is set if result's last bit is 1, otherwise cleared.
	/if load x absolute y or load y absolute x are used, add 1 cycle if page is crossed.
	*/
	const uint8_t OR_IMMEDIATE_OPCODE = 0x09;
	const uint8_t OR_ZERO_PAGE_OPCODE = 0x05;
	const uint8_t OR_ZERO_PAGE_X_OPCODE = 0x15;
	const uint8_t OR_ABSOLUTE_OPCODE = 0x0d;
	const uint8_t OR_ABSOLUTE_X_OPCODE = 0x1d;
	const uint8_t OR_ABSOLUTE_Y_OPCODE = 0x19;
	const uint8_t OR_INDIRECT_X_OPCODE = 0x01;
	const uint8_t OR_INDIRECT_Y_OPCODE = 0x11;

	/*PHA and PHP (Push Accumulator or Status onto stack respectively)*/
	const uint8_t PUSH_ACCUMULATOR_OPCODE = 0x48;
	const uint8_t PUSH_STATUS_OPCODE = 0x08;
	const uint8_t PUSH_ACCUMULATOR_OR_STATUS_CYCLE_COUNT = 3;

	/*PLA and PLP (Pull Accumulator or Status from stack respectively)*/
	const uint8_t PULL_ACCUMULATOR_OPCODE = 0x68;
	const uint8_t PULL_STATUS_OPCODE = 0x28;
	const uint8_t PULL_ACCUMULATOR_OR_STATUS_CYCLE_COUNT = 4;



	/*RTI and RTS (Return From Interrupt and Return from Subroutine)
	/ RTI is used at the end of an interrupt. pulls the status byte from the stack, then pulls the program counter.
	/RTS is used at the end of a subroutine. it pulls the program counter from the stack, subtracts 1, and puts in the PC register.
	*/
	const uint8_t RETURN_FROM_INTERRUPT_OPCODE = 0x40;
	const uint8_t RETURN_FROM_SUBROUTINE_OPCODE = 0x60;
	const uint8_t RETURN_FROM_CYCLE_COUNT = 6;


	/*Branch Opcodes
	/Each branch opcode branches if a status bit is set or not set.
	/each one takes a relative address and adds that to the program counter.
	/each one takes 2 cycles, 3 if branch is taken, and 4 if branch leads to a new page.
	//if (current PC & 0xFF00) != (new PC & 0xFF00), then we changed pages.
	*/
	const uint8_t BRANCH_NOT_EQUAL_OPCODE = 0xd0;
	const uint8_t BRANCH_EQUAL_ZERO_OPCODE = 0xf0;
	const uint8_t BRANCH_POSITIVE_OPCODE = 0x10;
	const uint8_t BRANCH_NEGATIVE_OPCODE = 0x30;
	const uint8_t BRANCH_OVERFLOW_CLEAR_OPCODE = 0x50;
	const uint8_t BRANCH_OVERFLOW_SET_OPCODE = 0x70;
	const uint8_t BRANCH_CARRY_CLEAR_OPCODE = 0x90;
	const uint8_t BRANCH_CARRY_SET_OPCODE = 0xb0;

	const uint8_t BRANCH_CYCLE_COUNT = 2; //or more, as noted above.

	/*BRK (Force Interrupt)
	/The Break instruction forces a software interrupt request. The following things happen
	/	1. PC and status are pushed onto the stack
	/	2. IRQ vector (0xFFFE and 0xFFFF) is loaded into PC
	/	3. break flag is set to one in status byte.
	/Read NES documentation to learn more about this.
	*/
	const uint8_t FORCE_INTERRUPT_OPCODE = 0x00;
	const uint8_t FORCE_INTERRUPT_CYCLE_COUNT = 7;


	/* BIT (bit test)
	/Zero flag is set to 1 if accumulator ANDed with memory is zero.
	/Overflow set to equal bit 6 of memory value
	/negative set to equal bit 7 (last bit) of memory value.
	*/
	const uint8_t BIT_SET_ZERO_PAGE_OPCODE =  0x24;
	const uint8_t BIT_SET_ABSOLUTE_OPCODE = 0x2cc;


	/*Status Flag
	/ The set status flag instructions set the respective bit in the status byte to 1,
	/while the clear flag instructions clear the bit.
	/*/
	const uint8_t CLEAR_CARRY_FLAG_OPCODE = 0x18;
	const uint8_t SET_CARRY_FLAG_OPCODE = 0x38;

	const uint8_t CLEAR_DECIMAL_FLAG_OPCODE = 0xd8;
	const uint8_t SET_DECIMAL_FLAG_PCODE = 0xf8;

	const uint8_t CLEAR_INTERRUPT_DISABLE_FLAG_OPCODE = 0x58;
	const uint8_t SET_INTERRUPT_DISABLE_FLAG_OPCODE = 0x78;

	const uint8_t CLEAR_OVERFLOW_FLAG_OPCODE = 0xb8;

	/*Sys Functions*/

};

