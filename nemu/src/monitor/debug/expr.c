#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_DEC, TK_HEX, TK_REG, TK_NEQ, TK_AND, TK_DEREF, TK_NEG

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},            // spaces or other useless token
  {"\\+", '+'},                 // plus
  {"-", '-'},                   // sub or negative number
  {"\\*", '*'},                 // mul or deference
  {"/", '/'},                   // div
  {"\\(", '('},                 // left parentheses
  {"\\)", ')'},                 // right parentheses
  {"==", TK_EQ},                // equal
  {"!=", TK_NEQ},               // non-equal
  {"&&", TK_AND},               // and
  {"0x[0-9|a-f|A-F]+", TK_HEX}, // hexadecimal number   
  {"0|[1-9][0-9]*", TK_DEC},    // decimal number
  {"\\$[a-z|A-Z]+", TK_REG}       // reg name

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[1024];
int nr_token;

inline bool check_type(int type) {
  switch(type) {
    case ')':
    case TK_DEC:
    case TK_HEX:
    case TK_REG: {
      // Next token is not TK_DEREF or TK_NEG
      return false;
    }
    default: {
      // Next token is not '-' or '*'
      return true;
    }
  }
}

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        switch (rules[i].token_type) {
          case '*':
          case '-': {
            if(nr_token <= 0 || check_type(tokens[nr_token - 1].type)) {
              tokens[nr_token].type = rules[i].token_type == '*' ? TK_DEREF : TK_NEG;
              ++nr_token;
              break;
            }
          }
          case '+':
          case '/':
          case '(':
          case ')':
          case TK_AND:
          case TK_NEQ:
          case TK_EQ: {
            tokens[nr_token].type = rules[i].token_type;
            ++nr_token;
            break;
          }
          case TK_HEX:
          case TK_REG:
          case TK_DEC: {
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            ++nr_token;
            break;
          }
          case TK_NOTYPE: break;
          default: assert(0);
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

inline bool check_parentheses(int beg,int end) {
  if(tokens[beg].type == '(' && tokens[end].type == ')') {
    int l = 0;
    for(int i = beg + 1;i < end;++i) {
      if(tokens[i].type == ')') {
        if(l) --l;
        else {
          return false;
        }
      } else if(tokens[i].type == '(') {
        ++l;
      }
    }
    return true;
  } else {
    return false;
  }
}

inline bool check_operator(int type) {return type == '+' || type == '-' || type == '*' || type == '/';}

int found_mainToken(int beg,int end,bool *success) {
  // int high_level_token = -1,low_level_token = -1;
  // while(tokens[end].type != ')' && end > beg) {
  //   if(check_operator(tokens[end].type)) {
  //     if(tokens[end].type == '+' || tokens[end].type == '-') {
  //       return end;
  //     } else {
  //       high_level_token = end;
  //     }
  //   }
  //   --end;
  // }
  // while(tokens[beg].type != '(' && end > beg) {
  //   if(check_operator(tokens[beg].type)) {
  //     if(tokens[beg].type == '+' || tokens[beg].type == '-') {
  //       low_level_token = beg;
  //     } else {
  //       high_level_token = high_level_token < beg ? beg : high_level_token;
  //     }
  //   }
  //   ++beg;
  // }
  // return low_level_token > 0 ? low_level_token : high_level_token;
  int rp = 0,token_mulordiv = -1;
  while(end > beg) {
    if(tokens[end].type == ')') {
      ++rp;
    } else if(tokens[end].type == '(') {
      --rp;
    } else if(!rp && check_operator(tokens[end].type)){
      if(tokens[end].type == '+' || tokens[end].type == '-') {
        return end;
      } else {
        if(token_mulordiv < 0) token_mulordiv = end;
      }
    }
    --end;
  }
  return token_mulordiv;
}

//debug tracer

uint32_t _eval(int beg,int end,bool *success);
uint32_t eval(int beg,int end,bool *success) {
  printf("in,beg %d,end %d \n",beg,end);
  uint32_t val = _eval(beg,end,success);
  printf("out:%u,0x%08x,beg %d,end %d\n",val,val,beg,end);
  return val;
}




uint32_t _eval(int beg,int end,bool *success) {
  if(beg > end) {
    return 0;
  } else if(beg == end) {
    switch(tokens[beg].type) {
      case TK_DEC: {
        return (uint32_t)atoi(tokens[beg].str);
        break;
      }
      case TK_HEX: {
        return (uint32_t)_.atox(tokens[beg].str);
        break;
      }
      case TK_REG: {
        uint32_t val = (uint32_t)_.getreg(tokens[beg].str,success);
        if(!*success) {
          _.log("Invalid register name : %s !\n",tokens[beg].str);
        }
        return val;
        break;
      }
      default: {
        *success = false;
        return 0;
      }
    }
  } else if(check_parentheses(beg,end)) {
    return eval(beg + 1,end - 1,success);
  } else if(tokens[beg].type == TK_NEG && !found_mainToken(beg,end,success)) {
    uint32_t val = eval(beg + 1,end,success);
    if(!*success) {
      return 0;
    }
    val = ~val + 1;
    return val;
  } else if(tokens[beg].type == TK_DEREF && !found_mainToken(beg,end,success)) {
    uint32_t val = eval(beg + 1,end,success);
    if(!*success) {
      return 0;
    }
    val = *(uint32_t *)guest_to_host(val);
    return val;
  } else {
    int main_token = found_mainToken(beg,end,success);
    if(main_token < 0) {
      *success = false;
      return 0;
    } else {
      printflog("Expression %d to %d, main token position : %d,type : %c\n",beg,end,main_token,tokens[main_token].type);
      uint32_t val_left = eval(beg,main_token - 1,success);
      if(!*success) return 0;
      uint32_t val_right = eval(main_token + 1,end,success);
      if(!*success) return 0;
      printf("left:%u,token:%c,right:%u\n",val_left,tokens[main_token].type,val_right);
      switch(tokens[main_token].type) {
        case '+': {
          return val_left + val_right;
        }
        case '-': {
          return val_left - val_right;
        }
        case '*': {
          return val_left * val_right;
        }
        case '/': {
          return val_left / val_right;
        }
        case TK_EQ: {
          return (uint32_t)(val_left == val_right ? 0x1 : 0x0);
        }
        case TK_NEQ: {
          return (uint32_t)(val_left == val_right ? 0x0 : 0x1);
        }
        case TK_AND: {
          return (uint32_t)(val_left != 0 && val_right != 0) ? 0x1 : 0x0;
        }
        default: assert(0);     // Unexcepted situation!
      }
    }
  }
}

uint32_t expr(char *e, bool *success) {
  assert(e != NULL && success != NULL);
  *success = true;
  _.log("Begin make tokens for expression : %s\n",e);
  
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
  
  uint32_t expr_val = eval(0,nr_token - 1,success);
  printf("expr return value:%d,%u,%x\n",expr_val,expr_val,expr_val);
  return expr_val;
}
