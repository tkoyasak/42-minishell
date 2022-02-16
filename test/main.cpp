#include <gtest/gtest.h>

extern "C" {
	#include "lexer.h"
}

TEST(lexer, lexer_test)
{

	char *input = (char *)"echo \"hello w\"'orld'";

	t_token *tokens = lexer(1, input);

	ASSERT_EQ(tokens->kind, TK_STRING);
	ASSERT_STREQ(tokens->str, "echo");
	tokens = tokens->next;
	ASSERT_EQ(tokens->kind, TK_STRING);
	ASSERT_STREQ(tokens->str, "\"hello w\"'orld'");
	tokens = tokens->next;
	ASSERT_EQ(tokens->kind, TK_EOF);
}
