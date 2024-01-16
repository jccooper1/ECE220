/*									tab:8
 *
 * mp11.c - skeleton code generation for ECE190 compiler MP (F08 MP5)
 *
 * "Copyright (c) 2008-2018 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Steve Lumetta
 * Version:	    3
 * Creation Date:   Wed Oct 15 10:16:14 2008
 * Filename:	    mp11.c
 * History:
 *	SL	3	Tue Apr 03 23:23:23 2018
 *		Changed MP5 to MP11 and 190 to 220 for ECE220.
 *	SL	2	Tue Oct 21 14:28:10 2008
 *		Extracted from gold version.
 *	SL	1	Wed Oct 15 10:16:14 2008
 *		First written.
 */

//all rights are reserved bu J.C. Chen-------------------------------------------
#include <stdio.h>
#include "ece220_label.h"
#include "ece220_parse.h"
#include "ece220_symtab.h"
/*
 * PARAMETERS AND CONSTANTS
 */

typedef enum {
    BR_NEVER, BR_P, BR_Z, BR_ZP, BR_N, BR_NP, BR_NZ, BR_ALWAYS, NUM_BR
} br_type_t;




/*
 * HELPER FUNCTION PROTOTYPES--see function headers for further information
 */

static void gen_long_branch (br_type_t type, ece220_label_t* label);

static void gen_statement (ast220_t* ast);
static void gen_for_statement (ast220_t* ast);
static void gen_if_statement (ast220_t* ast);
static void gen_return_statement (ast220_t* ast);
static void gen_pop_stack (ast220_t* ast);
static void gen_debug_marker (ast220_t* ast);

static void gen_expression (ast220_t* ast);
static void gen_push_int (ast220_t* ast);
static void gen_push_str (ast220_t* ast);
static void gen_push_variable (ast220_t* ast);
static void gen_func_call (ast220_t* ast);
static void gen_get_address (ast220_t* ast);
static void gen_op_assign (ast220_t* ast);
static void gen_op_pre_incr (ast220_t* ast);
static void gen_op_pre_decr (ast220_t* ast);
static void gen_op_post_incr (ast220_t* ast);
static void gen_op_post_decr (ast220_t* ast);
static void gen_op_add (ast220_t* ast);
static void gen_op_sub (ast220_t* ast);
static void gen_op_mult (ast220_t* ast);
static void gen_op_div (ast220_t* ast);
static void gen_op_mod (ast220_t* ast);
static void gen_op_negate (ast220_t* ast);
static void gen_op_log_not (ast220_t* ast);
static void gen_op_log_or (ast220_t* ast);
static void gen_op_log_and (ast220_t* ast);
static void gen_comparison (ast220_t* ast, const char* false_cond);
static void gen_op_cmp_ne (ast220_t* ast);
static void gen_op_cmp_less (ast220_t* ast);
static void gen_op_cmp_le (ast220_t* ast);
static void gen_op_cmp_eq (ast220_t* ast);
static void gen_op_cmp_ge (ast220_t* ast);
static void gen_op_cmp_greater (ast220_t* ast);


/*
 * FILE SCOPE VARIABLES
 * 
 * N.B.  You will need to have a file scope variable to implement one
 * of the statements.  Define it here.
 *
 * static int foo;  <-- a file scope variable ("static" means only this file)
 *
 */



/* 
 * INTERFACE FUNCTIONS -- called from other files; the one function listed
 * here is the equivalent of "main" for students working on this MP
 */

/* 
 * MP11_generate_code
 *   DESCRIPTION: perform LC-3 code generation (print to stdout)
 *   INPUTS: prog -- the main subroutine of the program (a list of
 *                   statements)
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static ece220_label_t* track_label=NULL;
/*the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
/*  R0-R3 available for your code’s use
    R4 global data pointer
    R5 main’s stack frame pointer
    R6 stack pointer
    R7 used by JSR/JSRR
*/

void 
MP11_generate_code (ast220_t* prog)
{
    if(track_label==NULL){
        track_label=label_create();
    }
    switch(prog->type){
        case AST220_FOR_STMT:     
            gen_statement (prog);
            break;
        case AST220_IF_STMT:
            gen_statement (prog);
            break;
        case AST220_RETURN_STMT:
            gen_statement (prog);
            break;
        case AST220_POP_STACK:
           gen_statement (prog);        
            break;  
        case AST220_DEBUG_MARKER:
            gen_statement (prog);    
            break;
        default:
            gen_expression(prog);
            break;
    }
    //keep track of label in a file-scoped variable
    //put the label in the right place
    if(prog->next!=NULL){
        MP11_generate_code(prog->next);
    }
    else if(prog->next==NULL){
        printf("%s\n",label_value(track_label));
    }
}

/*
 * HELPER FUNCTIONS (used only within this file)
 */

