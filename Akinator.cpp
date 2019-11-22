#include "Akinator.h"

#undef  TEST_LEVEL
#define TEST_LEVEL 0

int main(){

	std::setlocale(LC_ALL, "ru_RU.UTF-8");

	Sub_elem_t ans[MAX_LINE_LEN] = {};
	
	Akinator_t akin = {};

	AKINATOR_INIT(&akin);

	akin.root = node_make_from_file(TREE_FILE);

	while(true){
		
		GET_ANS;
		
		if(IS_ANS("Тест")){
			printf("ГЛаДОС: Привет, я ГЛаДОС. Отношениe твоих знаний о физтехе к моим стремится к нулю, но давай начнем ТЕСТЫ.\n\n");
			sleep(1);

			for(int i = 0; i < 100; i++){
				akin.definable = true;
				if(!akination_loop_step(&akin)){
					break;
				}
			}
		}
		else if(IS_ANS("end")) == 0){
			break;
		}
		else if(IS_ANS("Различие")){
			
			printf("Далее введи два названия\n");

			GET_ANS;
			Node_t * first = node_fine(akin.root, make_str(ans));

			GET_ANS;
			Node_t * second = node_fine(akin.root, make_str(ans));

			fprint_difference(first, second, stdout);

		}
		else if(IS_ANS("Определение")){
			
			printf("Далее введи название\n");

			GET_ANS;

			Node_t * first = node_fine(akin.root, make_str(ans));

			if(first == nullptr){
				printf("GLaDOS: Думаю ты не поймешь, что такое " ELEM_PRINT "\n", ans);
			}

			fprint_definition(akin.root, first, stdout);

		}
		else{

			printf("Доступные комманды:\n1) Тест        - пройти за странным человеком в лабораторию.\n2) Различие    - напечатать различие двух предметов.\n3) Определение - напечатать определение предмета.\n4) end         - закончить программу.\n6) help        - это меню.\n");
	
		}
	
	}

	test(1, "", (fprint_difference(root->right->left->left->right->left->right, root->right->left->right->right, stdout), true));
	test(1, "", (fprint_difference(root->right->left->left->left, root->right->left->right->right, stdout), true));
	
	dot_node(root, OFILE);

	akinator_deinit(&akin);

	return 0;
}

bool akination_loop_step(Akinator_t * akin){ //to_update

	node_assert(akin != nullptr);

	Sub_elem_t ans[MAX_LINE_LEN] = {};

	printf("ГЛаДОС: То, что ты загадал,\n");

	FILE * out = fopen(TREE_FILE, "r+");

	node_deinit(*root);
	*root = node_make_from_file(TREE_FILE);

	bool need_to_write = do_akination_recurcive(*root, *root);
	if(need_to_write){
		node_write_to_file(*root, out);
	}

	fclose(out);

	printf("Раттманн: Ты должен пройти ещё один тест, чтобы одолеть её! Ты готов?\n");
	GET_ANS

	if(!POS_ANS){

		printf("Раттманн: Ты хочешь уйти..?\n");
		GET_ANS

		if(POS_ANS){
			return false;
		}
	}

	return true;
}

bool do_akination_recurcive(Node_t * root, Node_t * node){

	Sub_elem_t ans[MAX_LINE_LEN] = {};

	if(LEFT == nullptr){

		printf("ГЛаДОС: Это " ELEM_PRINT "?\n", DATA);
		GET_ANS

		if(POS_ANS){
			printf("ГЛаДОС: Пришли результаты твоего теста. Ты - ужасный человек. Здесь так и написано.\n");
			sleep(1);
			return false;
		}
		else{
			return akinator_missed(root, node);
		}
	}
	else{
		printf("ГЛаДОС: " ELEM_PRINT "?\n", DATA);
		GET_ANS
		if(POS_ANS){ // add else if
			return do_akination_recurcive(root, RIGHT);
		}
		else{	
			return do_akination_recurcive(root, LEFT);
		}
		
	}
	return false;
}


