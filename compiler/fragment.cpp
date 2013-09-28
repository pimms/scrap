#include "fragment.h"

string Fragment::DbgGetString() {
	return "";
}


uint Fragment::RegisterVariable(Parser *parser, string name) {
	return parser->RegisterVariable(name);
}

uint Fragment::GetVariableId(Parser *parser, string name) {
	return parser->GetVariableId(name);
}