/* 
 * gen_long_branch
 *   DESCRIPTION: generate LC-3 assembly code for a branch without offset
 *                limitations
 *   INPUTS: type -- the type of branch to be performed
 *           label -- the branch target label
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_long_branch (br_type_t type, ece220_label_t* label)
{
    static const char* const br_names[NUM_BR] = {
        "; ", "BRp", "BRz", "BRzp", "BRn", "BRnp", "BRnz", "BRnzp"
    }; 
    br_type_t neg_type;
    ece220_label_t* target_label;
    ece220_label_t* false_label;

    neg_type = (type ^ 7);//it is type -7
    target_label = label_create ();
    false_label = label_create ();
    printf ("\t%s %s\n", br_names[neg_type], label_value (false_label));
    printf ("\tLD R3,%s\n\tJMP R3\n", label_value (target_label));
    printf ("%s\n", label_value (target_label));
    printf ("\t.FILL %s\n", label_value (label));
    printf ("%s\n", label_value (false_label));
}

/* 
 * gen_statement
 *   DESCRIPTION: generate LC-3 assembly code for an arbitrary statement
 *   INPUTS: ast -- the AST node corresponding to the statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_statement (ast220_t* ast)
{
    switch (ast->type) {
        case AST220_FOR_STMT:     gen_for_statement (ast);    break;
        case AST220_IF_STMT:      gen_if_statement (ast);     break;
        case AST220_RETURN_STMT:  gen_return_statement (ast); break;
        case AST220_POP_STACK:    gen_pop_stack (ast);        break;
        case AST220_DEBUG_MARKER: gen_debug_marker (ast);     break;
	default: fputs ("BAD STATEMENT TYPE\n", stderr);      break;
    }
}

/* 
 * gen_for_statement
 *   DESCRIPTION: generate LC-3 assembly code for an for loop
 *   INPUTS: ast -- the AST node corresponding to the for loop
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
/*AST220 FOR STMT a for statement
test test condition
left initialization expression (an AST220 POP STACK node, or NULL)
middle loop body (statements linked by next)
right update expression (an AST220 POP STACK node, or NULL)
*/
static void 
gen_for_statement (ast220_t* ast)
{
    ece220_label_t* current=label_create();
    ece220_label_t* next1=label_create();
    //first check if the  expression is NULL
    //if it is not NULL, then generate the statement
    if(ast->left!=NULL){
        gen_statement(ast->left);
    }
    printf("%s\n",label_value(current));
    //since the test is not NULL, then generate the test
    gen_expression(ast->test);
    //then is the body of loop
    printf("\tLDR R0,R6,#0  ;; Load test result into R0\n");
    printf("\tADD R6,R6,#1  ;; Pop test result off stack\n");
    printf("\tADD R0,R0,#0\n");
    gen_long_branch(2,next1);
    while(ast->middle!=NULL){
        gen_statement(ast->middle);
        ast->middle=ast->middle->next;
    }
    if(ast->right!=NULL){
        gen_statement(ast->right);
    }
    //just get to the beginning of the loop
    gen_long_branch(7,current);
    printf("%s\n",label_value(next1));

}

/* 
 * gen_if_statement
 *   DESCRIPTION: generate LC-3 assembly code for an if statement
 *   INPUTS: ast -- the AST node corresponding to the if statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
/*if statement: As with the for statement, you may want to look up the LC-3 implementation of the
conditional decomposition before implementing this statement. The test is an expression, and the then
and else blocks are linked lists of statements, either of which may be empty (but you don’t need to
optimize your code in such cases).*/
static void 
gen_if_statement (ast220_t* ast)
{
    ece220_label_t* end_loop=label_create();
    ece220_label_t* next1=label_create();
    //first generate the test
    gen_expression(ast->test);
    //then generate the body of the if statement
    printf("\tLDR R0,R6,#0  ;; Load test result into R0\n");
    printf("\tADD R6,R6,#1  ;; Pop test result off stack\n");
    printf("\tADD R0,R0,#0\n");
    gen_long_branch(2,next1);
    //then first for the true part
    while(ast->left!=NULL){
        gen_statement(ast->left);
        ast->left=ast->left->next;
    }
    gen_long_branch(7,end_loop);
    printf("%s\n",label_value(next1));
    while(ast->right!=NULL){
        gen_statement(ast->right);
        ast->right=ast->right->next;
    }
    printf("%s\n",label_value(end_loop));
}

/* 
 * gen_return_statement
 *   DESCRIPTION: generate LC-3 assembly code for a return statement
 *   INPUTS: ast -- the AST node corresponding to the return statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
/*  R0-R3 available for your code’s use
    R4 global data pointer
    R5 main’s stack frame pointer
    R6 stack pointer
    R7 used by JSR/JSRR
*/
static void 
gen_return_statement (ast220_t* ast)
{
    gen_expression(ast->left);
    // Assembly instructions
    printf("\tLDR R0,R6,#0\n");     // Load the top of the stack into R0
    printf("\tADD R6,R6,#1\n");    // Increment the stack pointer
    printf("\tSTR R0,R5,#3\n");   // Store the return value at the third position from the base pointer
    //-------// Return from the subroutine
    gen_long_branch(7,track_label);
}

/* 
 * gen_pop_stack
 *   DESCRIPTION: generate LC-3 assembly code to pop and discard a value
 *                from the stack
 *   INPUTS: ast -- the AST node corresponding to the pop stack statement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
static void 
gen_pop_stack (ast220_t* ast)
{
    gen_expression(ast->left);
    printf("\tADD R6,R6,#1\n");
}

/* 
 * gen_debug_marker
 *   DESCRIPTION: generate LC-3 assembly code for a debug marker used to
 *                help debug LC-3 code generation; the marker is simply
 *                a comment in the assembly code that can be used to
 *                identify pieces of code generated by the compiler
 *   INPUTS: ast -- the AST node corresponding to the debug marker
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an LC-3 assembly comment
 */