bool akinator_missed(Node_t * root, Node_t * node){

	Sub_elem_t ans[MAX_LINE_LEN] = {};

	printf("ГЛаДОС: И что же ты загадал на этот раз?\n");
	GET_ANS

	str tmp = {};
	tmp.str = ans;
	tmp.size = strlen(ans);
	
	Node_t *found = node_find(root, &tmp);
	if(found != nullptr){

		printf("ГЛаДОС: Ты не права, " ELEM_PRINT "  ", ans);
		fprint_definition(nullptr, found->parent, stdout, check_parent(found));
		printf("\n");
		sleep(1);

		return false;
	}

	printf("ГЛаДОС: Как неожиданно известие о моей осведомленности об этом.\n");
	sleep(1);
	node_add_right(THIS, ans);

	printf("ГЛаДОС: И чем же, ты считаешь, отличается " ELEM_PRINT " от " ELEM_PRINT "?\n", RIGHT->data, DATA);

	while(true){

		GET_ANS
		if(check_negation_in_question(ans)){
			break;
		}
	}

	printf("ГЛаДОС: Видимо твое примитивное мышление только так и может их различить\n");
	sleep(1);

	node_add_left(THIS, DATA);

	str temp = {};
	temp.str = ans;
	temp.size = strlen(ans);
	node_set(THIS, &temp);

	return true;
}

bool check_negation_in_question(Sub_elem_t *ans){

	int temp_size = sizeof(Sub_elem_t) > 1 ? 3 : 5;
	Sub_elem_t temp[5] = Long_string("не");
	bool do_break = false;

	for(int j = 0, i = 0; j < temp_size - 1; j++){

		if(!isalpha(ans[i + j])){
			i++;
			j--;
			continue;
		}

		if(tolower(ans[i + j]) != temp[j]){
			do_break = true;
			return true;	
		}	
		else if(isalpha(ans[i + j + 1])){
			do_break = true;
		}
		else{
			do_break = false;
		}
	}

	if(do_break){ return true;}
	printf("ГЛаДОС: Не не не не не не не не не не не не не можно их так различать.\n");

	return false;
}


void fprint_difference(Node_t * node1, Node_t * node2, FILE * file){

	node_assert(file != nullptr);

	if(node1 == nullptr && node2 != nullptr){

		printf("GLaDOS: Думаю ты не поймешь, что такое " ELEM_PRINT "\n", node2->data);
		return;
	}
	else if(node1 != nullptr && node2 == nullptr){

		printf("GLaDOS: Думаю ты не поймешь, что такое " ELEM_PRINT "\n", node1->data);
		return;
	}
	
	if(node1 == nullptr || node2 == nullptr){
		return;
	}

	Node_t * ptr1 = node1;
	Node_t * ptr2 = node2;

	while(ptr1 != nullptr && ptr2 != nullptr && ptr1 != ptr2){

		bool changed1 = false;
		bool changed2 = false; 	
		int level1 = ptr1->level;
		int level2 = ptr2->level;
	
		if(level1 >= level2){
			ptr1 = ptr1->parent == nullptr ? ptr1 : (changed1 = true, ptr1->parent);
		}
		if(level2 >= level1){
			ptr2 = ptr2->parent == nullptr ? ptr2 : (changed2 = true, ptr2->parent);	
		}

		if(!changed1 && !changed2){
			break;
		}
	}

	if(ptr2 != ptr1){
		assert(false == true);
	}
	
	fprintf(file, ELEM_PRINT " и " ELEM_PRINT " схожи %s", node1->data, node2->data, ptr1->parent != nullptr ? "тем, что оба " : "ничем");
	fprint_definition(nullptr, ptr1->parent, file, check_parent(ptr1));

	fprintf(file, ", но " ELEM_PRINT " ", node1->data);
	fprint_definition(ptr1->parent, node1->parent, file, check_parent(node1));

	fprintf(file, ", a "  ELEM_PRINT " ", node2->data);
	fprint_definition(ptr1->parent, node2->parent, file, check_parent(node2));

	fprintf(file, "\n");
}

