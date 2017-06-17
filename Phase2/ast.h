#ifndef AST_H
#define AST_H

struct LinkedList;
struct SymbolTable;
struct Symbol;

enum ASTNodeType
{
	AST_Identifier,
	AST_Statement,
	AST_Expression,
	AST_FunctionDefinition,
	AST_IndexedElement,
	AST_LValue,
	AST_Call
};

enum StatementType
{
	ST_Expression,
	ST_If,
	ST_While,
	ST_For,
	ST_Return,
	ST_Block,
	ST_FuncDef,
	ST_Break,
	ST_Continue,
	ST_Empty
};

enum ExpressionType
{
	ET_Assignment,
	ET_BinaryOp,
	ET_LogicOp,
	ET_UnaryOp,
	ET_Nested,
	ET_Const,
	ET_ObjectDefinition,
	ET_FunctionDefinition,
	ET_LValue,
	ET_Call
};

enum OperatorType
{
	Op_Add,
	Op_Subtract,
	Op_Multiply,
	Op_Divide,
	Op_Modulo,

	LogicOp_GreaterThan,
	LogicOp_GreaterThanOrEqual,
	LogicOp_LessThan,
	LogicOp_LessThanOrEqual,
	LogicOp_Equal,
	LogicOp_NotEqual,
	LogicOp_And,
	LogicOp_Or,
	
	UnaryOp_Minus,
	UnaryOp_Not,
	UnaryOp_PreInc,
	UnaryOp_PostInc,
	UnaryOp_PreDec,
	UnaryOp_PostDec
};

enum ObjectDefitionType
{
	ObjDef_ExpressionList,
	ObjDef_IndexedElementList
};

enum ConstType
{
	Const_Int,
	Const_Real,
	Const_String,
	Const_Bool,
	Const_Nil,
};

enum LValueType
{
	LValue_Identifier,
	LValue_ObjectMember,
	LValue_ArrayElement
};

enum IdentifierScopeType
{
	IST_Unknown,
	IST_Local,
	IST_Global
};

enum CallType
{
	Call_ReturnValue,
	Call_Normal,
	Call_Method,
	Call_Lambda
};

struct Program
{
	LinkedList* m_StatementList;
	SymbolTable* m_SymbolTable;
};

struct ASTNode
{
	ASTNodeType m_Type;
	unsigned int m_LineNumber;
};

struct ASTNodeIdentifier : public ASTNode
{
	char* m_Name;
};

struct ASTNodeStatement : public ASTNode
{
	StatementType m_StatementType;
};

struct ASTNodeStatementExpression : public ASTNodeStatement
{
	ASTNode* m_Expr;
};

struct ASTNodeStatementIf : public ASTNodeStatement
{
	ASTNode* m_Cond;
	ASTNode* m_Then;
	ASTNode* m_Else;
};

struct ASTNodeStatementWhile : public ASTNodeStatement
{
	ASTNode* m_Cond;
	ASTNode* m_Body;
};

struct ASTNodeStatementFor : public ASTNodeStatement
{
	LinkedList* m_Init;
	ASTNode* m_Cond;
	LinkedList* m_Iter;
	ASTNode* m_Body;
};

struct ASTNodeStatementReturn : public ASTNodeStatement
{
	ASTNode* m_Expr;
};

struct ASTNodeStatementBlock : public ASTNodeStatement
{
	LinkedList* m_Body;
};

struct ASTNodeStatementFuncDef : public ASTNodeStatement
{
	ASTNode* m_FuncDef;
};

struct ASTNodeExpression : public ASTNode
{
	ExpressionType m_ExpressionType;
};

struct ASTNodeExpressionAssignment : public ASTNodeExpression
{
	ASTNode* m_LValue;
	ASTNode* m_Expression;
};

struct ASTNodeExpressionOp : public ASTNodeExpression
{
	ASTNode* m_Left;
	ASTNode* m_Right;
	OperatorType m_Op;
};

struct ASTNodeExpressionLValue : public ASTNodeExpression
{
	ASTNode* m_LValue;
};

struct ASTNodeExpressionObjDef : public ASTNodeExpression
{
	LinkedList* m_List;
	ObjectDefitionType m_ObjDefType;
};

struct ASTNodeExpressionCall : public ASTNodeExpression
{
	ASTNode* m_Call;
};

struct ASTNodeExpressionFuncDef : public ASTNodeExpression
{
	ASTNode* m_FuncDef;
};

struct ASTNodeExpressionNested : public ASTNodeExpression
{
	ASTNode* m_Expr;
};

struct ASTNodeExpressionConst : public ASTNodeExpression
{
	ConstType m_ConstType;
	union
	{
		int m_IntConst;
		double m_RealConst;
		char* m_StringConst;
		bool m_BoolConst;
	};
};

struct ASTNodeFuncDef : public ASTNode
{
	ASTNodeIdentifier* m_Identifier;
	Symbol* m_Symbol;
	LinkedList* m_ArgList;
	LinkedList* m_Body;
};

struct ASTNodeIndexedElement : public ASTNode
{
	ASTNode* m_Elem;
	ASTNode* m_Index;
};