static void 
gen_debug_marker (ast220_t* ast)
{
    printf ("; --------------- DEBUG(%d) ---------------\n", ast->value);
}

/* 
 * gen_expression
 *   DESCRIPTION: generate LC-3 assembly code to calculate an arbitrary
 *                expression and push the result onto the stack
 *   INPUTS: ast -- the AST node corresponding to the expression
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_expression (ast220_t* ast)
{
    switch (ast->type) {
        case AST220_PUSH_INT:     gen_push_int (ast);       break;
        case AST220_PUSH_STR:     gen_push_str (ast);       break;
        case AST220_VARIABLE:     gen_push_variable (ast);  break;
        case AST220_FUNC_CALL:    gen_func_call (ast);      break;
	case AST220_GET_ADDRESS:  gen_get_address (ast);    break;
	case AST220_OP_ASSIGN:    gen_op_assign (ast);      break;
	case AST220_OP_PRE_INCR:  gen_op_pre_incr (ast);    break;
	case AST220_OP_PRE_DECR:  gen_op_pre_decr (ast);    break;
	case AST220_OP_POST_INCR: gen_op_post_incr (ast);   break;
	case AST220_OP_POST_DECR: gen_op_post_decr (ast);   break;
	case AST220_OP_ADD:       gen_op_add (ast);         break;
	case AST220_OP_SUB:       gen_op_sub (ast);         break;
	case AST220_OP_MULT:      gen_op_mult (ast);        break;
	case AST220_OP_DIV:       gen_op_div (ast);         break;
	case AST220_OP_MOD:       gen_op_mod (ast);         break;
	case AST220_OP_NEGATE:    gen_op_negate (ast);      break;
	case AST220_OP_LOG_NOT:   gen_op_log_not (ast);     break;
	case AST220_OP_LOG_OR:    gen_op_log_or (ast);      break;
	case AST220_OP_LOG_AND:   gen_op_log_and (ast);     break;
	case AST220_CMP_NE:       gen_op_cmp_ne (ast);      break;
	case AST220_CMP_LESS:     gen_op_cmp_less (ast);    break;
	case AST220_CMP_LE:       gen_op_cmp_le (ast);      break;
	case AST220_CMP_EQ:       gen_op_cmp_eq (ast);      break;
	case AST220_CMP_GE:       gen_op_cmp_ge (ast);      break;
	case AST220_CMP_GREATER:  gen_op_cmp_greater (ast); break;
	default: fputs ("BAD EXPRESSION TYPE\n", stderr);   break;
    }
}

/* 
 * gen_push_int
 *   DESCRIPTION: generate LC-3 assembly code to push a constant integer
 *                expression onto the stack
 *   INPUTS: ast -- the AST node corresponding to the integer
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
static void 
gen_push_int (ast220_t* ast)
{
    // Create two labels for the current and next instructions
    ece220_label_t* current1=label_create();
    ece220_label_t* next1=label_create();
    // Get the string representation of the labels
    char* next1_value=label_value(next1);
    char* current1_value=label_value(current1);
    // Load the value at the address labeled by current1 into register R0
    printf("\tLD R0,%s\n",current1_value);
    // Decrement the stack pointer (R6)
    printf("\tADD R6,R6,#-1\n");
    // Store the value in R0 at the top of the stack
    printf("\tSTR R0,R6,#0\n");
    // Branch to the instruction labeled by next1
    printf("\tBRnzp %s\n",next1_value);
    // Output the current1 label
    printf("%s\n",current1_value);
    // Store the integer constant in memory
    printf("\t.FILL %d\n",ast->value);
    // Output the next1 label
    printf("%s\n",next1_value);
}

/* 
 * gen_push_str
 *   DESCRIPTION: generate LC-3 assembly code to push a constant string
 *                expression onto the stack
 *   INPUTS: ast -- the AST node corresponding to the string
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_push_str (ast220_t* ast)
{
    ece220_label_t* current2=label_create();
    ece220_label_t* next2=label_create();
    char* next2_value=label_value(next2);
    char* current2_value=label_value(current2);
    // Load the value at the address labeled by current2 into register R0
    printf("\tLEA R0,%s\n",current2_value);
    // Decrement the stack pointer (R6)
    printf("\tADD R6,R6,#-1\n");
    // Store the value in R0 at the top of the stack
    printf("\tSTR R0,R6,#0\n");
    // Branch to the instruction labeled by next2
    printf("\tBRnzp %s\n",next2_value);
    // Output the current2 label
    printf("%s\n",current2_value);
    // Store the string constant in memory
    printf("\t.STRINGZ %s\n",ast->name);
    // Output the next2 label
    printf("%s\n",next2_value);

}

/* 
 * gen_push_variable
 *   DESCRIPTION: generate LC-3 assembly code to push a variable's value
 *                onto the stack
 *   INPUTS: ast -- the AST node corresponding to the variable reference
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
/*AST220 VARIABLE a variable reference
name variable name (look up in symbol table)
left array index expression (for array variables; NULL for scalar variables)
*/
/*scalar variable reference: Write the code to read a scalar variable and push it onto the stack. You’ll
need to look the variable up in the symbol table to know where to find it. For now, you may assume
that the variable is not an array (or get a headstart on the second checkpoint by handling both kinds).
*/
static void 
gen_push_variable (ast220_t* ast)
{
    //look up table for this question
    symtab_entry_t* becky=symtab_lookup(ast->name);
    //first check it's a scalar or an array
    //the following is scalar
    if(ast->left==NULL){
        printf("\tLDR R0,R%d,#%d\n",5-becky->is_global,becky->offset);
        printf("\tADD R6,R6,#-1\n");
        printf("\tSTR R0,R6,#0\n");

    }
    else{
        //at this time, it's array
        gen_expression(ast->left);
        printf("\tLDR R1,R6,#0\n");
        printf("\tADD R6,R6,#1\n");//pop the top of the stack
        //find the address
        printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);
        printf("\tADD R1,R0,R1\n");//add the address
        printf("\tLDR R0,R1,#0\n");//load the value
        printf("\tADD R6,R6,#-1\n");
        printf("\tSTR R0,R6,#0\n");//store the value
    }
}

