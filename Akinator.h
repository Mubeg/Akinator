#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <locale.h>
#include "includes/test-log.define"
#define USING_WCHAR_T
#include "includes/ionegin"

#define IS_ANS(arg) strcasecmp(ans, Long_string((arg))) == 0

#define asserted || (fprintf(stderr, "Asserted from %s on %d in %s\n", __LOCATION__), abort(), false);
#define node_assert(condition) if(!(condition)){fprintf(stderr, "File %s line %d function %s %s\n", __LOCATION__, #condition); abort();}
//#define node_assert(condition) 

#define check_parent(arg) arg->parent == nullptr ? false : arg->parent->right == arg 

#define NODE_INIT(name, ...) node_init((name), #name, ##__VA_ARGS__)

#define NODE_ADD(DIR, elem) 	if(DIR != nullptr && DIR != THIS){return nullptr;}\
				str temp = {};\
				temp.str = elem;\
				temp.size = strlen(elem);\
				new_node = (Node_t *) calloc(1, sizeof(Node_t));\
				NODE_INIT(new_node, THIS);\
				node_set(new_node, &temp);\
				\
				DIR = new_node;\
				THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

#define GET_ANS printf("Ты: "); ans[0] = Long_string('\0'); scanf(Long_string("%" MAX_LINE_LEN_PRINT IS_LONG_STR  "[^\n]"), ans); clear_stdin();

#define POS_ANS (strcasecmp(ans, Long_string("y")) == 0 || strcasecmp(ans, Long_string("yes")) == 0 || strcasecmp(ans, Long_string("да")) == 0) 
#define SNTY (strcasecmp(ans, Long_string("Скорее нет, чем да")) == 0)
#define SYTN (strcasecmp(ans, Long_string("Скорее да, чем нет")) == 0)
#define IDK  (strcasecmp(ans, Long_string("незнаю")) == 0 || strcasecmp(ans, Long_string("неуверен")) == 0)
#define NEG_ANS (strcasecmp(ans, Long_string("нет")) == 0)

#define PC_N (*pc)
#define PC_INCR (*pc)+=1

#define THIS   node
#define DATA   node->data
#define LEFT   node->left
#define RIGHT  node->right
#define PARENT node->parent

//#define TESTING

#ifdef TESTING
#define NDEBUG
#endif
#include <assert.h>

typedef wchar_t *Elem_t;
#define ELEM_PRINT "%ls" 
typedef wchar_t Sub_elem_t;
#define SUB_ELEM_PRINT "%lc"

typedef long long int Canary_t;
#define CANARY_PRINT "%lX"

typedef unsigned long long int hash_t;
#define HASH_PRINT "%Lu"

struct Akinator_t {

	Canary_t canary1;

	bool definable;
	Stack_t stack;
	Node_t root;

	Canary_t canary2;
	
}

struct Node_t {

	hash_t hash; // always comes first
	const char *name;

	Canary_t canary1;

	Sub_elem_t data[MAX_LINE_LEN];
	Node_t * left;
	Node_t * right;
	Node_t * parent;
	int level;

	int errnum;

	Canary_t canary2;

};


const int CHECK_ZERO_ 		   	= __LINE__;
const int CHECK_CANARY             	= 1 << (__LINE__ - CHECK_ZERO_ - 1); //= 1;
const int CHECK_HASH               	= 1 << (__LINE__ - CHECK_ZERO_ - 1); //= 2;
const int NAME_NULLPTR             	= 1 << (__LINE__ - CHECK_ZERO_ - 1); //= 4;
const int DATA_NULLPTR             	= 1 << (__LINE__ - CHECK_ZERO_ - 1); //= 8;
const int MAX_ERROR_NO 		   	= (__LINE__ - CHECK_ZERO_ - 1);


const bool DEBUG 			= 0;
const Canary_t CANARY_VALUE 		= 0xBEDAAAAAAAAAAAAA;
const long long int POISON 		= 'AAATIKIN';
const long long int POISON_POISON 	= 'AAA23DED';
const int LIST_INIT_SIZE 		= 5;
const int START_FUNCTION 		= 1;
const char LOG_FILE[]   		= "list_log.txt";
const char IMAGE_OUT[]  		= "Graph.pdf";	
const char OFILE[] 			= "tempfile";
const char TREE_FILE[]			= "tree_file.tree";
const int MAX_COMMAND_LEN 		= 100;
const int END_FUNCTION 			= 0;
const char SEPARATOR_START		= L'{';
const char SEPARATOR_END		= L'}';
//const int LIST_CLUSTER_SIZE = sizeof(Elem_t) + 2*sizeof(int);


bool akination_loop_step(Node_t ** root);


bool do_akination_recurcive(Node_t * root, Node_t * node);


bool akinator_missed(Node_t * root, Node_t * node);


bool check_negation_in_question(Sub_elem_t *ans);


void fprint_difference(Node_t * node1, Node_t * node2, FILE * file);


void fprint_definition(Node_t * root, Node_t * node, FILE * file, bool reverse);


void node_write_to_file(Node_t * node, FILE * file);


Node_t * node_make_from_file(const char file_name[]);


Node_t * node_make_from_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc);


bool node_init(Node_t *node, const char name[], Node_t *parent);


Node_t *node_add_right(Node_t *node, Elem_t elem);


Node_t *node_add_left(Node_t *node, Elem_t elem);


Node_t * node_find(Node_t *node, str_ptr cmp_elem);


bool node_deinit(Node_t *node);


Elem_t node_get(Node_t *node);


bool node_set(Node_t *node, str_ptr elem);


bool fill_data_with_poison(void * data, const size_t size, long long int poison, const size_t Elem_size);


void zero_data(void * const data, const int start, const int end);


bool check_node(Node_t *node, const char called_from_file[], int line, const char func[], bool print_all = true);


void node_dump(Node_t * const node, const char called_from_file[], int line, const char func[], bool ok, time_t curtime = -1);


void dot_node(Node_t * root, const char ofile[]);


void color_node(Node_t * node, FILE * file);


void node_dot_dependences(Node_t * node, FILE * file);


bool check_nullptr(const void * const node);


bool check_canary(Node_t * const node);


bool check_hash(Node_t * const node);


bool check_hashable(Node_t * const node);


bool is_equal_hash_t(const hash_t left, const hash_t right);


hash_t find_sum_hash(Node_t * const node, size_t size);


hash_t hash(const void * const node, size_t size);


bool check_size(Node_t * const node);


int clear_stdin(void);