struct ASTNodeLValue : public ASTNode
{
	LValueType m_LValueType;
	Symbol* m_Symbol;
};

struct ASTNodeLValueIdentifier : public ASTNodeLValue
{
	ASTNodeIdentifier* m_Identifier;
	IdentifierScopeType m_ScopeType;
};

struct ASTNodeLValueObjectMember : public ASTNodeLValue
{
	ASTNode* m_Object;
	ASTNodeIdentifier* m_Member;
};

struct ASTNodeLValueArrayElement : public ASTNodeLValue
{
	ASTNode* m_Array;
	ASTNode* m_Index;
};

struct ASTNodeCall : public ASTNode
{
	CallType m_CallType;
	LinkedList* m_Args;
	ASTNode* m_Func;
};

struct ASTNodeCallMethod : public ASTNodeCall
{
	ASTNodeIdentifier* m_Identifier;
};

// Program
Program* progCreate(LinkedList* statementList);
void progDestroy(Program* prog);

bool progBuildSymbolTable(Program* prog);
void progPrintSymbolTable(Program* prog);

// AST nodes
ASTNode* astCreateStatement_Expression(unsigned int lineno, ASTNode* expression);
ASTNode* astCreateStatement_If(unsigned int lineno, ASTNode* conditionalExpression, ASTNode* thenStatement, ASTNode* elseStatement);
ASTNode* astCreateStatement_While(unsigned int lineno, ASTNode* conditionalExpression, ASTNode* statement);
ASTNode* astCreateStatement_For(unsigned int lineno, LinkedList* initExpressionList, ASTNode* conditionalExpression, LinkedList* iterExpression, ASTNode* statement);
ASTNode* astCreateStatement_Return(unsigned int lineno, ASTNode* expression);
ASTNode* astCreateStatement_Generic(unsigned int lineno, StatementType type);
ASTNode* astCreateStatement_Block(unsigned int lineno, LinkedList* statementList);
ASTNode* astCreateStatement_FuncDef(unsigned int lineno, ASTNode* funcDef);

ASTNode* astCreateExpression_Assignment(unsigned int lineno, ASTNode* lvalue, ASTNode* expression);
ASTNode* astCreateExpression_Op(unsigned int lineno, ASTNode* l, ASTNode* r, OperatorType type);
ASTNode* astCreateExpression_LogicOp(unsigned int lineno, ASTNode* l, ASTNode* r, OperatorType type);
ASTNode* astCreateExpression_Nested(unsigned int lineno, ASTNode* expression);
ASTNode* astCreateExpression_UnaryOp(unsigned int lineno, ASTNode* expression, OperatorType type);
ASTNode* astCreateExpression_lValue(unsigned int lineno, ASTNode* lvalue);
ASTNode* astCreateExpression_Call(unsigned int lineno, ASTNode* call);
ASTNode* astCreateExpression_ObjDef(unsigned int lineno, LinkedList* list, ObjectDefitionType type);
ASTNode* astCreateExpression_FuncDef(unsigned int lineno, ASTNode* funcDef);
ASTNode* astCreateExpression_ConstInt(unsigned int lineno, int value);
ASTNode* astCreateExpression_ConstReal(unsigned int lineno, double value);
ASTNode* astCreateExpression_ConstString(unsigned int lineno, const char* value);
ASTNode* astCreateExpression_ConstNil(unsigned int lineno);
ASTNode* astCreateExpression_ConstTrue(unsigned int lineno);
ASTNode* astCreateExpression_ConstFalse(unsigned int lineno);

ASTNode* astCreateFuncDef(unsigned int lineno, ASTNode* id, LinkedList* argumentList, LinkedList* body);

ASTNode* astCreateIndexedElement(unsigned int lineno, ASTNode* expr1, ASTNode* expr2);

ASTNode* astCreateLValue_Identifier(unsigned int lineno, ASTNode* id);
ASTNode* astCreateLValue_LocalIdentifier(unsigned int lineno, ASTNode* id);
ASTNode* astCreateLValue_GlobalIdentifier(unsigned int lineno, ASTNode* id);
ASTNode* astCreateLValue_ObjectMember(unsigned int lineno, ASTNode* obj, ASTNode* id);
ASTNode* astCreateLValue_ArrayElement(unsigned int lineno, ASTNode* arr, ASTNode* expression);
ASTNode* astCreateLValue_CallObjectMember(unsigned int lineno, ASTNode* call, ASTNode* id);
ASTNode* astCreateLValue_CallArrayElement(unsigned int lineno, ASTNode* call, ASTNode* expression);

ASTNode* astCreateCall_Call(unsigned int lineno, ASTNode* call, LinkedList* args);
ASTNode* astCreateCall_Normal(unsigned int lineno, ASTNode* lvalue, LinkedList* args);
ASTNode* astCreateCall_Method(unsigned int lineno, ASTNode* lvalue, ASTNode* id, LinkedList* args);
ASTNode* astCreateCall_Lambda(unsigned int lineno, ASTNode* funcDef, LinkedList* args);

ASTNode* astCreateIdentifier(unsigned int lineno, char* name);

const char* astGetIdentifierName(ASTNode* id);

#endif
