#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <vector>


#define BUF_SIZE 327680

using namespace std;


class Xml_Bracket{
public:
	char *name;
	
	char **arg;

	char *buf;
	int arg_len[256];
	int num_arg;

	//char *dat;
	int flag;	 // start or end;
	void str_spacer(char *str, int len);
};

class Xml_Tree{
public:
	int fd ;

	char buf[BUF_SIZE];

	int gen;
	int size;

	vector<Xml_Bracket> xml_list;

	vector<Xml_Tree>children;

	void load(const char filename[]);
	char *xml_type;

	void lex();
	int bracket_start(int off );
	int bracket_end(int off);

	char *get_data(char* target, char *keyword);
	char *get_data_by_key(char* name, char *keyword, char* value, char* target);
	char *get_data_by_id(char* name, int value, char* target);

	void show();
	void show_buf();

	int type();
	int create_children(void);


	Xml_Tree(const char filename[]){
		gen = 0;
		load(filename);
		lex();
		type();
		create_children();
	}
	Xml_Tree(vector<Xml_Bracket> new_xml_list, int parent_gen){
		xml_list = new_xml_list;
		gen = parent_gen + 1;
		create_children();
	}

		
};


int str_match(char *s0, char *s1, int len);




