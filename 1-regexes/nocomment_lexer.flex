%option noyywrap

%{

#include "nocomment.hpp"

// The following line avoids an annoying warning in Flex
// See: https://stackoverflow.com/questions/46213840/
extern "C" int fileno(FILE *stream);

// Track // comments inside unclosed attributes
int pending_attr_comments = 0;

%}

%x ESCID
%x ATTR 
%x COMMENT
%x COMMENT_IN_ATTR
%x ESCID_IN_ATTR

%%


<INITIAL>"(*" {
  yylval.attr_sequence = "(*";
  BEGIN(ATTR);
}

<INITIAL>"\\" {
    yylval.escid_sequence = yytext[0];
    BEGIN(ESCID);
}

<INITIAL>"//" {
  BEGIN(COMMENT);
  return AddCommentCount;
}

<INITIAL>. {
  yylval.character = yytext[0];
  return DumpChar;
}

<ATTR>"\\" {
  yylval.escid_sequence = yytext[0];
  BEGIN(ESCID_IN_ATTR);
}

<ATTR>"//" {
  pending_attr_comments++;
  BEGIN(COMMENT_IN_ATTR);
}

<ATTR>"*)" {
  pending_attr_comments = 0;
  BEGIN(INITIAL);
  return AddCommentCount;
}

<ATTR>. {
  yylval.attr_sequence += yytext[0];
}


<ATTR>\n {
  yylval.attr_sequence += '\n';
}

<ATTR><<EOF>> {
  return EofAttr;
}

<COMMENT>. {}

<COMMENT>\n {
  BEGIN(INITIAL);
}

<COMMENT><<EOF>> {
    return Eof;
}

<ESCID>[^ \n]  {
  yylval.escid_sequence += yytext[0];
}

<ESCID>[ \n] {
  yylval.escid_sequence += yytext[0];
  BEGIN(INITIAL);
  return DumpEscId;
}

<ESCID><<EOF>> {
  return EofEscId;
}

<ESCID_IN_ATTR>[^ \n] {
  yylval.escid_sequence += yytext[0];
}

<ESCID_IN_ATTR>[ \n] {
  yylval.escid_sequence += yytext[0];
  yylval.attr_sequence += yylval.escid_sequence;
  BEGIN(ATTR);
}

<ESCID_IN_ATTR><<EOF>> {
  return EofAttr;
}

<COMMENT_IN_ATTR>. {}

<COMMENT_IN_ATTR>\n {
  BEGIN(ATTR);
}

<COMMENT_IN_ATTR><<EOF>> {
    return EofAttr;
}

<INITIAL><<EOF>> {
    return Eof;
}

%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s);
  exit(1);
}