/* 
 * gen_func_call
 *   DESCRIPTION: generate LC-3 assembly code to perform a function call,
 *                remove the arguments from the stack, and leave the
 *                return value on top of the stack
 *   INPUTS: ast -- the AST node corresponding to the function call
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
/*AST220 FUNC CALL a function call; be sure to pop off arguments, but leave the return value on top of stack
fnum C library function identifier; one of:
AST220 PRINTF
AST220 RAND
AST220 SCANF
AST220 SRAND
left argument list (expressions from right to left linked by next)
*/
static void 
gen_func_call (ast220_t* ast)
{
    int32_t count=0;
    ast220_t* temp=ast->left;
    //find the number of the arguments
    while(temp!=NULL){
        count++;
        gen_expression(temp);
        temp=temp->next;
    }
    //pop all the arguments out of the stack
    //leaving the only wanted return value on the top of the stack
    ece220_label_t* current3=label_create();
    ece220_label_t* next3=label_create();
    char* next3_value=label_value(next3);
    char* current3_value=label_value(current3);
    // Load the value at the address labeled by current3 into register R0
    printf("\tLD R0,%s\n",current3_value);
    printf("\tJSRR R0\n");
    printf("\tBRnzp %s\n",next3_value);
    printf("%s\n",current3_value);
    printf("\t.FILL");
    switch(ast->fnum){
        case AST220_PRINTF:
            printf("\tPRINTF\n");
            break;
        case AST220_RAND:
            printf("\tRAND\n");
            break;
        case AST220_SCANF:
            printf("\tSCANF\n");
            break;
        case AST220_SRAND:
            printf("\tSRAND\n");
            break;
        default:
            break;
    }
    printf("%s\n",next3_value);
    //pop the arguments out of the stack
    printf("\tLDR R0,R6,#0\n");
    printf("\tADD R6,R6,#1\n");
    printf("\tADD R6,R6,#%d\n",count);
    // Store the value in R0 at the top of the stack
    printf("\tADD R6,R6,#-1\n");
    printf("\tSTR R0,R6,#0\n");
}


/* 
 * gen_get_address
 *   DESCRIPTION: generate LC-3 assembly code to push the address of
 *                a variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to the variable reference
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
/*get variable address: For the scanf call, you’ll need to be able to pass variable addresses. Write the
function that finds the address of a variable, be it a scalar or an array element, and pushes it onto the
stack.
*/
static void 
gen_get_address (ast220_t* ast)
{
    symtab_entry_t* becky=symtab_lookup(ast->left->name);
    //first check if it is a scalar or an array
    if(ast->left->left==NULL){
        printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);//find the address of input
        printf("\tADD R6,R6,#-1\n");
        printf("\tSTR R0,R6,#0\n");
    }
    else{
        gen_expression(ast->left->left);
        printf("\tLDR R1,R6,#0\n");
        printf("\tADD R6,R6,#1\n");//find the offset of R0,and add its address to R1
        printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);
        printf("\tADD R0,R0,R1\n");
        printf("\tADD R6,R6,#-1\n");//push back
        printf("\tSTR R0,R6,#0\n");
    }
}

/* 
 * gen_op_assign
 *   DESCRIPTION: generate LC-3 assembly code to perform an assignment,
 *                leaving the value assigned on the stack (assignments
 *                are also expressions, with value equal to that of the
 *                value assigned)
 *   INPUTS: ast -- the AST node corresponding to the assignment
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
/*AST220 OP ASSIGN an assignment; copy the expression into the variable, but do not pop the
expression result off of the stack
left the variable, an AST220 VARIABLE node
right the expression
*/
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
/*  R0-R3 available for your code’s use
    R4 global data pointer
    R5 main’s stack frame pointer
    R6 stack pointer
    R7 used by JSR/JSRR
*/
static void 
gen_op_assign (ast220_t* ast)
{
    symtab_entry_t* becky=symtab_lookup(ast->left->name);
    gen_expression(ast->right);
    //first check if it is a scalar or an array
    //the following is a scalar
    if(ast->left->left==NULL){
        printf("\tLDR R1,R6,#0\n");
        printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);
        printf("\tSTR R1,R0,#0\n");
    }
    else{
        gen_expression(ast->left->left);
        printf("\tLDR R1,R6,#0\n");
        printf("\tADD R6,R6,#1\n");//find the offset and add it to R0
        printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);
        printf("\tADD R0,R0,R1\n"); 
        printf("\tLDR R1,R6,#0\n");//push back
        printf("\tSTR R1,R0,#0\n");
    }
}