void fprint_definition(Node_t * root, Node_t * node, FILE * file, bool reverse){

	node_assert(file != nullptr);

	if(THIS == root || THIS == nullptr){
		return;
	}
	
	fprint_definition(root, PARENT, file, check_parent(THIS));

	Sub_elem_t buff[MAX_LINE_LEN] = {};
	int pc = 0;

	(THIS->parent == root) ? 0 : (pc += 2, buff[pc - 2] = Long_string(','), buff[pc - 1] = Long_string(' '));

	(!reverse) ? (strcpy(buff + pc, Long_string("не ")), 1) : 1;

	fprintf(file, ELEM_PRINT ELEM_PRINT, buff, DATA);
}


void node_write_to_file(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;
	}

	node_assert(check_nullptr(THIS));

	fprintf(file, SUB_ELEM_PRINT, SEPARATOR_START);
	fprintf(file, "\"" ELEM_PRINT "\"", DATA);

	node_write_to_file(LEFT, file);
	node_write_to_file(RIGHT, file);

	fprintf(file, SUB_ELEM_PRINT, SEPARATOR_END);

}

Node_t * node_make_from_file(const char file_name[]){

	int buff_size = 0;
	Buff_elem_t * buff = read_file_to_created_buff(file_name, &buff_size);

	int pc = 0;
	Node_t * root = node_make_from_buff(nullptr, buff, buff_size, &pc);
	free(buff);
	return root;
}

Node_t * node_make_from_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc){

	if(*pc >= buff_size){
		return nullptr;
	}

	Node_t * new_node = nullptr;
	if(buff[PC_N] == SEPARATOR_START){
		NODE_ADD(THIS, Long_string(""));
		PC_INCR;
	}

	Sub_elem_t temp_buff[MAX_LINE_LEN] = {};
	int temp = 0;
	int ans = sscanf(buff + *pc, Long_string("\"%l[^\"]\"%n"), temp_buff, &temp);

	if(ans == 0){
		return nullptr;
	}
	*pc += temp;

	str str = {};
	str.str = temp_buff;
	str.size = strlen(str.str);

	node_set(THIS, &str);

	LEFT  = node_make_from_buff(THIS, buff, buff_size, pc);
	RIGHT = node_make_from_buff(THIS, buff, buff_size, pc);

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	if(buff[PC_N] == SEPARATOR_END){
		PC_INCR;
		return node;
	}

	return nullptr;
}


bool akinator_init(Akinator_t *akin, const char name[]){ // to do

	THIS->canary1 = CANARY_VALUE;

	LEFT   = nullptr;
	RIGHT  = nullptr;
	PARENT = parent;
	
	THIS->level = parent == nullptr ? 0 : parent->level + 1;

	THIS->errnum = 0;

	THIS->name = name[0] == '&' ? name + 1 : name;

	THIS->canary2 = CANARY_VALUE;

	THIS->definable = false;

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	node_assert(check_node(THIS, __LOCATION__));
 
	return true;
}

bool akinator_deinit(Akinator_t *akin){ //to_do

	if(THIS == nullptr){
		return true;
	}

	node_assert(check_node(THIS, __LOCATION__));

	node_deinit(LEFT);
	node_deinit(RIGHT);
	
	zero_data(THIS, 0, sizeof(Node_t));
	free(THIS);

	return true;
}


bool node_init(Node_t *node, const char name[], Node_t *parent){

	node_assert(check_nullptr(THIS));

	THIS->canary1 = CANARY_VALUE;

	LEFT   = nullptr;
	RIGHT  = nullptr;
	PARENT = parent;
	
	THIS->level = parent == nullptr ? 0 : parent->level + 1;

	THIS->errnum = 0;

	THIS->name = name[0] == '&' ? name + 1 : name;

	THIS->canary2 = CANARY_VALUE;

	THIS->definable = false;

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	node_assert(check_node(THIS, __LOCATION__));
 
	return true;
}

Node_t *node_add_right(Node_t *node, Elem_t elem){

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * new_node = nullptr;
	NODE_ADD(RIGHT, elem);
	
	node_assert(check_node(THIS, __LOCATION__));

	return new_node;
}

Node_t *node_add_left(Node_t *node, Elem_t elem){

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * new_node = nullptr;
	NODE_ADD(LEFT, elem);
	
	node_assert(check_node(THIS, __LOCATION__));

	return new_node;
}

