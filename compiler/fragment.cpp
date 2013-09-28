#include "fragment.h"

#include "tokens.h"
#include "parser.h"
#include "interop.h"
#include "../common/opcode.h"

string Fragment::DbgGetString() {
	return "";
}


uint Fragment::RegisterVariable(Parser *parser, string name) {
	return parser->RegisterVariable(name);
}

uint Fragment::GetVariableId(Parser *parser, string name) {
	return parser->GetVariableId(name);
}


void Fragment::AllocateVariable(Opcode *opcode, uint varId) {
	opcode->AddInterop(new ByteOperation(OP_ALLOC));
	opcode->AddInterop(new DwordOperation(&varId));
}