/* 
 * gen_op_pre_incr
 *   DESCRIPTION: generate LC-3 assembly code to for a pre-increment 
 *                expression, which increments a variable and pushes
 *                the new value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to pre-increment
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 * 
 * AST220 OP PRE INCR pre- and post-increment and decrement
AST220 OP PRE DECR left the variable, an AST220 VARIABLE node
AST220 OP POST INCR
AST220 OP POST DECR
 */
static void 
gen_op_pre_incr (ast220_t* ast)
{
    symtab_entry_t*becky=symtab_lookup(ast->left->name);//find the name of this variable
    printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);//R0=Rx+offest
    printf("\tLDR R1,R0,#0\n");//R1=mem[R0]
    printf("\tADD R1,R1,#1\n");//R1=R1+1
    printf("\tSTR R1,R0,#0\n");//mem[R0]=R1
    printf("\tADD R6,R6,#-1\n");//R6=R6-1
    printf("\tSTR R1,R6,#0\n");//mem[R6]=R1 push the value onto the stack
}

/* 
 * gen_op_pre_decr
 *   DESCRIPTION: generate LC-3 assembly code to for a pre-decrement 
 *                expression, which decrements a variable and pushes
 *                the new value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to pre-decrement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_pre_decr (ast220_t* ast)
{
    symtab_entry_t*becky=symtab_lookup(ast->left->name);//find the name of this variable
    printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);//R0=Rx+offest
    printf("\tLDR R1,R0,#0\n");//R1=mem[R0]
    printf("\tADD R1,R1,#-1\n");//R1=R1-1
    printf("\tSTR R1,R0,#0\n");//mem[R0]=R1
    printf("\tADD R6,R6,#-1\n");//R6=R6-1
    printf("\tSTR R1,R6,#0\n");//mem[R6]=R1 push the value onto the stack
}

/* 
 * gen_op_post_incr
 *   DESCRIPTION: generate LC-3 assembly code to for a post-increment 
 *                expression, which increments a variable and pushes
 *                the original value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to post-increment
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_post_incr (ast220_t* ast)
{
    symtab_entry_t*becky=symtab_lookup(ast->left->name);//find the name of this variable
    printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);//R0=Rx+offest
    printf("\tLDR R1,R0,#0\n");//R1=mem[R0]
    printf("\tADD R6,R6,#-1\n");//R6=R6-1
    printf("\tSTR R1,R6,#0\n");//mem[R6]=R1 push the value onto the stack
    printf("\tADD R1,R1,#1\n");//R1=R1+1
    printf("\tSTR R1,R0,#0\n");//mem[R0]=R1
    printf("\tADD R6,R6,#-1\n");//R6=R6-1
    printf("\tSTR R1,R6,#0\n");//mem[R6]=R1 push the value onto the stack

}

/* 
 * gen_op_post_decr
 *   DESCRIPTION: generate LC-3 assembly code to for a post-decrement 
 *                expression, which decrements a variable and pushes
 *                the original value of the variable onto the stack
 *   INPUTS: ast -- the AST node corresponding to post-decrement
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_post_decr (ast220_t* ast)
{
    symtab_entry_t*becky=symtab_lookup(ast->left->name);//find the name of this variable
    printf("\tADD R0,R%d,#%d\n",5-becky->is_global,becky->offset);//R0=Rx+offest
    printf("\tLDR R1,R0,#0\n");//R1=mem[R0]
    printf("\tADD R6,R6,#-1\n");//R6=R6-1
    printf("\tSTR R1,R6,#0\n");//mem[R6]=R1 push the value onto the stack
    printf("\tADD R1,R1,#-1\n");//R1=R1-1
    printf("\tSTR R1,R0,#0\n");//mem[R0]=R1
    printf("\tADD R6,R6,#-1\n");//R6=R6-1
    printf("\tSTR R1,R6,#0\n");//mem[R6]=R1 push the value onto the stack
}

/* 
 * gen_op_add
 *   DESCRIPTION: generate LC-3 assembly code to calculate the sum of two
 *                expressions, leaving the sum on the stack
 *   INPUTS: ast -- the AST node corresponding to the add operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_add (ast220_t* ast)
{
    gen_expression(ast->left);
    gen_expression(ast->right);
    printf("\tLDR R0,R6,#0\n");     // Load the top of the stack into R0, the left
    printf("\tLDR R1,R6,#1\n") ;    // Load the second value on the stack into R1, the right
    printf("\tADD R6,R6,#2\n")  ;   // Increment the stack pointer
    printf("\tADD R0,R0,R1\n");     // Add the values in R0 and R1 and store the result in R0
    printf("\tADD R6,R6,#-1\n");    // Decrement the stack pointer
    printf("\tSTR R0,R6,#0\n");     // Store the value in R0 at the top of the stack


}

/* 
 * gen_op_sub
 *   DESCRIPTION: generate LC-3 assembly code to calculate the difference 
 *                of two expressions, leaving the difference on the stack
 *   INPUTS: ast -- the AST node corresponding to the subtraction operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_sub (ast220_t* ast)
{
    gen_expression(ast->left);
    gen_expression(ast->right);
    printf("\tLDR R1,R6,#0\n");     // Load the top of the stack into R0, the left
    printf("\tLDR R0,R6,#1\n") ;    // Load the second value on the stack into R1, the right
    printf("\tADD R6,R6,#2\n") ;    // Increment the stack pointer
    printf("\tNOT R1,R1\n")     ;   // Negate the value in R1
    printf("\tADD R1,R1,#1\n")   ;  // Add 1 to the value in R1
    printf("\tADD R0,R0,R1\n")    ; // Add the values in R0 and R1 and store the result in R0
    printf("\tADD R6,R6,#-1\n")    ;// Decrement the stack pointer
    printf("\tSTR R0,R6,#0\n")    ; // Store the value in R0 at the top of the stack
}

/* 
 * gen_op_mult
 *   DESCRIPTION: generate LC-3 assembly code to calculate the product 
 *                of two expressions (using the MULTIPLY subroutine in
 *                the LC-3 library), leaving the product on the stack
 *   INPUTS: ast -- the AST node corresponding to the multiplication operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
static void 
gen_op_mult (ast220_t* ast)
{
    gen_expression(ast->left);
    gen_expression(ast->right);
    ece220_label_t* current4=label_create();
    ece220_label_t* next4=label_create();
    printf("\tLDR R1,R6,#0\n");     // Load the top of the stack into R1, the right
    printf("\tLDR R0,R6,#1\n") ;    // Load the second value on the stack into R0, the left
    printf("\tADD R6,R6,#2\n")  ;   // Increment the stack pointer
    printf("\tLD R2,%s\n",label_value(current4));// Load the address of the MULTIPLY subroutine into R2
    printf("\tJSRR R2\n")       ;   // Jump to the MULTIPLY subroutine
    printf("\tBRnzp %s\n",label_value(next4));// Branch to the instruction labeled by next4
    printf("%s\n",label_value(current4));// Output the current4 label
    printf("\t.FILL MULTIPLY\n") ;  // Store the address of the MULTIPLY subroutine
    printf("%s\n",label_value(next4));// Output the next4 label
    printf("\tADD R6,R6,#-1\n") ;   // Decrement the stack pointer
    printf("\tSTR R0,R6,#0\n") ;    // Store the value in R0 at the top of the stack
}

/* 
 * gen_op_div
 *   DESCRIPTION: generate LC-3 assembly code to calculate the quotient 
 *                of two expressions (using the DIVIDE subroutine in
 *                the LC-3 library), leaving the quotient on the stack
 *   INPUTS: ast -- the AST node corresponding to the division operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_div (ast220_t* ast)
{
    gen_expression(ast->left);
    gen_expression(ast->right);
    ece220_label_t* current5=label_create();
    ece220_label_t* next5=label_create();
    printf("\tLDR R1,R6,#0\n");     // Load the top of the stack into R1, the right
    printf("\tLDR R0,R6,#1\n");     // Load the second value on the stack into R0, the left
    printf("\tADD R6,R6,#2\n");     // Increment the stack pointer
    printf("\tLD R2,%s\n",label_value(current5));// Load the address of the DIVIDE subroutine into R2
    printf("\tJSRR R2\n");          // Jump to the DIVIDE subroutine
    printf("\tBRnzp %s\n",label_value(next5));// Branch to the instruction labeled by next5
    printf("%s\n",label_value(current5));// Output the current5 label
    printf("\t.FILL DIVIDE\n");   // Store the address of the DIVIDE subroutine
    printf("%s\n",label_value(next5));// Output the next5 label
    printf("\tADD R6,R6,#-1\n");    // Decrement the stack pointer
    printf("\tSTR R0,R6,#0\n");     // Store the value in R0 at the top of the stack
}

/* 
 * gen_op_mod
 *   DESCRIPTION: generate LC-3 assembly code to calculate the modulus 
 *                of one expression relative to a second expression (using 
 *                the MODULUS subroutine in the LC-3 library), leaving 
 *                the result on the stack
 *   INPUTS: ast -- the AST node corresponding to the division operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_mod (ast220_t* ast)
{
    gen_expression(ast->left);
    gen_expression(ast->right);
    ece220_label_t* current6=label_create();
    ece220_label_t* next6=label_create();
    printf("\tLDR R1,R6,#0\n");     // Load the top of the stack into R1, the right
    printf("\tLDR R0,R6,#1\n");     // Load the second value on the stack into R0, the left
    printf("\tADD R6,R6,#2\n");     // Increment the stack pointer
    printf("\tLD R2,%s\n",label_value(current6));// Load the address of the MODULUS subroutine into R2
    printf("\tJSRR R2\n") ;         // Jump to the MODULUS subroutine
    printf("\tBRnzp %s\n",label_value(next6));// Branch to the instruction labeled by next6
    printf("%s\n",label_value(current6));// Output the current6 label
    printf("\t.FILL MODULUS\n");   // Store the address of the MODULUS subroutine
    printf("%s\n",label_value(next6));// Output the next6 label
    printf("\tADD R6,R6,#-1\n");    // Decrement the stack pointer
    printf("\tSTR R0,R6,#0\n");     // Store the value in R0 at the top of the stack

}

/* 
 * gen_op_negate
 *   DESCRIPTION: generate LC-3 assembly code to negate the value of an
 *                expression, leaving the result on the stack
 *   INPUTS: ast -- the AST node corresponding to the negation operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
//the following is the struct ast220_t which just helps me to write this code
//typedef struct ast220_t ast220_t;
//struct ast220_t {
//  ast220_type_t   type;       /* type of AST node                   */
//  int32_t         value;      /* a number (PUSH_INT, DEBUG_MARKER)  */
//  char*           name;       /* a string (PUSH_STR, VARIABLE)      */
//  ast220_builtin_func_t fnum; /* function number (FUNC_CALL)        */
//  ast220_t*       test;       /* test condition (IF_STMT, FOR_STMT) */
//  ast220_t*       left;       /* left child/first operand           */
//  ast220_t*       middle;     /* middle child (FOR_STMT)            */
//  ast220_t*       right;      /* right child/second operand         */
//  ast220_t*       next;       /* next AST node                      */
/*  R0-R3 available for your code’s use
    R4 global data pointer
    R5 main’s stack frame pointer
    R6 stack pointer
    R7 used by JSR/JSRR
*/
static void 
gen_op_negate (ast220_t* ast)
{
    gen_expression(ast->left);
    // Assembly instructions
    printf("\tLDR R0,R6,#0\n");     // Load the top of the stack into R0
    printf("\tADD R6,R6,#1\n");     // Increment the stack pointer
    printf("\tNOT R0,R0\n");        // Negate the value in R0
    printf("\tADD R0,R0,#1\n");     // Add 1 to the value in R0
    printf("\tADD R6,R6,#-1\n");    // Decrement the stack pointer
    printf("\tSTR R0,R6,#0\n");     // Store the value in R0 at the top of the stack

}