Node_t * node_find(Node_t *node, str_ptr cmp_elem){

	if(cmp_elem == nullptr || cmp_elem.size < 0){
		return nullptr;
	}
	
	if(THIS == nullptr){
		return nullptr;	
	}

	if(cmp_elem->size > MAX_LINE_LEN){	
		cmp_elem->str[MAX_LINE_LEN] = '\0';
		cmp_elem->size = MAX_LINE_LEN;
	}

	if(strcasecmp(DATA, cmp_elem->str) == 0){
		return THIS;
	}

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * ans = node_find(LEFT, cmp_elem);
	if(ans != nullptr){
		return ans;	
	}

	ans = node_find(RIGHT, cmp_elem);
	
	return ans;
}

bool node_deinit(Node_t *node){

	if(THIS == nullptr){
		return true;
	}

	node_assert(check_node(THIS, __LOCATION__));

	node_deinit(LEFT);
	node_deinit(RIGHT);
	
	zero_data(THIS, 0, sizeof(Node_t));
	free(THIS);

	return true;
}


Elem_t node_get(Node_t *node){

	node_assert(check_node(THIS, __LOCATION__));
	
	return DATA;
}


bool node_set(Node_t *node, str_ptr elem){

	node_assert(check_node(THIS, __LOCATION__));
	
	if(elem->size > MAX_LINE_LEN){
		elem->str[MAX_LINE_LEN] = '\0';
		elem->size = MAX_LINE_LEN;
	}
	strcpy(DATA, elem->str);

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	node_assert(check_node(THIS, __LOCATION__));

	return true;
}

bool fill_data_with_poison(void * data, const size_t size, long long int poison, const size_t Elem_size){
	
	node_assert(data != nullptr);

	for(unsigned int i = 0; i < size * Elem_size; i++){
		((char *)data)[i] = ((char *) &poison)[i % Elem_size];
	}

	return true;

}

void zero_data(void * const data, const int start, const int end){

	node_assert(data != nullptr);
	node_assert(start < end);	

	for(int i = start; i < end; i++){
		((char *) data)[i] = '\0';
	}

}

