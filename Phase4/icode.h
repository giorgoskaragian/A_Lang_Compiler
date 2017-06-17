#ifndef ICODE_H
#define ICODE_H

struct Symbol;
struct Scope;
struct Program;

enum ICodeOperationType
{
	ICOT_Assign = 0,
	ICOT_Add,
	ICOT_Sub,
	ICOT_Mul,
	ICOT_Div,
	ICOT_Mod,
	ICOT_UnaryMinus,
//	ICOT_And,
//	ICOT_Or,
//	ICOT_Not, // PHASE4: Removed
	ICOT_IfEqual,
	ICOT_IfNotEqual,
	ICOT_IfLessEqual,
	ICOT_IfGreaterEqual,
	ICOT_IfLess,
	ICOT_IfGreater,
	ICOT_Jump,
	ICOT_Call,
	ICOT_Param,
	ICOT_Return,
	ICOT_GetRetValue,
	ICOT_FunctionStart,
	ICOT_FunctionEnd,
	ICOT_TableCreate,
	ICOT_TableGetElement,
	ICOT_TableSetElement,
};

enum ICodeExpressionType
{
	ICET_Variable,
	ICET_TableItem,
	ICET_ProgramFunction,
	ICET_LibraryFunction,
	ICET_ConstNum,
	ICET_ConstBool,
	ICET_ConstString,
	ICET_Nil
};

struct ICodeExpression
{
	ICodeExpressionType m_Type;
	Symbol* m_Symbol; // Variable, TableItem (table), Program/Library function
	ICodeExpression* m_Index; // TableItem (index)
	Scope* m_FuncScope; // Program function scope from the symbol table
	double m_NumConst; // ConstNum
	char* m_StrConst; // ConstString
	bool m_BoolConst; // ConstBool
};

struct ICodeQuad
{
	ICodeOperationType m_Op;
	ICodeExpression* m_Result;
	ICodeExpression* m_Arg1;
	ICodeExpression* m_Arg2;
	unsigned int m_Label;
	unsigned int m_Line;
};

struct ICode
{
	ICodeQuad* m_Quads;
	unsigned int m_NumQuads;
	unsigned int m_QuadCapacity;
};

ICode* icodeCompileProgram(Program* prog);
void icodeDestroy(ICode* ic);

void icodePrintQuads(ICode* ic);

#endif