/* 
 * gen_op_log_not
 *   DESCRIPTION: generate LC-3 assembly code to perform a logical NOT
 *                operation on an expression, leaving the result (0 or 1)
 *                on the stack
 *   INPUTS: ast -- the AST node corresponding to the logical NOT operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
/*logical operations: The logical NOT, OR, and AND operations are somewhat more complicated than
the operators that you have implemented already. First, they always result in 0 or 1, and you will
have to make sure that you generate code to place the right value on the stack. Next, both OR and
AND require shortcutting, which means that the second expression is only evaluated if the first allows
it to change the outcome of the logical operation. For example, if the first operand of OR is true, the
second operand is not evaluated.*/
static void 
gen_op_log_not (ast220_t* ast)
{
    ece220_label_t*becky=label_create();
    ece220_label_t*pol=label_create();
    gen_expression(ast->left);//find the expression of the left
    printf("\tAND R2,R2,#0\n");//R2=0
    printf("\tLDR R0,R6,#0\n");//load the top of the stack into R0
    printf("\tADD R6,R6,#1\n");//increment the stack pointer
    printf("\tADD R0,R0,#0\n");//R0=R0+0
    printf("\tBRz %s\n",label_value(becky));//branch to the instruction labeled by becky
    printf("\tADD R0,R0,#0\n");//R0=R0+0
    printf("\tBRnp %s\n",label_value(pol));//branch to the instruction labeled by pol
    printf("%s\n",label_value(becky));//output the becky label
    printf("ADD R2,R2,#1\n");//R2=R2+1
    printf("%s\n",label_value(pol));//output the pol label
    printf("\tADD R6,R6,#-1\n");//decrement the stack pointer
    printf("\tSTR R2,R6,#0\n");//store the value in R2 at the top of the stack
}