bool check_node(Node_t *node, const char called_from_file[], int line, const char func[], bool print_all/* = true*/){


	bool passed = true;
	bool hashable = true;

	if(!check_nullptr(THIS)){
		return false;
	}

	if(THIS->errnum != 0){
		return false;	
	}

// A Lot of checks below
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_canary(THIS)                     ? passed : (THIS->errnum += CHECK_CANARY, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_hash(THIS)		        ? passed : (THIS->errnum += CHECK_HASH, false);
	if(DEBUG) printf("Passed: %d\n", passed);
 
	passed = check_nullptr(THIS->name)              ? passed : (THIS->errnum += NAME_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_nullptr(THIS->data)              ? passed : (THIS->errnum += DATA_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);

/*	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);*/

	if(DEBUG) printf("\n");

// End of a Lot of checks
	if(print_all || !passed){
		node_dump(THIS, called_from_file, line, func, passed);
	}

	return passed;
}


#undef fprintf

void node_dump(Node_t * const node, const char called_from_file[], int line, const char func[], bool ok, time_t curtime /* = -1*/){

	if(THIS == nullptr) return;

	bool cur_time_setted = true;

	if(curtime == -1){
		time(&curtime);	
		cur_time_setted = false;
	}

	FILE *log_file = fopen(LOG_FILE, "a");
	if(log_file == nullptr){
		fprintf(stderr, "Not able to open %s in %s line %d in %s\n", LOG_FILE, __LOCATION__);
	}


	fprintf(log_file, (cur_time_setted) ? "%s" : "\nTime and date: %s\n"
			  "Dumping node from file %s, line %d, function %s [%s]\n"
			  "node [%p] %s [%p] {\n"
			  "\n\t hash    = " HASH_PRINT "\n"
			  "\n\t canary1 = " CANARY_PRINT "\n"
			  "\t data    = [%p] {" ELEM_PRINT "}\n"
			  "\t right   = [%p]\n"
			  "\t left    = [%p]\n"
			  "\t parent  = [%p]\n",
		(cur_time_setted) ? "" : ctime(&curtime),
		called_from_file, line, func, ok ? "ok" : "ERROR!!!",
		THIS, THIS->name, THIS->name,
		THIS->hash,
		THIS->canary1,
		DATA, DATA,
		RIGHT,
		LEFT,
		PARENT);
	fflush(log_file);
	
	fprintf(log_file, "\n\t errnum   = ");
	for(int i = 0; i < MAX_ERROR_NO; i++){
		fprintf(log_file, "%d", (THIS->errnum >> i) % 2);
	}
	fprintf(log_file, "\n");
	fflush(log_file); 

	fprintf(log_file, "\n\t canary2 = " CANARY_PRINT "\n\n}\n\n",
		THIS->canary2);

	fflush(log_file);

	fclose(log_file);

}

void dot_node(Node_t * root, const char ofile[]){

	node_assert(check_node(root, __LOCATION__));

	FILE *file = fopen(ofile, "wb");
	fprintf(file, "digraph %s{\n", root->name);

	color_node(root, file);
	
	node_dot_dependences(root, file);	

	fprintf(file, "}\n");

	fclose(file);
	
	char command[MAX_COMMAND_LEN];

	sprintf(command, "dot -Tpdf %s -o %s\n", ofile, IMAGE_OUT);
	std::system(command);

	sprintf(command, "rm %s\n", ofile);
	std::system(command);
	
}

void color_node(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	char color[MAX_COMMAND_LEN] = {};
	sprintf(color, "color = \"%s\"", PARENT == nullptr ? "orange" :
			(LEFT == nullptr && RIGHT == nullptr ? "red" :
			(LEFT != nullptr && RIGHT != nullptr ? "green" : "black")));

	Sub_elem_t data[MAX_COMMAND_LEN] = {};
	for(int i = 0, j = 0; j < MAX_COMMAND_LEN;){
		if(DATA[i] == ' '){
			data[j++] = Long_string('&');
			data[j++] = Long_string('#');
			data[j++] = Long_string('9');
			data[j++] = Long_string('2');
			data[j++] = Long_string(';');		
		}
		data[j++] = DATA[i++];
	}

	fprintf(file, "%d [%s; shape = record; label =\"" ELEM_PRINT "\"]\n", THIS, color, data);

	color_node(LEFT, file);
	color_node(RIGHT, file);

}

void node_dot_dependences(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	if(LEFT   != nullptr){
		fprintf(file, "%d -> %d [label = \"Нет\"]\n", THIS, LEFT);
	}
	if(RIGHT  != nullptr){
		fprintf(file, "%d -> %d [label = \"Да\"]\n", THIS, RIGHT);
	}
	
	node_dot_dependences(LEFT, file);
	node_dot_dependences(RIGHT, file);
}

bool check_nullptr(const void * const data){
	
	return data != nullptr;
}

bool check_canary(Node_t * const node){

	if(THIS == nullptr) return false;
	return THIS->canary1 == THIS->canary2 && THIS->canary2 == CANARY_VALUE;
}

bool check_hash(Node_t * const node){

	if(!check_hashable(THIS)) return false;
	return is_equal_hash_t(find_sum_hash(THIS, sizeof(Node_t)), THIS->hash);
}

bool check_hashable(Node_t * const node){
	
	return true;
}

bool is_equal_hash_t(const hash_t left, const hash_t right){
	
	return left == right;
}

hash_t find_sum_hash(Node_t * const node, size_t size){

	if(THIS == nullptr) return false;

	hash_t hash_sum = 0;
	
	hash_sum += hash(((char *)THIS) + sizeof(hash_t), size - sizeof(hash_t));

	return hash_sum;

}

hash_t hash(const void * const data, size_t size){

	if(data == nullptr) return false;

	hash_t hash_sum = 0;

	for(size_t counter = 0; counter < size; counter++){
		
		hash_sum = (hash_sum << 2) + (counter+1)*((*((char *) data + counter))^0xfc);
	}
	
	return hash_sum;

}


int clear_stdin(void){

	int did = 0;
	Sub_elem_t c = fgetc(stdin);

	while(c != Long_string('\n') && c != EOF){
		did = 1;
		c = fgetc(stdin);
	}

	return did;	
}