/* 
 * gen_op_log_or
 *   DESCRIPTION: generate LC-3 assembly code to perform a logical OR
 *                operation on two expressions, leaving the result (0 or 1)
 *                on the stack; the code generated must perform proper
 *                shortcutting, i.e., if the first expression is true
 *                the second is not evaluated (the code is not executed)
 *   INPUTS: ast -- the AST node corresponding to the logical OR operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
/*logical operations: The logical NOT, OR, and AND operations are somewhat more complicated than
the operators that you have implemented already. First, they always result in 0 or 1, and you will
have to make sure that you generate code to place the right value on the stack. Next, both OR and
AND require shortcutting, which means that the second expression is only evaluated if the first allows
it to change the outcome of the logical operation. For example, if the first operand of OR is true, the
second operand is not evaluated.*/
static void 
gen_op_log_or (ast220_t* ast)
{   //first we create two labels for the following usage
    ece220_label_t*pol=label_create();//end
    ece220_label_t*becky=label_create();
    gen_expression(ast->left);
    printf("\tAND R2,R2,#0\n");//R2=0
    printf("\tLDR R0,R6,#0\n");//load the top of the stack into R0
    printf("\tADD R6,R6,#1\n");//increment the stack pointer
    printf("\tADD R0,R0,#0\n");//R0=R0+0
    printf("\tBRnp %s\n",label_value(pol));//branch to the end if the first is 1
    gen_expression(ast->right);
    printf("\tLDR R0,R6,#0\n");//load the top of the stack into R0
    printf("\tADD R6,R6,#1\n");//increment the stack pointer
    printf("\tADD R0,R0,#0\n");//R0=R0+0
    printf("\tBRnp %s\n",label_value(pol));//branch to the end if the first is 1
    printf("\tBRz %s\n",label_value(becky));//branch to the end if the second is 0
    printf("%s\n",label_value(pol));//output the pol label
    printf("ADD R2,R2,#1\n");//R2=R2+1
    printf("%s\n",label_value(becky));//output the becky label
    printf("\tADD R6,R6,#-1\n");//decrement the stack pointer
    printf("\tSTR R2,R6,#0\n");//store the value in R2 at the top of the stack
}

/* 
 * gen_op_log_and
 *   DESCRIPTION: generate LC-3 assembly code to perform a logical AND
 *                operation on two expressions, leaving the result (0 or 1)
 *                on the stack; the code generated must perform proper
 *                shortcutting, i.e., if the first expression is false
 *                the second is not evaluated (the code is not executed)
 *   INPUTS: ast -- the AST node corresponding to the logical AND operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
/*logical operations: The logical NOT, OR, and AND operations are somewhat more complicated than
the operators that you have implemented already. First, they always result in 0 or 1, and you will
have to make sure that you generate code to place the right value on the stack. Next, both OR and
AND require shortcutting, which means that the second expression is only evaluated if the first allows
it to change the outcome of the logical operation. For example, if the first operand of OR is true, the
second operand is not evaluated.*/
static void 
gen_op_log_and (ast220_t* ast)
{
    ece220_label_t*pol=label_create();
    ece220_label_t*becky=label_create();
    gen_expression(ast->left);
    printf("\tAND R2,R2,#0\n");//R2=0
    printf("\tLDR R0,R6,#0\n");//load the top of the stack into R0
    printf("\tADD R6,R6,#1\n");//increment the stack pointer
    printf("\tADD R0,R0,#0\n");//R0=R0+0
    printf("\tBRz %s\n",label_value(becky));//branch to the becky if the second is 0
    gen_expression(ast->right);
    printf("\tLDR R0,R6,#0\n");//load the top of the stack into R0
    printf("\tADD R6,R6,#1\n");//increment the stack pointer
    printf("\tADD R0,R0,#0\n");//R0=R0+0
    printf("\tBRz %s\n",label_value(becky));//branch to the becky if the second is 0
    printf("\tBRnp %s\n",label_value(pol));//branch to the pol if the second is 0
    printf("%s\n",label_value(pol));//output the pol label
    printf("\tADD R2,R2,#1\n");//R2=R2+1
    printf("%s\n",label_value(becky));//output the becky label
    printf("\tADD R6,R6,#-1\n");//decrement the stack pointer
    printf("\tSTR R2,R6,#0\n");//store the value in R2 at the top of the stack
}

/* 
 * gen_comparison
 *   DESCRIPTION: generate LC-3 assembly code for a comparison operator,
 *                leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the comparison
 *           false_cond -- a string representation of the condition codes 
 *                         for which the comparison is false (as calculated
 *                         by subtracting the second expression from the 
 *                         first)
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 *   KNOWN BUGS: ignores 2's complement overflow in making the comparison
 */
static void
gen_comparison (ast220_t* ast, const char* false_cond)
{
    ece220_label_t* false_label;

    false_label = label_create ();
    gen_expression (ast->left);
    gen_expression (ast->right);
    printf ("\tLDR R1,R6,#0\n\tLDR R0,R6,#1\n\tADD R6,R6,#2\n");
    printf ("\tAND R2,R2,#0\n\tNOT R1,R1\n\tADD R1,R1,#1\n\tADD R0,R0,R1\n");
    printf ("\tBR%s %s\n\tADD R2,R2,#1\n", false_cond,
	    label_value (false_label));
    printf ("%s\n", label_value (false_label));
    printf ("\tADD R6,R6,#-1\n\tSTR R2,R6,#0\n");
}

/* 
 * gen_op_cmp_ne
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is not equal to the value of a second 
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the inequality operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_ne (ast220_t* ast)
{
    gen_comparison (ast, "z");
}

/* 
 * gen_op_cmp_less
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is less than the value of a second
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the less-than operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_less (ast220_t* ast)
{
    gen_comparison (ast, "zp");
}

/* 
 * gen_op_cmp_le
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is less than or equal to the value 
 *                of a second expression, leaving the result (0 or 1) on 
 *                the stack
 *   INPUTS: ast -- the AST node corresponding to the less-or-equal-to 
 *                  operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_le (ast220_t* ast)
{
    gen_comparison (ast, "p");
}

/* 
 * gen_op_cmp_eq
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is equal to the value of a second 
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the equality operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_eq (ast220_t* ast)
{
    gen_comparison (ast, "np");
}

/* 
 * gen_op_cmp_ge
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is greater than or equal to the value 
 *                of a second expression, leaving the result (0 or 1) on 
 *                the stack
 *   INPUTS: ast -- the AST node corresponding to the greater-or-equal-to 
 *                  operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_ge (ast220_t* ast)
{
    gen_comparison (ast, "n");
}

/* 
 * gen_op_cmp_greater
 *   DESCRIPTION: generate LC-3 assembly code to compare whether the value
 *                of one expression is greater than the value of a second
 *                expression, leaving the result (0 or 1) on the stack
 *   INPUTS: ast -- the AST node corresponding to the greater-than operation
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints LC-3 instructions
 */
static void 
gen_op_cmp_greater (ast220_t* ast)
{
    gen_comparison (ast, "nz");
}

